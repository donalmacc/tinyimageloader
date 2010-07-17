/*
	TinyImageLoader

	Image Format Template

	Note: this file isn't actually used in TinyImageLoader.
	You can copy this file to extend TinyImageLoader with new formats.
*/

// change these defines
#ifndef _TILIMAGETEMPLATE_H_
#define _TILIMAGETEMPLATE_H_

#include "TILImage.h"

// change this to your format
// or remove it if it should always be supported
#if (TIL_FORMAT & TIL_FORMAT_MYFORMAT)

namespace til
{

	struct TinyImageLoader;

	// change the class name
	class ImageTemplate : public Image
	{

		friend struct TinyImageLoader;

	public:

		// make sure you support these functions
		uint32 GetFrameCount();
		byte* GetPixels(uint32 a_Frame = 0);

	private:

		ImageTemplate();
		~ImageTemplate();

		// this one does all the magic
		// check the cpp for the correct implementation
		bool Parse(uint32 a_ColorDepth);

		// go wild, add as many members and functions as you need here

	}; // class ImageTemplate

}; // namespace til

#endif

#endif