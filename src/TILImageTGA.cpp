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

#include "..\SDK\headers\TILImageTGA.h"

#if (TIL_FORMAT & TIL_FORMAT_TGA)

#ifdef TIL_DEBUG
	#define TGA_DEBUG(msg, ...)        TIL_PRINT_DEBUG("TGA: "msg, __VA_ARGS__)
#else
	#define TGA_DEBUG(msg, ...)
#endif

namespace til
{

	typedef uint8* (*ColorFuncComp)(uint8*, uint8*, int, int);

	int g_Depth;

	uint8* ColorFunc_R8G8B8_Comp(uint8* a_Dst, uint8* a_Src, int a_Repeat, int a_Unique)
	{
		color_32b* dst = (color_32b*)a_Dst;

		for (int j = 0; j < a_Unique; j++)
		{
			color_32b comp = AlphaBlend_32b_R8G8B8(a_Src[2], a_Src[1], a_Src[0], 255);
			for (int i = 0; i < a_Repeat; i++) { *dst++ = comp; }
			a_Src += g_Depth;
		}
		return (uint8*)dst;
	}

	uint8* ColorFunc_A8R8G8B8_Comp(uint8* a_Dst, uint8* a_Src, int a_Repeat, int a_Unique)
	{
		color_32b* dst = (color_32b*)a_Dst;

		for (int j = 0; j < a_Unique; j++)
		{
			color_32b comp = AlphaBlend_32b_A8R8G8B8(a_Src[2], a_Src[1], a_Src[0], (g_Depth > 3) ? a_Src[3] : 255);
			for (int i = 0; i < a_Repeat; i++) { *dst++ = comp; }
			a_Src += g_Depth;
		}
		return (uint8*)dst;
	}

	uint8* ColorFunc_A8B8G8R8_Comp(uint8* a_Dst, uint8* a_Src, int a_Repeat, int a_Unique)
	{
		color_32b* dst = (color_32b*)a_Dst;

		for (int j = 0; j < a_Unique; j++)
		{
			color_32b comp = AlphaBlend_32b_A8B8G8R8(a_Src[2], a_Src[1], a_Src[0], (g_Depth > 3) ? a_Src[3] : 255);
			for (int i = 0; i < a_Repeat; i++) { *dst++ = comp; }
			a_Src += g_Depth;
		}
		return (uint8*)dst;
	}

	uint8* ColorFunc_R8G8B8A8_Comp(uint8* a_Dst, uint8* a_Src, int a_Repeat, int a_Unique)
	{
		color_32b* dst = (color_32b*)a_Dst;

		for (int j = 0; j < a_Unique; j++)
		{
			color_32b comp = AlphaBlend_32b_R8G8B8A8(a_Src[2], a_Src[1], a_Src[0], (g_Depth > 3) ? a_Src[3] : 255);
			for (int i = 0; i < a_Repeat; i++) { *dst++ = comp; }
			a_Src += g_Depth;
		}
		return (uint8*)dst;
	}

	uint8* ColorFunc_B8G8R8A8_Comp(uint8* a_Dst, uint8* a_Src, int a_Repeat, int a_Unique)
	{
		color_32b* dst = (color_32b*)a_Dst;

		for (int j = 0; j < a_Unique; j++)
		{
			color_32b comp = AlphaBlend_32b_B8G8R8A8(a_Src[2], a_Src[1], a_Src[0], (g_Depth > 3) ? a_Src[3] : 255);
			for (int i = 0; i < a_Repeat; i++) { *dst++ = comp; }
			a_Src += g_Depth;
		}
		return (uint8*)dst;
	}

	uint8* ColorFunc_R5G6B5_Comp(uint8* a_Dst, uint8* a_Src, int a_Repeat, int a_Unique)
	{
		color_16b* dst = (color_16b*)a_Dst;

		for (int j = 0; j < a_Unique; j++)
		{
			color_16b comp = AlphaBlend_16b_R5G6B5(a_Src[2], a_Src[1], a_Src[0], (g_Depth > 3) ? a_Src[3] : 255);
			for (int i = 0; i < a_Repeat; i++) { *dst++ = comp; }
			a_Src += g_Depth;
		}
		return (uint8*)dst;
	}

	ColorFuncComp g_ColorFunc = NULL;

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

		uint8* src = new uint8[m_Width * m_Depth];

		for (uint32 y = 0; y < m_Height; y++)
		{
			byte* dst = m_Target;

			fread(src, m_Width, m_Depth, m_Handle);

			uint8* src_copy = src;
			dst = g_ColorFunc(dst, src_copy, 1, m_Width);

			m_Target -= m_Pitch;
		}

		return false;
	}

	bool ImageTGA::CompileRunLengthEncoded()
	{
		byte buffer[128 * 4];
		byte src_buffer[4];
		byte* src_color;

		uint32 total = m_Width * m_Height;

		for (uint32 y = 0; y < m_Height; y++)
		{
			uint8* dst = m_Target;

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
					fread(src_buffer, 1, m_Depth, m_Handle);
					src_color = src_buffer;

					repeat = count;
				}
				// raw packet
				else
				{
					fread(buffer, 1, count * m_Depth, m_Handle);
					src_color = buffer;

					unique = count;
				}

				dst = g_ColorFunc(dst, src_color, repeat, unique);

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

		fread(&m_Depth, 1, 1, m_Handle);
		m_Depth >>= 3;

		g_Depth = m_Depth;

		TGA_DEBUG("Depth: %i", m_Depth);

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

		switch (m_BPPIdent)
		{

		case BPP_32B_R8G8B8: 
			g_ColorFunc = ColorFunc_R8G8B8_Comp; 
			break;

		case BPP_32B_A8R8G8B8: 
			g_ColorFunc = ColorFunc_A8R8G8B8_Comp; 
			break;

		case BPP_32B_A8B8G8R8:
			g_ColorFunc = ColorFunc_A8B8G8R8_Comp;
			break;

		case BPP_32B_R8G8B8A8: 
			g_ColorFunc = ColorFunc_R8G8B8A8_Comp; 
			break;

		case BPP_32B_B8G8R8A8: 
			g_ColorFunc = ColorFunc_B8G8R8A8_Comp; 
			break;

		case BPP_16B_R5G6B5: 
			g_ColorFunc = ColorFunc_R5G6B5_Comp; 
			break;

		default:
			TIL_ERROR_EXPLAIN("Unhandled color format: %i", m_BPPIdent);
			break;
		}

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

	uint32 ImageTGA::GetWidth(uint32 a_Frame)
	{
		return m_Width;
	}
	uint32 ImageTGA::GetHeight(uint32 a_Frame)
	{
		return m_Height;
	}

}; // namespace til

#endif