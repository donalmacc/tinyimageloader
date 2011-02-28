/*
	TinyImageLoader - load images, just like that

	Copyright (C) 2010 - 2011 by Quinten Lansu
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

#include "..\SDK\headers\TILImageDDS.h"

#if (TIL_FORMAT & TIL_FORMAT_DDS)

#ifdef TIL_PRINT_DEBUG
	#define DDS_DEBUG(msg, ...)        TIL_PRINT_DEBUG("DDS: "msg, __VA_ARGS__)
#endif

#include <math.h>

namespace til
{



	#define DDSD_CAPS          0x00000001
	#define DDSD_HEIGHT        0x00000002
	#define DDSD_WIDTH         0x00000004
	#define DDSD_PITCH         0x00000008
	#define DDSD_PIXELFORMAT   0x00001000
	#define DDSD_MIPMAPCOUNT   0x00020000
	#define DDSD_LINEARSIZE    0x00080000
	#define DDSD_DEPTH         0x00800000

	#define DDSCAPS_COMPLEX    0x00000008
	#define DDSCAPS_TEXTURE    0x00001000
	#define DDSCAPS_MIPMAP     0x00400000
	#define DDSCAPS2_CUBEMAP   0x00000200
	#define DDSCAPS2_VOLUME    0x00200000

	struct DDPixelFormat
	{
		int size;
		int flags;
		int fourCC;
		int bpp;
		int redMask;
		int greenMask;
		int blueMask;
		int alphaMask;
	};

	struct DDSCaps
	{
		int caps;
		int caps2;
		int caps3;
		int caps4;
	};

	struct DDColorKey
	{
		int lowVal;
		int highVal;
	};

	struct DDSurfaceDesc
	{
		int size;
		int flags;
		int height;
		int width;
		int pitch;
		int depth;
		int mipMapLevels;
		int alphaBitDepth;
		int reserved;
		int surface;

		DDColorKey ckDestOverlay;
		DDColorKey ckDestBlt;
		DDColorKey ckSrcOverlay;
		DDColorKey ckSrcBlt;

		DDPixelFormat format;
		DDSCaps caps;

		int textureStage;
	};

	struct DataDXT1
	{
		byte c0_lo, c0_hi, c1_lo, c1_hi;
		byte bits_0, bits_1, bits_2, bits_3;
	};

#define GETCODE(x, y, data) (data & ((2 * (4 * y + x)) + 1))

	typedef void (*ColorFuncDDS_DXT1)(byte* a_DstColors, color_16b& a_Color0, color_16b& a_Color1);

	void ColorFuncDDS_DXT1_A8R8G8B8(byte* a_DstColors, color_16b& a_Color0, color_16b& a_Color1)
	{
		color_32b* colors = (color_32b*)a_DstColors;

		colors[0] = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(a_Color0);
		colors[1] = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(a_Color1);
		colors[2] = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(Blend_16b_R5G6B5(a_Color0, a_Color1, 0xAA));
		colors[3] = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(Blend_16b_R5G6B5(a_Color0, a_Color1, 0x55));
		colors[4] = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(Blend_16b_R5G6B5(a_Color0, a_Color1, 0x80));
		colors[5] = 0;
	}

	void Decompress_DXT1_A8R8G8B8(byte* a_Dst, byte* a_Src, uint32 a_Blocks, uint32 a_Width)
	{
		color_32b* write = (color_32b*)a_Dst;
		color_32b* dst_x = write;

		uint32 pitch = a_Width * 4;
		uint32 pitch_blocks = a_Width / 4;
		uint32 curr = 0;

		DataDXT1* src = (DataDXT1*)a_Src;

		/*
			RGB0,              if color0 > color1 and code(x,y) == 0
			RGB1,              if color0 > color1 and code(x,y) == 1
			(2*RGB0+RGB1)/3,   if color0 > color1 and code(x,y) == 2
			(RGB0+2*RGB1)/3,   if color0 > color1 and code(x,y) == 3
		*/

		int glob_x = 0;
		int glob_y = 0;
		int pos_x = 0;
		int pos_y = 0;

		for (int i = 0; i < a_Blocks; i++)
		{
			color_16b color0 = (src->c0_lo + (src->c0_hi * 256));
			color_16b color1 = (src->c1_lo + (src->c1_hi * 256));
			dword bits = src->bits_0 + (256 * (src->bits_1 + 256 * (src->bits_2 + 256 * src->bits_3)));

			color_32b color_dst0 = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(color0);
			color_32b color_dst1 = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(color1);
			color_32b color_dst2 = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(Blend_16b_R5G6B5(color0, color1, 0x55));
			color_32b color_dst3 = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(Blend_16b_R5G6B5(color0, color1, 0xAA)); 
			color_32b color_dst4 = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(Blend_16b_R5G6B5(color0, color1, 0x80));

			if (color0 > color1)
			{
				pos_y = glob_y;

				for (int y = 0; y < 4; y++)
				{
					pos_x = glob_x;

					for (int x = 0; x < 4; x++)
					{
						bool bit0 = (bits & (1 << ((2 * (4 * y + x)) + 0))) ? true : false;
						bool bit1 = (bits & (1 << ((2 * (4 * y + x)) + 1))) ? true : false;

						color_32b* dst = &write[pos_x + (pos_y * a_Width)];

						if (bit0) // 1 or 3
						{
							if (bit1) // 3
							{
								*dst = color_dst3;
							}
							else // 1
							{
								*dst = color_dst1;	
							}
						}
						else // 0 or 2
						{
							if (bit1) // 2
							{
								*dst = color_dst2;
							}
							else // 0
							{
								*dst = color_dst0;
							}
						}

						pos_x++;
					}

					pos_y++;
				}
			}
			else
			{
				/*
					RGB0,              if color0 <= color1 and code(x,y) == 0
					RGB1,              if color0 <= color1 and code(x,y) == 1
					(RGB0+RGB1)/2,     if color0 <= color1 and code(x,y) == 2
					BLACK,             if color0 <= color1 and code(x,y) == 3
				*/

				pos_y = glob_y;

				for (int y = 0; y < 4; y++)
				{
					pos_x = glob_x;

					for (int x = 0; x < 4; x++)
					{
						bool bit0 = (bits & (1 << ((2 * (4 * y + x)) + 0))) ? true : false;
						bool bit1 = (bits & (1 << ((2 * (4 * y + x)) + 1))) ? true : false;

						color_32b* dst = &write[pos_x + (pos_y * a_Width)];

						if (bit0) // 1 or 3
						{
							if (bit1) // 3
							{
								*dst = 0;
							}
							else // 1
							{
								*dst = color_dst1;	
							}
						}
						else // 0 or 2
						{
							if (bit1) // 2
							{
								*dst = color_dst4;
							}
							else // 0
							{
								*dst = color_dst0;
							}
						}

						pos_x++;
					}

					pos_y++;
				}
			}

			glob_x += 4;
			//dst_x += 4;
			if (++curr == pitch_blocks)
			{
				//write += pitch * 4;
				//dst_x = write;
				curr = 0;

				glob_y += 4;
				glob_x = 0;
			}

			src++;
		}
	}

	//ColorFuncDDS g_ColorFunc;

	ImageDDS::ImageDDS() : Image()
	{
		m_Data = NULL;
		m_Pixels = NULL;
		m_Colors = NULL;
	}

	ImageDDS::~ImageDDS()
	{
		if (m_Data) { delete m_Data; }
		if (m_Pixels) { delete m_Pixels; }
		if (m_Colors) { delete m_Colors; }
	}

	bool ImageDDS::Parse(uint32 a_ColorDepth)
	{
		dword header;
		//fread(&header, sizeof(dword), 1, m_Handle);
		m_Stream->ReadDWord(&header);

		if (header != DDS_FOURCC_DDS)
		{
			TIL_ERROR_EXPLAIN("%s is not a DDS file or header is invalid.", m_FileName);
			return false;
		}

		DDSurfaceDesc ddsd;
		//in.read(reinterpret_cast<char*>(&ddsd), sizeof ddsd);
		//fread(&ddsd, 1, sizeof(DDSurfaceDesc), m_Handle);
		m_Stream->Read(&ddsd, sizeof(DDSurfaceDesc));

		m_Format = 0;
		m_InternalBPP = 0;
		m_InternalDepth = 0;

		if (ddsd.format.fourCC != 0)
		{
			m_Format = ddsd.format.fourCC;

			m_BlockSize = 0;
			switch (m_Format)
			{

			case DDS_FOURCC_DXT1:
				{
					DDS_DEBUG("Format: DXT1");
					m_BlockSize = 8;
					break;
				}

			case DDS_FOURCC_ATI1:
				{
					DDS_DEBUG("Format: ATI1");
					m_BlockSize = 8;
					break;
				}
				
			case DDS_FOURCC_DXT2:
				{
					DDS_DEBUG("Format: DXT2");
					m_BlockSize = 16;
					break;
				}
			case DDS_FOURCC_DXT3:
				{
					DDS_DEBUG("Format: DXT3");
					m_BlockSize = 16;
					break;
				}
			case DDS_FOURCC_DXT4:
				{
					DDS_DEBUG("Format: DXT4");
					m_BlockSize = 16;
					break;
				}
			case DDS_FOURCC_DXT5:
				{
					DDS_DEBUG("Format: DXT5");
					m_BlockSize = 16;
					break;
				}
			case DDS_FOURCC_RXGB:
				{
					DDS_DEBUG("Format: RXGB");
					m_BlockSize = 16;
					break;
				}
			case DDS_FOURCC_ATI2:
				{
					DDS_DEBUG("Format: ATI2");
					m_BlockSize = 16;
					break;
				}
			default:
				{
					TIL_ERROR_EXPLAIN("Unknown FourCC in DDS file: %d", ddsd.format.fourCC);
					return false;
				}
				
			}
		}
		else
		{
			if (ddsd.format.bpp == 32)
			{
				if (ddsd.format.redMask == 0x00ff0000)
				{
					m_InternalDepth = TIL_DEPTH_A8R8G8B8;
				}
				else
				{
					m_InternalDepth = TIL_DEPTH_A8B8G8R8;
				}
				m_InternalBPP = 4;
			}
			else if (ddsd.format.bpp == 24)
			{
				if (ddsd.format.redMask == 0x00ff0000)
				{
					m_InternalDepth = TIL_DEPTH_R8G8B8;
				}
				else
				{
					m_InternalDepth = TIL_DEPTH_B8G8R8;
				}
				m_InternalBPP = 3;
			}
			else
			{
				TIL_ERROR_EXPLAIN("Unknown bit-depth: %d", ddsd.format.bpp);
				return false;
			}
		}

		m_Width = ddsd.width;
		m_Height = ddsd.height;
		m_Depth = ddsd.depth;
		m_MipMaps = (ddsd.mipMapLevels >= 1 ? ddsd.mipMapLevels : 1) - 1;

		DDS_DEBUG("Dimensions: (%d, %d)", m_Width, m_Height);
		DDS_DEBUG("Depth: %d", m_Depth);
		DDS_DEBUG("Mipmaps: %d", m_MipMaps);

		m_CubeMap = false;
		if ((ddsd.caps.caps2 & DDSCAPS2_CUBEMAP) == DDSCAPS2_CUBEMAP)
		{
			DDS_DEBUG("Cubemap");
			m_CubeMap = true;
		}

		ReadData();

		m_Colors = new byte[m_BPP * 6];

		m_Pixels = new byte[m_Width * m_Height * m_BPP];

		if (m_Format == DDS_FOURCC_DXT1)
		{
			DecompressDXT1();
		}
		else
		{
			TIL_ERROR_EXPLAIN("Unknown compression algorithm: %d", m_Format);
		}

		return true;
	}

	uint32 ImageDDS::GetFrameCount()
	{
		return 1;
	}

	byte* ImageDDS::GetPixels(uint32 a_Frame /*= 0*/)
	{
		return m_Pixels;
	}

	uint32 ImageDDS::GetWidth(uint32 a_Frame /*= 0*/)
	{
		return m_Width;
	}

	uint32 ImageDDS::GetHeight(uint32 a_Frame /*= 0*/)
	{
		return m_Height;
	}

	void ImageDDS::ReadData()
	{
		int powres = 1 << m_MipMaps;
		int nW = m_Width / powres;
		int nH = m_Height / powres;
		int nD = m_Depth / powres;

		m_Blocks = 0;

		// 4x4 blocks, 8 bytes per block
		if (m_Format == DDS_FOURCC_DXT1)
		{
			//m_Size = (((nW + 3) / 4) * ((nH + 3) / 4) * 8);
			m_Blocks = (m_Width / 4) * (m_Height / 4);
		}
		// 4x4 blocks, 16 bytes per block
		else if (m_Format == DDS_FOURCC_DXT5 || m_Format == DDS_FOURCC_DXT3)
		{
			//m_Size = (((nW + 3) / 4) * ((nH + 3) / 4) * 16);
			m_Blocks = (m_Width / 4) * (m_Height / 4);
		}
		else if (m_Depth > 0)
		{
			// Width   Height   Depth   Width*Height*Depth     Width*Height*Depth
			// ----- * ------ * ----- = ------------         = ------------
			// 2^i     2^i		2^i     2^i*2^i*2^i		       2^(i+i+i)

			m_Size = (((m_Width * m_Height * m_Depth) / (powres * powres * powres)) * m_InternalBPP);
		}
		// No 3d image loaded, so mipmapping is different.
		else 
		{
			m_Size = (((m_Width * m_Height) / (powres * powres)) * m_InternalBPP);
		}
		m_Size = m_Blocks * m_BlockSize;

		if (m_CubeMap)
		{
			m_Size *= 6;
		}

		m_Data = new byte[m_Size];
		//fread(m_Data, m_Size, 1, m_Handle);
		m_Stream->ReadByte(m_Data, m_Size);
	}

	void ImageDDS::DecompressDXT1()
	{
		color_32b* write = (color_32b*)m_Pixels;
		color_32b* dst_x = write;

		uint32 pitch = m_Width * 4;
		uint32 pitch_blocks = m_Width / 4;
		uint32 curr = 0;

		DataDXT1* src = (DataDXT1*)m_Data;

		/*
			RGB0,              if color0 > color1 and code(x,y) == 0
			RGB1,              if color0 > color1 and code(x,y) == 1
			(2*RGB0+RGB1)/3,   if color0 > color1 and code(x,y) == 2
			(RGB0+2*RGB1)/3,   if color0 > color1 and code(x,y) == 3
		*/

		int glob_x = 0;
		int glob_y = 0;
		int pos_x = 0;
		int pos_y = 0;

		for (int i = 0; i < m_Blocks; i++)
		{
			color_16b color0 = (src->c0_lo + (src->c0_hi * 256));
			color_16b color1 = (src->c1_lo + (src->c1_hi * 256));
			dword bits = src->bits_0 + (256 * (src->bits_1 + 256 * (src->bits_2 + 256 * src->bits_3)));

			/*color_32b color_dst0 = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(color0);
			color_32b color_dst1 = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(color1);
			color_32b color_dst2 = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(Blend_16b_R5G6B5(color0, color1, 0x55));
			color_32b color_dst3 = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(Blend_16b_R5G6B5(color0, color1, 0xAA)); 
			color_32b color_dst4 = Convert_From_16b_B5G6R5_To_32b_A8R8G8B8(Blend_16b_R5G6B5(color0, color1, 0x80));*/

			ColorFuncDDS_DXT1_A8R8G8B8(m_Colors, color0, color1);
			color_32b* colors = (color_32b*)m_Colors;

			if (color0 > color1)
			{
				pos_y = glob_y;

				for (int y = 0; y < 4; y++)
				{
					pos_x = glob_x;

					for (int x = 0; x < 4; x++)
					{
						//int enabled = (bits & (1 << ((2 * (4 * y + x)) + 0))) + (bits & (1 << ((2 * (4 * y + x)) + 1)));
						int curr = (2 * (4 * y + x));
						int enabled = 
							((bits & (1 << (curr + 0))) >> (curr + 0)) +
							((bits & (1 << (curr + 1))) >> (curr + 0));

						color_32b* dst = &write[pos_x + (pos_y * m_Width)];

						*dst = colors[enabled];

						/*bool bit0 = (bits & (1 << ((2 * (4 * y + x)) + 0))) ? true : false;
						bool bit1 = (bits & (1 << ((2 * (4 * y + x)) + 1))) ? true : false;

						color_32b* dst = &write[pos_x + (pos_y * m_Width)];

						if (bit0) // 1 or 3
						{
							if (bit1) // 3
							{
								*dst = colors[3];
							}
							else // 1
							{
								*dst = colors[1];	
							}
						}
						else // 0 or 2
						{
							if (bit1) // 2
							{
								*dst = colors[2];
							}
							else // 0
							{
								*dst = colors[0];
							}
						}*/

						pos_x++;
					}

					pos_y++;
				}
			}
			else
			{
				/*
					RGB0,              if color0 <= color1 and code(x,y) == 0
					RGB1,              if color0 <= color1 and code(x,y) == 1
					(RGB0+RGB1)/2,     if color0 <= color1 and code(x,y) == 2
					BLACK,             if color0 <= color1 and code(x,y) == 3
				*/

				pos_y = glob_y;

				for (int y = 0; y < 4; y++)
				{
					pos_x = glob_x;

					for (int x = 0; x < 4; x++)
					{
						int curr = (2 * (4 * y + x));
						int enabled = 
							((bits & (1 << (curr + 0))) >> (curr + 0)) +
							((bits & (1 << (curr + 1))) >> (curr - 1));
							
						color_32b* dst = &write[pos_x + (pos_y * m_Width)];
						*dst = colors[enabled];

						/*bool bit0 = (bits & (1 << ((2 * (4 * y + x)) + 0))) ? true : false;
						bool bit1 = (bits & (1 << ((2 * (4 * y + x)) + 1))) ? true : false;

						color_32b* dst = &write[pos_x + (pos_y * m_Width)];

						if (bit0) // 1 or 3
						{
							if (bit1) // 3
							{
								*dst = colors[5];
							}
							else // 1
							{
								*dst = colors[1];	
							}
						}
						else // 0 or 2
						{
							if (bit1) // 2
							{
								*dst = colors[4];
							}
							else // 0
							{
								*dst = colors[0];
							}
						}*/

						pos_x++;
					}

					pos_y++;
				}
			}

			glob_x += 4;
			if (++curr == pitch_blocks)
			{
				curr = 0;

				glob_y += 4;
				glob_x = 0;
			}

			src++;
		}
	}

}; // namespace til

#endif