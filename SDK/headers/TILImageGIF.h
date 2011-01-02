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

	class ImageGIF : public Image
	{

	public:

		ImageGIF();
		~ImageGIF();

		bool Parse(uint32 a_ColorDepth);

		uint32 GetFrameCount();
		float GetDelay();
		byte* GetPixels(uint32 a_Frame = 0);

		uint32 GetWidth(uint32 a_Frame = 0);
		uint32 GetHeight(uint32 a_Frame = 0);

	private:

		void AddBuffer();
		void CompileColors(bool a_LocalTable = true);

		BufferLinked* m_First;
		BufferLinked* m_Current;
		uint32 m_Frames;
		float m_Delay;

		byte m_Buffer[256];

		byte* m_PrevBuffer;

		byte* m_CurrentColors;
		
		uint32 m_ColorDepth;

		byte* m_Palette;
		byte* m_Colors;
		uint32 m_ColorTableSize;

		byte* m_GlobalPalette;
		byte* m_GlobalColorTable;
		uint32 m_GlobalColorTableSize;

		bool m_Transparency;
		uint8 m_TransparentIndex;

		uint32 m_Width, m_Height, m_Pitch;

		uint32 m_OffsetX, m_OffsetY;
		uint32 m_LocalWidth, m_LocalHeight, m_LocalPitch;
		uint32 m_TotalBytes;
		
	}; // class ImageGIF

}; // namespace til

#endif

#endif