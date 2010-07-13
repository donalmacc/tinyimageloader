#include "..\SDK\TILImagePNG.h"

#if (TIL_FORMAT & TIL_FORMAT_PNG)

#include <math.h>
#include <string.h>
#include <malloc.h>

namespace til
{

	#define PNG_TYPE(a, b, c, d)           (((a) << 24) + ((b) << 16) + ((c) << 8) + (d))

	#define PNG_DEBUG(msg, ...)            printf(msg"\n", __VA_ARGS__)

	// fast-way is faster to check than jpeg huffman, but slow way is slower
	#define ZFAST_BITS     9 // accelerate all cases in default tables
	#define ZFAST_MASK     ((1 << ZFAST_BITS) - 1)

	#define F_none         0
	#define F_sub          1
	#define F_up           2
	#define F_avg          3
	#define F_paeth        4
	#define F_avg_first    5 
	#define F_paeth_first  6

	static uint8 first_row_filter[5] =
	{
		F_none, F_sub, F_none, F_avg_first, F_paeth_first
	};

	static int length_base[31] = {
		3,4,5,6,7,8,9,10,11,13,
		15,17,19,23,27,31,35,43,51,59,
		67,83,99,115,131,163,195,227,258,0,0 
	};

	static int length_extra[31] = { 
		0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0 
	};

	static int dist_base[32] = { 
		1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,
		257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577,0,0
	};

	static int dist_extra[32] = { 
		0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13
	};

	// Stolen from FreeImage. Yoink!

	#define PNG_COLOR_MASK_PALETTE     0x00000001
	#define PNG_COLOR_MASK_COLOR       0x00000002
	#define PNG_COLOR_MASK_ALPHA       0x00000004

	#define PNG_COLOR_TYPE_GRAY        0
	#define PNG_COLOR_TYPE_PALETTE     (PNG_COLOR_MASK_COLOR | PNG_COLOR_MASK_PALETTE)
	#define PNG_COLOR_TYPE_RGB         (PNG_COLOR_MASK_COLOR)
	#define PNG_COLOR_TYPE_RGB_ALPHA   (PNG_COLOR_MASK_COLOR | PNG_COLOR_MASK_ALPHA)
	#define PNG_COLOR_TYPE_GRAY_ALPHA  (PNG_COLOR_MASK_ALPHA)

	int32 BitReverse16(int32 a_Number)
	{
		a_Number  = ((a_Number & 0xAAAA) >> 1) | ((a_Number & 0x5555) << 1);
		a_Number  = ((a_Number & 0xCCCC) >> 2) | ((a_Number & 0x3333) << 2);
		a_Number  = ((a_Number & 0xF0F0) >> 4) | ((a_Number & 0x0F0F) << 4);
		a_Number  = ((a_Number & 0xFF00) >> 8) | ((a_Number & 0x00FF) << 8);
		return a_Number;
	}

	int32 BitReverse(int32 a_Number, int32 a_Bits)
	{
		//assert(bits <= 16);
		// to bit reverse n bits, reverse 16 and shift
		// e.g. 11 bits, bit reverse and shift away 5
		return BitReverse16(a_Number) >> (16 - a_Bits);
	}

	int paeth(int a, int b, int c)
	{
		int p = a + b - c;
		int pa = abs(p - a);
		int pb = abs(p - b);
		int pc = abs(p - c);
		if (pa <= pb && pa <= pc) { return a; }
		if (pb <= pc) { return b; }
		return c;
	}

	// =========================================
	// Huffman
	// =========================================

	class Huffman
	{

	public:

		Huffman() { }
		~Huffman() { }

