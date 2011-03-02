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

#ifndef _TILIMAGEPNG_H_
#define _TILIMAGEPNG_H_

/*!
	\file TILImagePNG.h
	\brief A PNG image loader
*/

#include "TILImage.h"

#if (TIL_FORMAT & TIL_FORMAT_PNG)

namespace til
{

	class Huffman;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	
	struct chunk
	{
		uint32 length;
		char header[5];
		uint32 type;
	};
	
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	
	class zbuf
	{

	public:

		zbuf();
		~zbuf();

		uint32 GetLength();
		byte* GetData();

		int32 GetByte();
		bool ParseUncompressedBlock();
		int HuffmanDecode(Huffman* a_Huffman);
		Huffman* ZLibDecode(uint8* a_Data, uint32 a_Length);
		uint32 GetCode(uint32 a_Amount);
		bool Expand(int32 a_Amount);

		char* bigger_data;

		char* buffer;
		uint8* zbuffer;
		uint8* zbuffer_end;
		uint32 num_bits;
		uint32 code_buffer;

		char* zout;
		char* zout_start;
		char* zout_end;
		int32 z_expandable;

		Huffman* z_length;
		Huffman* z_distance;

		uint8 default_length[288];
		uint8 default_distance[32];
	};
	
#endif

	struct AnimationData;

	/*!
		\brief til::Image implementation of a PNG loader.
	*/
	class ImagePNG : public Image
	{

	public:

		ImagePNG();
		~ImagePNG();

		

		bool Parse(uint32 a_ColorDepth);

		uint32 GetWidth(uint32 a_Frame = 0);
		uint32 GetHeight(uint32 a_Frame = 0);

		uint32 GetFrameCount();
		byte* GetPixels(uint32 a_Frame = 0);

		/*!
			@name Internal
			These functions are internal and shouldn't be called by developers.
		*/
		//@{
		
		byte GetByte();
		word GetWord();
		dword GetDWord();

		void Skip(uint32 a_Bytes);

		chunk* GetChunkHeader();

		bool Compile();

		bool Compose();
		
		//@}

	private:

		byte** m_Pixels;
		uint32 m_Width, m_Height, m_Pitch;

		uint8 *idata, *expanded, *out;
		uint32 ioff, m_RawLength;
		int32 img_n, img_out_n;
		zbuf m_ZBuffer;
		Huffman* m_Huffman;
		chunk* m_Chunk;

		int req_comp;
		uint8 has_trans;
		uint8 pal_img_n;

		uint32 m_Frames;
		uint32 m_DefaultImage;
		AnimationData* m_Ani;

		uint8 default_length[288];
		uint8 default_distance[32];
		uint32 m_ColorType;

		byte m_TempByte;
		word m_TempWord;
		dword m_TempDWord;
	};

}; // namespace til

#endif

#endif