/*
    TinyImageLoader - load images, just like that
    Copyright (C) 2010 Quinten Lansu (knight666)

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

	struct zbuf
	{
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
	};

	struct TinyImageLoader;

	class ImagePNG : public Image
	{

		friend struct TinyImageLoader;

	public:

		int32 GetByte();
		int32 GetWord();
		int32 GetDWord();

		void Skip(uint32 a_Bytes);

		int32 GetZBufByte();

		uint32 GetZCode(uint32 a_Amount);

		chunk GetChunkHeader();

		int HuffmanDecode(Huffman* a_Huffman);

		bool ZLibExpand(int a_Amount);

		bool ZLibDecode();

		bool Compile();

		bool Parse(uint32 a_ColorDepth);

		void InitDefaults()
		{
			for (int i = 0; i <= 287; i++)
			{
				if (i <= 143)      { default_length[i] = 8; }
				else if (i <= 255) { default_length[i] = 9; }
				else if (i <= 279) { default_length[i] = 7; }
				else               { default_length[i] = 8; }
			}

			for (int i = 0; i <= 31; ++i) { default_distance[i] = 5; }
		}

		uint32 GetWidth(uint32 a_Frame = 0)
		{
			return m_Width;
		}
		uint32 GetHeight(uint32 a_Frame = 0)
		{
			return m_Height;
		}

		byte* GetPixels(uint32 a_Frame = 0)
		{
			/*if (a_ColorDepth == TIL_DEPTH_A8R8G8B8)
			{
				// 390 468
				// 237, 156, 64
				COLORDATA* data = (COLORDATA*)out;
				for (uint32 i = 0; i < m_Width * m_Height; i++)
				{
					//m_Pixels[i] = ((out[i] & 0xFF000000) >> 24) + (out[i] & 0x00FFFF00) + ((out[i] & 0x000000FF) << 24);
					//m_Pixels[i] = ((data[i] & 0xFF000000) >> 24) + (data[i] & 0x00FFFF00) + ((data[i] & 0x000000FF) << 24);
					//m_Pixels[i] = (data[i] & 0x00FFFF00) >> 8;
					//m_Pixels[i] = ((data[i] & 0x00FFFFFF) << 8) + 0x000000FF;

					uint32 r = (data[i] & 0xFF0000) >> 16;
					uint32 g = (data[i] & 0x00FF00) >> 8;
					uint32 b = (data[i] & 0x0000FF);

					//m_Pixels[i] = (data[i] & 0xFFFFFF) >> 8; 
					//m_Pixels[i] = data[i];
					m_Pixels[i] = (b << 16) + (g << 8) + (r);
				}

				return m_Pixels;
			}*/

			//return (unsigned char*)out;
			return m_Pixels;
		}

	private:

		ImagePNG() : Image()
		{

		}
		~ImagePNG() { }

		byte* m_Pixels;
		uint32 m_Width, m_Height, m_Pitch;

		uint8 *idata, *expanded, *out;
		uint32 ioff, m_RawLength;
		int32 img_n, img_out_n;
		uint8* img_buffer;
		uint8* img_buffer_end;
		zbuf m_ZBuffer;
		Huffman* m_Huffman;

		uint8 default_length[288];
		uint8 default_distance[32];
		uint32 m_ColorType;

	};

}; // namespace til

#endif

#endif