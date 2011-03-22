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

/*!
	\file TILInternal.h
	\brief Internal functions
*/

#ifndef _TILINTERNAL_H_
#define _TILINTERNAL_H_

#include "TILSettings.h"

namespace til
{
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

	extern void TIL_AddWorkingDirectory(char* a_Dst, size_t a_MaxLength, const char* a_Path);

	class FileStream;

	//! Default FileStream function.
	/*!
		\param a_Path File path
		\param a_Options Options to load with

		\return FileStream handle

		\note Internal method.

		Opens a file and returns a FileStream handle. If unsuccessful, it returns NULL.
	*/
	extern FileStream* OpenStreamDefault(const char* a_Path, uint32 a_Options);

	//! Opens a FileStream.
	/*!
		\note Internal method.	

		This function pointer is called by implementations to get a handle to a file.
	*/
	static FileStreamFunc g_FileFunc = OpenStreamDefault;

	inline void MemCpy(uint8* a_Dst, uint8* a_Src, uint32 a_Size)
	{
		for (uint32 i = 0; i < a_Size; i++) { *a_Dst++ = *a_Src++; }
	}

	inline void MemSet(byte* a_Dst, byte a_Value, uint32 a_Size)
	{
		for (uint32 i = 0; i < a_Size; i++) { *a_Dst++ = a_Value; }
	}

	//@}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	#if (TIL_PLATFORM == TIL_PLATFORM_WINDOWS && TIL_RUN_TARGET == TIL_TARGET_DEVEL)
		#undef _CRTDBG_MAP_ALLOC
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>

		#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
		#define new DEBUG_NEW
	#endif
#endif

#endif