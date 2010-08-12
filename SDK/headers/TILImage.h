/*
    TinyImageLoader - load images, just like that
    Copyright (C) 2010 Quinten Lansu (knight666)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

	inline color_32b Construct_32b_A8R8G8B8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		return
			((a_Alpha & 0xFF) << 24) |
			((a_Red   & 0xFF) << 16) |
			((a_Green & 0xFF) << 8 ) |
			((a_Blue  & 0xFF));
	}

	inline color_32b Construct_32b_R8G8B8A8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		return
			((a_Red   & 0xFF) << 24) |
			((a_Green & 0xFF) << 16) |
			((a_Blue  & 0xFF) << 8 ) |
			((a_Alpha & 0xFF));
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

	struct TinyImageLoader;

	class Image
	{

		friend struct TinyImageLoader;

	public:

		enum BitDepth
		{
			BPP_32B_A8R8G8B8 = 1,
			BPP_32B_R8G8B8A8 = 2,
			BPP_32B_R8G8B8 = 3,
			BPP_16B_R5G6B5 = 4,
		};
	
		bool Load(const char* a_FileName, uint32 a_Options = TIL_FILE_ABSOLUTEPATH);
		virtual bool Parse(uint32 a_ColorDepth) { return false; }

		virtual uint32 GetFrameCount() { return 1; }
		virtual float GetDelay() { return 0; }
		virtual byte* GetPixels(uint32 a_Frame = 0) = 0;

		virtual uint32 GetWidth(uint32 a_Frame = 0) = 0;
		virtual uint32 GetHeight(uint32 a_Frame = 0) = 0;
		BitDepth GetBitDepth() { return m_BPPIdent; }

	protected:

		Image();
		~Image();

		void SetBPP(uint32 a_Options);

		FILE* m_Handle;
		char* m_FileName;
		BitDepth m_BPPIdent;
		uint8 m_BPP;

	}; // class Image

}; // namespace til

#endif