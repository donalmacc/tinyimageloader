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

#ifndef _TILIMAGE_H_
#define _TILIMAGE_H_

#include "TILSettings.h"

#include <stdio.h>
#include <string.h>

namespace til
{

	inline color_16b Construct_16b_R5G6B5(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha = 0)
	{
		return
			(((a_Red   * 0xF800) >> 8) & 0xF800) |
			(((a_Green * 0x07E0) >> 8) & 0x07E0) |
			(((a_Blue  * 0x001F) >> 8) & 0x001F);
	}

	typedef color_32b (*color_func_32b)(uint8, uint8, uint8, uint8);
	typedef color_16b (*color_func_16b)(uint8, uint8, uint8, uint8);

	inline color_32b Construct_32b_R8G8B8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha = 0)
	{
		return
			((a_Red   & 0xFF) << 16) |
			((a_Green & 0xFF) << 8) |
			((a_Blue  & 0xFF));
	}

	inline color_32b Construct_32b_R8G8B8(color_8b a_Color)
	{
		
		color_32b r = (a_Color & 0xD0) >> 5;
		color_32b g = (a_Color & 0x1C) >> 2;
		color_32b b = (a_Color & 0x03);	

		return (
			(((r * 0x0000FF00) >> 8) & 0x000000FF) |
			(((g * 0x00FF0000) >> 8) & 0x0000FF00) |
			(((b * 0xFF000000) >> 8) & 0x00FF0000)
		);

		/*return
			((a_Red   & 0xFF) << 16) |
			((a_Green & 0xFF) << 8) |
			((a_Blue  & 0xFF));*/
	}

	inline color_32b Construct_32b_A8B8G8R8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		return (a_Alpha << 24) | (a_Blue << 16) | (a_Green << 8) | (a_Red);
	}

	inline color_32b Construct_32b_A8R8G8B8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		return (a_Alpha << 24) | (a_Red << 16) | (a_Green << 8) | (a_Blue);
	}

	inline color_32b Construct_32b_B8G8R8A8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		return (a_Blue << 24) | (a_Green << 16) | (a_Red << 8) | (a_Alpha);
	}

	inline color_32b Construct_32b_R8G8B8A8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		return (a_Red << 24) | (a_Green << 16) | (a_Blue << 8) | (a_Alpha);

		/*return
			((a_Red   & 0xFF) << 24) |
			((a_Green & 0xFF) << 16) |
			((a_Blue  & 0xFF) << 8 ) |
			((a_Alpha & 0xFF));*/
	}


	inline color_32b AlphaBlend_32b_R8G8B8(color_32b a_Color, uint8 a_Amount)
	{
		const color_32b rb = (a_Color & 0x00FF00FF);
		const color_32b g = (a_Color & 0x0000FF00);

		return (
			(((rb * a_Amount) >> 8) & 0x00FF00FF) | 
			(((g * a_Amount) >> 8) & 0x0000FF00)
		);
	}

	inline color_32b AlphaBlend_32b_R8G8B8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		const color_32b rb  = ((a_Red << 16) | (a_Blue))  & 0x00FF00FF;
		const color_32b g   = (a_Green << 8)              & 0x0000FF00;

		return (
			(((rb * a_Alpha) >> 8) & 0x00FF00FF) | 
			(((g  * a_Alpha) >> 8) & 0x0000FF00)
		);
	}

	inline color_32b AlphaBlend_32b_A8R8G8B8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		const color_32b rb  = ((a_Red << 16) | (a_Blue))  & 0x00FF00FF;
		const color_32b g   = (a_Green << 8)              & 0x0000FF00;

		return (
			((a_Alpha << 24)       & 0xFF000000) |
			(((rb * a_Alpha) >> 8) & 0x00FF00FF) | 
			(((g  * a_Alpha) >> 8) & 0x0000FF00)
		);
	}

	inline color_32b AlphaBlend_32b_A8B8G8R8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		const color_32b br  = ((a_Blue << 16) | (a_Red)) & 0x00FF00FF;
		const color_32b g   = (a_Green << 8)             & 0x0000FF00;

		const color_32b a    = (a_Alpha << 24) & 0xFF000000; 
		const color_32b br_a = ((br * a_Alpha) >> 8) & 0x00FF00FF;
		const color_32b g_a  = ((g  * a_Alpha) >> 8) & 0x0000FF00;

		return (a | br_a | g_a);

		/*return (
			((a_Alpha << 24)       & 0xFF000000) |
			(((br * a_Alpha) >> 8) & 0x00FF00FF) | 
			(((g  * a_Alpha) >> 8) & 0x0000FF00)
		);*/
	}

	inline color_32b AlphaBlend_32b_B8G8R8A8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		const color_32b br  = ((a_Blue << 16) | (a_Red)) & 0xFF00FF;
		const color_32b g   = (a_Green << 8);

		const color_32b br_a = (br * a_Alpha) & 0xFF00FF00;
		const color_32b g_a  = (g  * a_Alpha) & 0x00FF0000;
		const color_32b a    = a_Alpha        & 0x000000FF;

		return br_a | g_a | a;

		/*return (
			(((rb * a_Alpha) >> 8) & 0xFF00FF00) | 
			(((g  * a_Alpha) >> 8) & 0x00FF0000) |
			(a_Alpha               & 0x000000FF)
		);*/
	}


	inline color_32b AlphaBlend_32b_R8G8B8A8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		const color_32b rb  = ((a_Red << 16) | (a_Blue)) & 0xFF00FF;
		const color_32b g   = (a_Green << 8);

		return (
			((rb * a_Alpha) & 0xFF00FF00) | 
			((g  * a_Alpha) & 0x00FF0000) | 
			(a_Alpha        & 0x000000FF)
		);

		/*const color_32b rb  = ((a_Red  << 24) | (a_Blue << 8)) & 0xFF00FF00;
		const color_32b g   = (a_Green << 16)                  & 0x00FF0000;

		return (
			(((rb * a_Alpha) >> 8) & 0xFF00FF00) | 
			(((g  * a_Alpha) >> 8) & 0x00FF0000) |
			(a_Alpha               & 0x000000FF)
		);*/
	}

	inline color_16b AlphaBlend_16b_R5G6B5(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		color_16b r = (((a_Red   * 0xF800) >> 8) & 0xF800);
		color_16b g = (((a_Green * 0x07E0) >> 8) & 0x07E0);
		color_16b b = (((a_Blue  * 0x001F) >> 8) & 0x001F);

		return (
			(((r * a_Alpha) >> 8) & 0xF800) |
			(((g * a_Alpha) >> 8) & 0x07E0) |
			(((b * a_Alpha) >> 8) & 0x001F)
		);
	}

	class Image
	{

	public:

		enum BitDepth
		{
			BPP_32B_A8R8G8B8 = 1,
			BPP_32B_A8B8G8R8 = 2,
			BPP_32B_R8G8B8A8 = 3,
			BPP_32B_B8G8R8A8 = 4,
			BPP_32B_R8G8B8   = 5,
			BPP_16B_R5G6B5   = 6,
		};

		Image();
		~Image();

		void SetBPP(uint32 a_Options);
	
		bool Load(const char* a_FileName, uint32 a_Options = TIL_FILE_ABSOLUTEPATH);
		virtual bool Parse(uint32 a_ColorDepth) { return false; }
		bool Close();

		virtual uint32 GetFrameCount() { return 1; }
		virtual float GetDelay() { return 0; }
		virtual byte* GetPixels(uint32 a_Frame = 0) = 0;

		virtual uint32 GetWidth(uint32 a_Frame = 0) = 0;
		virtual uint32 GetHeight(uint32 a_Frame = 0) = 0;
		BitDepth GetBitDepth() { return m_BPPIdent; }

	protected:

		FILE* m_Handle;
		char* m_FileName;
		BitDepth m_BPPIdent;
		uint8 m_BPP;

	}; // class Image

}; // namespace til

#endif