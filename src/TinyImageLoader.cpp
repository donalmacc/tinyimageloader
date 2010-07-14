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

namespace til
{

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
		if (1 == 0) { }
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