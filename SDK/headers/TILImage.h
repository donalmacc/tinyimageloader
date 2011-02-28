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

/*!
	\file TILImage.h
	\brief Image interface
*/

#include "TILSettings.h"
#include "TILInternal.h"
#include "TILColors.h"
#include "TILFileStream.h"

#include <stdio.h>
#include <string.h>

namespace til
{

	// very strange doxygen workaround
	//; // do not remove!

	// this seemingly pointless forward declaration
	// is necessary to fool doxygen into documenting
	// the class
	class DoxygenSaysWhat;

	//! Construct a 32-bit RGB color from an 8-bit RGB color
	/*!
		\param a_Color An 8-bit RGB color

		\return 32-bit color
	*/
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
	}

	/*!
		\brief The virtual interface for loading images and extracting image data.

		All image operations go through this interface. Every image loader is an implementation of this class.
	*/
	class Image
	{

	public:

		//! The amount of bits per pixel and its arrangement.
		enum BitDepth
		{
			BPP_32B_A8R8G8B8 = 1, /**< 32-bit ARGB color */
			BPP_32B_A8B8G8R8 = 2, /**< 32-bit ABGR color */
			BPP_32B_R8G8B8A8 = 3, /**< 32-bit RGBA color */
			BPP_32B_B8G8R8A8 = 4, /**< 32-bit BGRA color */
			BPP_32B_R8G8B8   = 5, /**< 32-bit RGB color */
			BPP_16B_R5G6B5   = 6, /**< 16-bit RGB color */
		};

		Image();
		virtual ~Image();

		//! Sets the bit depth to convert to when parsing.
		void SetBPP(uint32 a_Options);
	
		//! Loads a filepath using a series of options.
		/*!
			\param a_FileName A path to an image file.
			\param a_Options A number of options bitwise or'd together.

			\return A boolean indicating success or failure.

			The main entrypoint for #til::TIL_Load().
		*/
		bool Load(const char* a_FileName, uint32 a_Options = TIL_FILE_ABSOLUTEPATH);

		//! Parses the actual image data.
		/*!
			\param a_ColorDepth The color depth received from #SetBPP;
			
			This method is pure virtual and should be overwritten by an
			image loading implementation.
		*/
		virtual bool Parse(uint32 a_ColorDepth) = 0;

		//! Not actually used. Maybe I should do something about that.
		bool Close();

		//! Returns the amount of frames this image contains.
		/*!
			Used when dealing with formats that support animation or multiple images.

			\note There is never going to be support for other video formats.
			This is because TinyImageLoader is an *image* loader, not a video loader.
			The exception to the rule are GIF89 and APNG, because it concerns an extension
			to a normally single-framed format.
		*/
		virtual uint32 GetFrameCount() { return 1; }

		//! Returns the delay between frames.
		/*!
			\return The delay in seconds between frames

			Used when dealing with formats that support animation.
		*/
		virtual float GetDelay() { return 0; }

		//! Get the pixel data from this image.
		/*!
			\param a_Frame The frame of an animation to return.

			\return Pixel array as a byte array.

			The data is encoded according to the color depth specified.
			For instance, when loading images as 32-bit ARGB, the stream
			of bytes must be converted to unsigned long before being
			used.

			\code
			til::Image* load = TIL_Load("media\\texture.png", TIL_DEPTH_A8B8G8R8 | TIL_FILE_ADDWORKINGDIR);
			unsigned long* pixels = (unsigned long*)load->GetPixels();
			\endcode
		*/
		virtual byte* GetPixels(uint32 a_Frame = 0) = 0;

		//! Get the width of a frame
		/*!
			\param a_Frame The frame of an animation or image to return.

			\return The width as a uint32.

			Some formats support multiple frames or images with different dimensions.
			You can call this function with a frame number to get the correct dimensions.
		*/
		virtual uint32 GetWidth(uint32 a_Frame = 0) = 0;

		//! Get the height of a frame
		/*!
			\param a_Frame The frame of an animation or image to return.

			\return The height as a uint32.

			Some formats support multiple frames or images with different dimensions.
			You can call this function with a frame number to get the correct dimensions.
		*/
		virtual uint32 GetHeight(uint32 a_Frame = 0) = 0;

		//! Get the color depth as an enum
		BitDepth GetBitDepth() { return m_BPPIdent; }

	protected:

		FileStream* m_Stream;
		FILE* m_Handle; //!< The file handle
		char* m_FileName; //!< The filename
		BitDepth m_BPPIdent; //!< The bit depth to convert to
		uint8 m_BPP; //!< The amount of bytes per pixel

	}; // class Image

}; // namespace til

#endif