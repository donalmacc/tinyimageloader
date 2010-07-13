/*

	About this loader:

	I took this code almost verbatim from a folder I found at Breakpoint 2010. 
	You see, one of the patrons was kind enough to set up an FTP server. What
	he put up there (besides porn of course), was a folder called 'coding'.
	This turned out to be treasure trove of old, old code. The year of writing
	is 2010, and the code dates from early 1993 to 2004. Old stuff. Ancient
	stuff. As it turns out, so is the .gif file format. 

	In the folder "Demo tools" was a folder inconspicuously named
	"Neon 08.11.2004". In there, in Old\Despair, I found PIX.C. Which had a
	complete GIF loader! Like I said, the code was taken almost verbatim. Even
	though it was demoscene code and almost two decades old, it worked very
	well.

	I hacked animated GIF's in it myself.

	Something to think about: when PIX.C was written, Netscape hadn't even
	written their extension yet! .GIF's didn't support animation back then!

	My salutations go to you, Mr. Javier Baeza, may your code live forever.

	-knight666

*/

#include "..\SDK\TILImageGIF.h"

#if (TIL_FORMAT & TIL_FORMAT_GIF)

namespace til
{

#define GIF_TYPE(a, b, c, d)           (((d) << 24) + ((c) << 16) + ((b) << 8) + (a))

#define GIF_DEBUG(msg, ...)            printf(msg"\n", __VA_ARGS__)

	ImageGIF::ImageGIF() : Image()
	{
		m_First = m_Current = NULL;
		m_Palette = NULL;
		m_Colors = NULL;
	}

	ImageGIF::~ImageGIF()
	{

	}

	void ImageGIF::AddBuffer()
	{
		if (m_First)
		{
			BufferLinked* temp = new BufferLinked;
			m_Current->next = temp;
			temp->next    = NULL;
			temp->buffer  = new byte[m_Width * m_Height * m_BPP];
			m_Current = temp;
		}
		else
		{ 
			m_First = new BufferLinked;
			m_Current = m_First;
			m_Current->next    = NULL;
			m_Current->buffer  = new byte[m_Width * m_Height * m_BPP];
		}
	}

	void ImageGIF::CompileColors()
	{
		if (m_Palette) { delete m_Palette; }
		m_Palette = new char[m_ColorTableSize * 3];
		fread(m_Palette, m_ColorTableSize * 3, 1, m_Handle);

		if (m_Colors) { delete m_Colors; }
		m_Colors = new byte[m_ColorTableSize * m_BPP];
		byte* colors_write = m_Colors;

		switch (m_ColorDepth)
		{

		case TIL_DEPTH_A8R8G8B8:
			{
				for (uint32 i = 0; i < m_ColorTableSize; i++)
				{
					colors_write[3] = 255;
					colors_write[2] = m_Palette[(i * 3) + 0];
					colors_write[1] = m_Palette[(i * 3) + 1];
					colors_write[0] = m_Palette[(i * 3) + 2];

					colors_write += m_BPP;
				}

				break;
			}

		case TIL_DEPTH_R8G8B8A8:
			{
				for (uint32 i = 0; i < m_ColorTableSize; i++)
				{					
					colors_write[3] = m_Palette[(i * 3) + 0];
					colors_write[2] = m_Palette[(i * 3) + 1];
					colors_write[1] = m_Palette[(i * 3) + 2];
					colors_write[0] = 255;

					colors_write += m_BPP;
				}

				break;
			}

		case TIL_DEPTH_R5G6B5:
			{
				color_16b* dst = (color_16b*)colors_write;

				for (uint32 i = 0; i < m_ColorTableSize; i++)
				{
					*dst =
						(((m_Palette[(i * 3) + 0] * 0xF800) >> 8) & 0xF800) |
						(((m_Palette[(i * 3) + 1] * 0x07E0) >> 8) & 0x07E0) |
						(((m_Palette[(i * 3) + 2] * 0x001F) >> 8) & 0x001F);

					colors_write += m_BPP;
					dst++;
				}

				break;
			}
		}
	}

