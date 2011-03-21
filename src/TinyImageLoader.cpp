/*
	TinyImageLoader - load images, just like that

	Copyright (C) 2010 - 2011 by Quinten Lansu
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

#include "TinyImageLoader.h"
#include "TILInternal.h"

#if (TIL_FORMAT & TIL_FORMAT_PNG)
	#include "TILImagePNG.h"
#endif

#if (TIL_FORMAT & TIL_FORMAT_GIF)
	#include "TILImageGIF.h"
#endif

#if (TIL_FORMAT & TIL_FORMAT_TGA)
	#include "TILImageTGA.h"
#endif

#if (TIL_FORMAT & TIL_FORMAT_BMP)
	#include "TILImageBMP.h"
#endif

#if (TIL_FORMAT & TIL_FORMAT_ICO)
	#include "TILImageICO.h"
#endif

#if (TIL_FORMAT & TIL_FORMAT_DDS)
	#include "TILImageDDS.h"
#endif

#include "TILFileStreamStd.h"

#include <stdarg.h>
#include <windows.h>
//#include <MMSystem.h>

namespace til
{

	static uint32 g_Options;
	static char* g_LineFeed = NULL;

	static char* g_WorkingDir = NULL;
	static size_t g_WorkingDirLength = 0;

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	void AddErrorDefault(MessageData* a_Data);
	static MessageFunc g_ErrorFunc = AddErrorDefault;
	static char* g_Error = NULL;
	static char* g_ErrorTemp = NULL;
	static size_t g_ErrorMaxSize = 1024;
	static size_t g_ErrorTempMaxSize = 1024;

	void AddDebugDefault(MessageData* a_Data);
	static MessageFunc g_DebugFunc = AddDebugDefault;
	static char* g_Debug = NULL;
	static char* g_DebugTemp = NULL;
	static size_t g_DebugMaxSize = 2048;
	static size_t g_DebugTempMaxSize = 1024;

#endif

	static MessageData g_Msg;

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	void InitLineFeed()
	{
		if (!g_LineFeed)
		{
			g_LineFeed = new char[4];
			memset(g_LineFeed, 0, 4);

			if (g_Options & TIL_FILE_CRLF)
			{
				strcpy_s(g_LineFeed, 4, "\r\n");
			}
			else if (g_Options & TIL_FILE_LF)
			{
				strcpy_s(g_LineFeed, 4, "\n");
			}
			else if (g_Options & TIL_FILE_CR)
			{
				strcpy_s(g_LineFeed, 4, "\r");
			}
		}
	}

#endif

	/*! Initializes TinyImageLoader. */
	void TIL_Init(uint32 a_Settings)
	{
		g_Options = a_Settings;

		if (!g_Error) 
		{
			g_Error = new char[g_ErrorMaxSize];
			MemSet((byte*)g_Error, 0, g_ErrorMaxSize);

			InitLineFeed();
		}

#if (TIL_RUN_TARGET == TIL_TARGET_DEVEL)

		if (!g_Debug) 
		{
			g_Debug = new char[g_DebugMaxSize];
			MemSet((byte*)g_Debug, 0, g_DebugMaxSize);

			InitLineFeed();
		}

#endif

#if (TIL_PLATFORM == TIL_PLATFORM_WINDOWS)

		char path[TIL_MAX_PATH];
		char dir[TIL_MAX_PATH];

		GetModuleFileNameA(NULL, dir, TIL_MAX_PATH);
		char* lastslash = strrchr(dir, '\\');

		strncpy_s(path, TIL_MAX_PATH, dir, lastslash - dir + 1);
		path[lastslash - dir + 1] = 0;

		TIL_SetWorkingDirectory(path, strlen(path));

#else

		// do nothing, for now

#endif

	}

	void TIL_ShutDown()
	{
		if (g_WorkingDir)
		{
			delete g_WorkingDir;
			g_WorkingDir = NULL;
		}
		delete g_Error;       g_Error = NULL;
		delete g_ErrorTemp;   g_ErrorTemp = NULL;
		delete g_Debug;       g_Debug = NULL;
		delete g_DebugTemp;   g_DebugTemp = NULL;
		delete g_LineFeed;    g_LineFeed = NULL;
	}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	void AddErrorDefault(MessageData* a_Data)
	{
		sprintf_s(
			g_ErrorTemp,
			g_ErrorTempMaxSize,
			"%s (in file %s at line %i)", 
			a_Data->message, 
			a_Data->source_file, 
			a_Data->source_line
		);
		strcat_s(g_ErrorTemp, g_ErrorTempMaxSize, g_LineFeed);

		bool resize = false;
		while (strlen(g_ErrorTemp) + strlen(g_Error) >= g_ErrorMaxSize) { g_ErrorMaxSize *= 2; resize = true; }
		if (resize)
		{
			char* move = new char[g_ErrorMaxSize];
			strcpy_s(move, g_ErrorMaxSize, g_Error);
			g_Error = move;
		}

		strcat_s(g_Error, g_ErrorMaxSize, g_ErrorTemp);

		/*if (resize)
		{
			delete g_ErrorTemp;
			g_ErrorTemp = NULL;
		}*/
	}

