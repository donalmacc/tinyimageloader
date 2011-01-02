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

#ifndef _TILIMAGEICO_H_
#define _TILIMAGEICO_H_

#include "TILImage.h"

#if (TIL_FORMAT & TIL_FORMAT_ICO)

namespace til
{

	struct BufferICO
	{
		byte* buffer;
		uint32 width, height;
		uint32 datasize, offset;
		uint32 bytespp, bitspp;
		BufferICO* next;
	};

	class ImageICO : public Image
	{

	public:

		ImageICO();
		~ImageICO();

		bool Parse(uint32 a_ColorDepth);

		uint32 GetFrameCount();
		byte* GetPixels(uint32 a_Frame = 0);

		uint32 GetWidth(uint32 a_Frame = 0);
		uint32 GetHeight(uint32 a_Frame = 0);

	private:

		void AddBuffer(uint32 a_Width, uint32 a_Height);

		uint32 m_Images;

		byte* m_Pixels;
		uint32 m_Width, m_Height, m_Pitch;

		BufferICO* m_First;
		BufferICO* m_Current;

	}; // class ImageICO

}; // namespace til

#endif

#endif