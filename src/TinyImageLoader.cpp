#include "..\SDK\TinyImageLoader.h"

#if (TIL_FORMAT & TIL_FORMAT_PNG)
	#include "..\SDK\TILImagePNG.h"
#endif

#if (TIL_FORMAT & TIL_FORMAT_GIF)
	#include "..\SDK\TILImageGIF.h"
#endif

#if (TIL_FORMAT & TIL_FORMAT_TGA)
	#include "..\SDK\TILImageTGA.h"
#endif

namespace til
{

	Image* TinyImageLoader::Load(const char* a_FileName, uint32 a_Options)
	{
		Image* result = NULL;
		uint32 tag = *(uint32*)&a_FileName[strlen(a_FileName) - 4];

		switch (tag)
		{

#if (TIL_FORMAT & TIL_FORMAT_PNG)

		case TIL_FORMAT_PNG_EXT:
			{
				result = new ImagePNG();
				break;
			}
			
#endif

#if (TIL_FORMAT & TIL_FORMAT_GIF)

		case TIL_FORMAT_GIF_EXT:
			{
				result = new ImageGIF();
				break;
			}

#endif

#if (TIL_FORMAT & TIL_FORMAT_TGA)

		case TIL_FORMAT_TGA_EXT:
			{
				result = new ImageTGA();
				break;
			}

#endif

		default:
			{
				TIL_ERROR_EXPLAIN("Can't parse file: unknown format.");
				result = NULL;
				break;
			}

		};

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