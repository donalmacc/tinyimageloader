#include "..\SDK\headers\TILImage.h"

#include <stdlib.h>

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

namespace til
{

	Image::Image()
	{

	}

	Image::~Image()
	{

	}

	bool Image::Load(const char* a_FileName, uint32 a_Options)
	{
		uint32 length = strlen(a_FileName);
		m_FileName = new char[length + 1];
		strcpy(m_FileName, a_FileName);

		char path[256] = { 0 };

		if (a_Options == TIL_FILE_ABSOLUTEPATH)
		{
			strcpy(path, a_FileName);
		}
		else if (a_Options == TIL_FILE_ADDWORKINGDIR)
		{

#if (defined(TIL_STRINGS_WIDE))

			wchar_t* dir = new wchar_t[256];
			GetModuleFileNameW(NULL, dir, _MAX_PATH);
			wchar_t* lastslash = wcsrchr(dir, '\\');

			wchar_t widepath[256] = { 0 };
			wcsncpy(widepath, dir, lastslash - dir + 1);

			wcstombs(path, widepath, 256);

#else

			char* dir = new char[256];	
			GetModuleFileNameA(NULL, dir, _MAX_PATH);
			char* lastslash = strrchr(dir, '\\');

			strncpy(path, dir, lastslash - dir + 1);

#endif

			strcat(path, a_FileName);
		}
		
		m_Handle = fopen(path, "rb");
		if (!m_Handle)
		{
			TIL_ERROR_EXPLAIN("Could not find file '%s'.", a_FileName);
			return false;
		}

		return true;
	}

	void Image::SetBPP(uint32 a_Options)
	{
		switch (a_Options)
		{

		case TIL_DEPTH_A8R8G8B8:
		case TIL_DEPTH_R8G8B8A8:
		case TIL_DEPTH_R8G8B8:
			{
				m_BPP = 4;
				break;
			}
		case TIL_DEPTH_R5G6B5:
			{
				m_BPP = 2;
				break;
			}

		}	

		m_BPPIdent = (BitDepth)(a_Options >> 16);
	}

}; // namespace til