	bool ImageGIF::Parse(uint32 a_ColorDepth/*= TIL_DEPTH_A8R8G8B8*/)
	{
		fread(m_Buffer, 6, 1, m_Handle);

		if (*(uint32*)m_Buffer != GIF_TYPE('G', 'I', 'F', '8'))
		{
			TIL_ERROR_EXPLAIN("No GIF header found.");
			return false;
		}

		fread(m_Buffer, 7, 1, m_Handle);

		bool colortable = ((m_Buffer[4] & 0x80) == 0x80);
		m_ColorTableSize = 2 << (m_Buffer[4] & 0x07);

		m_ColorDepth = a_ColorDepth;
		CompileColors();

		bool animation = false;
		bool transparancy = false;
		bool looping = false;

		fread(m_Buffer, 1, 1, m_Handle);

		/* It's an extension */
		while (m_Buffer[0] == 0x21)
		{                    
			fread(m_Buffer, 1, 1, m_Handle);
			
			switch (m_Buffer[0]) 
			{

				// Animation
			case 0xFF:
				{
					fread(m_Buffer, 1, 1, m_Handle);  // rest of header
					fread(m_Buffer, 11, 1, m_Handle); // NETSCAPE2.0
					fread(m_Buffer, 2, 1, m_Handle);  // data follows

					uint16 set_looping;
					fread(&set_looping, 1, 2, m_Handle);
					looping = (set_looping & 0xFFFF) ? true : false;

					fread(m_Buffer, 1, 1, m_Handle);  // end;

					animation = true;

					break;
				}

				// Graphic Control Extension
			case 0xF9: 
				{
					fread(m_Buffer, 2, 1, m_Handle);  // rest of header

					uint16 seconds;
					fread(&seconds, 1, 2, m_Handle);

					fread(m_Buffer, 2, 1, m_Handle);					
					
					//b->transparent = m_Buffer[4];            /* Index in colortable */
					break;
				}

			default: /* Other extension */
				{
					fread(m_Buffer, 1, 1, m_Handle);
					
					while (m_Buffer[0] != 0) 
					{
						fread(m_Buffer, m_Buffer[0], 1, m_Handle);
						fread(m_Buffer, 1, 1, m_Handle);
					}
				}

			}

			fread(m_Buffer, 1, 1, m_Handle);
			bool bleh = true;
		}

		if (m_Buffer[0] != 0x2c) 
		{
			TIL_ERROR_EXPLAIN("No image descriptor found.");
			return false;
		}
		if (!fread(m_Buffer, 9, 1, m_Handle)) 
		{
			TIL_ERROR_EXPLAIN("No image descriptor found.");
			return false;
		}

		m_Width  = ((int)m_Buffer[5] * 256) | (int)m_Buffer[4];
		m_Height = ((int)m_Buffer[7] * 256) | (int)m_Buffer[6];
		
		//m_Buffer = new byte[m_Width * m_Height * m_BPP];
		
		/*colortable = (buffer[8] & 0x80) == 0x80;
		bool interlaced = (buffer[8] & 0x40) == 0x40;
		m_ColorTableSize = 2 << (buffer[8] & 0x07);*/

		// =========================================
		// LZW
		// =========================================

		int32 min_code_size, clear_code, eoi_code, free_code, code_size, code_mask;
		int32 work_number;
		uint32 bufidx, buflen, workbits;
		int32 prev_code, first, sp;

		int32 stack[4096];
		int32 prefix[4096];
		int32 extnsn[4096];

		byte* target;

		m_Frames = 1;

		while (1)
		{
			GIF_DEBUG("Images: %i", m_Frames);
			m_Frames++;

			AddBuffer();
			target = m_Current->buffer;

			fread(m_Buffer, 1, 1, m_Handle);

			min_code_size = m_Buffer[0];
			clear_code = 1 << min_code_size;
			eoi_code = clear_code + 1;
			free_code = clear_code + 2;
			code_size = min_code_size + 1;
			code_mask = (1 << code_size) - 1;

			work_number = 0;
			bufidx = 0;
			buflen = 0;
			workbits = 0;
			prev_code = -2;
			first = 0;
			sp = 0;

			/* Decode until we find an End-of-Information code */
			int code = -1;
			while (code != eoi_code) 
			{
				/* Add bytes until we have enough bits for next code */
				while (work_number < code_size) 
				{
					/* Read new data block if needed */
					if (bufidx == buflen) 
					{
						fread(m_Buffer, 1, 1, m_Handle);
						if (m_Buffer[0] == 0)
						{
							GIF_DEBUG("Zero length of buffer.");
							return false;
						}
						buflen = m_Buffer[0];
						fread(m_Buffer, buflen, 1, m_Handle);
						bufidx = 0;
					}
					workbits |= ((uint32)m_Buffer[bufidx++]) << work_number;
					work_number += 8;
				}

				/* Get next code */
				code = workbits & code_mask;
				workbits >>= code_size;
				work_number -= code_size;

				/* Force first code of image to be a clear code */
				if (prev_code == -2) 
				{
					code = clear_code;
				}

				/* Branch on type of code */
				
				/* Reset the decoder */
				if (code == clear_code) 
				{
					code_size = min_code_size + 1;      /* Original code size */
					code_mask = (1 << code_size) - 1;   /* Corresponding mask */
					free_code = clear_code + 2;         /* First pos. in tables */
					prev_code = -1;                     /* Next code is a root code */
				} 
				/* End of Information */
				else if (code == eoi_code) 
				{         
					GIF_DEBUG("End of info.");
					fread(m_Buffer, 1, 1, m_Handle);
					break;
				} 
				/* 1st code after clearcode */
				else if (prev_code == -1) 
				{
					/* Add to image */
					
					if (m_BPP == 2)
					{
						*(color_16b*)target = *(color_16b*)&m_Colors[code * m_BPP];
					}
					else if (m_BPP == 4)
					{
						*(color_32b*)target = *(color_32b*)&m_Colors[code * m_BPP];
					}
					target += m_BPP;

					first = prev_code = code;
				} 
				/* We've got a normal code */
				else 
				{                
					/* It's a new code */
					if (code >= free_code) 
					{            
						stack[sp++] = first;
						first = prev_code;
					} 
					/* It's an existing code */
					else    
					{
						first = code;
					}

					/* Push string of pixels */
					while (first >= clear_code) 
					{       
						stack[sp++] = extnsn[first];
						first = prefix[first];
					}
					/* Push string's root code */
					stack[sp++] = first;

					/* Now add pixels to image */
					while (sp != 0) 
					{
						if (m_BPP == 2)
						{
							*(color_16b*)target = *(color_16b*)&m_Colors[stack[--sp] * m_BPP];
						}
						else if (m_BPP == 4)
						{
							*(color_32b*)target = *(color_32b*)&m_Colors[stack[--sp] * m_BPP];
						}

						target += m_BPP;
					}
					prefix[free_code] = prev_code;
					extnsn[free_code++] = first;
					prev_code = code;

					/* Check if code_size needs to increase */
					if (free_code > code_mask && code_size != 12) 
					{
						code_size++;
						code_mask = (1 << code_size) - 1;
					}
				}
			}

			// image block identifier: 21 F9 04
			uint32 header = 0;
			fread(&header, 1, 3, m_Handle);
			if (header != 0x0004F921)
			{
				fread(m_Buffer, 100, 1, m_Handle);

				GIF_DEBUG("End of file.");
				break;
			}

			fread(m_Buffer, 6, 1, m_Handle);
			if (m_Buffer[5] != 0x2c)
			{
				TIL_ERROR_EXPLAIN("No start of data.");
				break;
			}

			// get header of next block
			fread(m_Buffer, 9, 1, m_Handle);

			// get the new colors
			colortable = (m_Buffer[8] & 0x80) == 0x80;
			m_ColorTableSize = 2 << (m_Buffer[8] & 0x07);
			if (colortable) { CompileColors(); }
		}

		return true;
	}

	byte* ImageGIF::GetPixels(uint32 a_Frame)
	{
		if (a_Frame >= 0 && a_Frame < m_Frames - 1)
		{
			BufferLinked* current = m_First;
			for (uint32 i = 0; i < a_Frame; i++) { current = current->next; }
			return current->buffer;
		}

		return NULL;
	}

	uint32 ImageGIF::GetFrameCount()
	{
		return m_Frames;
	}

}; // namespace til

#endif