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

#ifndef _TILPIXELDATA_H_
#define _TILPIXELDATA_H_

#include "TILSettings.h"

namespace til
{

	class PixelData
	{
	
	public:
	
		PixelData(uint32 a_Width, uint32 a_Height, uint8 a_BPP);
		~PixelData();

		virtual void Init();

		bool CreateData();

		virtual bool Clear();
		virtual bool CopyFrom(PixelData* a_Other);

		inline byte* GetData() { return m_Data; }
		inline uint32 GetOffsetX() { return m_OffsetX; }
		inline uint32 GetOffsetY() { return m_OffsetY; }

		inline uint32 GetWidth() { return m_Width; }
		inline uint32 GetHeight() { return m_Height; }
		inline uint32 GetSizeHor() { return m_PitchHor * m_BPP; }
		inline uint32 GetSizeVer() { return m_PitchVer * m_BPP; }
		inline uint32 GetPitchHor() { return m_PitchHor; }
		inline uint32 GetPitchVer() { return m_PitchVer; }

		//inline uint32 GetStartByte() { return m_OffsetY * m_PitchHor * m_BPP; }
		//inline uint32 GetNextLineByte() { return m_PitchHor * m_BPP; }
		inline uint32 GetSizeInBytes() { return m_PitchHor * m_BPP * m_PitchVer; }
	
	protected:

		byte* m_Data;
		uint8 m_BPP;
		uint32 m_Width, m_Height;
		uint32 m_OffsetX, m_OffsetY;
		uint32 m_PitchHor, m_PitchVer;
	
	}; // class ImageData

}; // namespace til
	
#endif