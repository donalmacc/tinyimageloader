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

	struct TinyImageLoader;

	class ImageICO : public Image
	{

		friend struct TinyImageLoader;

	public:

		uint32 GetFrameCount();
		byte* GetPixels(uint32 a_Frame = 0);

		uint32 GetWidth(uint32 a_Frame = 0);
		uint32 GetHeight(uint32 a_Frame = 0);

	private:

		ImageICO();
		~ImageICO();

		bool Parse(uint32 a_ColorDepth);

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