		bool Build(uint8* a_SizeList, uint32 a_Amount)
		{
			int code, next_code[16];

			int sizes[17] = { 0 };

			// DEFLATE spec for generating codes
			memset(sizes, 0, sizeof(sizes));
			memset(fast, 255, sizeof(fast));
			for (uint32 i = 0; i < a_Amount; ++i) { ++sizes[a_SizeList[i]]; }
			sizes[0] = 0;
			//for (int i = 1; i < 16; ++i) { assert(sizes[i] <= (1 << i)); }
			code = 0;

			uint32 k = 0;
			for (uint32 i = 1; i < 16; ++i) 
			{
				next_code[i] = code;
				firstcode[i] = (uint16)code;
				firstsymbol[i] = (uint16)k;
				code = (code + sizes[i]);
				if (sizes[i] && code - 1 >= (1 << i))
				{
					TIL_ERROR_EXPLAIN("Bad codelengths.");
					return false;
				}
				maxcode[i] = code << (16 - i); // preshift for inner loop
				code <<= 1;
				k += sizes[i];
			}

			maxcode[16] = 0x10000; // sentinel
			for (uint32 i = 0; i < a_Amount; ++i) 
			{
				int s = a_SizeList[i];
				if (s > 0) 
				{
					int c = next_code[s] - firstcode[s] + firstsymbol[s];
					size[c] = (uint8)s;
					value[c] = (uint16)i;
					if (s <= ZFAST_BITS) 
					{
						int k = BitReverse(next_code[s], s);
						while (k < (1 << ZFAST_BITS)) 
						{
							fast[k] = (uint16) c;
							k += (1 << s);
						}
					}
					++next_code[s];
				}
			}

			return true;
		}

		uint16 fast[1 << ZFAST_BITS];
		uint16 firstcode[16];
		int maxcode[17];
		uint16 firstsymbol[16];
		uint8  size[288];
		uint16 value[288];

	}; // class Huffman

	// =========================================
	// ImagePNG
	// =========================================

	int32 ImagePNG::GetByte()
	{
		int32 result = fgetc(m_Handle);
		if (result == EOF) { return 0; }
		return result;
	}

	int32 ImagePNG::GetWord()
	{
		return ((GetByte() << 8) | GetByte());
	}

	int32 ImagePNG::GetDWord()
	{
		return ((GetWord() << 16) | GetWord());
	}

	void ImagePNG::Skip( uint32 a_Bytes )
	{
		fseek(m_Handle, a_Bytes, SEEK_CUR);
	}

	int32 ImagePNG::GetZBufByte()
	{
		if (m_ZBuffer.zbuffer >= m_ZBuffer.zbuffer_end) return 0;
		return *m_ZBuffer.zbuffer++;
	}

	uint32 ImagePNG::GetZCode( uint32 a_Amount )
	{
		if (m_ZBuffer.num_bits < a_Amount) 
		{				
			do 
			{
				if (m_ZBuffer.code_buffer >= (1U << m_ZBuffer.num_bits))
				{
					TIL_ERROR_EXPLAIN("Something went terribly wrong.");
					return -1;
				}

				m_ZBuffer.code_buffer |= (GetZBufByte() << m_ZBuffer.num_bits);
				m_ZBuffer.num_bits += 8;
			} 
			while (m_ZBuffer.num_bits <= 24);
		}
		uint32 result = m_ZBuffer.code_buffer & ((1 << a_Amount) - 1);
		m_ZBuffer.code_buffer >>= a_Amount;
		m_ZBuffer.num_bits -= a_Amount;

		return result;
	}

	chunk ImagePNG::GetChunkHeader()
	{
		chunk result;
		result.length    = GetDWord();

		result.header[0] = (char)GetByte();
		result.header[1] = (char)GetByte();
		result.header[2] = (char)GetByte();
		result.header[3] = (char)GetByte();
		result.header[4] = 0;

		result.type      = (result.header[0] << 24) | (result.header[1] << 16) | (result.header[2] << 8) | (result.header[3]);

		return result;
	}

