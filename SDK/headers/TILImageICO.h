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

#ifndef _TILIMAGEICO_H_
#define _TILIMAGEICO_H_

#include "TILImage.h"

#if (TIL_FORMAT & TIL_FORMAT_ICO)

namespace til
{

	struct BufferICO
	{
		byte* buffer;
		uint32 width, height;
		uint32 datasize, offset;
		uint32 bytespp, bitspp;
		BufferICO* next;
	};

	class ImageICO : public Image
	{

	public:

		ImageICO();
		~ImageICO();

		bool Parse(uint32 a_ColorDepth);

		uint32 GetFrameCount();
		byte* GetPixels(uint32 a_Frame = 0);

		uint32 GetWidth(uint32 a_Frame = 0);
		uint32 GetHeight(uint32 a_Frame = 0);

	private:

		void AddBuffer(uint32 a_Width, uint32 a_Height);

		uint32 m_Images;

		byte* m_Pixels;
		uint32 m_Width, m_Height, m_Pitch;

		BufferICO* m_First;
		BufferICO* m_Current;

	}; // class ImageICO

}; // namespace til

#endif

#endif