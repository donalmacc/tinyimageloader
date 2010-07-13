#ifndef _TILIMAGEGIF_H_
#define _TILIMAGEGIF_H_

#include "TILImage.h"

#if (TIL_FORMAT & TIL_FORMAT_GIF)

namespace til
{

	struct BufferLinked
	{
		byte* buffer;
		BufferLinked* next;
	};

	struct TinyImageLoader;

	class ImageGIF : public Image
	{

		friend struct TinyImageLoader;

	public:

		bool Parse(uint32 a_ColorDepth);

		uint32 GetFrameCount();
		byte* GetPixels(uint32 a_Frame = 0);

		void AddBuffer();
		void CompileColors();

	private:

		ImageGIF();
		~ImageGIF();

		BufferLinked* m_First;
		BufferLinked* m_Current;
		uint32 m_Frames;

		byte m_Buffer[256];
		uint32 m_ColorDepth;
		uint32 m_ColorTableSize;
		char* m_Palette;
		byte* m_Colors;
		
	}; // class ImageGIF

}; // namespace til

#endif

#endif