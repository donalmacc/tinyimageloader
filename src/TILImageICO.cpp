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

#include "TILImageICO.h"
#include "TILInternal.h"

#if (TIL_FORMAT & TIL_FORMAT_ICO)

#if (TIL_FORMAT & TIL_FORMAT_PNG)
	#include "TILImagePNG.h"
#endif

#if (TIL_RUN_TARGET == TIL_TARGET_DEVEL)
	#define ICO_DEBUG(msg, ...)        TIL_PRINT_DEBUG("ICO: "msg, ##__VA_ARGS__)
#else
	#define ICO_DEBUG(msg, ...)
#endif

//#define GETBIT(data, place) 0
#define GETBIT(data, place) ((data >> (8 - place)) & 1)

namespace til
{

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	typedef uint8* (*ColorFuncRow)(uint8*, uint8*, uint8*, ImageICO::BufferICO*);

	uint8* ColorFunc_4b_A8B8G8R8_Row(uint8* a_Dst, uint8* a_Src, uint8* a_AndMask, ImageICO::BufferICO* a_Buffer)
	{
		color_32b* dst = (color_32b*)a_Dst;

		uint8* src = a_Src;
		uint8* andmask = a_AndMask;

		if (a_Buffer->palette == 16)
		{
			for (uint32 i = 0; i < a_Buffer->width; i += 8)
			{
				for (int32 j = 1; j < 8; j += 2)
				{
					if (GETBIT(*andmask, j    ) == 0) 
					{ 
						*dst = a_Buffer->colors[(*src & 0xF0) >> 4];
					}
					else
					{
						*dst = 0x00000000;
					}
					dst++;

					if (GETBIT(*andmask, j - 1) == 0) 
					{ 
						*dst = a_Buffer->colors[(*src & 0x0F)     ]; 
					}
					else
					{
						*dst = 0x00000000;
					}
					dst++;

					/*
					*dst++ = a_Buffer->colors[(*src & 0xF0) >> 4];
					*dst++ = a_Buffer->colors[(*src & 0x0F)     ];
					*/

					src++;
				}
				andmask++;
			}

			return (uint8*)dst;
		}
		else if (a_Buffer->palette == 256)
		{
			for (uint32 i = 0; i < a_Buffer->width; i++)
			{
				*dst++ = a_Buffer->colors[*src];
				src++;
			}

			return (uint8*)dst;
		}
		else
		{
			if (a_Buffer->bytespp > 3)
			{
				for (uint32 j = 0; j < a_Buffer->width; j++)
				{
					*dst++ = AlphaBlend_32b_A8B8G8R8(src[2], src[1], src[0], src[3]);
					src += a_Buffer->bytespp;
				}
			}
			else
			{
				for (uint32 j = 0; j < a_Buffer->width; j++)
				{
					*dst++ = AlphaBlend_32b_A8B8G8R8(src[2], src[1], src[0], 255);
					src += a_Buffer->bytespp;
				}
			}
			

			return (uint8*)dst;
		}
	}

	uint8* ColorFunc_1b_A8B8G8R8_Row(uint8* a_Dst, uint8* a_Src, uint8* a_AndMask, ImageICO::BufferICO* a_Buffer)
	{
		color_32b* dst = (color_32b*)a_Dst;

		uint8* src = a_Src;

		for (uint32 j = 0; j < a_Buffer->width; j++)
		{
			//*dst++ = AlphaBlend_32b_A8B8G8R8(src[2], src[1], src[0], src[3]);
			src++;
		}

		return (uint8*)dst;
	}

	ColorFuncRow g_ColorFuncRow = NULL;

#endif

	ImageICO::ImageICO()
	{
		m_First = m_Current = NULL;
	}

	ImageICO::~ImageICO()
	{
		ReleaseMemory(m_First);
		delete m_First;
	}

	void ImageICO::AddBuffer(uint32 a_Width, uint32 a_Height)
	{
		if (m_First)
		{	
			m_Current->next = new BufferICO;
			m_Current->next->next    = NULL;
			m_Current->next->width   = a_Width;
			m_Current->next->height  = a_Height;
			m_Current->next->pitch   = a_Width;
			m_Current->next->buffer  = new byte[a_Width * a_Height * m_BPP];
			m_Current->next->colors  = NULL;
			m_Current = m_Current->next;
		}
		else
		{ 
			m_First = new BufferICO;
			m_Current = m_First;
			m_Current->next    = NULL;
			m_Current->width   = a_Width;
			m_Current->height  = a_Height;
			m_Current->pitch   = a_Width;
			m_Current->buffer  = new byte[a_Width * a_Height * m_BPP];
			m_Current->colors  = NULL;
		}
	}

	void ImageICO::ExpandPalette(BufferICO* a_Buffer)
	{
		a_Buffer->colors = new color_32b[a_Buffer->palette];

		color_32b* dst = a_Buffer->colors;

		for (int i = 0; i < a_Buffer->palette; i++)
		{
			byte read[4];
			m_Stream->ReadByte(read, 4);
			*dst++ = Construct_32b_A8R8G8B8(read[0], read[1], read[2], (a_Buffer->palette > 0) ? 255 : read[3]);
		}
	}

