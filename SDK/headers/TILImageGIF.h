/*
    TinyImageLoader - load images, just like that
    Copyright (C) 2010 Quinten Lansu (knight666)

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

#ifndef _TILIMAGEGIF_H_
#define _TILIMAGEGIF_H_

#include "TILImage.h"

#if (TIL_FORMAT & TIL_FORMAT_GIF)

namespace til
{

	struct BufferLinked
	{
		byte* buffer;
		BufferLinked* next;
	};

	struct TinyImageLoader;

	class ImageGIF : public Image
	{

		friend struct TinyImageLoader;

	public:

		uint32 GetFrameCount();
		float GetDelay();
		byte* GetPixels(uint32 a_Frame = 0);

		uint32 GetWidth(uint32 a_Frame = 0);
		uint32 GetHeight(uint32 a_Frame = 0);

	private:

		ImageGIF();
		~ImageGIF();

		bool Parse(uint32 a_ColorDepth);

		void AddBuffer();
		void CompileColors();

		BufferLinked* m_First;
		BufferLinked* m_Current;
		uint32 m_Frames;
		float m_Delay;

		byte m_Buffer[256];
		uint32 m_ColorDepth;
		uint32 m_ColorTableSize;
		char* m_Palette;
		byte* m_Colors;

		uint32 m_Width, m_Height, m_Pitch;
		
	}; // class ImageGIF

}; // namespace til

#endif

#endif