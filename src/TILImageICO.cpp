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

#include "..\SDK\headers\TILImageICO.h"

#if (TIL_FORMAT & TIL_FORMAT_ICO)

#ifdef TIL_DEBUG
	#define ICO_DEBUG(msg, ...)        TIL_PRINT_DEBUG("ICO: "msg, __VA_ARGS__)
#else
	#define ICO_DEBUG(msg, ...)
#endif

namespace til
{

	ImageICO::ImageICO()
	{
		m_First = m_Current = NULL;
	}

	ImageICO::~ImageICO()
	{
		
	}

	void ImageICO::AddBuffer(uint32 a_Width, uint32 a_Height)
	{
		if (m_First)
		{
			BufferICO* temp = new BufferICO;
			m_Current->next = temp;
			temp->next    = NULL;
			temp->width   = a_Width;
			temp->height  = a_Height;
			temp->buffer  = new byte[a_Width * a_Height * m_BPP];
			m_Current = temp;
		}
		else
		{ 
			m_First = new BufferICO;
			m_Current = m_First;
			m_Current->next    = NULL;
			m_Current->width   = a_Width;
			m_Current->height  = a_Height;
			m_Current->buffer  = new byte[a_Width * a_Height * m_BPP];
		}
	}

	bool ImageICO::Parse(uint32 a_ColorDepth)
	{
		// empty anyway
		fseek(m_Handle, 2, SEEK_CUR);

		word header;          fread(&header, 2, 1, m_Handle);
		if (header != 1 && header != 2)
		{
			TIL_ERROR_EXPLAIN("Not a valid ICO file!");
			return false;
		}
	
		word imagecount;      fread(&imagecount, 2, 1, m_Handle);
		m_Images = imagecount;

		byte width, height, palette, reserved;
		word planes, bpp;
		dword datasize, offset;

		for (uint16 i = 0; i < imagecount; i++)
		{
			fread(&width, 1, 1, m_Handle);
			fread(&height, 1, 1, m_Handle);

			fread(&palette, 1, 1, m_Handle);
			fread(&reserved, 1, 1, m_Handle);

			fread(&planes, 2, 1, m_Handle);
			fread(&bpp, 2, 1, m_Handle);

			fread(&datasize, 4, 1, m_Handle);
			fread(&offset, 4, 1, m_Handle);

			AddBuffer(width, height);
			m_Current->offset    = offset;
			m_Current->datasize  = datasize;
			//m_Current->bpp       = bpp >> 3;

			if (bpp == 0) 
			{ 
				m_Current->bytespp = datasize / (width * height); 
				m_Current->bitspp = 0;
			}
			else
			{
				m_Current->bytespp = bpp >> 3;
				m_Current->bitspp = bpp;
			}

			bool bleh = true;
		}

		BufferICO* cur = m_First;
		for (uint32 i = 0; i < m_Images; cur = cur->next, i++)
		{
			fseek(m_Handle, cur->offset + 36, SEEK_SET);

			uint32 current = 0;

			uint32 pitch = cur->width * m_BPP;

			byte* target = cur->buffer + ((cur->height - 1) * pitch);
			for (uint32 y = 0; y < cur->height; y++)
			{
				color_16b* dst_16b = (color_16b*)target;
				color_32b* dst_32b = (color_32b*)target;

				byte src[4];

				for (uint32 x = 0; x < cur->width; x++)
				{
					fread(src, 1, cur->bytespp, m_Handle); 
					//current += cur->bytespp;

					if (cur->bytespp == 4)
					{
						switch (m_BPPIdent)
						{

						case BPP_16B_R5G6B5:
							{
								if (src[3] > 0)  { dst_16b[x] = AlphaBlend_16b_R5G6B5(src[2], src[1], src[0], src[3]); }
								else             { dst_16b[x] = 0x000000; }

								break;
							}

						case BPP_32B_R8G8B8:
							{
								if (src[3] > 0)  { dst_32b[x] = AlphaBlend_32b_R8G8B8(src[2], src[1], src[0], src[3]); }
								else             { dst_32b[x] = 0x000000; }

								break;
							}

						case BPP_32B_A8R8G8B8:
							{
								dst_32b[x] = Construct_32b_A8R8G8B8(src[2], src[1], src[0], src[3]); 
								break;
							}

						}
					}
					else if (cur->bytespp == 1)
					{
						//Bit   07 06 05 04 03 02 01 00
						//Data   R  R  R  G  G  G  B  B

						switch (m_BPPIdent)
						{

						case BPP_32B_R8G8B8:
							{
								dst_32b[x] = Construct_32b_R8G8B8(src[0]);
								//dst_32b[x] = 0xFF0000;
								break;
							}

						};

						//bool bleh = true;
					}
				}
				
				target -= pitch;
			}

			//uint32 offset = datasize - current;

			//fseek(m_Handle, datasize, SEEK_SET);
			//fseek(m_Handle, offset, SEEK_CUR);

			bool bleh = true;
		}

		return true;
	}

	uint32 ImageICO::GetFrameCount()
	{
		return m_Images;
	}

	byte* ImageICO::GetPixels(uint32 a_Frame /*= 0*/)
	{
		if (a_Frame >= 0 && a_Frame < m_Images)
		{
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

}; // namespace til

#endif