#endif

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
		if (!g_ErrorTemp) { g_ErrorTemp = new char[g_ErrorTempMaxSize]; }
		vsprintf_s(g_ErrorTemp, g_ErrorTempMaxSize, a_Message, args);
		va_end(args);

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

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	void AddDebugDefault(MessageData* a_Data)
	{
		sprintf_s(g_DebugTemp, g_DebugTempMaxSize, "%s", a_Data->message);
		strcat_s(g_DebugTemp, g_DebugTempMaxSize, g_LineFeed);

		bool resize = false;

		while (strlen(g_DebugTemp) + strlen(g_Debug) >= g_DebugMaxSize) 
		{ 
			g_DebugMaxSize *= 2; 
			resize = true; 
		}
		if (resize)
		{
			g_Debug = (char*)realloc(g_Debug, g_DebugMaxSize);
		}

		if (strlen(g_DebugTemp) + strlen(g_Debug) >= g_DebugMaxSize) 
		{ 
			TIL_ClearDebug();
		}

		strcat_s(g_Debug, g_DebugMaxSize, g_DebugTemp);
	}

	void AddDebug(char* a_Message, char* a_File, int a_Line, ...)
	{
		va_list args;
		va_start(args, a_Line);
		if (!g_DebugTemp) { g_DebugTemp = new char[g_DebugTempMaxSize + 1]; }
		vsprintf_s(g_DebugTemp, g_DebugTempMaxSize, a_Message, args);
		va_end(args);

		g_Msg.message = g_DebugTemp;
		g_Msg.source_file = a_File;
		g_Msg.source_line = a_Line;
		g_DebugFunc(&g_Msg);
	}

#endif

	void TIL_GetVersion(char* a_Target, size_t a_MaxLength)
	{
		sprintf_s(a_Target, a_MaxLength, "%i.%i.%i", TIL_VERSION_MAJOR, TIL_VERSION_MINOR, TIL_VERSION_BUGFIX);
	}

	Image* TIL_Load(FileStream* a_Stream, uint32 a_Options)
	{
		// i don't know the path at this point
		// oh well, good luck, have fun
		if (!a_Stream)
		{
			return NULL;
		}

		char* filepath = a_Stream->GetFilePath();

		size_t end = strlen(filepath) - 4;
		if (end < 4)
		{
			TIL_ERROR_EXPLAIN("Filename isn't long enough.");
			return NULL;
		}

		Image* result = NULL;

		// lol hack
		if (0) { }
#if (TIL_FORMAT & TIL_FORMAT_PNG)
		else if (!strncmp(filepath + end, ".png", 4)) { result = new ImagePNG(); }
#endif
#if (TIL_FORMAT & TIL_FORMAT_GIF)
		else if (!strncmp(filepath + end, ".gif", 4)) { result = new ImageGIF(); }
#endif
#if (TIL_FORMAT & TIL_FORMAT_TGA)
		else if (!strncmp(filepath + end, ".tga", 4)) { result = new ImageTGA(); }
#endif
#if (TIL_FORMAT & TIL_FORMAT_BMP)
		else if (!strncmp(filepath + end, ".bmp", 4)) { result = new ImageBMP(); }
#endif
#if (TIL_FORMAT & TIL_FORMAT_ICO)
		else if (!strncmp(filepath + end, ".ico", 4)) { result = new ImageICO(); }
#endif
#if (TIL_FORMAT & TIL_FORMAT_DDS)
		else if (!strncmp(filepath + end, ".dds", 4)) { result = new ImageDDS(); }
#endif
		else
		{
			TIL_PRINT_DEBUG("Filename: '%s' (end: '%s')", filepath, filepath + end);
			TIL_ERROR_EXPLAIN("Can't parse file: unknown format.");
			a_Stream->Close();
			return NULL;
		}

		result->Load(a_Stream);

		if (!result->SetBPP(a_Options & TIL_DEPTH_MASK))
		{
			TIL_ERROR_EXPLAIN("Invalid bit-depth option: %i.", a_Options & TIL_DEPTH_MASK);
			delete result;
			a_Stream->Close();
			return NULL;
		}

		if (!result->Parse(a_Options & TIL_DEPTH_MASK))
		{
			TIL_ERROR_EXPLAIN("Could not parse file.");
			delete result;
			a_Stream->Close();
			return NULL;
		}
		else if (!result->Close())
		{
			TIL_ERROR_EXPLAIN("Could not close file.");
			delete result;
			return NULL;
		}

		return result;
	}

	Image* TIL_Load(const char* a_FileName, uint32 a_Options)
	{
		FileStream* load = g_FileFunc(a_FileName, a_Options & TIL_FILE_MASK);
		if (!load) 
		{
			TIL_ERROR_EXPLAIN("Could not find file '%s'.", a_FileName);
			return NULL;
		}

		return TIL_Load(load, a_Options);
	}

	size_t TIL_SetWorkingDirectory(const char* a_Path, size_t a_Length )
	{
		if (!g_WorkingDir)
		{
			g_WorkingDir = new char[a_Length + 1];
		}

		g_WorkingDirLength = strlen(a_Path);
		strcpy_s(g_WorkingDir, g_WorkingDirLength + 1, a_Path);

		return g_WorkingDirLength;
	}

	void TIL_AddWorkingDirectory( char* a_Dst, size_t a_MaxLength, const char* a_Path )
	{
		if (g_WorkingDir) 
		{ 
			strncpy_s(a_Dst, a_MaxLength, g_WorkingDir, g_WorkingDirLength); 
		}
		strcat_s(a_Dst, a_MaxLength, a_Path);
	}

	void TIL_SetFileStreamFunc(FileStreamFunc a_Func)
	{
		g_FileFunc = a_Func;
	}

	FileStream* OpenStreamDefault(const char* a_Path, uint32 a_Options)
	{
		FileStream* result = new FileStreamStd();
		if (result->Open(a_Path, a_Options)) { return result; }
		
		return NULL;
	}

	void TIL_ClearDebug()
	{
		if (g_Debug) { delete g_Debug; }
		g_DebugMaxSize = 2048;
		g_Debug = new char[g_DebugMaxSize];
		MemSet((byte*)g_Debug, 0, g_DebugMaxSize);
	}

}; // namespace til