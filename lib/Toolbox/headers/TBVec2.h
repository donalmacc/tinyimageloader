#ifndef _TBVEC2_H_
#define _TBVEC2_H_

#include "TBSettings.h"
#include "TBMath.h"

namespace tb
{

	class Vec2
	{

	public:

		Vec2() 
		{ 
			x = y = 0.f; 
		}

		Vec2(float a_X, float a_Y)
		{
			x = a_X;
			y = a_Y;
		}

		Vec2(float* a_Vector)
		{ 
			x = a_Vector[0];
			y = a_Vector[1];
		}

		////////////////////////////////////////
		//                                    //
		//             DATA ACCESS            //
		//                                    //
		////////////////////////////////////////

		Vec2& operator = (const Vec2& a_Point)
		{
			x = a_Point.x;
			y = a_Point.y;
			return *this;
		}
		Vec2& operator = (float a_Value)
		{
			x = y = a_Value;
			return *this;
		}

		float operator [] (int a_Member)
		{
			switch (a_Member)
			{
			case 0:
				return vector[0];
				break;
			case 1:
				return vector[1];
				break;
			}

			return (0);
		}

		////////////////////////////////////////
		//                                    //
		//              OPERATORS             //
		//                                    //
		////////////////////////////////////////

		Vec2& operator += (const Vec2& a_Point)
		{ 
			x += a_Point.x;
			y += a_Point.y;
			return *this;
		}
		Vec2 operator + (const Vec2& a_Point) 
		{ 
			Vec2 result(x + a_Point.x, y + a_Point.y);
			return result;
		}

		Vec2& operator -= (const Vec2& a_Point)
		{ 
			x -= a_Point.x;
			y -= a_Point.y;
			return *this;
		}
		Vec2 operator - (const Vec2& a_Point)
		{
			Vec2 result(x - a_Point.x, y - a_Point.y);
			return result;
		}

		Vec2& operator *= (float a_Scalar)			
		{ 
			x *= a_Scalar;
			y *= a_Scalar;
			return *this;
		}
		Vec2& operator *= (const Vec2& a_Point)		
		{ 
			x *= a_Point.x;
			y *= a_Point.y;
			return *this;
		}
		Vec2 operator * (float a_Value) 
		{ 
			Vec2 result(x * a_Value, y * a_Value);
			return result;
		}
		Vec2 operator * (const Vec2& a_Point) 
		{ 
			Vec2 result(x * a_Point.x, y * a_Point.y);
			return result;
		}

		Vec2& operator /= (float a_Value)				
		{ 
			const float inv = 1 / a_Value;
			x *= inv;
			y *= inv;
			return *this;
		}
		Vec2& operator /= (const Vec2& a_Point)		
		{ 
			x /= a_Point.x;
			x /= a_Point.y;
			return *this;
		}
		Vec2 operator / (float a_Value) 
		{ 
			const float inv = 1 / a_Value;
			Vec2 result(x * inv, y * inv);
			return result;
		}
		Vec2 operator / (const Vec2& a_Point) 
		{ 
			Vec2 result(x / a_Point.x, y / a_Point.y);
			return result;
		}

		////////////////////////////////////////
		//                                    //
		//             COMPARISONS            //
		//                                    //
		////////////////////////////////////////

		bool operator == (const Vec2& a_Point)
		{			
			if (x != a_Point.x || y != a_Point.y) { return false; }
			return true;
		}
		bool operator == (float a_Value)
		{
			if (x != a_Value || y != a_Value) { return false; }
			return true;
		}

		bool operator != (const Vec2& a_Point)
		{
			if (x == a_Point.x || y == a_Point.y) { return false; }
			return true;
		}
		bool operator != (float a_Value)
		{
			if (x == a_Value || y == a_Value) { return false; }
			return true;
		}

		bool operator > (const Vec2& a_Point)
		{
			if (x <= a_Point.x || y <= a_Point.y) { return false; }
			return true;
		}
		bool operator > (float a_Value)
		{
			if (x <= a_Value || y <= a_Value) { return false; }
			return true;
		}