	bool ImageICO::Parse(uint32 a_ColorDepth)
	{
		// empty anyway
		m_Stream->Seek(2, TIL_FILE_SEEK_CURR);

		word header;          m_Stream->ReadWord(&header);
		if (header != 1 && header != 2)
		{
			TIL_ERROR_EXPLAIN("Not a valid ICO file!");
			return false;
		}

		word imagecount;      m_Stream->ReadWord(&imagecount);
		m_Images = imagecount;

		byte width, height, palette, reserved;
		word planes, bpp;
		dword datasize, offset;

		for (uint16 i = 0; i < imagecount; i++)
		{
			m_Stream->ReadByte(&width);
			m_Stream->ReadByte(&height);

			m_Stream->ReadByte(&palette);
			m_Stream->ReadByte(&reserved);

			m_Stream->ReadWord(&planes);
			m_Stream->ReadWord(&bpp);

			m_Stream->ReadDWord(&datasize);
			m_Stream->ReadDWord(&offset);

			unsigned long w = (width == 0) ? 256 : width;
			unsigned long h = (height == 0) ? 256 : height;

			AddBuffer(w, h);

			m_Current->offset    = offset;
			m_Current->datasize  = datasize;
			m_Current->palette   = palette;

			if (bpp == 0) 
			{ 
				m_Current->bytespp = datasize / (width * height); 
				m_Current->bitspp = 0;
			}
			else
			{
				if (bpp == 4)
				{
					m_Current->bytespp = 1;
				}
				else
				{
					m_Current->bytespp = bpp >> 3;
				}
				m_Current->bitspp = bpp;
			}
		}

		BufferICO* cur = m_First;
		for (uint32 i = 0; i < m_Images; cur = cur->next, i++)
		{
			/*
				typedef struct tagBITMAPINFOHEADER{
				DWORD      biSize;
				LONG       biWidth;
				LONG       biHeight;
				WORD       biPlanes;
				WORD       biBitCount;
				DWORD      biCompression;
				DWORD      biSizeImage;
				LONG       biXPelsPerMeter;
				LONG       biYPelsPerMeter;
				DWORD      biClrUsed;
				DWORD      biClrImportant;
				} BITMAPINFOHEADER, FAR *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;
			*/

			//unsigned long size2 = sizeof(BITMAPINFOHEADER);

			m_Stream->Seek(cur->offset, TIL_FILE_SEEK_START);

			//BITMAPINFOHEADER bleh;
			//fread(&bleh, sizeof(bleh), 1, m_Handle);

			dword size;           m_Stream->ReadDWord(&size);
			int32 width;          m_Stream->Read(&width, sizeof(int32));
			int32 height;         m_Stream->Read(&height, sizeof(int32));
			word planes;          m_Stream->ReadWord(&planes);
			word bitcount;        m_Stream->ReadWord(&bitcount);
			dword compression;    m_Stream->ReadDWord(&compression);
			dword sizeimage;      m_Stream->ReadDWord(&sizeimage);
			int32 xpermeter;      m_Stream->Read(&xpermeter, sizeof(int32));
			int32 ypermeter;      m_Stream->Read(&ypermeter, sizeof(int32));
			dword colorused;      m_Stream->ReadDWord(&colorused);
			dword colorimportant; m_Stream->ReadDWord(&colorimportant);

			//m_Stream->Seek(24, TIL_FILE_SEEK_CURR);

			ICO_DEBUG("Image: %i", i);
			ICO_DEBUG("Dimensions: (%i, %i)", cur->width, cur->height);
			ICO_DEBUG("Bytes per pixel: %i", cur->bytespp);
			ICO_DEBUG("Bitcount: %i", bitcount);

			uint16 colors_used = 0;
			if (bitcount == 8) 
			{ 
				colors_used = 256; 
			} 
			else if (bitcount == 4) 
			{ 
				colors_used = 16; 
			}

			cur->palette = colors_used;

			uint32 current = 0;

			g_ColorFuncRow =  ColorFunc_4b_A8B8G8R8_Row;

			uint32 pitch = cur->pitch * m_BPP;
			cur->pitchy = cur->height;

			byte* src;

			byte* target = cur->buffer + ((cur->height - 1) * pitch);

			ICO_DEBUG("Palette: %i", cur->palette);

			if (cur->palette > 0)
			{
				ExpandPalette(cur);

				/*for(i=0;i<(icon.bWidth/8)*icon.bHeight;i++)
					*(icon.icAND+i)=getc(fp);*/

				/*for (int i = 0; i < (cur->width / 8) * cur->height; i++)
				{

				}*/

				int div = 1; // palette = 256
				if (cur->palette == 16) { div = 2; }

				cur->readpx = cur->width / div;
				cur->readpy = cur->height / div;

				src = new byte[cur->width * cur->readpy * cur->bytespp];
				m_Stream->Read(src, cur->bytespp, cur->width * cur->readpy);

				cur->andmask = new byte[(cur->width / 8) * cur->height];
				m_Stream->Read(cur->andmask, sizeof(byte), (cur->width / 8) * cur->height);

				byte* read = src;
				byte* andmask = cur->andmask;

				int offset = (cur->width / 8);

				if (cur->palette == 16)
				{
					for (uint32 y = 0; y < cur->readpy; y++)
					{
						g_ColorFuncRow(target, read, andmask, cur);
						target -= pitch;
						read += cur->readpx;
						andmask += cur->width / 8;

						g_ColorFuncRow(target, read, andmask, cur);
						target -= pitch;
						read += cur->readpx;
						andmask += cur->width / 8;
					}
				}
				else
				{
					for (uint32 y = 0; y < cur->height; y++)
					{
						g_ColorFuncRow(target, read, andmask, cur);
						target -= pitch;
						read += cur->readpx;
						andmask += cur->width / 8;
					}
				}

				delete src;
				delete cur->andmask;

				/*for(i=0,y=(icon.bHeight)-1;y>-1;y--)
					for(x=0;x<icon.bWidth;x+=8,i++){
						for(z=0;z<8;z++){
							if(!(getbits(icon.icAND[i],z,1)))
								drwpoint(AND,0x00,x+z+icon.x,y+icon.y);
						}
					}*/


			}
			else
			{
				m_Stream->Seek(cur->offset, TIL_FILE_SEEK_START);

				dword png_header;
				m_Stream->ReadDWord(&png_header);

				if (png_header == PNG_TYPE('G', 'N', 'P', 0x89))
				{

#if (TIL_FORMAT & TIL_FORMAT_PNG)

					ICO_DEBUG("Loading PNG.");

					m_Stream->Seek(cur->offset, TIL_FILE_SEEK_START);

					ImagePNG* png_compressed = new ImagePNG();
					png_compressed->Load(m_Stream);
					png_compressed->SetBPP(a_ColorDepth);
					png_compressed->Parse(a_ColorDepth);

					Internal::MemCpy(
						cur->buffer, 
						png_compressed->GetPixels(), 
						png_compressed->GetPitchX() * png_compressed->GetPitchY() * m_BPP
					);

					delete png_compressed;

#else

					TIL_ERROR_EXPLAIN("Can't parse ICO without PNG loader.", 0);
					Internal::MemSet(cur->buffer, 0, cur->width * cur->height * cur->bytespp);

#endif

				}
				else
				{
					ICO_DEBUG("Loading uncompressed data.");

					m_Stream->Seek(cur->offset + 40, TIL_FILE_SEEK_START);

					src = new byte[cur->width * cur->height * bpp];
					m_Stream->Read(src, bpp, cur->width * cur->height);

					byte* read = src;

					for (uint32 y = 0; y < cur->height; y++)
					{
						g_ColorFuncRow(target, read, NULL, cur);

						target -= pitch;
						read += cur->pitch * cur->bytespp;
					}

					delete src;
				}
			}
		}

		return true;
	}

