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

#ifndef _TILIMAGETGA_H_
#define _TILIMAGETGA_H_

#include "TILImage.h"

#if (TIL_FORMAT & TIL_FORMAT_TGA)

namespace til
{

	class ImageTGA : public Image
	{

		enum ColorType
		{
			COLOR_BLACKANDWHITE,
			COLOR_MAPPED,
			COLOR_TRUECOLOR
		};
		
		enum Compression
		{
			COMP_NONE,
			COMP_RLE
		};

	public:

		ImageTGA();
		~ImageTGA();

		bool CompileUncompressed();
		bool CompileRunLengthEncoded();

		bool Parse(uint32 a_ColorDepth);

		uint32 GetFrameCount();
		byte* GetPixels(uint32 a_Frame = 0);

		uint32 GetWidth(uint32 a_Frame = 0);
		uint32 GetHeight(uint32 a_Frame = 0);
		
	private:

		ColorType m_Type;
		Compression m_Comp;
		byte* m_Pixels;
		byte* m_Target;
		byte m_Src[4];
		byte m_Depth;

		uint32 m_Width, m_Height, m_Pitch;

	}; // class ImageTGA

}; // namespace til

#endif

#endif