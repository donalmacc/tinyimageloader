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

#if (TIL_FORMAT & TIL_FORMAT_ICO)
	#include "..\SDK\headers\TILImageICO.h"
#endif

#include <stdarg.h>
#include <windows.h>
#include <MMSystem.h>

namespace til
{

	static uint32 g_Options;
	static char* g_LineFeed = NULL;

	static char* g_WorkingDir = NULL;
	static size_t g_WorkingDirLength = 0;

	void AddErrorDefault(MessageData* a_Data);
	static MessageFunc g_ErrorFunc = AddErrorDefault;
	static char* g_Error = NULL;
	static char* g_ErrorTemp = NULL;
	static size_t g_ErrorMaxSize = 1024;

	void AddDebugDefault(MessageData* a_Data);
	static MessageFunc g_DebugFunc = AddDebugDefault;
	static char* g_Debug = NULL;
	static char* g_DebugTemp = NULL;
	static size_t g_DebugMaxSize = 1024;

	static MessageData g_Msg;

	void TIL_Init(uint32 a_Settings)
	{
		g_Options = a_Settings;

		byte swaptest[] = { 1, 0 };
		if (*(word*)swaptest == 1)
		{
			TIL_PRINT_DEBUG("Little endian.");
		}
		else
		{
			TIL_PRINT_DEBUG("Big endian.");
		}

#if (TIL_PLATFORM == TIL_PLATFORM_WINDOWS)

		char path[TIL_MAX_PATH];

		char* dir = new char[TIL_MAX_PATH];
		GetModuleFileNameA(NULL, dir, TIL_MAX_PATH);
		char* lastslash = strrchr(dir, '\\');

		strncpy(path, dir, lastslash - dir + 1);
		path[lastslash - dir + 1] = 0;

		TIL_SetWorkingDirectory(path, strlen(path));

#else

		// do nothing, for now

#endif

	}

	void InitLineFeed()
	{
		if (!g_LineFeed)
		{
			g_LineFeed = new char[4];
			memset(g_LineFeed, 0, 4);

			if (g_Options & TIL_FILE_CRLF)
			{
				strcpy(g_LineFeed, "\r\n");
			}
			else if (g_Options * TIL_FILE_LF)
			{
				strcpy(g_LineFeed, "\n");
			}
			else if (g_Options * TIL_FILE_CR)
			{
				strcpy(g_LineFeed, "\r");
			}
		}
	}

	void AddErrorDefault(MessageData* a_Data)
	{
		if (!g_Error) 
		{
			g_Error = new char[g_ErrorMaxSize];
			memset(g_Error, 0, g_ErrorMaxSize);

			InitLineFeed();
		}

		sprintf(
			g_ErrorTemp, 
			"%s (in file %s at line %i)", 
			a_Data->message, 
			a_Data->source_file, 
			a_Data->source_line
		);
		strcat(g_ErrorTemp, g_LineFeed);

		bool resize = false;
		while (strlen(g_ErrorTemp) + strlen(g_Error) > g_ErrorMaxSize) { g_ErrorMaxSize *= 2; resize = true; }
		if (resize)
		{
			char* move = new char[g_ErrorMaxSize];
			strcpy(move, g_Error);
			g_Error = move;
		}

		strcat(g_Error, g_ErrorTemp);
	}

	void TIL_SetErrorFunc( MessageFunc a_Func )
	{
		g_ErrorFunc = a_Func;
	}

	char* TIL_GetError()
	{
		return g_Error;
	}

	size_t TIL_GetErrorLength()
	{
		if (g_Error) { return strlen(g_Error); }
		return 0;
	}

