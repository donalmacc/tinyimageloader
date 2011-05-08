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

#include "TILPixelData.h"

#include "TILInternal.h"

namespace til
{

	PixelData::PixelData(uint32 a_Width, uint32 a_Height, uint8 a_BPP)
	{
		m_Data = NULL;
		m_Width = a_Width;
		m_Height = a_Height;
		m_OffsetX = 0;
		m_OffsetY = 0;
		m_BPP = a_BPP;
	}

	PixelData::~PixelData()
	{
		if (m_Data) { delete m_Data; }
	}

	void PixelData::Init()
	{
		// standard implementation
		// the pitch is the same as the dimensions

		m_PitchHor = m_Width;
		m_PitchVer = m_Height;
	}

	bool PixelData::CreateData()
	{
		Init();

		if (m_PitchHor * m_PitchVer < m_Width * m_Height)
		{
			TIL_ERROR_EXPLAIN("Pixel buffer not big enough for data. (%i vs %i)", m_PitchHor * m_PitchVer, m_Width * m_Height);
			return false;
		}

		m_Data = new byte[m_PitchHor * m_BPP * m_PitchVer];
		return true;
	}

	bool PixelData::Clear()
	{
		// standard implementation
		// clears all pixels

		memset(m_Data, 0, m_PitchHor * m_BPP * m_PitchVer);
		return true;
	}

	bool PixelData::CopyFrom(PixelData* a_Other)
	{
		memcpy(m_Data, a_Other->GetData(), a_Other->GetSizeInBytes());
		return true;
	}

};