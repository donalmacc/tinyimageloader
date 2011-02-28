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

#ifndef _TILFILESTREAM_H_
#define _TILFILESTREAM_H_

#include "TILSettings.h"
#include "TILInternal.h"

namespace til
{

	class FileStream
	{
	
	public:
	
		FileStream();
		~FileStream();
	
		virtual bool Open(const char* a_File, uint32 a_Options) = 0;

		virtual bool Read(void* a_Dst, uint32 a_ElementSize, uint32 a_Count = 1) = 0;
		virtual bool ReadByte(byte* a_Dst, uint32 a_Count = 1) = 0;
		virtual bool ReadWord(word* a_Dst, uint32 a_Count = 1) = 0;
		virtual bool ReadDWord(dword* a_Dst, uint32 a_Count = 1) = 0;

		virtual bool Seek(uint32 a_Bytes, uint32 a_Options) = 0;

		virtual bool EndOfFile() = 0;

		virtual bool Close() = 0;

	protected:

		char* m_FilePath;
	
	}; // class FileStream

}; // namespace til
	
#endif