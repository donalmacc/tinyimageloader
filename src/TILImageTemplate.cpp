// note: the header is one directory up, in the SDK\headers folder
#include "..\SDK\headers\TILImageTemplate.h"

// change this to your format
// or remove it if it should always be supported
#if (TIL_FORMAT & TIL_FORMAT_MYFORMAT)

// change this to your format
#ifdef TIL_PRINT_DEBUG
	#define TEMPLATE_DEBUG(msg, ...)        TIL_PRINT_DEBUG("Template: "msg, __VA_ARGS__)
#endif

namespace til
{

	ImageTemplate::ImageTemplate()
	{
		// go wild
	}

	ImageTemplate::~ImageTemplate()
	{
		// probably needs to be empty
	}

	bool ImageTemplate::Parse(uint32 a_ColorDepth)
	{
		// if you want to output debug info, use the *_DEBUG macro, like so:
	
		/*
			TEMPLATE_DEBUG("Is this thing on?");
		*/

		// m_Handle is a valid FILE* that starts at the beginning of the file
	
		/*
			dword header;         fread(&header, 4, 1, m_Handle);
		*/

		// if anything goes wrong, you use TIL_ERROR_EXPLAIN to explain what went wrong
		// and return false

		/*
			if (header != 0xAABBCCDD)
			{
				TIL_ERROR_EXPLAIN("File is of the wrong format.");
				return false;
			}
		*/

		// before you can begin parsing, a bit about colors:

		// m_BPP has already been set for you, which is the amount of bytes per pixel
		// this is either 4 (R8G8B8, A8R8G8B8 or equivalent) or 2 (R5G6B5 or equivalent)

		// you need to make sure you have enough space for your pixel data
		// usually you will have a byte* called m_Pixels
		// but this isn't necessarily the case with image formats that contain multiple images

		/*
			m_Pixels = new byte[m_Width * m_Height * m_BPP];
		*/

		// a_ColorDepth tells you the color format, these are the TIL_DEPTH_* defines

		/*
			switch (a_ColorDepth)
			{
				case TIL_DEPTH_A8R8G8B8:
					break;
				case TIL_DEPTH_R5G6B5:
					break;
				default:
					break;
			}
		*/

		// now you can begin parsing your image

		/*
			PARSE HERE
		*/

		// if everything went well, return true

		// because this function is not yet implemented we'll return false

		return false;
	}

	uint32 ImageTemplate::GetFrameCount()
	{
		// return the amount of frames your data contains
		// this is only necessary for formats that contain multiple images
		// otherwise, return 1

		return 1;
	}

	byte* ImageTemplate::GetPixels(uint32 a_Frame /*= 0*/)
	{
		// return your pixel data
		// if an invalid parameter has been specified, return NULL
		// otherwise return a pointer to your data in byte's that has a length of:
		//
		// width * height * bytesperpixel
		//
		// bytes.

		return NULL;
	}

}; // namespace til

#endif