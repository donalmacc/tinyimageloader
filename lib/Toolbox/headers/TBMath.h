#ifndef _TBMATH_H_
#define _TBMATH_H_

// ===========================================
// Includes
// ===========================================

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "TBSettings.h"

namespace tb
{

	namespace Math
	{

		// ====================================
		// Constants
		// ====================================

		// hmm... pi...

		const float pi			= 3.141592654f;
		const float pi_radians	= pi / 180.f;
		const float pi_degrees	= 180.f / pi;
		const float pi_half		= pi / 2;
		const float pi_two		= pi * 2;
		const float pi_rcp		= 1 / pi;
		const float pi_rcp_two	= 1 / (pi_two);

		// ====================================
		// Basic functions
		// ====================================

		const float fp_amount = (float)(0xFFFF);
		const float fp_amount_inv = 1 / fp_amount;

		// Returns the fractional part of a float.
		inline float Frac(float a_X)
		{
			return (a_X - (float)(int)a_X);

			//return ((int)(a_X * fp_amount) & 0xFFFF) * fp_amount_inv;
		}

		// Returns the inverse fractional part of a float (1 - x).
		inline float InvFrac(float a_X)
		{
			return (1.f - (a_X - (float)(int)a_X));

			//return (0xFFFF - ((int)(a_X * fp_amount) & 0xFFFF)) * fp_amount_inv;
		}

		// Returns the number rounded to the nearest integer.
		inline int Round(float a_X) 
		{ 
			return ((int)(a_X + 0.5f)); 
		} 

		// Returns the number rounded down to the nearest integer.
		inline float Floor(float a_X) 
		{ 
			return (floorf(a_X));
			//return ((float)(int)a_X); 
		}

		// Returns the number rounded up to the nearest integer.
		inline float Ceil(float a_X) 
		{ 
			return (ceilf(a_X));
			//return ((float)((int)(a_X + 1))); 
		}

		// Returns the absolute value of a number.
		inline float Abs(float a_X)
		{
			return (fabsf(a_X));
			//return ((a_X < 0) ? -a_X : a_X);
		}

		// Returns the square root of a number.
		inline float Sqrt(float a_X) 
		{ 
			return (sqrtf(a_X)); 
		}

		// ====================================
		// Angle math
		// ====================================

		// Converts degrees (0 - 360) to radians (0 - 2 pi).
		inline float DegToRad(float a_Degrees) { return (a_Degrees * pi_radians); }

		// Converts radians (0 - 2 pi) to degrees (0 - 360).
		inline float RadToDeg(float a_Radians) { return (a_Radians * pi_degrees); }

		// Returns the sine of an amount of radians.
		inline float Sin(float a_Radians) { return sinf(a_Radians); }

		// Returns the sine of an amount of degrees. (You'll probably want this.)
		inline float SinDeg(float a_Degrees) { return sinf(a_Degrees * pi_radians); }

		// Returns the cosine of an amount of radians.
		inline float Cos(float a_Radians) { return cosf(a_Radians); }

		// Returns the cosine of an amount of degrees. (You'll probably want this.)
		inline float CosDeg(float a_Degrees) { return cosf(a_Degrees * pi_radians); }

		inline float ArcCos(float a_Radians) { return acosf(a_Radians); }
		inline float ArcCosDeg(float a_Degrees) { return acosf(a_Degrees * pi_radians); }

		inline float ArcSin(float a_Radians) { return asinf(a_Radians); }
		inline float ArcSinDeg(float a_Degrees) { return asinf(a_Degrees * pi_radians); }

		// ====================================
		// Randomness
		// ====================================

		// Seeds the random number generator
		// If a seed of 0 is specified, the function inputs the current time in milliseconds as the seed.
		inline void Seed(unsigned int a_Seed = 0) 
		{ 
			unsigned int seed = (a_Seed == 0) ? (unsigned int)(time(0)) : a_Seed;
			srand(seed);
		}

		// Outputs a random number between 0 and 1.
		inline float Random() 
		{ 
			return (((float)rand() / RAND_MAX)); 
		}

		// Outputs a random number between 0 and a_Max.
		inline float Random(float a_Max) 
		{ 
			return (((float)rand() / RAND_MAX) * (a_Max)); 
		}

		// Outputs a random number between a_Min and a_Max.
		inline float Random(float a_Min, float a_Max) 
		{ 
			return a_Min + (((float)rand() / RAND_MAX) * (a_Max - a_Min)); 
		}

	}; // namespace Math

}; // namespace tb

#define MALLOC64(x) _aligned_malloc(x,64)
#define MALLOC32(x) _aligned_malloc(x,32)
#define FREE64(x) _aligned_free(x)
#define FREE32(x) _aligned_free(x)

#define MIN(a, b)		(((a) < (b)) ? (a) : (b))
#define MAX(a, b)		(((a) > (b)) ? (a) : (b))
#define SWAP(a, b, tmp)	(tmp = a, a = b, b = tmp)
//#define LERP(a,b,f)		((a) + (((b) - (a)) * (f)))
//#define ILERP(a,b,f)	(((f)-(a))/((b)-(a)))

#endif