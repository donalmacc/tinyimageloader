#ifndef _TBCOLOR_H_
#define _TBCOLOR_H_

#include "TBSettings.h"
#include "TBPixel.h"
#include "TBMath.h"

#include <string.h>

namespace tb
{

	class Color
	{

	public:

		// Constructors

		inline Color()
		{
			r = g = b = 0.f;
			a = 1.f;
		}
		inline Color(float a_RGB)
		{
			r = g = b = a_RGB;
			a = 1.f;
		}
		inline Color(float a_R, float a_G, float a_B)
		{
			r = a_R;
			g = a_G;
			b = a_B;
			a = 1.f;
		}
		inline Color(unsigned long a_Color)
		{
			r = (float)((a_Color & TB_COLOR_MASK_R) >> TB_COLOR_SHIFT_R) / TB_COLOR_BITMASK_FR;
			g = (float)((a_Color & TB_COLOR_MASK_G) >> TB_COLOR_SHIFT_G) / TB_COLOR_BITMASK_FG;
			b = (float)((a_Color & TB_COLOR_MASK_B) >> TB_COLOR_SHIFT_B) / TB_COLOR_BITMASK_FB;
			a = 1.f;
		}

		// Public methods

		inline Color& Clear()
		{
			r = g = b = 0.f;
			a = 1.f;
			return *this;
		}

		void ConstructFromHTML(char* a_HTMLColor)
		{
			// a valid color is #FFFF00
			if (strlen(a_HTMLColor) != 7) { return; }
			if (a_HTMLColor[0] != '#') { return; }

			// skip the #
			a_HTMLColor++;

			int number[2];
			int current = 0;
			int color = 0;
			while (*a_HTMLColor)
			{
				// A - F = 10 - 15
				if (*a_HTMLColor >= 65)			{ number[current] = *a_HTMLColor - 55; }

				// 0 - 9 = 1 - 10
				else if (*a_HTMLColor >= 48)	{ number[current] = *a_HTMLColor - 48; }

				current++;
				if (current == 2)
				{
					rgb[color] = (float)((number[0] * 16) + number[1]) / 255.f;
					if (color++ == 3) { break; }
					current = 0;
				}

				a_HTMLColor++;
			}
		}

		inline float* GetOpenGLColor()
		{
			return &rgb[0];
		}

		inline Pixel GetColor()
		{
			Pixel R, G, B;

			if (r > 1.f)		{ R = TB_COLOR_BITMASK_R; }
			else if (r < 0.f)	{ R = 0x000000; }
			else				{ R = (unsigned long)(r * TB_COLOR_BITMASK_FR) << TB_COLOR_SHIFT_R; }

			if (g > 1.f)		{ G = TB_COLOR_BITMASK_G; }
			else if (g < 0.f)	{ G = 0x000000; }
			else				{ G = (unsigned long)(g * TB_COLOR_BITMASK_FG) << TB_COLOR_SHIFT_G; }

			if (b > 1.f)		{ B = TB_COLOR_BITMASK_B; }
			else if (b < 0.f)	{ B = 0x000000; }
			else				{ B = (unsigned long)(b * TB_COLOR_BITMASK_FB) << TB_COLOR_SHIFT_B; }

			return (R + G + B);
		}

		inline Color& operator = (Pixel a_Color)
		{
			r = (float)((a_Color & TB_COLOR_MASK_R) >> TB_COLOR_SHIFT_R) / TB_COLOR_BITMASK_FR;
			g = (float)((a_Color & TB_COLOR_MASK_G) >> TB_COLOR_SHIFT_G) / TB_COLOR_BITMASK_FG;
			b = (float)((a_Color & TB_COLOR_MASK_B) >> TB_COLOR_SHIFT_B) / TB_COLOR_BITMASK_FB;
			return *this;
		}

		inline Color& operator = (const Color& a_Other)
		{
			r = a_Other.r;
			g = a_Other.g;
			b = a_Other.b;
			return *this;
		}

		inline Color& operator = (float a_Scalar)
		{
			r = a_Scalar;
			g = a_Scalar;
			b = a_Scalar;
			return *this;
		}

