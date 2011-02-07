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

/*! 
	\file TILSettings.h
	\brief Settings and global defines.

	Details
*/

// =========================================
// Defines
// =========================================

#define TIL_VERSION_MAJOR                 1
#define TIL_VERSION_MINOR                 5
#define TIL_VERSION_BUGFIX                0

#define TIL_PLATFORM_WINDOWS              0
#define TIL_PLATFORM_WINMO                1
#define TIL_PLATFORM_PSP                  2

//! The platform TinyImageLoader should be built for.
/*!
	If no platform was defined in the preprocessor, it's assumed to be on Windows.
*/
#ifndef TIL_PLATFORM
	#define TIL_PLATFORM                  TIL_PLATFORM_WINDOWS
#endif

#define TIL_DEBUG                         1
#define TIL_RELEASE                       2
#ifndef TIL_RUN_TARGET
	#define TIL_RUN_TARGET                TIL_RELEASE
#endif

#define TIL_FILE_MASK                     0x0000FFFF

//! The image path is absolute.
/*!
	Useful for loading images from a networked drive.

	\code
	til::Image* load = TIL_Load("\\my-share\\texture.png", TIL_DEPTH_A8B8G8R8 | TIL_FILE_ABSOLUTEPATH);
	\endcode
*/
#define TIL_FILE_ABSOLUTEPATH             0x00000001
//! The image path is relative to the working directory.
/*!
	The most common way to load an image.

	\code
	til::Image* load = TIL_Load("media\\texture.png", TIL_DEPTH_A8B8G8R8 | TIL_FILE_ADDWORKINGDIR);
	\endcode
*/
#define TIL_FILE_ADDWORKINGDIR            0x00000002
//! Add \\r\\n as the line ending
#define TIL_FILE_CRLF                     0x00000004
//! Add \\r as the line ending
#define TIL_FILE_CR                       0x00000008
//! Add \\n as the line ending
#define TIL_FILE_LF                       0x00000010

#define TIL_DEBUG_MASK                    0xFFFF0000
#define TIL_DEBUG_LOGGING                 0x00010000
#define TIL_DEBUG_TIMER                   0x00020000

//! Default settings for initialization
#ifndef TIL_SETTINGS
	#define TIL_SETTINGS                  (TIL_FILE_CRLF)
#endif

#define TIL_DEPTH_MASK                    0xFFFF0000
#define TIL_DEPTH_A8R8G8B8                0x00010000 //!< 32-bit ARGB color depth
#define TIL_DEPTH_A8B8G8R8                0x00020000 //!< 32-bit ABGR color depth
#define TIL_DEPTH_R8G8B8A8                0x00030000 //!< 32-bit RGBA color depth
#define TIL_DEPTH_B8G8R8A8                0x00040000 //!< 32-bit BGRA color depth
#define TIL_DEPTH_R8G8B8                  0x00050000 //!< 32-bit RGB color depth
#define TIL_DEPTH_R5G6B5                  0x00060000 //!< 16-bit RGB color depth

#define TIL_FORMAT_MASK                   0x0000FFFF
#define TIL_FORMAT_PNG                    0x00000001 //!< PNG format
#define TIL_FORMAT_GIF                    0x00000002 //!< GIF format
#define TIL_FORMAT_BMP                    0x00000004 //!< BMP format
#define TIL_FORMAT_TGA                    0x00000008 //!< TGA format
#define TIL_FORMAT_ICO                    0x00000010 //!< ICO format

//! Determine which formats should be included in compilation.
/*!
	Define this macro in the preprocessor definitions to overwrite the default.

	If a format is not included in the macro, its code is not compiled in.
	The following formats are standard:
	- PNG
	- GIF
	- TGA
	- BMP
	- ICO
*/
#ifndef TIL_FORMAT
	#define TIL_FORMAT                    (TIL_FORMAT_PNG | TIL_FORMAT_GIF | TIL_FORMAT_TGA | TIL_FORMAT_BMP | TIL_FORMAT_ICO)
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif

//! Print debug info
/*!
	If in release mode, no debug info is printed.	
*/
#if (TIL_RUN_TARGET == TIL_RELEASE)
	#define TIL_PRINT_DEBUG(msg, ...)
#else
	#define TIL_PRINT_DEBUG(msg, ...)  til::AddDebug("TinyImageLoader - Debug: "msg" ", __FILE__, __LINE__, __VA_ARGS__)
#endif
#define TIL_ERROR_EXPLAIN(msg, ...)    til::AddError("TinyImageLoader - Error: "msg" ", __FILE__, __LINE__, __VA_ARGS__)

#if (TIL_PLATFORM == TIL_PLATFORM_WINDOWS)
	#define TIL_MAX_PATH _MAX_PATH

	#if (TIL_RUN_TARGET == TIL_DEBUG)
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>

		#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
		#define new DEBUG_NEW
	#endif
#else
	#define TIL_MAX_PATH 256
#endif

namespace til
{

	// =========================================
	// Unsigned
	// =========================================

	typedef long long                         uint64; //!< 64-bit unsigned integer
	typedef unsigned long                     uint32; //!< 32-bit unsigned integer
	typedef unsigned short                    uint16; //!< 16-bit unsigned integer
	typedef unsigned char                     uint8;  //!< 8-bit unsigned integer

	// =========================================
	// Signed
	// =========================================

	typedef unsigned long long                int64;  //!< 64-bit signed integer
	typedef long                              int32;  //!< 32-bit signed integer
	typedef short                             int16;  //!< 16-bit signed integer
	typedef char                              int8;   //!< 8-bit signed integer

	// =========================================
	// Datatypes
	// =========================================

	typedef unsigned char                     byte;  //!< smallest chunk of data
	typedef unsigned short                    word;  //!< two bytes
	typedef unsigned long                     dword; //!< four bytes or two words

	// =========================================
	// Colors
	// =========================================

	typedef uint8                             color_8b;  //!< 8-bit color
	typedef uint16                            color_16b; //!< 16-bit color
	/** @cond IGNORE */
	typedef struct { uint8 d[3]; }            color_24b; //!< 24-bit color
	/** @endcond IGNORE */
	typedef uint32                            color_32b; //!< 32-bit color

	// =========================================
	// Function pointers
	// =========================================

	//! Message structure
	struct MessageData
	{
		char* message;      /**< Contains the message provided by TinyImageLoader. */
		char* source_file;  /**< The file where the message originated. */
		int source_line;    /**< The line the message came from. */
	};

	//! Message function
	/*! 
		\param MessageData* A pointer containing the message data.
	*/
	typedef void *MessageFunc(MessageData*);

	//! Memory allocation function
	typedef void* *MemAllocFunc(size_t a_Size, size_t* a_Allocated);
	//! Memory freeing function
	typedef void *MemFreeFunc(void* a_Free, size_t a_Size);

	// =========================================
	// Internal functions
	// =========================================

	extern void AddError(char* a_Message, char* a_File, int a_Line, ...);
	extern void AddDebug(char* a_Message, char* a_File, int a_Line, ...);

	extern void AddWorkingDirectory(char* a_Dst, size_t a_MaxLength, const char* a_Path);
	
}; // namespace til

#endif