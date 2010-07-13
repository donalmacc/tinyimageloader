// Template, major revision 3, beta
// IGAD/NHTV - Jacco Bikker - 2006-2009

#include "template.h"
#include "surface.h"

#include "Settings.h"
#include "Includes.h"

namespace Tmpl8 
{

	void NotifyUser( char* s );

	// -----------------------------------------------------------
	// True-color surface class implementation
	// -----------------------------------------------------------

	Surface::Surface( int a_Width, int a_Height, Pixel* a_Buffer, int a_Pitch ) :
	m_Width( a_Width ),
		m_Height( a_Height ),
		m_Buffer( a_Buffer ),
		m_Pitch( a_Pitch )
	{
	}

	Surface::Surface( int a_Width, int a_Height ) :
	m_Width( a_Width ),
		m_Height( a_Height ),
		m_Pitch( a_Width )
	{
		m_Buffer = (Pixel*)MALLOC64( a_Width * a_Height * sizeof( Pixel ) );
	}

	Surface::Surface( char* a_File ) :
	m_Buffer( NULL ),
		m_Width( 0 ), m_Height( 0 )
	{
		FILE* f = fopen( a_File, "rb" );
		if (!f) 
		{
			char t[128];
			sprintf( t, "File not found: %s", a_File );
			NotifyUser( t ); 
			return;
		}
		else fclose( f );
		LoadImage( a_File );
	}