		bool operator >= (const Vec2& a_Point)
		{
			if (x < a_Point.x || y < a_Point.y) { return false; }
			return true;
		}
		bool operator >= (float a_Value)
		{
			if (x < a_Value || y < a_Value) { return false; }
			return true;
		}

		bool operator < (const Vec2& a_Point)
		{
			if (x >= a_Point.x || y >= a_Point.y) { return false; }
			return true;
		}
		bool operator < (float a_Value)
		{
			if (x >= a_Value || y >= a_Value) { return false; }
			return true;
		}

		bool operator <= (const Vec2& a_Point)
		{
			if (x < a_Point.x || y < a_Point.y) { return false; }
			return true;
		}
		bool operator <= (float a_Value)
		{
			if (x > a_Value || y > a_Value) { return false; }
			return true;
		}

		////////////////////////////////////////
		//                                    //
		//      VALUE CHANGING FUNCTIONS      //
		//                                    //
		////////////////////////////////////////

		Vec2& Reverse()
		{
			x *= -1.f;
			y *= -1.f;

			return *this;
		}

		Vec2& Reciprocral()
		{
			x = 1 / x;
			y = 1 / y;

			return *this;
		}

		Vec2& Normalize()
		{
			const float length = 1 / GetLength();
			x *= length;
			y *= length;

			return *this;
		}

		Vec2& LERP(Vec2& a_Other, float a_Percentage)
		{
			x = (a_Percentage * x) + ((1.f - a_Percentage) * a_Other.x);
			y = (a_Percentage * y) + ((1.f - a_Percentage) * a_Other.y);

			return *this;
		}

		////////////////////////////////////////
		//                                    //
		//      VALUE RETURNING FUNCTIONS     //
		//                                    //
		////////////////////////////////////////

		// Returns the squared length of this Vec2.

		float GetSquaredLength() const
		{
			return ((x * x) + (y * y));
		}

		// Returns the length of this Vec2. NOTE: USE GetSquaredLength WHEREVER POSSIBLE DUE TO SPEED

		float GetLength() const
		{
			return (sqrtf((x * x) + (y * y)));
		}

		// Returns the normalized version of this Vec2. NOTE: SLOW

		Vec2 GetNormalized() const
		{
			const float length = 1 / sqrtf((x * x) + (y * y));
			Vec2 result(x * length, y * length);
			return (result);
		}

		// Returns a Vec2 with the reciprocal (1 / x) of this Vec2.

		Vec2 GetReciprocal() const
		{
			Vec2 result(1 / x, 1 / y);
			return result;
		}

		Vec2 GetReverse()
		{
			Vec2 result(-x, -y);
			return result;
		}

		// Returns the dot product between this Vec2 and another Vec2.

		float GetDotProduct(const Vec2& a_Other) const
		{
			return (
				(x * a_Other.x) +
				(y * a_Other.y)
			);
		}

		// Returns the squared distance between this Vec3 and another Vec3.

		float GetSquaredDistanceBetween(const Vec2& a_Other) const
		{
			const float diffX = x - a_Other.x;
			const float diffY = y - a_Other.y;

			return (
				(diffX * diffX) + 
				(diffY * diffY)
			);
		}

		// Returns the distance between this Vec2 and another Vec2. NOTE: USE GetSquaredDistanceBetween WHEREVER POSSIBLE DUE TO SPEED

		float GetDistanceBetween(const Vec2& a_Other) const
		{
			return (sqrtf(GetSquaredDistanceBetween(a_Other)));
		}

		// Return the LERP between this and another vector

		Vec2 GetLERP(Vec2& a_Other, float a_Percentage)
		{
			Vec2 result(
				(a_Percentage * x) + ((1.f - a_Percentage) * a_Other.x),
				(a_Percentage * y) + ((1.f - a_Percentage) * a_Other.y)
			);

			return result;
		}

		// Storage container for the vector data.

		union
		{
			struct { float x, y; };
			struct { float u, v; };
			float vector[2];
		};

	}; // class Vec2

}; // namespace tb

#endif