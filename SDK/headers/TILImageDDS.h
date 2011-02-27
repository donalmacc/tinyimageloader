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

#ifndef _TILIMAGEDDS_H_
#define _TILIMAGEDDS_H_

#include "TILImage.h"

#if (TIL_FORMAT & TIL_FORMAT_DDS)

namespace til
{

	class ImageDDS : public Image
	{

	public:

		enum EnumFormat
		{
			FormatDXT1,
			FormatDXT3,
			FormatDXT5
		};

		enum EnumDepth
		{
			DepthRGB,
			DepthARGB,
			DepthBGR,
			DepthBGRA
		};

		ImageDDS();
		~ImageDDS();

		// make sure you support these functions
		uint32 GetFrameCount();
		byte* GetPixels(uint32 a_Frame = 0);

		uint32 GetWidth(uint32 a_Frame = 0);
		uint32 GetHeight(uint32 a_Frame = 0);

		bool Parse(uint32 a_ColorDepth);

	private:
		
		void AllocateSpace();
		void DecompressDXT1();

		uint32 m_Width, m_Height, m_Depth;
		uint32 m_MipMaps;
		bool m_CubeMap;
		EnumFormat m_Format;
		uint32 m_InternalDepth, m_InternalBPP;
		uint32 m_Size, m_Blocks;

		byte* m_Data;
		byte* m_Pixels;

	}; // class ImageDDS

}; // namespace til

#endif

#endif