	void AddError( char* a_Message, char* a_File, int a_Line, ... )
	{
		va_list args;
		va_start(args, a_Line);
		if (!g_ErrorTemp) { g_ErrorTemp = new char[1024]; }
		vsprintf(g_ErrorTemp, a_Message, args);
		va_end(args);

		/*if (!g_Error) 
		{
			g_Error = new char[g_ErrorMaxSize];
			memset(g_Error, 0, g_ErrorMaxSize);

			InitLineFeed();
		}

		sprintf(g_ErrorTemp, "%s (in file %s at line %i)", g_ErrorTemp, a_File, a_Line);
		strcat(g_ErrorTemp, g_LineFeed);

		bool resize = false;
		while (strlen(g_ErrorTemp) + strlen(g_Error) > g_ErrorMaxSize) { g_ErrorMaxSize *= 2; resize = true; }
		if (resize)
		{
			char* move = new char[g_ErrorMaxSize];
			strcpy(move, g_Error);
			g_Error = move;
		}

		strcat(g_Error, g_ErrorTemp);*/

		g_Msg.message = g_ErrorTemp;
		g_Msg.source_file = a_File;
		g_Msg.source_line = a_Line;
		g_ErrorFunc(&g_Msg);
	}


	void TIL_SetDebugFunc( MessageFunc a_Func )
	{
		g_DebugFunc = a_Func;
	}

	char* TIL_GetDebug()
	{
		return g_Debug;
	}

	size_t TIL_GetDebugLength()
	{
		if (g_Debug) { return strlen(g_Debug); }
		return 0;
	}

	void AddDebugDefault(MessageData* a_Data)
	{
		if (!g_Debug) 
		{
			g_Debug = new char[g_DebugMaxSize];
			memset(g_Debug, 0, g_DebugMaxSize);

			InitLineFeed();
		}

		sprintf(g_DebugTemp, "%s", a_Data->message);
		strcat(g_DebugTemp, g_LineFeed);

		bool resize = false;
		while (strlen(g_DebugTemp) + strlen(g_Debug) > g_DebugMaxSize) { g_DebugMaxSize *= 2; resize = true; }
		if (resize)
		{
			char* move = new char[g_DebugMaxSize];
			strcpy(move, g_Debug);
			g_Debug = move;
		}

		strcat(g_Debug, g_DebugTemp);
	}

	void AddDebug(char* a_Message, char* a_File, int a_Line, ...)
	{
		va_list args;
		va_start(args, a_Line);
		if (!g_DebugTemp) { g_DebugTemp = new char[1024]; }
		vsprintf(g_DebugTemp, a_Message, args);
		va_end(args);

		g_Msg.message = g_DebugTemp;
		g_Msg.source_file = a_File;
		g_Msg.source_line = a_Line;
		g_DebugFunc(&g_Msg);
	}


	void TIL_GetVersion(char* a_Target, size_t a_MaxLength)
	{
		sprintf(a_Target, "%i.%i.%i", TIL_VERSION_MAJOR, TIL_VERSION_MINOR, TIL_VERSION_BUGFIX);
	}

	Image* TIL_Load(const char* a_FileName, uint32 a_Options)
	{
		Image* result = NULL;

		size_t end = strlen(a_FileName) - 4;
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
			if (!result->Load(a_FileName, a_Options & TIL_FILE_MASK))
			{
				TIL_ERROR_EXPLAIN("Could not find file '%s'.", a_FileName);
				delete result;
				result = NULL;
			}
			else 
			{
				result->SetBPP(a_Options & TIL_DEPTH_MASK);

				if (!result->Parse(a_Options & TIL_DEPTH_MASK))
				{
					TIL_ERROR_EXPLAIN("Could not parse file.");
					delete result;
					result = NULL;
				}
				else if (!result->Close())
				{
					TIL_ERROR_EXPLAIN("Could not close file.");
					delete result;
					result = NULL;
				}
			}
		}



		return result;
	}

	extern size_t TIL_SetWorkingDirectory(const char* a_Path, size_t a_Length )
	{
		if (!g_WorkingDir)
		{
			g_WorkingDir = new char[a_Length];
		}

		strcpy(g_WorkingDir, a_Path);
		g_WorkingDirLength = strlen(g_WorkingDir);

		TIL_PRINT_DEBUG("Setting working directory to '%s'", g_WorkingDir);

		return g_WorkingDirLength;
	}

	extern void AddWorkingDirectory( char* a_Dst, size_t a_MaxLength, const char* a_Path )
	{
		if (g_WorkingDir) { strncpy(a_Dst, g_WorkingDir, g_WorkingDirLength); }
		strcat(a_Dst, a_Path);
	}


}; // namespace til