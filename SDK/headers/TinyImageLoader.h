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

#ifndef _TINYIMAGELOADER_H_
#define _TINYIMAGELOADER_H_

#include "TILSettings.h"
#include "TILImage.h"

namespace til
{

	extern void TIL_Init(uint32 a_Settings = TIL_SETTINGS);
	extern Image* TIL_Load(const char* a_FileName, uint32 a_Options = (TIL_FILE_ABSOLUTEPATH | TIL_DEPTH_A8R8G8B8));

	extern size_t TIL_SetWorkingDirectory(const char* a_Path, size_t a_Length);

	extern void TIL_GetVersion(char* a_Target, size_t a_MaxLength);

	extern void TIL_SetErrorFunc(MessageFunc a_Func);
	extern char* TIL_GetError();
	extern size_t TIL_GetErrorLength();

	extern void TIL_SetDebugFunc(MessageFunc a_Func);
	extern char* TIL_GetDebug();
	extern size_t TIL_GetDebugLength();

}; // namespace til

#endif