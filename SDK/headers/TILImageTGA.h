#ifndef _TILIMAGETGA_H_
#define _TILIMAGETGA_H_

#include "TILImage.h"

#if (TIL_FORMAT & TIL_FORMAT_TGA)

namespace til
{

	class ImageTGA : public Image
	{

		enum ColorType
		{
			COLOR_BLACKANDWHITE,
			COLOR_MAPPED,
			COLOR_TRUECOLOR
		};
		
		enum Compression
		{
			COMP_NONE,
			COMP_RLE
		};

	public:

		ImageTGA();
		~ImageTGA();

		bool CompileUncompressed();
		bool CompileRunLengthEncoded();

		bool Parse(uint32 a_ColorDepth);

		uint32 GetFrameCount();
		byte* GetPixels(uint32 a_Frame = 0);
		
	private:

		ColorType m_Type;
		Compression m_Comp;
		byte* m_Pixels;
		byte* m_Target;
		byte m_Src[4];

	}; // class ImageTGA

}; // namespace til

#endif

#endif