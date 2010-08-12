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

#include "..\SDK\headers\TinyImageLoader.h"

#if (TIL_FORMAT & TIL_FORMAT_PNG)
	#include "..\SDK\headers\TILImagePNG.h"
#endif

#if (TIL_FORMAT & TIL_FORMAT_GIF)
	#include "..\SDK\headers\TILImageGIF.h"
#endif

#if (TIL_FORMAT & TIL_FORMAT_TGA)
	#include "..\SDK\headers\TILImageTGA.h"
#endif

#if (TIL_FORMAT & TIL_FORMAT_BMP)
	#include "..\SDK\headers\TILImageBMP.h"
#endif

#if (TIL_FORMAT & TIL_FORMAT_BMP)
	#include "..\SDK\headers\TILImageICO.h"
#endif

namespace til
{

	void TinyImageLoader::Init()
	{
		byte swaptest[] = { 1, 0 };
		if (*(word*)swaptest == 1)
		{
			TIL_PRINT_DEBUG("Little endian.");
		}
		else
		{
			TIL_PRINT_DEBUG("Big endian.");
		}
	}

	Image* TinyImageLoader::Load(const char* a_FileName, uint32 a_Options)
	{
		Image* result = NULL;

		uint32 end = strlen(a_FileName) - 4;
		if (end < 4)
		{
			TIL_ERROR_EXPLAIN("Filename isn't long enough.");
			return NULL;
		}

		// lol hack
		if (0) { }
#if (TIL_FORMAT & TIL_FORMAT_PNG)
		else if (!strncmp(a_FileName + end, ".png", 4)) { result = new ImagePNG(); }
#endif
#if (TIL_FORMAT & TIL_FORMAT_GIF)
		else if (!strncmp(a_FileName + end, ".gif", 4)) { result = new ImageGIF(); }
#endif
#if (TIL_FORMAT & TIL_FORMAT_TGA)
		else if (!strncmp(a_FileName + end, ".tga", 4)) { result = new ImageTGA(); }
#endif
#if (TIL_FORMAT & TIL_FORMAT_BMP)
		else if (!strncmp(a_FileName + end, ".bmp", 4)) { result = new ImageBMP(); }
#endif
#if (TIL_FORMAT & TIL_FORMAT_ICO)
		else if (!strncmp(a_FileName + end, ".ico", 4)) { result = new ImageICO(); }
#endif
		else
		{
			TIL_ERROR_EXPLAIN("Can't parse file: unknown format.");
			result = NULL;
		}

		if (result)
		{
			if (!result->Load(a_FileName, a_Options & 0x0000FFFF))
			{
				TIL_ERROR_EXPLAIN("Could not find file '%s'.", a_FileName);
				delete result;
				result = NULL;
			}
			else 
			{
				result->SetBPP(a_Options & 0xFFFF0000);
				if (!result->Parse(a_Options & 0xFFFF0000))
				{
					TIL_ERROR_EXPLAIN("Could not parse file.");
					delete result;
					result = NULL;
				}
			}
		}

		return result;
	}
}; // namespace til