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