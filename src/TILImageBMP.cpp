#include "..\SDK\headers\TILImageBMP.h"

#define BMP_DEBUG(msg, ...)        printf(msg"\n", __VA_ARGS__)

namespace til
{

	ImageBMP::ImageBMP()
	{

	}

	ImageBMP::~ImageBMP()
	{

	}

	dword ImageBMP::GetDWord()
	{
		fread(m_Data, 1, 4, m_Handle);
		return (m_Data[3] << 24) | (m_Data[2] << 16) | (m_Data[1] << 8) | (m_Data[0]);
	}

	bool ImageBMP::Parse(uint32 a_ColorDepth)
	{
		byte header[3];         fread(header, 1, 2, m_Handle);
		dword size =            GetDWord();
		dword unused;           fread(&unused, 4, 1, m_Handle);
		dword pixel_offset =    GetDWord();

		dword header_size =     GetDWord();
		switch (header_size)
		{
		case HDR_OS2V1:
			BMP_DEBUG("Header: OS/2 V1");
			break;
		case HDR_OS2V2:
			BMP_DEBUG("Header: OS/2 V2");
			break;
		case HDR_WINDOWSV3:
			BMP_DEBUG("Header: Windows V3");
			break;
		case HDR_WINDOWSV4:
			BMP_DEBUG("Header: Windows V4");
			break;
		case HDR_WINDOWSV5:
			BMP_DEBUG("Header: Windows V5");
			break;
		}

		dword width =           GetDWord();
		m_Width =               (uint32)width;
		dword height =          GetDWord();
		m_Height =              (uint32)height;

		word color_planes;      fread(&color_planes, 2, 1, m_Handle);
		word bpp;               fread(&bpp, 2, 1, m_Handle);

		dword compresion =      GetDWord();
		switch (compresion)
		{

		case COMP_RGB:
			BMP_DEBUG("No compression.");
			break;
		case COMP_RLE8:
			BMP_DEBUG("Run length encoding with 8 bits per pixel.");
			break;
		case COMP_RLE4:
			BMP_DEBUG("Run length encoding with 4 bits per pixel.");
			break;
		case COMP_BITFIELDS:
			BMP_DEBUG("Bitfields.");
			break;
		case COMP_JPEG:
			BMP_DEBUG("Bitmap contains a JPEG image.");
			break;
		case COMP_PNG:
			BMP_DEBUG("Bitmap contains a PNG image.");
			break;

		}

		dword raw_size =        GetDWord();

		fseek(m_Handle, 16, SEEK_CUR);

		m_Pixels = new byte[m_Width * m_Height * m_BPP];

		byte data[8];
		uint32 total = (m_Width * m_Height) >> 1;
		m_Pitch = m_Width * m_BPP;

		m_Target = m_Pixels + ((m_Height - 1) * m_Pitch);

		// uncompressed

		if (m_BPP == 4)
		{
			for (uint32 y = 0; y < m_Height; y++)
			{
				color_32b* dst = (color_32b*)m_Target;

				for (uint32 x = 0; x < m_Width; x++)
				{
					fread(data, 1, 3, m_Handle);
					dst[x] = Construct_32b_R8G8B8(data[2], data[1], data[0]);
				}

				m_Target -= m_Pitch;
			}
		}
		else if (m_BPP == 2)
		{
			for (uint32 y = 0; y < m_Height; y++)
			{
				color_16b* dst = (color_16b*)m_Target;

				for (uint32 x = 0; x < m_Width; x++)
				{
					fread(data, 1, 3, m_Handle);
					dst[x] = Construct_16b_R5G6B5(data[2], data[1], data[0]);
				}

				m_Target -= m_Pitch;
			}
		}

		return true;
	}

	til::uint32 ImageBMP::GetFrameCount()
	{
		return 1;
	}

	byte* ImageBMP::GetPixels( uint32 a_Frame /*= 0*/ )
	{
		return m_Pixels;
	}
}; // namespace til