	int ImagePNG::HuffmanDecode(Huffman* a_Huffman)
	{
		if (m_ZBuffer.num_bits < 16) 
		{
			do 
			{
				if (m_ZBuffer.code_buffer >= (1U << m_ZBuffer.num_bits))
				{
					TIL_ERROR_EXPLAIN("Something went terribly wrong.");
					return -1;
				}

				m_ZBuffer.code_buffer |= (GetZBufByte() << m_ZBuffer.num_bits);
				m_ZBuffer.num_bits += 8;
			} 
			while (m_ZBuffer.num_bits <= 24);
		}

		int b = a_Huffman->fast[m_ZBuffer.code_buffer & ZFAST_MASK];
		if (b < 0xffff) 
		{
			int size = a_Huffman->size[b];
			m_ZBuffer.code_buffer >>= size;
			m_ZBuffer.num_bits -= size;
			return a_Huffman->value[b];
		}

		// not resolved by fast table, so compute it the slow way
		// use jpeg approach, which requires MSbits at top
		int k = BitReverse(m_ZBuffer.code_buffer, 16);
		int s;
		for (s = ZFAST_BITS + 1; ; ++s)
		{
			if (k < a_Huffman->maxcode[s])
			{
				break;
			}
		}

		if (s == 16) 
		{
			TIL_ERROR_EXPLAIN("Invalid s: 16.");
			return -1; // invalid code!
		}
		// code size is s, so:
		b = (k >> (16 - s)) - a_Huffman->firstcode[s] + a_Huffman->firstsymbol[s];
		if (a_Huffman->size[b] != s)
		{
			TIL_ERROR_EXPLAIN("Huffman size doesn't match code size.");
			return -1;
		}
		m_ZBuffer.code_buffer >>= s;
		m_ZBuffer.num_bits -= s;

		return (a_Huffman->value[b]);
	}

	bool ImagePNG::ZLibExpand( int a_Amount )
	{
		if (!m_ZBuffer.z_expandable) 
		{
			TIL_ERROR_EXPLAIN("Could not expand ZLib buffer, reached limit.");
			return false;
		}

		int cur   = (int)(m_ZBuffer.zout     - m_ZBuffer.zout_start);
		int limit = (int)(m_ZBuffer.zout_end - m_ZBuffer.zout_start);

		// double until greater
		while (cur + a_Amount > limit)
		{
			limit *= 2;
		}
		
		char* bigger_data = new char[limit];
		memcpy(bigger_data, m_ZBuffer.zout_start, limit / 2);
		delete [] m_ZBuffer.zout_start;

		m_ZBuffer.zout_start = bigger_data;
		m_ZBuffer.zout       = bigger_data + cur;
		m_ZBuffer.zout_end   = bigger_data + limit;

		return true;
	}