		inline Color& operator += (const Color& a_Other)
		{
			r += a_Other.r;
			g += a_Other.g;
			b += a_Other.b;
			//ClampDown();
			return *this;
		}
		inline Color& operator += (float a_Scalar)
		{
			r += a_Scalar;
			g += a_Scalar;
			b += a_Scalar;
			//ClampDown();
			return *this;
		}
		inline Color& operator += (unsigned long a_Color)
		{
			Color temp(a_Color);
			*this += temp;
			//ClampUp();
			return *this;
		}
		inline Color operator + (const Color& a_Other)
		{
			Color result(r, g, b);
			result += a_Other;
			return result;
		}
		inline Color operator + (float a_Scalar)
		{
			Color result(r, g, b);
			result += a_Scalar;
			return result;
		}


		inline Color& operator -= (const Color& a_Other)
		{
			r -= a_Other.r;
			g -= a_Other.g;
			b -= a_Other.b;
			//ClampDown();
			return *this;
		}
		inline Color& operator -= (float a_Scalar)
		{
			r -= a_Scalar;
			g -= a_Scalar;
			b -= a_Scalar;
			//ClampDown();
			return *this;
		}
		inline Color operator - (const Color& a_Other)
		{
			Color result(r, g, b);
			result -= a_Other;
			return result;
		}
		inline Color operator - (float a_Scalar)
		{
			Color result(r, g, b);
			result -= a_Scalar;
			return result;
		}


		inline Color& operator *= (const Color& a_Other)
		{
			r *= a_Other.r;
			g *= a_Other.g;
			b *= a_Other.b;
			//ClampDown();
			return *this;
		}
		inline Color& operator *= (float a_Scalar)
		{
			r *= a_Scalar;
			g *= a_Scalar;
			b *= a_Scalar;
			//ClampDown();
			return *this;
		}
		inline Color operator * (const Color& a_Other)
		{
			Color result(r, g, b);
			result *= a_Other;
			return result;
		}
		inline Color operator * (float a_Scalar)
		{
			Color result(r, g, b);
			result *= a_Scalar;
			return result;
		}


		inline Color& operator /= (const Color& a_Other)
		{
			r /= a_Other.r;
			g /= a_Other.g;
			b /= a_Other.b;
			//ClampDown();
			return *this;
		}
		inline Color& operator /= (float a_Scalar)
		{
			r /= a_Scalar;
			g /= a_Scalar;
			b /= a_Scalar;
			//ClampDown();
			return *this;
		}
		inline Color operator / (const Color& a_Other)
		{
			Color result(r, g, b);
			result /= a_Other;
			return result;
		}
		inline Color operator / (float a_Scalar)
		{
			Color result(r, g, b);
			result /= a_Scalar;
			return result;
		}

		float GetLength()
		{
			return (r + g + b);
		}

		inline Color LERP(Color* a_Other, float a_Percentage)
		{
			Color result;
			result.r = r + (a_Other->r - r) * a_Percentage;
			result.g = g + (a_Other->g - g) * a_Percentage;
			result.b = b + (a_Other->b - b) * a_Percentage;
			return result;
		}

		inline Color& ClampUp(float a_Upper = 1.f)
		{
			r = (r > a_Upper) ? a_Upper : r;
			g = (g > a_Upper) ? a_Upper : g;
			b = (b > a_Upper) ? a_Upper : b;
			return *this;
		}

		inline Color& ClampDown(float a_Lower = 0.f)
		{
			r = (r < a_Lower) ? a_Lower : r;
			g = (g < a_Lower) ? a_Lower : g;
			b = (b < a_Lower) ? a_Lower : b;
			return *this;
		}

		inline Color& Clamp(float a_Lower = 0.f, float a_Upper = 1.f)
		{
			ClampDown(a_Lower);
			ClampUp(a_Upper);
			return *this;
		}

		// Public members

		union
		{
			struct { float rgb[4]; };
			struct { float r, g, b, a; };
		};

	}; // class Color

}; // namespace tb

#endif