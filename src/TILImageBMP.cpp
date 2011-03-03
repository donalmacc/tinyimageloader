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

/*!
	\file TILImageBMP.h
*/

#include "TILInternal.h"
#include "TILImageBMP.h"

#if (TIL_FORMAT & TIL_FORMAT_BMP)

#ifdef TIL_TARGET_DEBUG
	#define BMP_DEBUG(msg, ...)        TIL_PRINT_DEBUG("BMP: "msg, __VA_ARGS__)
#else
	#define BMP_DEBUG(msg, ...)
#endif

namespace til
{

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	typedef void (*ColorFuncBMP)(uint8*, uint8*);

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

	void ColorFuncBMP_R8G8B8(uint8* a_Dst, uint8* a_Src)
	{
		color_32b* dst = (color_32b*)a_Dst;
		*dst = Construct_32b_R8G8B8(a_Src[0], a_Src[1], a_Src[2]);
	}

	void ColorFuncBMP_B8G8R8(uint8* a_Dst, uint8* a_Src)
	{
		color_32b* dst = (color_32b*)a_Dst;
		*dst = Construct_32b_B8G8R8(a_Src[0], a_Src[1], a_Src[2]);
	}

	void ColorFuncBMP_R5G6B5(uint8* a_Dst, uint8* a_Src)
	{
		color_16b* dst = (color_16b*)a_Dst;
		*dst = Construct_16b_R5G6B5(a_Src[0], a_Src[1], a_Src[2]);
	}

	void ColorFuncBMP_B5G6R5(uint8* a_Dst, uint8* a_Src)
	{
		color_16b* dst = (color_16b*)a_Dst;
		*dst = Construct_16b_B5G6R5(a_Src[0], a_Src[1], a_Src[2]);
	}

	ColorFuncBMP g_ColorFuncBMP = NULL;

#endif

	ImageBMP::ImageBMP()
	{
		m_ReadData = NULL;
		m_Pixels = NULL;
	}

	ImageBMP::~ImageBMP()
	{
		if (m_ReadData) { delete m_ReadData; }
		if (m_Pixels) { delete m_Pixels; }
	}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

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

#endif

	dword ImageBMP::GetDWord()
	{
		m_Stream->ReadByte(m_Data, 4);
		return (m_Data[3] << 24) | (m_Data[2] << 16) | (m_Data[1] << 8) | (m_Data[0]);
	}

	bool ImageBMP::Parse(uint32 a_ColorDepth)
	{
		byte header[3];         m_Stream->ReadByte(header, 2);
		dword size =            GetDWord();
		dword unused;           m_Stream->ReadDWord(&unused);
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

		word color_planes;      m_Stream->ReadWord(&color_planes);
		word bpp;               m_Stream->ReadWord(&bpp);
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
		m_ReadData = new byte[readbytes];
		m_Stream->ReadByte(m_ReadData, readbytes);

		byte* read = m_ReadData;

		byte color[4];

		switch (m_BPPIdent)
		{

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

		case BPP_32B_R8G8B8: 
			g_ColorFuncBMP = ColorFuncBMP_R8G8B8; 
			break;

		case BPP_32B_B8G8R8: 
			g_ColorFuncBMP = ColorFuncBMP_B8G8R8; 
			break;

		case BPP_16B_R5G6B5: 
			g_ColorFuncBMP = ColorFuncBMP_R5G6B5; 
			break;

		case BPP_16B_B5G6R5: 
			g_ColorFuncBMP = ColorFuncBMP_B5G6R5;
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