	uint32 ImageICO::GetFrameCount()
	{
		return m_Images;
	}

	void ImageICO::ReleaseMemory(BufferICO* a_Buffer)
	{
		if (a_Buffer && a_Buffer->next) 
		{
			ReleaseMemory(a_Buffer->next); 
		}
		delete a_Buffer->next;
		delete a_Buffer->buffer;
		if (a_Buffer->colors) { delete a_Buffer->colors; }
	}

	byte* ImageICO::GetPixels(uint32 a_Frame /*= 0*/)
	{
		if (a_Frame >= 0 && a_Frame < m_Images)
		{
			//if (a_Frame == 6) { a_Frame--; }
			BufferICO* current = m_First;
			for (uint32 i = 0; i < a_Frame; i++) { current = current->next; }
			return current->buffer;
		}

		return NULL;
	}

	uint32 ImageICO::GetWidth(uint32 a_Frame)
	{
		if (a_Frame >= 0 && a_Frame < m_Images)
		{
			BufferICO* current = m_First;
			for (uint32 i = 0; i < a_Frame; i++) { current = current->next; }
			return current->width;
		}

		return 0;
	}

	uint32 ImageICO::GetHeight(uint32 a_Frame)
	{
		if (a_Frame >= 0 && a_Frame < m_Images)
		{
			BufferICO* current = m_First;
			for (uint32 i = 0; i < a_Frame; i++) { current = current->next; }
			return current->height;
		}

		return 0;
	}

	uint32 ImageICO::GetPitchX(uint32 a_Frame /*= 0*/)
	{
		if (a_Frame >= 0 && a_Frame < m_Images)
		{
			BufferICO* current = m_First;
			for (uint32 i = 0; i < a_Frame; i++) { current = current->next; }
			return current->pitch;
		}

		return 0;
	}

	uint32 ImageICO::GetPitchY(uint32 a_Frame /*= 0*/)
	{
		if (a_Frame >= 0 && a_Frame < m_Images)
		{
			BufferICO* current = m_First;
			for (uint32 i = 0; i < a_Frame; i++) { current = current->next; }
			return current->pitchy;
		}

		return 0;
	}

}; // namespace til

#endif