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

#ifndef _TILSETTINGS_H_
#define _TILSETTINGS_H_

// =========================================
// Defines
// =========================================

#define TIL_VERSION_MAJOR                 1
#define TIL_VERSION_MINOR                 5
#define TIL_VERSION_BUGFIX                0

#define TIL_PLATFORM_WINDOWS              0
#define TIL_PLATFORM_WINMO                1
#define TIL_PLATFORM_PSP                  2

#ifndef TIL_PLATFORM
	#define TIL_PLATFORM                  TIL_PLATFORM_WINDOWS
#endif

#define TIL_FILE_MASK                     0x0000FFFF
#define TIL_FILE_ABSOLUTEPATH             0x00000001
#define TIL_FILE_ADDWORKINGDIR            0x00000002
#define TIL_FILE_CRLF                     0x00000004
#define TIL_FILE_CR                       0x00000008
#define TIL_FILE_LF                       0x00000010

#define TIL_DEBUG_MASK                    0xFFFF0000
#define TIL_DEBUG_LOGGING                 0x00010000
#define TIL_DEBUG_TIMER                   0x00020000

#ifndef TIL_SETTINGS
	#define TIL_SETTINGS                  (TIL_FILE_CRLF | TIL_DEBUG_LOGGING | TIL_DEBUG_TIMER)
#endif

#define TIL_DEPTH_MASK                    0xFFFF0000
#define TIL_DEPTH_A8R8G8B8                0x00010000
#define TIL_DEPTH_A8B8G8R8                0x00020000
#define TIL_DEPTH_R8G8B8A8                0x00030000
#define TIL_DEPTH_B8G8R8A8                0x00040000
#define TIL_DEPTH_R8G8B8                  0x00050000
#define TIL_DEPTH_R5G6B5                  0x00060000

#define TIL_FORMAT_MASK                   0x0000FFFF
#define TIL_FORMAT_PNG                    0x00000001
#define TIL_FORMAT_GIF                    0x00000002
#define TIL_FORMAT_BMP                    0x00000004
#define TIL_FORMAT_TGA                    0x00000008
#define TIL_FORMAT_ICO                    0x00000010

#ifndef TIL_FORMAT
	#define TIL_FORMAT                    (TIL_FORMAT_PNG | TIL_FORMAT_GIF | TIL_FORMAT_TGA | TIL_FORMAT_BMP | TIL_FORMAT_ICO)
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef TIL_RELEASE
	#define TIL_PRINT_DEBUG(msg, ...)
	#define	TIL_ERROR_EXPLAIN(msg, ...)    til::AddError("TinyImageLoader - Error: "msg" ", __FILE__, __LINE__, __VA_ARGS__)
#else
	#define TIL_PRINT_DEBUG(msg, ...)      til::AddDebug("TinyImageLoader - Debug: "msg" ", __FILE__, __LINE__, __VA_ARGS__)
	#define TIL_ERROR_EXPLAIN(msg, ...)    til::AddError("TinyImageLoader - Error: "msg" ", __FILE__, __LINE__, __VA_ARGS__)
#endif

#if (TIL_PLATFORM == TIL_PLATFORM_WINDOWS)
	#define TIL_MAX_PATH _MAX_PATH
#else
	#define TIL_MAX_PATH 256
#endif

namespace til
{

	// =========================================
	// Unsigned
	// =========================================

	typedef long long                         uint64;
	typedef unsigned long                     uint32;
	typedef unsigned short                    uint16;
	typedef unsigned char                     uint8;

	// =========================================
	// Signed
	// =========================================

	typedef unsigned long long                int64;
	typedef long                              int32;
	typedef short                             int16;
	typedef char                              int8;

	typedef unsigned long                     COLORDATA;

	// =========================================
	// Datatypes
	// =========================================

	typedef unsigned char                     byte;
	typedef unsigned short                    word;
	typedef unsigned long                     dword;

	// =========================================
	// Colors
	// =========================================

	typedef uint8                             color_8b;
	typedef uint16                            color_16b;
	typedef struct { uint8 d[3]; }            color_24b;
	typedef uint32                            color_32b;

	// =========================================
	// Function pointers
	// =========================================

	struct MessageData
	{
		char* message;
		char* source_file;
		int source_line;
	};
	typedef void (*MessageFunc)(MessageData*);

	// =========================================
	// Internal functions
	// =========================================

	extern void AddError(char* a_Message, char* a_File, int a_Line, ...);
	extern void AddDebug(char* a_Message, char* a_File, int a_Line, ...);

	extern void AddWorkingDirectory(char* a_Dst, size_t a_MaxLength, const char* a_Path);
	
}; // namespace til

#endif