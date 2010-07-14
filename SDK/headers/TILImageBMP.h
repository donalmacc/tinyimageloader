#ifndef _TILIMAGEBMP_H_
#define _TILIMAGEBMP_H_

#include "TILImage.h"

#if (TIL_FORMAT & TIL_FORMAT_BMP)

namespace til
{

	class ImageBMP : public Image
	{

		enum ColorType
		{
			COLOR_BLACKANDWHITE,
			COLOR_MAPPED,
			COLOR_TRUECOLOR
		};

		enum Header
		{
			HDR_OS2V1 = 12,
			HDR_OS2V2 = 64,
			HDR_WINDOWSV3 = 40,
			HDR_WINDOWSV4 = 108,
			HDR_WINDOWSV5 = 124
		};

		enum Compression
		{
			COMP_RGB = 0,
			COMP_RLE8 = 1,
			COMP_RLE4 = 2,
			COMP_BITFIELDS = 3,
			COMP_JPEG = 4,
			COMP_PNG = 5
		};

	public:

		ImageBMP();
		~ImageBMP();

		bool Parse(uint32 a_ColorDepth);

		uint32 GetFrameCount();
		byte* GetPixels(uint32 a_Frame = 0);

	private:

		dword GetDWord();

		byte m_Data[4];
		byte* m_Pixels;
		byte* m_Target;

	}; // class ImageTGA

}; // namespace til

#endif

#endif