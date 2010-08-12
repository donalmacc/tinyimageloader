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

#ifndef _TINYIMAGELOADER_H_
#define _TINYIMAGELOADER_H_

#include "TILSettings.h"
#include "TILImage.h"

namespace til
{

	struct TinyImageLoader
	{
	
		static void Init();
		static Image* Load(const char* a_FileName, uint32 a_Options = (TIL_FILE_ABSOLUTEPATH | TIL_DEPTH_A8R8G8B8));
		
	}; // class TinyImageLoader

}; // namespace til

#endif