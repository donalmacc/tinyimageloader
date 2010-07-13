#include "..\SDK\headers\TILImageTGA.h"

#define TGA_DEBUG(msg, ...)            printf(msg"\n", __VA_ARGS__)

namespace til
{

	ImageTGA::ImageTGA()
	{

	}

	ImageTGA::~ImageTGA()
	{

	}

	bool ImageTGA::CompileUncompressed()
	{
		// silly TGA, why do you store your pixels in reverse y?
		// now i can't just memcpy it :(

		if (m_BPP == 4)
		{
			if (m_BPPIdent == BPP_32B_A8R8G8B8)
			{
				for (uint32 y = 0; y < m_Height; y++)
				{
					byte* dst = m_Target;

					for (uint32 x = 0; x < m_Width; x++)
					{
						fread(m_Src, 1, 4, m_Handle);

						color_32b* write = (color_32b*)dst;
						*write = Construct_32b_A8R8G8B8(m_Src[0], m_Src[1], m_Src[2], m_Src[3]);

						dst += m_BPP;
					}

					m_Target -= m_Pitch;
				}
			}
			else if (m_BPPIdent == BPP_32B_R8G8B8)
			{
				for (uint32 y = 0; y < m_Height; y++)
				{
					byte* dst = m_Target;

					for (uint32 x = 0; x < m_Width; x++)
					{
						fread(m_Src, 1, 4, m_Handle);

						color_32b* write = (color_32b*)dst;
						*write = Construct_32b_R8G8B8(m_Src[0], m_Src[1], m_Src[2]);

						dst += m_BPP;
					}

					m_Target -= m_Pitch;
				}
			}
		}
		else if (m_BPP == 2)
		{
			for (uint32 y = 0; y < m_Height; y++)
			{
				byte* dst = m_Target;

				for (uint32 x = 0; x < m_Width; x++)
				{
					fread(m_Src, 1, 4, m_Handle);

					color_16b* write = (color_16b*)dst;
					*write = Construct_16b_R5G6B5(m_Src[0], m_Src[1], m_Src[2]);

					dst += m_BPP;
				}

				m_Target -= m_Pitch;
			}
		}

		return true;
	}

	bool ImageTGA::CompileRunLengthEncoded()
	{
		byte buffer[128 * 4];
		byte src_buffer[4];
		byte* src;

		uint32 total = m_Width * m_Height;

		for (uint32 y = 0; y < m_Height; y++)
		{
			color_16b* write_16b = (color_16b*)m_Target;
			color_32b* write_32b = (color_32b*)m_Target;

			for (uint32 x = 0; x < m_Width;)
			{
				byte packet;
				fread(&packet, 1, 1, m_Handle);

				uint8 count = (packet & 0x7F) + 1;
				int repeat = 1;
				int unique = 1;

				// run length packet
				if (packet & 0x80)
				{
					fread(src_buffer, 1, 4, m_Handle);
					src = src_buffer;

					repeat = count;
				}
				// raw packet
				else
				{
					fread(buffer, 1, count * 4, m_Handle);
					src = buffer;

					unique = count;
				}

				if (m_BPP == 4)
				{
					color_32b final;
					for (int j = 0; j < unique; j++)
					{
						final = Construct_32b_R8G8B8(src[2], src[1], src[0]);

						for (uint8 i = 0; i < repeat; i++) { *write_32b++ = final; }

						src += 4;
					}
				}
				else if (m_BPP == 2)
				{
					color_16b final;
					for (int j = 0; j < unique; j++)
					{
						final = Construct_16b_R5G6B5(src[2], src[1], src[0]);

						for (uint8 i = 0; i < repeat; i++) { *write_16b++ = final; }

						src += 4;
					}
				}

				x += count;
			}

			m_Target -= m_Pitch;
		}

		bool bleh = true;

		return true;
	}

	bool ImageTGA::Parse(uint32 a_ColorDepth)
	{
		byte id;              fread(&id, 1, 1, m_Handle);
		byte colormap;        fread(&colormap, 1, 1, m_Handle);
		byte compression;     fread(&compression, 1, 1, m_Handle);

		switch (compression)
		{
		case 0:
			TIL_ERROR_EXPLAIN("No image data is present.");
			return false;

		case 1:
			TGA_DEBUG("Uncompressed color mapped image.");
			m_Type = COLOR_MAPPED;
			m_Comp = COMP_NONE;
			break;

		case 2:
			TGA_DEBUG("Uncompressed true color image.");
			m_Type = COLOR_TRUECOLOR;
			m_Comp = COMP_NONE;
			break;

		case 3:
			TGA_DEBUG("Uncompressed black and white image.");
			m_Type = COLOR_BLACKANDWHITE;
			m_Comp = COMP_NONE;
			break;

		case 9:
			TGA_DEBUG("Run-length encoded color mapped image.");
			m_Type = COLOR_MAPPED;
			m_Comp = COMP_RLE;
			break;

		case 10:
			TGA_DEBUG("Run-length encoded true color image.");
			m_Type = COLOR_TRUECOLOR;
			m_Comp = COMP_RLE;
			break;

		case 11:
			TGA_DEBUG("Run length encoded black and white image.");
			m_Type = COLOR_BLACKANDWHITE;
			m_Comp = COMP_RLE;
			break;

		default:
			TIL_ERROR_EXPLAIN("This isn't a TGA file!");
			return false;

		}

		word colormap_offset; fread(&colormap_offset, 2, 1, m_Handle);
		word colormap_length; fread(&colormap_length, 2, 1, m_Handle);
		byte colormap_bpp;    fread(&colormap_bpp, 1, 1, m_Handle);

		word origin_x;        fread(&origin_x, 2, 1, m_Handle);
		word origin_y;        fread(&origin_y, 2, 1, m_Handle);

		word width;           fread(&width, 2, 1, m_Handle);
		word height;          fread(&height, 2, 1, m_Handle);

		byte depth;           fread(&depth, 1, 1, m_Handle);

		byte img_descriptor;  fread(&img_descriptor, 1, 1, m_Handle);

		if (id > 0)
		{
			TGA_DEBUG("Hey maybe I should do something with the id field.");
			return false;
		}

		if (colormap > 0)
		{
			TGA_DEBUG("Hey maybe I should do something with the colormap field.");
			return false;
		}

		m_Width = (uint32)width;
		m_Height = (uint32)height;

		m_Pixels = new byte[width * height * m_BPP];
		m_Pitch = m_Width * m_BPP;
		m_Target = m_Pixels + ((m_Height - 1) * m_Pitch);

		if (m_Comp == COMP_RLE)
		{
			CompileRunLengthEncoded();	
		}
		else
		{
			CompileUncompressed();
		}
	
		return true;
	}

	uint32 ImageTGA::GetFrameCount()
	{
		return 1;
	}

	byte* ImageTGA::GetPixels( uint32 a_Frame /*= 0*/ )
	{
		return m_Pixels;
	}

}; // namespace til