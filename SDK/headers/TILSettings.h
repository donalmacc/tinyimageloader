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

#define TIL_VERSION_MAJOR                 1 //!< The version major
#define TIL_VERSION_MINOR                 5 //!< The version minor
#define TIL_VERSION_BUGFIX                5 //!< The bugfix version

#define TIL_PLATFORM_WINDOWS              0 //!< Windows platform
#define TIL_PLATFORM_WINMO                1 //!< Windows Mobile platform
#define TIL_PLATFORM_LINUX                2 //!< Linux platform
#define TIL_PLATFORM_PSP                  3 //!< PSP platform

//! The platform TinyImageLoader should be built for.
/*!
	If no platform was defined in the preprocessor, it's assumed to be on Windows.
*/
#ifndef TIL_PLATFORM
	#define TIL_PLATFORM                  TIL_PLATFORM_WINDOWS
#endif

#define TIL_DEBUG                         1 //!< Debug target
#define TIL_RELEASE                       2 //!< Release target
//! The target to build for
/*!
	If no target was set in the preprocessor, the library will compile for release mode.
*/
#ifndef TIL_RUN_TARGET
	#define TIL_RUN_TARGET                TIL_RELEASE
#endif

//! Internal define used to extract file options from the options
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

//! Internal define used to extract debug options from the options
#define TIL_DEBUG_MASK                    0xFFFF0000

//! Default settings for initialization
#ifndef TIL_SETTINGS
	#define TIL_SETTINGS                  (TIL_FILE_CRLF)
#endif

//! Internal define used to extract color depth options from the options
#define TIL_DEPTH_MASK                    0xFFFF0000
#define TIL_DEPTH_A8R8G8B8                0x00010000 //!< 32-bit ARGB color depth
#define TIL_DEPTH_A8B8G8R8                0x00020000 //!< 32-bit ABGR color depth
#define TIL_DEPTH_R8G8B8A8                0x00030000 //!< 32-bit RGBA color depth
#define TIL_DEPTH_B8G8R8A8                0x00040000 //!< 32-bit BGRA color depth
#define TIL_DEPTH_R8G8B8                  0x00050000 //!< 32-bit RGB color depth
#define TIL_DEPTH_R5G6B5                  0x00060000 //!< 16-bit RGB color depth

//! Internal define used to extract format options from the options
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

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif

#endif

/*!
	\def TIL_PRINT_DEBUG
	\brief Print a debug message
	If in release mode, no debug info is printed.

	\def TIL_ERROR_EXPLAIN
	\brief Prints an error message
	Error messages are always posted, even in release mode.
*/
#if (TIL_RUN_TARGET == TIL_RELEASE)
	#define TIL_PRINT_DEBUG(msg, ...)
#else
	#define TIL_PRINT_DEBUG(msg, ...)  til::AddDebug("TinyImageLoader - Debug: "msg" ", __FILE__, __LINE__, __VA_ARGS__)
#endif
#define TIL_ERROR_EXPLAIN(msg, ...)    til::AddError("TinyImageLoader - Error: "msg" ", __FILE__, __LINE__, __VA_ARGS__)

/*!
	\def TIL_MAX_PATH
	The maximum path length for the platform
*/
#if (TIL_PLATFORM == TIL_PLATFORM_WINDOWS)
	#define TIL_MAX_PATH _MAX_PATH

	// used for detecting memory leaks
	#ifdef _DEBUG
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
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	typedef struct { uint8 d[3]; }            color_24b; //!< 24-bit color
#endif
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

		Message functions are used for logging. You can create your own and attach them to TinyImageLoader.
	*/
	typedef void (*MessageFunc)(MessageData*);

	//! Memory allocation function
	typedef void* (*MemAllocFunc)(size_t a_Size, size_t* a_Allocated);
	//! Memory freeing function
	typedef void (*MemFreeFunc)(void* a_Free, size_t a_Size);

	// =========================================
	// Internal functions
	// =========================================

	/*!
		@name Internal
		These functions are internal and shouldn't be called by developers.
	*/
	//@{

	//! Adds an error to the logging stack
	/*!
		\param a_Message The message to post
		\param a_File The file it originated from
		\param a_Line The line it originated from

		\note Internal method. 

		An implementation of printf for errors. 
		The parameters in a_Message are parsed and sent to the attached logging function.
		This can be the internal logger or one of your own.
	*/
	extern void AddError(char* a_Message, char* a_File, int a_Line, ...);
	//! Adds a debug message to the logging stack
	/*!
		\param a_Message The message to post
		\param a_File The file it originated from
		\param a_Line The line it originated from

		\note Internal method. 

		An implementation of printf for debug messages. 
		The parameters in a_Message are parsed and sent to the attached logging function.
		This can be the internal logger or one of your own.
	*/
	extern void AddDebug(char* a_Message, char* a_File, int a_Line, ...);

	//! Adds working directory to a path
	/*!
		\param a_Dst Where to put it
		\param a_MaxLength The length of the destination buffer
		\param a_Path The path to add

		\note Internal method. 
		
		Adds the working directory to a path and puts it in the destination buffer.
		If the destination buffer is too small, the path is truncated to fit.
	*/
	extern void AddWorkingDirectory(char* a_Dst, size_t a_MaxLength, const char* a_Path);

	//@}
	
}; // namespace til

#endif