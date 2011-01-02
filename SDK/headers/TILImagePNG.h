/*
    TinyImageLoader - load images, just like that
    Copyright (C) 2010 - 2011 Quinten Lansu (knight666)

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

#ifndef _TILIMAGEPNG_H_
#define _TILIMAGEPNG_H_

#include "TILImage.h"

#if (TIL_FORMAT & TIL_FORMAT_PNG)

namespace til
{

	class Huffman;

	struct chunk
	{
		uint32 length;
		char header[5];
		uint32 type;
	};

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

	struct AnimationData;

	class ImagePNG : public Image
	{

	public:

		ImagePNG() : Image()
		{

		}
		~ImagePNG() { }

		int32 GetByte();
		int32 GetWord();
		int32 GetDWord();

		void Skip(uint32 a_Bytes);

		chunk GetChunkHeader();

		bool Compile();

		bool Parse(uint32 a_ColorDepth);

		uint32 GetWidth(uint32 a_Frame = 0)
		{
			return m_Width;
		}
		uint32 GetHeight(uint32 a_Frame = 0)
		{
			return m_Height;
		}

		uint32 GetFrameCount();
		byte* GetPixels(uint32 a_Frame = 0);

	private:

		bool Compose();

		byte** m_Pixels;
		uint32 m_Width, m_Height, m_Pitch;

		uint8 *idata, *expanded, *out;
		uint32 ioff, m_RawLength;
		int32 img_n, img_out_n;
		uint8* img_buffer;
		uint8* img_buffer_end;
		zbuf m_ZBuffer;
		Huffman* m_Huffman;

		int req_comp;
		uint8 has_trans;
		uint8 pal_img_n;

		uint32 m_Frames;
		uint32 m_DefaultImage;
		AnimationData* m_Ani;

		uint8 default_length[288];
		uint8 default_distance[32];
		uint32 m_ColorType;

	};

}; // namespace til

#endif

#endif