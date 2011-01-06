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

#ifndef _TINYIMAGELOADER_H_
#define _TINYIMAGELOADER_H_

/*! \file TinyImageLoader.h
    \brief The main include for using TinyImageLoader in your project.
    
    Details.
*/


#include "TILSettings.h"
#include "TILImage.h"

namespace til
{

	//! Initializes TinyImageLoader. 
	/*! 
		\param a_Settings Bla bla.

		Starts TinyImageLoader and determines vital settings.
	*/
	extern void TIL_Init(uint32 a_Settings = TIL_SETTINGS);


	//! Main interface for loading images.
	extern Image* TIL_Load(const char* a_FileName, uint32 a_Options = (TIL_FILE_ABSOLUTEPATH | TIL_DEPTH_A8R8G8B8));

	extern size_t TIL_SetWorkingDirectory(const char* a_Path, size_t a_Length);

	//! Get the version as a string.
	/*!
		\param a_Target The string to write to.
		\param a_MaxLength The size of the destination buffer.

		Bla bla.
	*/
	extern void TIL_GetVersion(char* a_Target, size_t a_MaxLength);

	extern void TIL_SetErrorFunc(MessageFunc a_Func);
	extern char* TIL_GetError();
	extern size_t TIL_GetErrorLength();

	extern void TIL_SetDebugFunc(MessageFunc a_Func);
	extern char* TIL_GetDebug();
	extern size_t TIL_GetDebugLength();

}; // namespace til

#endif