	void Surface::LoadImage( char* a_File )
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		fif = FreeImage_GetFileType( a_File, 0 );
		if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename( a_File );
		FIBITMAP* tmp = FreeImage_Load( fif, a_File );
		FIBITMAP* dib = FreeImage_ConvertTo32Bits( tmp );
		FreeImage_Unload( tmp );
		unsigned char* bits = FreeImage_GetBits( dib );
		m_Width = m_Pitch = FreeImage_GetWidth( dib );
		m_Height = FreeImage_GetHeight( dib );
		m_Buffer = (Pixel*)MALLOC64( m_Width * m_Height * sizeof( Pixel ) );
		for( int y = 0; y < m_Height; y++) 
		{
			unsigned char* line = FreeImage_GetScanLine( dib, m_Height - 1 - y );
			memcpy( m_Buffer + y * m_Pitch, line, m_Width * sizeof( Pixel ) );
		}
		FreeImage_Unload( dib );
	}

	Surface::~Surface()
	{
		FREE64( m_Buffer );
	}

	void Surface::Clear( Pixel a_Color )
	{
		/*int s = m_Width * m_Height;
		// loop unrolling ftw
		for (int i = 0; i < s; i += 4)
		{
			m_Buffer[i + 0] = a_Color;
			m_Buffer[i + 1] = a_Color;
			m_Buffer[i + 2] = a_Color;
			m_Buffer[i + 3] = a_Color;
		}*/

		memset(m_Buffer, a_Color, m_Pitch * m_Height * sizeof(Pixel));
	}

	void Surface::Centre( char* a_String, int y1, Pixel color )
	{
		int x = (m_Width - (int)strlen( a_String ) * 6) / 2;
		Print( a_String, x, y1, color );
	}

	void Surface::Print( char* a_String, int x1, int y1, Pixel color )
	{
		Pixel* t = m_Buffer + x1 + y1 * m_Pitch;
		int i;
		for ( i = 0; i < (int)(strlen( a_String )); i++ )
		{	
			long pos = 0;
			if ((a_String[i] >= 'A') && (a_String[i] <= 'Z')) pos = s_Transl[(unsigned short)(a_String[i] - ('A' - 'a'))];
			else pos = s_Transl[(unsigned short)a_String[i]];
			Pixel* a = t;
			char* c = (char*)s_Font[pos];
			int h, v;
			for ( v = 0; v < 5; v++ ) 
			{
				for ( h = 0; h < 5; h++ ) if (*c++ == 'o') *(a + h) = color, *(a + h + m_Pitch) = 0;
				a += m_Pitch;
			}
			t += 6;
		}
	}

	void Surface::Resize( int a_Width, int a_Height, Surface* a_Orig )
	{
		unsigned int newpitch = (a_Width + 16) & 0xffff0;
		Pixel* src = a_Orig->GetBuffer(), *dst = m_Buffer;
		int u, v, owidth = a_Orig->GetWidth(), oheight = a_Orig->GetHeight();
		int dx = (owidth << 10) / a_Width, dy = (oheight << 10) / a_Height;
		for ( v = 0; v < a_Height; v++ )
		{
			for ( u = 0; u < a_Width; u++ )
			{
				int su = u * dx, sv = v * dy;
				Pixel* s = src + (su >> 10) + (sv >> 10) * owidth;
				int ufrac = su & 1023, vfrac = sv & 1023;
				int w4 = (ufrac * vfrac) >> 12;
				int w3 = ((1023 - ufrac) * vfrac) >> 12;
				int w2 = (ufrac * (1023 - vfrac)) >> 12;
				int w1 = ((1023 - ufrac) * (1023 - vfrac)) >> 12;
				int x2 = ((su + dx) > ((owidth - 1) << 10))?0:1;
				int y2 = ((sv + dy) > ((oheight - 1) << 10))?0:1;
				Pixel p1 = *s, p2 = *(s + x2), p3 = *(s + owidth * y2), p4 = *(s + owidth * y2 + x2);
				unsigned int r = (((p1 & REDMASK) * w1 + (p2 & REDMASK) * w2 + (p3 & REDMASK) * w3 + (p4 & REDMASK) * w4) >> 8) & REDMASK;
				unsigned int g = (((p1 & GREENMASK) * w1 + (p2 & GREENMASK) * w2 + (p3 & GREENMASK) * w3 + (p4 & GREENMASK) * w4) >> 8) & GREENMASK;
				unsigned int b = (((p1 & BLUEMASK) * w1 + (p2 & BLUEMASK) * w2 + (p3 & BLUEMASK) * w3 + (p4 & BLUEMASK) * w4) >> 8) & BLUEMASK;
				*(dst + u + v * newpitch) = (Pixel)(r + g + b);
			}
		}
		m_Width = a_Width, m_Height = a_Height, m_Pitch = newpitch;
	}

	void Surface::Line( float x1, float y1, float x2, float y2, Pixel c )
	{
		if ((x1 < 0) || (y1 < 0) || (x1 >= m_Width) || (y1 >= m_Height) ||
			(x2 < 0) || (y2 < 0) || (x2 >= m_Width) || (y2 >= m_Height))
		{
			return;
		}
		float b = x2 - x1;
		float h = y2 - y1;
		float l = fabsf( b );
		if (fabsf ( h ) > l) l = fabsf( h );
		int il = (int)l;
		float dx = b / (float)l;
		float dy = h / (float)l;
		for ( int i = 0; i <= il; i++ )
		{
			*(m_Buffer + (int)x1 + (int)y1 * m_Pitch) = c;
			x1 += dx, y1 += dy;
		}
	}

	void Surface::Plot( int x, int y, Pixel c )
	{ 
		if ((x >= 0) && (y >= 0) && (x < m_Width) && (y < m_Height)) m_Buffer[x + y * m_Pitch] = c;
	}

	void Surface::Box( int x1, int y1, int x2, int y2, Pixel c )
	{
		Line( (float)x1, (float)y1, (float)x2, (float)y1, c );
		Line( (float)x2, (float)y1, (float)x2, (float)y2, c );
		Line( (float)x1, (float)y2, (float)x2, (float)y2, c );
		Line( (float)x1, (float)y1, (float)x1, (float)y2, c );
	}

	void Surface::Bar( int x1, int y1, int x2, int y2, Pixel c )
	{
		Pixel* a = x1 + y1 * m_Pitch + m_Buffer;
		for ( int y = y1; y <= y2; y++ )
		{
			for ( int x = 0; x <= (x2 - x1); x++ ) a[x] = c;
			a += m_Pitch;
		}
	}

	void Surface::CopyTo( Surface* a_Dst, int a_X, int a_Y )
	{
		Pixel* dst = a_Dst->GetBuffer();
		Pixel* src = m_Buffer;
		if ((src) && (dst)) 
		{
			int srcwidth = m_Width;
			int srcheight = m_Height;
			int srcpitch = m_Pitch;
			int dstwidth = a_Dst->GetWidth();
			int dstheight = a_Dst->GetHeight();
			int dstpitch = a_Dst->GetPitch();
			if ((srcwidth + a_X) > dstwidth) srcwidth = dstwidth - a_X;
			if ((srcheight + a_Y) > dstheight) srcheight = dstheight - a_Y;
			if (a_X < 0) src -= a_X, srcwidth += a_X, a_X =0;
			if (a_Y < 0) src -= a_Y * srcpitch, srcheight += a_Y, a_Y = 0;
			if ((srcwidth > 0) && (srcheight > 0))
			{
				dst += a_X + dstpitch * a_Y;
				for ( int y = 0; y < srcheight; y++ )
				{
					memcpy( dst, src, srcwidth * 4 );
					dst += dstpitch;
					src += srcpitch;
				}
			}
		}
	}

	void Surface::DumpTo(Surface* a_Destination)
	{
		memcpy(a_Destination->m_Buffer, m_Buffer, sizeof(Pixel) * m_Width * m_Height);
	}

	int Surface::SaveToTGA(const char* a_File)
	{
		// open the file
		FILE* save = fopen(a_File, "wb");
		if (!save) { return -1; }

		putc(0, save); putc(0, save);
		putc(2, save); putc(0, save);			// uncompressed RGB

		putc(0, save); putc(0, save); 
		putc(0, save); putc(0, save);

		putc(0, save); putc(0, save);			// x origin 
		putc(0, save); putc(0, save);			// y origin

		putc(m_Width & 0x00FF, save);			// lower part of width
		putc((m_Width & 0xFF00) / 256, save);	// higher part of width

		putc(m_Height & 0x00FF, save);			// lower part of height
		putc((m_Height & 0xFF00) / 256, save);	// higher part of height

		putc(32, save); putc(0, save);			// 24-bit bitmap

		// buffer is reversed on the y-axis
		for (int y = m_Height; y > 0; y--)
		{
			for (int x = 0; x < m_Width; x++) 
			{
				int xy = x + (y * m_Pitch);

				putc((m_Buffer[xy] & MASK_BLUE)		>> SHIFT_BLUE,	save);
				putc((m_Buffer[xy] & MASK_GREEN)	>> SHIFT_GREEN,	save);
				putc((m_Buffer[xy] & MASK_RED)		>> SHIFT_RED,	save);
				putc(0xFF, save);					// alpha
			}
		}

		fclose(save);

		return 0;
	}

	void Surface::BlendCopyTo( Surface* a_Dst, int a_X, int a_Y )
	{
		Pixel* dst = a_Dst->GetBuffer();
		Pixel* src = m_Buffer;
		if ((src) && (dst)) 
		{
			int srcwidth = m_Width;
			int srcheight = m_Height;
			int srcpitch = m_Pitch;
			int dstwidth = a_Dst->GetWidth();
			int dstheight = a_Dst->GetHeight();
			int dstpitch = a_Dst->GetPitch();
			if ((srcwidth + a_X) > dstwidth) srcwidth = dstwidth - a_X;
			if ((srcheight + a_Y) > dstheight) srcheight = dstheight - a_Y;
			if (a_X < 0) src -= a_X, srcwidth += a_X, a_X =0;
			if (a_Y < 0) src -= a_Y * srcpitch, srcheight += a_Y, a_Y = 0;
			if ((srcwidth > 0) && (srcheight > 0))
			{
				dst += a_X + dstpitch * a_Y;
				for ( int y = 0; y < srcheight; y++ )
				{
					for ( int x = 0; x < srcwidth; x++ ) dst[x] = AddBlend( dst[x], src[x] );
					dst += dstpitch;
					src += srcpitch;
				}
			}
		}
	}

	void Surface::SetChar( int c, char* c1, char* c2, char* c3, char* c4, char* c5 )
	{
		strcpy( s_Font[c][0], c1 );
		strcpy( s_Font[c][1], c2 );
		strcpy( s_Font[c][2], c3 );
		strcpy( s_Font[c][3], c4 );
		strcpy( s_Font[c][4], c5 );
	}

	void Surface::InitCharset()
	{
		SetChar( 0, ":ooo:", "o:::o", "ooooo", "o:::o", "o:::o" );
		SetChar( 1, "oooo:", "o:::o", "oooo:", "o:::o", "oooo:" );
		SetChar( 2, ":oooo", "o::::", "o::::", "o::::", ":oooo" );
		SetChar( 3, "oooo:", "o:::o", "o:::o", "o:::o", "oooo:" );
		SetChar( 4, "ooooo", "o::::", "oooo:", "o::::", "ooooo" );
		SetChar( 5, "ooooo", "o::::", "ooo::", "o::::", "o::::" );
		SetChar( 6, ":oooo", "o::::", "o:ooo", "o:::o", ":ooo:" );
		SetChar( 7, "o:::o", "o:::o", "ooooo", "o:::o", "o:::o" );
		SetChar( 8, "::o::", "::o::", "::o::", "::o::", "::o::" );
		SetChar( 9, ":::o:", ":::o:", ":::o:", ":::o:", "ooo::" );
		SetChar(10, "o::o:", "o:o::", "oo:::", "o:o::", "o::o:" );
		SetChar(11, "o::::", "o::::", "o::::", "o::::", "ooooo" );
		SetChar(12, "oo:o:", "o:o:o", "o:o:o", "o:::o", "o:::o" );
		SetChar(13, "o:::o", "oo::o", "o:o:o", "o::oo", "o:::o" );
		SetChar(14, ":ooo:", "o:::o", "o:::o", "o:::o", ":ooo:" );
		SetChar(15, "oooo:", "o:::o", "oooo:", "o::::", "o::::" );
		SetChar(16, ":ooo:", "o:::o", "o:::o", "o::oo", ":oooo" );
		SetChar(17, "oooo:", "o:::o", "oooo:", "o:o::", "o::o:" );
		SetChar(18, ":oooo", "o::::", ":ooo:", "::::o", "oooo:" );
		SetChar(19, "ooooo", "::o::", "::o::", "::o::", "::o::" );
		SetChar(20, "o:::o", "o:::o", "o:::o", "o:::o", ":oooo" );
		SetChar(21, "o:::o", "o:::o", ":o:o:", ":o:o:", "::o::" );
		SetChar(22, "o:::o", "o:::o", "o:o:o", "o:o:o", ":o:o:" );
		SetChar(23, "o:::o", ":o:o:", "::o::", ":o:o:", "o:::o" );
		SetChar(24, "o:::o", "o:::o", ":oooo", "::::o", ":ooo:" );
		SetChar(25, "ooooo", ":::o:", "::o::", ":o:::", "ooooo" );
		SetChar(26, ":ooo:", "o::oo", "o:o:o", "oo::o", ":ooo:" );
		SetChar(27, "::o::", ":oo::", "::o::", "::o::", ":ooo:" );
		SetChar(28, ":ooo:", "o:::o", "::oo:", ":o:::", "ooooo" );
		SetChar(29, "oooo:", "::::o", "::oo:", "::::o", "oooo:" );
		SetChar(30, "o::::", "o::o:", "ooooo", ":::o:", ":::o:" );
		SetChar(31, "ooooo", "o::::", "oooo:", "::::o", "oooo:" );
		SetChar(32, ":oooo", "o::::", "oooo:", "o:::o", ":ooo:" );
		SetChar(33, "ooooo", "::::o", ":::o:", "::o::", "::o::" );
		SetChar(34, ":ooo:", "o:::o", ":ooo:", "o:::o", ":ooo:" );
		SetChar(35, ":ooo:", "o:::o", ":oooo", "::::o", ":ooo:" );
		SetChar(36, "::o::", "::o::", "::o::", ":::::", "::o::" );
		SetChar(37, ":ooo:", "::::o", ":::o:", ":::::", "::o::" );
		SetChar(38, ":::::", ":::::", "::o::", ":::::", "::o::" );
		SetChar(39, ":::::", ":::::", ":ooo:", ":::::", ":ooo:" );
		SetChar(40, ":::::", ":::::", ":::::", ":::o:", "::o::" );
		SetChar(41, ":::::", ":::::", ":::::", ":::::", "::o::" );
		SetChar(42, ":::::", ":::::", ":ooo:", ":::::", ":::::" );
		SetChar(43, ":::o:", "::o::", "::o::", "::o::", ":::o:" );
		SetChar(44, "::o::", ":::o:", ":::o:", ":::o:", "::o::" );
		SetChar(45, ":::::", ":::::", ":::::", ":::::", ":::::" );
		SetChar(46, "ooooo", "ooooo", "ooooo", "ooooo", "ooooo" );
		SetChar(47, "::o::", "::o::", ":::::", ":::::", ":::::" ); // Tnx Ferry
		SetChar(48, "o:o:o", ":ooo:", "ooooo", ":ooo:", "o:o:o" );
		SetChar(49, "::::o", ":::o:", "::o::", ":o:::", "o::::" );
		char c[] = "abcdefghijklmnopqrstuvwxyz0123456789!?:=,.-() #'*/";
		int i;
		for ( i = 0; i < 256; i++ ) s_Transl[i] = 45;
		for ( i = 0; i < 50; i++ ) s_Transl[(unsigned char)c[i]] = i;
	}

	void Surface::ScaleColor( unsigned int a_Scale )
	{
		int s = m_Pitch * m_Height;
		for ( int i = 0; i < s; i++ )
		{
			Pixel c = m_Buffer[i];
			unsigned int rb = (((c & (REDMASK|BLUEMASK)) * a_Scale) >> 5) & (REDMASK|BLUEMASK);
			unsigned int g = (((c & GREENMASK) * a_Scale) >> 5) & GREENMASK;
			m_Buffer[i] = rb + g;
		}
	}

	Font::Font( char* a_File, char* a_Chars )
	{
		m_Surface = new Surface( a_File );
		Pixel* b = m_Surface->GetBuffer();
		int w = m_Surface->GetWidth();
		int h = m_Surface->GetHeight();
		unsigned int charnr = 0, width = 0, start = 0;
		m_Trans = new int[256];
		memset( m_Trans, 0, 1024 );
		unsigned int i;
		for ( i = 0; i < strlen( a_Chars ); i++ ) m_Trans[(unsigned char)a_Chars[i]] = i;
		m_Offset = new int[strlen( a_Chars )];
		m_Width = new int[strlen( a_Chars )];
		m_Height = h;
		m_CY1 = 0, m_CY2 = 1024;
		int x, y;
		bool lastempty = true;
		for ( x = 0; x < w; x++ )
		{
			bool empty = true;
			for ( y = 0; y < h; y++ ) if (*(b + x + y * w) & 0xffffff) 
			{
				if (lastempty)
				{
					width = 0;
					start = x;
				}
				empty = false;
			}
			if ((empty) && (!lastempty))
			{
				m_Width[charnr] = x - start;
				m_Offset[charnr] = start;
				if (++charnr == strlen( a_Chars )) break;
			}
			lastempty = empty;
		}
	}

	Font::~Font()
	{
		delete m_Surface;
		delete m_Trans;
		delete m_Width;
		delete m_Offset;
	}

	int Font::Width( char* a_Text )
	{
		int w = 0;
		unsigned int i;
		for ( i = 0; i < strlen( a_Text ); i++ )
		{
			unsigned char c = (unsigned char)a_Text[i];
			if (c == 32) w += 4; else w += m_Width[m_Trans[c]] + 2;
		}
		return w;
	}

	void Font::Centre( Surface* a_Target, char* a_Text, int a_Y )
	{
		int x = (a_Target->GetPitch() - Width( a_Text )) / 2;
		Print( a_Target, a_Text, x, a_Y );
	}

	void Font::Print( Surface* a_Target, char* a_Text, int a_X, int a_Y, bool clip )
	{
		Pixel* b = a_Target->GetBuffer() + a_X + a_Y * a_Target->GetPitch();
		Pixel* s = m_Surface->GetBuffer();
		unsigned int i, cx;
		int x, y;
		if (((a_Y + m_Height) < m_CY1) || (a_Y > m_CY2)) return;
		for ( cx = 0, i = 0; i < strlen( a_Text ); i++ )
		{
			if (a_Text[i] == ' ') cx += 4; else
			{
				int c = m_Trans[(unsigned char)a_Text[i]];
				Pixel* t = s + m_Offset[c], *d = b + cx;
				if (clip)
				{
					for ( y = 0; y < m_Height; y++ )
					{
						if (((a_Y + y) >= m_CY1) && ((a_Y + y) <= m_CY2))
						{
							for ( x = 0; x < m_Width[c]; x++ ) 
								if ((t[x]) && ((x + (int)cx + a_X) < a_Target->GetPitch())) 
									d[x] = AddBlend( t[x], d[x] );
						}
						t += m_Surface->GetPitch(), d += a_Target->GetPitch();
					}
				}
				else
				{
					for ( y = 0; y < m_Height; y++ )
					{
						if (((a_Y + y) >= m_CY1) && ((a_Y + y) <= m_CY2))
							for ( x = 0; x < m_Width[c]; x++ ) if (t[x]) d[x] = AddBlend( t[x], d[x] );
						t += m_Surface->GetPitch(), d += a_Target->GetPitch();
					}
				}
				cx += m_Width[c] + 2;
				if ((int)(cx + a_X) >= a_Target->GetPitch()) break;
			}
		}
	}

}; // namespace Tmpl8
