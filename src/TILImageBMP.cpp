/*
    TinyImageLoader - load images, just like that
    Copyright (C) 2010 - 2011 Quinten Lansu (knight666)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "..\SDK\headers\TILImageBMP.h"

#if (TIL_FORMAT & TIL_FORMAT_BMP)

#ifdef TIL_DEBUG
	#define BMP_DEBUG(msg, ...)        TIL_PRINT_DEBUG("BMP: "msg, __VA_ARGS__)
#else
	#define BMP_DEBUG(msg, ...)
#endif

namespace til
{

	typedef void (*ColorFuncBMP)(uint8*, uint8*);

	void ColorFuncBMP_R8G8B8(uint8* a_Dst, uint8* a_Src)
	{
		color_32b* dst = (color_32b*)a_Dst;
		*dst = Construct_32b_R8G8B8(a_Src[0], a_Src[1], a_Src[2]);
	}

	void ColorFuncBMP_A8R8G8B8(uint8* a_Dst, uint8* a_Src)
	{
		color_32b* dst = (color_32b*)a_Dst;
		*dst = Construct_32b_A8R8G8B8(a_Src[0], a_Src[1], a_Src[2], a_Src[3]);
	}

	void ColorFuncBMP_A8B8G8R8(uint8* a_Dst, uint8* a_Src)
	{
		color_32b* dst = (color_32b*)a_Dst;
		*dst = Construct_32b_A8B8G8R8(a_Src[0], a_Src[1], a_Src[2], a_Src[3]);
	}

	void ColorFuncBMP_R8G8B8A8(uint8* a_Dst, uint8* a_Src)
	{
		color_32b* dst = (color_32b*)a_Dst;
		*dst = Construct_32b_R8G8B8A8(a_Src[0], a_Src[1], a_Src[2], a_Src[3]);
	}

	void ColorFuncBMP_B8G8R8A8(uint8* a_Dst, uint8* a_Src)
	{
		color_32b* dst = (color_32b*)a_Dst;
		*dst = Construct_32b_B8G8R8A8(a_Src[0], a_Src[1], a_Src[2], a_Src[3]);
	}

	void ColorFuncBMP_R5G6B5(uint8* a_Dst, uint8* a_Src)
	{
		color_16b* dst = (color_16b*)a_Dst;
		*dst = Construct_16b_R5G6B5(a_Src[0], a_Src[1], a_Src[2]);
	}

	ColorFuncBMP g_ColorFuncBMP = NULL;

	ImageBMP::ImageBMP()
	{

	}

	ImageBMP::~ImageBMP()
	{

	}

	void GetComponents(byte* a_Dst, byte* a_Src, word a_Size)
	{
		switch (a_Size)
		{

		case 3:
			{
				a_Dst[0] = a_Src[2];
				a_Dst[1] = a_Src[1];
				a_Dst[2] = a_Src[0];
				a_Dst[3] = 255;

				break;
			}

		case 2:
			{
				break;
			}

		case 1:
			{
				byte r = (((a_Src[0] & 0xD0) >> 5) * 0xFF) >> 3;
				byte g = (((a_Src[0] & 0x1C) >> 2) * 0xFF) >> 3;
				byte b = (((a_Src[0] & 0x03)     ) * 0xFF) >> 2;

				a_Dst[0] = b;
				a_Dst[1] = g;
				a_Dst[2] = r;
				a_Dst[3] = 255;

				break;
			}

		}
	}

	dword ImageBMP::GetDWord()
	{
		fread(m_Data, 1, 4, m_Handle);
		return (m_Data[3] << 24) | (m_Data[2] << 16) | (m_Data[1] << 8) | (m_Data[0]);
	}

	bool ImageBMP::Parse(uint32 a_ColorDepth)
	{
		byte header[3];         fread(header, 1, 2, m_Handle);
		dword size =            GetDWord();
		dword unused;           fread(&unused, 4, 1, m_Handle);
		dword pixel_offset =    GetDWord();

		dword header_size =     GetDWord();
		switch (header_size)
		{
		case HDR_OS2V1:
			BMP_DEBUG("Header: OS/2 V1");
			break;
		case HDR_OS2V2:
			BMP_DEBUG("Header: OS/2 V2");
			break;
		case HDR_WINDOWSV3:
			BMP_DEBUG("Header: Windows V3");
			break;
		case HDR_WINDOWSV4:
			BMP_DEBUG("Header: Windows V4");
			break;
		case HDR_WINDOWSV5:
			BMP_DEBUG("Header: Windows V5");
			break;
		}

		dword width =           GetDWord();
		m_Width =               (uint32)width;
		dword height =          GetDWord();
		m_Height =              (uint32)height;

		word color_planes;      fread(&color_planes, 2, 1, m_Handle);
		word bpp;               fread(&bpp, 2, 1, m_Handle);
		BMP_DEBUG("BPP: %i", bpp);
		word bytesperpixel = bpp >> 3;

		dword compresion =      GetDWord();
		switch (compresion)
		{

		case COMP_RGB:
			BMP_DEBUG("No compression.");
			break;
		case COMP_RLE8:
			BMP_DEBUG("Run length encoding with 8 bits per pixel.");
			break;
		case COMP_RLE4:
			BMP_DEBUG("Run length encoding with 4 bits per pixel.");
			break;
		case COMP_BITFIELDS:
			BMP_DEBUG("Bitfields.");
			break;
		case COMP_JPEG:
			BMP_DEBUG("Bitmap contains a JPEG image.");
			break;
		case COMP_PNG:
			BMP_DEBUG("Bitmap contains a PNG image.");
			break;

		}

		dword raw_size =        GetDWord();
		BMP_DEBUG("Raw size: %i", raw_size);

		dword XPelsPerMeter = GetDWord();
		dword YPelsPermeter = GetDWord();

		dword colors_used = GetDWord();
		dword colors_important = GetDWord();

		BMP_DEBUG("Colors used: %i", colors_used);
		BMP_DEBUG("Colors important: %i", colors_important);

		//fseek(m_Handle, 16, SEEK_CUR);

		m_Pixels = new byte[m_Width * m_Height * m_BPP];

		uint32 total = (m_Width * m_Height) >> 1;
		m_Pitch = m_Width * m_BPP;

		m_Target = m_Pixels + ((m_Height - 1) * m_Pitch);
		
		uint32 readpitch = m_Width * bytesperpixel;
		uint32 readbytes = readpitch * m_Height;
		byte* data = new byte[readbytes];
		fread(data, 1, readbytes, m_Handle);

		byte* read = data;

		byte color[4];

		switch (m_BPPIdent)
		{

		case BPP_32B_R8G8B8: 
			g_ColorFuncBMP = ColorFuncBMP_R8G8B8; 
			break;

		case BPP_32B_A8R8G8B8: 
			g_ColorFuncBMP = ColorFuncBMP_A8R8G8B8; 
			break;

		case BPP_32B_A8B8G8R8:
			g_ColorFuncBMP = ColorFuncBMP_A8B8G8R8;
			break;

		case BPP_32B_R8G8B8A8: 
			g_ColorFuncBMP = ColorFuncBMP_R8G8B8A8; 
			break;

		case BPP_32B_B8G8R8A8: 
			g_ColorFuncBMP = ColorFuncBMP_B8G8R8A8; 
			break;

		case BPP_16B_R5G6B5: 
			g_ColorFuncBMP = ColorFuncBMP_R5G6B5; 
			break;

		default:
			TIL_ERROR_EXPLAIN("Unhandled color format: %i", m_BPPIdent);
			return false;
		}

		// uncompressed

		if (compresion == COMP_RGB)
		{
			for (uint32 y = 0; y < m_Height; y++)
			{
				byte* src = read;
				uint8* dst = m_Target;

				for (uint32 x = 0; x < m_Width; x++)
				{
					GetComponents(color, src, bytesperpixel);
					g_ColorFuncBMP(dst, color);

					src += bytesperpixel;
					dst += m_BPP;
				}

				read += readpitch;
				m_Target -= m_Pitch;
			}
		}
		else
		{
			TIL_ERROR_EXPLAIN("Unhandled compression method: %i", compresion);
			return false;
		}

		return true;
	}

	til::uint32 ImageBMP::GetFrameCount()
	{
		return 1;
	}

	byte* ImageBMP::GetPixels( uint32 a_Frame /*= 0*/ )
	{
		return m_Pixels;
	}

	uint32 ImageBMP::GetWidth(uint32 a_Frame)
	{
		return m_Width;
	}
	uint32 ImageBMP::GetHeight(uint32 a_Frame)
	{
		return m_Height;
	}

}; // namespace til

#endif