	bool ImagePNG::ZLibDecode()
	{
		const int32 initial_size = 16384;

		char* p = new char[initial_size]; // initial size
		if (p == NULL) return NULL;
		m_ZBuffer.zbuffer = (uint8*)idata;
		m_ZBuffer.zbuffer_end = (uint8*)idata + ioff;

		m_ZBuffer.zout_start    = p;
		m_ZBuffer.zout          = p;
		m_ZBuffer.zout_end      = p + initial_size;
		m_ZBuffer.z_expandable  = 1;

		// check header	

		int cmf   = GetZBufByte();
		int cm    = cmf & 15;
		int flg   = GetZBufByte();
		if ((cmf * 256 + flg) % 31 != 0) 
		{
			TIL_ERROR_EXPLAIN("Bad ZLib header.");
			return false;
		}
		if (flg & 32) 
		{
			TIL_ERROR_EXPLAIN("Preset dictionary not allowed.");
			return false;
		}
		if (cm != 8)
		{
			TIL_ERROR_EXPLAIN("Bad compression.");
			return false;
		}

		m_ZBuffer.num_bits = 0;
		m_ZBuffer.code_buffer = 0;

		int final = 0;

		do 
		{
			final = GetZCode(1);
			int type = GetZCode(2);

			if (type == 0) 
			{
				//if (!parse_uncompressed_block(a)) return 0;	
				TIL_ERROR_EXPLAIN("TODO: Parse uncompressed block.");
				return false;
			} 
			// indexed color
			else if (type == 3) 
			{
				TIL_ERROR_EXPLAIN("TODO: Type == 3.");
				return false;
			} 
			else 
			{
				if (type == 1) 
				{
					InitDefaults();

					m_ZBuffer.z_length->Build(default_length, 288);
					m_ZBuffer.z_distance->Build(default_distance, 32);
				} 
				else 
				{
					static uint8 length_dezigzag[19] = { 16, 17, 18,  0,  8,  7,  9,  6, 10, 5, 11,  4, 12,  3, 13,  2, 14,  1, 15 };
					uint8 lencodes[286 + 32 + 137]; //padding for maximum single op
					uint8 codelength_sizes[19];

					int hlit  = GetZCode(5) + 257;
					int hdist = GetZCode(5) + 1;
					int hclen = GetZCode(4) + 4;

					memset(codelength_sizes, 0, sizeof(codelength_sizes));
					for (int32 i = 0; i < hclen; ++i)
					{
						int s = GetZCode(3);
						codelength_sizes[length_dezigzag[i]] = (uint8)s;
					}

					m_Huffman = new Huffman();
					if (!m_Huffman->Build(codelength_sizes, 19))
					{
						TIL_ERROR_EXPLAIN("Failed to build Huffman thing.");
						return false;
					}

					int n = 0;
					while (n < hlit + hdist) 
					{
						//int c = zhuffman_decode(a, &z_codelength);
						int c = HuffmanDecode(m_Huffman);
						//assert(c >= 0 && c < 19);
						if (c < 0 || c >= 19)
						{
							TIL_ERROR_EXPLAIN("Couldn't Huffman decode data.");
							return NULL;
						}
						if (c < 16)
						{
							lencodes[n++] = (uint8)c;
						}
						else if (c == 16) 
						{
							c = GetZCode(2) + 3;
							memset(lencodes + n, lencodes[n - 1], c);
							n += c;
						} 
						else if (c == 17) 
						{
							c = GetZCode(3) + 3;
							memset(lencodes + n, 0, c);
							n += c;
						} 
						else 
						{
							if (c != 18)
							{
								TIL_ERROR_EXPLAIN("C is too big.");
								return NULL;
							}
							c = GetZCode(7) + 11;
							memset(lencodes + n, 0, c);
							n += c;
						}
					}

					if (n != hlit + hdist) 
					{
						TIL_ERROR_EXPLAIN("Bad codelength.");
						return NULL;
					}

					m_ZBuffer.z_length = new Huffman();
					m_ZBuffer.z_length->Build(lencodes, hlit);
					m_ZBuffer.z_distance = new Huffman();
					m_ZBuffer.z_distance->Build(lencodes + hlit, hdist);
				}

				// parse block

				while (1)
				{
					int z = HuffmanDecode(m_ZBuffer.z_length);
					if (z < 256)
					{
						if (z < 0) 
						{
							TIL_ERROR_EXPLAIN("Bad Huffman code. (%i)", z);
							return NULL;
						}

						if (m_ZBuffer.zout >= m_ZBuffer.zout_end) 
						{
							PNG_DEBUG("Expanding buffer.");
							if (!ZLibExpand(1))
							{
								TIL_ERROR_EXPLAIN("Could not expand stuff.");
								return 0;
							}
						}
						*m_ZBuffer.zout++ = (char)z;
					} 
					else 
					{
						if (z == 256) 
						{ 
							break;
							//return 1; 
						}

						z -= 257;
						int len = length_base[z];
						if (length_extra[z]) 
						{
							len += GetZCode(length_extra[z]);
						}

						z = HuffmanDecode(m_ZBuffer.z_distance); 
						if (z < 0) 
						{
							TIL_ERROR_EXPLAIN("Bad Huffman code. (%i)", z);
							return NULL;
						}

						int dist = dist_base[z];
						if (dist_extra[z]) 
						{
							dist += GetZCode(dist_extra[z]);
						}
						if (m_ZBuffer.zout - m_ZBuffer.zout_start < dist) 
						{
							TIL_ERROR_EXPLAIN("Bad distance. (%i)", dist);
							return NULL;
						}
						if (m_ZBuffer.zout + len > m_ZBuffer.zout_end) 
						{
							if (!ZLibExpand(len)) 
							{
								TIL_ERROR_EXPLAIN("Could not expand stuff.");
								return NULL;
							}
						}

						uint8* p = (uint8*)(m_ZBuffer.zout - dist);
						while (len--)
						{
							*m_ZBuffer.zout++ = *p++;
						}
					}
				}
			}
		} 
		while (!final);

		return true;
	}

