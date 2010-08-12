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

/*
	TinyImageLoader

	Image Format Template

	Note: this file isn't actually used in TinyImageLoader.
	You can copy this file to extend TinyImageLoader with new formats.
*/

// change these defines
#ifndef _TILIMAGETEMPLATE_H_
#define _TILIMAGETEMPLATE_H_

#include "TILImage.h"

// change this to your format
// or remove it if it should always be supported
#if (TIL_FORMAT & TIL_FORMAT_MYFORMAT)

namespace til
{

	struct TinyImageLoader;

	// change the class name
	class ImageTemplate : public Image
	{

		friend struct TinyImageLoader;

	public:

		// make sure you support these functions
		uint32 GetFrameCount();
		byte* GetPixels(uint32 a_Frame = 0);

	private:

		ImageTemplate();
		~ImageTemplate();

		// this one does all the magic
		// check the cpp for the correct implementation
		bool Parse(uint32 a_ColorDepth);

		// go wild, add as many members and functions as you need here

	}; // class ImageTemplate

}; // namespace til

#endif

#endif