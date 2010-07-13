#ifndef _TILIMAGE_H_
#define _TILIMAGE_H_

#include "TILSettings.h"

#include <stdio.h>
#include <string.h>

namespace til
{

	inline color_16b Construct_16b_R5G6B5(uint8 a_Red, uint8 a_Green, uint8 a_Blue)
	{
		return
			(((a_Red   * 0xF800) >> 8) & 0xF800) |
			(((a_Green * 0x07E0) >> 8) & 0x07E0) |
			(((a_Blue  * 0x001F) >> 8) & 0x001F);
	}

	inline color_32b Construct_32b_R8G8B8(uint8 a_Red, uint8 a_Green, uint8 a_Blue)
	{
		return
			((a_Red   & 0xFF) << 16) |
			((a_Green & 0xFF) << 8) |
			((a_Blue  & 0xFF));
	}

	inline color_32b Construct_32b_A8R8G8B8(uint8 a_Red, uint8 a_Green, uint8 a_Blue, uint8 a_Alpha)
	{
		return
			0xFF000000 |
			((a_Red   & 0xFF) << 16) |
			((a_Green & 0xFF) << 8) |
			((a_Blue  & 0xFF));
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
		virtual byte* GetPixels(uint32 a_Frame = 0) = 0;

		uint32 GetWidth() { return m_Width; }
		uint32 GetHeight() { return m_Height; }
		BitDepth GetBitDepth() { return m_BPPIdent; }

	protected:

		Image();
		~Image();

		void SetBPP(uint32 a_Options);

		FILE* m_Handle;
		char* m_FileName;
		uint32 m_Width, m_Height, m_Pitch;
		BitDepth m_BPPIdent;
		uint8 m_BPP;

	}; // class Image

}; // namespace til

#endif