	bool ImagePNG::Compile()
	{
		return false;
	}

	bool ImagePNG::Parse(uint32 a_ColorDepth /*= TIL_DEPTH_A8R8G8B8*/)
	{
		int req_comp = 1;
		uint8* result = NULL;
		expanded = NULL;
		idata = NULL;
		out = NULL;
		if (req_comp < 0 || req_comp > 4)
		{
			TIL_ERROR_EXPLAIN("Bad req_comp.");
			return false;
		}

		uint8 palette[1024];
		uint8 pal_img_n = 0;
		uint8 has_trans = 0;
		uint8 tc[3];
		ioff = 0;
		uint32 idata_limit = 0;
		uint32 i;
		uint32 pal_len = 0;
		int32 first = 1;
		//int32 k;

		uint8 png_sig[8] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };

		for (int8 i = 0; i < 8; ++i)
		{
			int32 c = GetByte();
			if (c != png_sig[i]) 
			{
				TIL_ERROR_EXPLAIN("Bad PNG signature.");
				return false;
			}
		}

		for (;;first = 0)
		{
			chunk current = GetChunkHeader();

			if (first && current.type != PNG_TYPE('I','H','D','R'))
			{
				TIL_ERROR_EXPLAIN("Could not find IHDR tag.");
				return NULL;
			}

			switch (current.type) 
			{

			case PNG_TYPE('I','H','D','R'): 
				{
					PNG_DEBUG("Found tag 'IHDR'");

					int32 depth, color, interlace, compression, filter;

					if (!first)
					{
						TIL_ERROR_EXPLAIN("Multiple IHDR tags.");
						return NULL;
					}

					if (current.length != 13) 
					{
						TIL_ERROR_EXPLAIN("Bad IHDR length.");
						return NULL;
					}

					m_Width = GetDWord(); 
					if (m_Width > (1 << 24)) 
					{
						TIL_ERROR_EXPLAIN("Very large width (%i), possibly corrupted image.", m_Width);
						return NULL;
					}
					else if (m_Width == 0) 
					{
						TIL_ERROR_EXPLAIN("Zero-length width.");
						return NULL;
					}

					m_Height = GetDWord(); 
					if (m_Height > (1 << 24)) 
					{
						TIL_ERROR_EXPLAIN("Very large height (%i), possibly corrupted image.", m_Height);
						return NULL;
					}
					else if (m_Height == 0)
					{
						TIL_ERROR_EXPLAIN("Zero-length height.");
						return NULL;
					}

					PNG_DEBUG("Width: %i", m_Width);
					PNG_DEBUG("Height: %i", m_Height);

					depth = GetByte(); 
					if (depth != 8)
					{
						TIL_ERROR_EXPLAIN("Only 8-bit images are supported, received: %i.", depth);
						return NULL;
					}

					color = GetByte();  
					if (color > 6)
					{
						TIL_ERROR_EXPLAIN("Unsupported colortype. (%i)", color);
						return NULL;
					}
					else if (color == 3) 
					{
						pal_img_n = 3; 
					}
					else if (color & 1) 
					{
						TIL_ERROR_EXPLAIN("Unsupported colortype. (%i)", color);
						return NULL;
					}

					int channels = 0;
					m_ColorType = color;

					switch (color)
					{

					case PNG_COLOR_TYPE_GRAY:
						PNG_DEBUG("Color type: grayscale");

					case PNG_COLOR_TYPE_PALETTE:
						PNG_DEBUG("Color type: paletted");
						channels = 1;
						break;

					case PNG_COLOR_TYPE_RGB:
						PNG_DEBUG("Color type: RGB");
						channels = 3;
						break;

					case PNG_COLOR_TYPE_GRAY_ALPHA:
						PNG_DEBUG("Color type: grayscale with alpha");
						channels = 2;
						break;

					case PNG_COLOR_TYPE_RGB_ALPHA:
						PNG_DEBUG("Color type: RGB with alpha");
						channels = 4;
						break;

					}

					PNG_DEBUG("Color depth: %i", color * channels);

					compression = GetByte();
					if (compression) 
					{
						TIL_ERROR_EXPLAIN("Bad compression method. (%i)", compression);
						return NULL;
					}

					filter = GetByte();  
					if (filter) 
					{
						TIL_ERROR_EXPLAIN("Bad filter method (%i)", filter);
						return NULL;
					}

					interlace = GetByte(); 
					if (interlace) 
					{
						TIL_ERROR_EXPLAIN("Interlaced mode not supported.");
						return NULL;
					}

					if (!pal_img_n) 
					{
						img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);
						if ((1 << 30) / (m_Width * img_n) < m_Height) 
						{
							TIL_ERROR_EXPLAIN("Image too large to decode.");
							return NULL;
						}
					}
					else 
					{
						// if paletted, then pal_n is our final components, and
						// img_n is # components to decompress/filter.
						img_n = 1;
						if ((1 << 30) / m_Width / 4 < m_Height) 
						{
							TIL_ERROR_EXPLAIN("Too much data.");
							return NULL;
						}
						// if SCAN_header, have to scan to see if we have a tRNS
					}

					break;
				}

			case PNG_TYPE('P','L','T','E'):  
				{
					PNG_DEBUG("Found tag 'PLTE'");

					if (current.length > 256 * 3) 
					{
						TIL_ERROR_EXPLAIN("Invalid PLTE, chunk is too long: %i.", current.length);
						return NULL;
					}

					pal_len = current.length / 3;
					if (pal_len * 3 != current.length) 
					{
						TIL_ERROR_EXPLAIN("Invalid PLTE.");
						return NULL;
					}

					for (i = 0; i < pal_len; ++i) 
					{
						palette[i * 4 + 0] = (uint8)GetByte();
						palette[i * 4 + 1] = (uint8)GetByte();
						palette[i * 4 + 2] = (uint8)GetByte();
						palette[i * 4 + 3] = 255;
					}

					break;
				}

			case PNG_TYPE('t','R','N','S'): 
				{
					PNG_DEBUG("Found tag 'tRNS'");

					if (idata) 
					{
						TIL_ERROR_EXPLAIN("tRNS after IDAT.");
						return NULL;
					}
					if (pal_img_n) 
					{
						if (pal_len == 0) 
						{
							TIL_ERROR_EXPLAIN("tRNS before PLTE.");
							return NULL;
						}
						if (current.length > pal_len) 
						{
							TIL_ERROR_EXPLAIN("Chunk length is greater than specified. (%i > %i)", current.length, pal_len);
							return NULL;
						}

						pal_img_n = 4;
						for (uint32 i = 0; i < current.length; ++i)
						{
							palette[i * 4 + 3] = (uint8)GetByte();
						}
					} 
					else 
					{
						if (!(img_n & 1)) 
						{
							TIL_ERROR_EXPLAIN("tRNS should contain alpha channel.");
							return NULL;
						}

						if (current.length != (uint32)img_n * 2) 
						{
							TIL_ERROR_EXPLAIN("Chunk length does not equal specified length. (%i != %i)", current.length, (uint32)img_n);
							return NULL;
						}

						has_trans = 1;
						for (int32 k = 0; k < img_n; ++k)
						{
							tc[k] = (uint8)GetWord(); // non 8-bit images will be larger
						}
					}
					break;
				}

			case PNG_TYPE('I','D','A','T'): 
				{
					PNG_DEBUG("Found tag 'IDAT'");

					if (pal_img_n && !pal_len) 
					{
						TIL_ERROR_EXPLAIN("IDAT tag before PLTE tag.");
						return NULL;
					}

					if (ioff + current.length > idata_limit) 
					{
						uint8* p;
						if (idata_limit == 0) { idata_limit = current.length > 4096 ? current.length : 4096; }
						while (ioff + current.length > idata_limit)
						{
							idata_limit *= 2;
						}
						p = (uint8*)realloc(idata, idata_limit); 
						if (p == NULL)
						{
							TIL_ERROR_EXPLAIN("Out of memory.");
							return NULL;
						}
						idata = p;
					}

					if (m_Handle)
					{
						if (fread(idata + ioff, 1, current.length, m_Handle) != current.length) 
						{	
							TIL_ERROR_EXPLAIN("Not enough data.");
							return NULL;
						}
					}

					ioff += current.length;
					break;
				}

			case PNG_TYPE('I','E','N','D'): 
				{	
					PNG_DEBUG("Found tag 'IEND'");

					if (idata == NULL) 
					{
						TIL_ERROR_EXPLAIN("No IDAT tag.");
						return NULL;
					}

					ZLibDecode();

					m_RawLength = (uint32)(m_ZBuffer.zout - m_ZBuffer.zout_start);
					unsigned char* target = (unsigned char*)m_ZBuffer.zout_start;

					//int compare = img_n + (((pal_img_n || req_comp != img_n + 1 || req_comp == 3) && !has_trans) ? 0 : 1);

					/*if ((req_comp == img_n + 1 && req_comp != 3 && !pal_img_n) || has_trans)
					{
						img_out_n = img_n + 1;
					}
					else
					{
						img_out_n = img_n;
					}*/

					img_out_n = 4;

					// create png image

					//uint32 stride = m_Width * 4;
					m_Pitch = m_Width * 4;

					int img_copy_n = img_n; // copy it into a local for later

					if (img_out_n != img_n && img_out_n != img_n + 1)
					{
						TIL_ERROR_EXPLAIN("Wrong bitdepth.");
						return NULL;
					}

					out = new uint8[m_Width * m_Height * img_out_n];
					if (!out) 
					{
						TIL_ERROR_EXPLAIN("Out of memory.");
						return NULL;
					}

					if (m_RawLength != (img_copy_n * m_Width + 1) * m_Height) 
					{
						TIL_ERROR_EXPLAIN("Not enough pixels. (%i vs %i)", m_RawLength, (img_copy_n * m_Width + 1) * m_Height);
						return NULL;
					}

					for (uint32 j = 0; j < m_Height; ++j) 
					{
						uint8* cur = out + m_Pitch * j;
						uint8* prior = cur - m_Pitch;

						int filter = *target++;
						if (filter > 4) 
						{
							TIL_ERROR_EXPLAIN("Invalid filter.");
							return NULL;								
						}

						// if first row, use special filter that doesn't sample previous row
						if (j == 0) 
						{
							filter = first_row_filter[filter];
						}

						// handle first pixel explicitly
						for (int k = 0; k < img_n; ++k) 
						{
							switch (filter) 
							{

							case F_none: 
								{
									cur[k] = target[k];
									break;
								}
							case F_sub: 
								{
									cur[k] = target[k];
									break;
								}
							case F_up: 
								{
									cur[k] = target[k] + prior[k]; 
									break;
								}
							case F_avg: 
								{
									cur[k] = target[k] + (prior[k] >> 1);
									break;
								}
							case F_paeth: 
								{
									cur[k] = (uint8)(target[k] + paeth(0, prior[k], 0)); 
									break;
								}
							case F_avg_first: 
								{
									cur[k] = target[k]; 
									break;
								}
							case F_paeth_first: 
								{
									cur[k] = target[k];
									break;
								}

							}
						}

						if (img_n != img_out_n) 
						{
							cur[img_n] = 255;
						}
						target += img_n;
						cur += img_out_n;
						prior += img_out_n;

						// this is a little gross, so that we don't switch per-pixel or per-component
						if (img_n == img_out_n) 
						{

							#define CASE(f) \
								case f: \
									for (int i = m_Width - 1; i >= 1; --i, target += img_n, cur += img_n, prior += img_n) \
										for (int k = 0; k < img_n; ++k)

							switch(filter)
							{
								CASE(F_none)         cur[k] = target[k]; break;
								CASE(F_sub)          cur[k] = target[k] + cur[k-img_n]; break;
								CASE(F_up)           cur[k] = target[k] + prior[k]; break;
								CASE(F_avg)          cur[k] = target[k] + ((prior[k] + cur[k-img_n])>>1); break;
								CASE(F_paeth)        cur[k] = (uint8) (target[k] + paeth(cur[k-img_n],prior[k],prior[k-img_n])); break;
								CASE(F_avg_first)    cur[k] = target[k] + (cur[k-img_n] >> 1); break;
								CASE(F_paeth_first)  cur[k] = (uint8) (target[k] + paeth(cur[k-img_n],0,0)); break;
							}

							#undef CASE
						} 
						else 
						{
							//assert(img_n+1 == out_n);

							#define CASE(f) \
								case f: \
									for (int i = m_Width - 1; i >= 1; --i, cur[img_n] = 255, target += img_n, cur += img_out_n, prior += img_out_n) \
										for (int k = 0; k < img_n; ++k)

							switch(filter) 
							{
								CASE(F_none)         cur[k] = target[k]; break;
								CASE(F_sub)          cur[k] = target[k] + cur[k-img_out_n]; break;
								CASE(F_up)           cur[k] = target[k] + prior[k]; break;
								CASE(F_avg)          cur[k] = target[k] + ((prior[k] + cur[k-img_out_n])>>1); break;
								CASE(F_paeth)        cur[k] = (uint8) (target[k] + paeth(cur[k-img_out_n],prior[k],prior[k-img_out_n])); break;
								CASE(F_avg_first)    cur[k] = target[k] + (cur[k-img_out_n] >> 1); break;
								CASE(F_paeth_first)  cur[k] = (uint8) (target[k] + paeth(cur[k-img_out_n],0,0)); break;
							}

							#undef CASE
						}
					}

					if (has_trans)
					{
						TIL_ERROR_EXPLAIN("TODO: Compute transparancy.");

						/*if (!compute_transparency(z, tc, s->img_out_n)) 
						{
						return 0;
						}*/
					}

					if (pal_img_n) 
					{
						// pal_img_n == 3 or 4
						img_n = pal_img_n; // record the actual colors we had
						img_out_n = pal_img_n;
						if (req_comp >= 3) 
						{
							img_out_n = req_comp;
						}

						TIL_ERROR_EXPLAIN("TODO: Expand palette.");

						/*if (!expand_palette(z, palette, pal_len, img_out_n))
						{
							return 0;
						}*/
					}

					// convert to correct format

					m_Pixels = new byte[m_Width * m_Height * m_BPP];

					uint8* src = out;

					switch (m_BPPIdent)
					{

					case BPP_32B_R8G8B8:
						{
							uint32* dst = (uint32*)m_Pixels;

							for (uint32 i = 0; i < m_Width * m_Height; i++)
							{
								*dst = Construct_32b_R8G8B8(src[0], src[1], src[2]);

								dst++;
								src += 4;
							}

							break;
						}

					case BPP_32B_A8R8G8B8:
						{
							uint32* dst = (uint32*)m_Pixels;

							for (uint32 i = 0; i < m_Width * m_Height; i++)
							{
								*dst = 0xFF000000 | Construct_32b_R8G8B8(src[0], src[1], src[2]);

								dst++;
								src += 4;
							}

							break;
						}

					case BPP_32B_R8G8B8A8:
						{
							uint32* dst = (uint32*)m_Pixels;

							for (uint32 i = 0; i < m_Width * m_Height; i++)
							{
								*dst = (Construct_32b_R8G8B8(src[0], src[1], src[2]) << 4) | 0x000000FF;

								dst++;
								src += 4;
							}

							break;
						}

					case BPP_16B_R5G6B5:
						{
							uint16* dst = (uint16*)m_Pixels;

							for (uint32 i = 0; i < m_Width * m_Height; i++)
							{
								*dst = Construct_16b_R5G6B5(src[0], src[1], src[2]);

								dst++;
								src += 4;
							}

							break;
						}

					}
					

					return true;
				}

			default:
				{
					TIL_ERROR_EXPLAIN("Unknown chunk type. (%s)", current.header);

					// if critical, fail
					if ((current.type & (1 << 29)) == 0) 
					{	
						return NULL;
					}

					Skip(current.length);

					break;
				}
			}

			// skip CRC
			GetDWord();
		}
	}

}; // namespace til

#endif