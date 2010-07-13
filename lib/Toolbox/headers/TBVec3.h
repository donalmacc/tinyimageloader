#ifndef _TBVEC3_H_
#define _TBVEC3_H_

#include "TBSettings.h"
#include "TBMath.h"

namespace tb
{

	class Vec3
	{

	public:

		inline Vec3() {}

		inline Vec3(float a_X, float a_Y, float a_Z)
		{ 
			x = a_X;
			y = a_Y;
			z = a_Z;
		}

		inline Vec3(float* a_Vector)
		{ 
			x = a_Vector[0];
			y = a_Vector[1];
			z = a_Vector[2];
		}

		////////////////////////////////////////
		//                                    //
		//             DATA ACCESS            //
		//                                    //
		////////////////////////////////////////

		inline Vec3& operator = (const Vec3& a_Point)
		{
			x = a_Point.x;
			y = a_Point.y;
			z = a_Point.z;

			return *this;
		}
		inline Vec3& operator = (float a_Value)
		{
			x = y = z = a_Value;
			return *this;
		}

		inline float operator [] (int a_Member)
		{
			if (a_Member <= 2 && a_Member >= 0) { return (vector[a_Member]); }
			return (vector[3]);
		}

		////////////////////////////////////////
		//                                    //
		//              OPERATORS             //
		//                                    //
		////////////////////////////////////////

		inline Vec3 operator - ()
		{
			return (Vec3(-x, -y, -z));
		}

		
		inline Vec3& operator += (const Vec3& a_Point)
		{ 
			x += a_Point.x;
			y += a_Point.y;
			z += a_Point.z;

			return *this;
		}
		inline Vec3 operator + (const Vec3& a_Point) const
		{ 
			return Vec3(x + a_Point.x, y + a_Point.y, z + a_Point.z);
		}


		inline Vec3& operator -= (const Vec3& a_Point)
		{ 
			x -= a_Point.x;
			y -= a_Point.y;
			z -= a_Point.z;

			return *this;
		}
		inline Vec3 operator - (const Vec3& a_Point) const
		{
			return Vec3(x - a_Point.x, y - a_Point.y, z - a_Point.z);
		}


		inline Vec3& operator *= (float a_Scalar)
		{ 
			x *= a_Scalar;
			y *= a_Scalar;
			z *= a_Scalar;

			return *this;
		}
		inline Vec3& operator *= (const Vec3& a_Point)
		{ 
			x *= a_Point.x;
			y *= a_Point.y;
			z *= a_Point.z;

			return *this;
		}
		inline Vec3 operator * (float a_Value) const
		{ 
			return Vec3(x * a_Value, y * a_Value, z * a_Value);
		}
		inline Vec3 operator * (const Vec3& a_Point) const
		{ 
			return Vec3(x * a_Point.x, y * a_Point.y, z * a_Point.z);
		}


		inline Vec3& operator /= (float a_Value)				
		{ 
			x /= a_Value;
			y /= a_Value;
			z /= a_Value;

			return *this;
		}
		inline Vec3& operator /= (const Vec3& a_Point)		
		{ 
			x /= a_Point.x;
			y /= a_Point.y;
			z /= a_Point.z;

			return *this;
		}
		inline Vec3 operator / (float a_Value) const
		{ 
			const float rcp = 1 / a_Value;
			return Vec3(x * rcp, y * rcp, z * rcp);
		}
		inline Vec3 operator / (const Vec3& a_Point) const
		{ 
			return Vec3(x / a_Point.x, y / a_Point.y, z / a_Point.z);
		}


		////////////////////////////////////////
		//                                    //
		//             COMPARISONS            //
		//                                    //
		////////////////////////////////////////

		inline bool operator == (const Vec3& a_Point)
		{			
			if (x != a_Point.x || y != a_Point.y || z != a_Point.z) { return false; }
			return true;
		}
		inline bool operator == (float a_Value)
		{
			if (x != a_Value || y != a_Value || z != a_Value) { return false; }
			return true;
		}

		inline bool operator != (const Vec3& a_Point)
		{
			if (x == a_Point.x || y == a_Point.y || z == a_Point.z) { return false; }
			return true;
		}
		inline bool operator != (float a_Value)
		{
			if (x == a_Value || y == a_Value || z == a_Value) { return false; }
			return true;
		}

		inline bool operator > (const Vec3& a_Point)
		{
			if (x <= a_Point.x || y <= a_Point.y && z <= a_Point.z) { return false; }
			return true;
		}
		inline bool operator > (float a_Value)
		{
			if (x <= a_Value || y <= a_Value && z <= a_Value) { return false; }
			return true;
		}

		inline bool operator >= (const Vec3& a_Point)
		{
			if (x < a_Point.x || y < a_Point.y && z < a_Point.z) { return false; }
			return true;
		}
		inline bool operator >= (float a_Value)
		{
			if (x < a_Value || y < a_Value && z < a_Value) { return false; }
			return true;
		}

		inline bool operator < (const Vec3& a_Point)
		{
			if (x >= a_Point.x || y >= a_Point.y && z >= a_Point.z) { return false; }
			return true;
		}
		inline bool operator < (float a_Value)
		{
			if (x >= a_Value || y >= a_Value && z >= a_Value) { return false; }
			return true;
		}

		inline bool operator <= (const Vec3& a_Point)
		{
			if (x > a_Point.x || y > a_Point.y && z > a_Point.z) { return false; }
			return true;
		}
		inline bool operator <= (float a_Value)
		{
			if (x > a_Value || y > a_Value && z > a_Value) { return false; }
			return true;
		}

		// Clears the values of this Vec3.

		inline Vec3& Clear()
		{
			x = y = z = 0.f;
			return *this;
		}

		// Turns this Vec3 in the vector going in the opposite direction.

		inline Vec3& Reverse()
		{
			x *= -1.f;
			y *= -1.f;
			z *= -1.f;

			return *this;
		}

		// Turns this Vec3 into the minimum of itself and another Vec3.

		inline void Min(Vec3& a_Other)
		{
			x = (x < a_Other.x) ? x : a_Other.x;
			y = (y < a_Other.y) ? y : a_Other.y;
			z = (z < a_Other.z) ? z : a_Other.z;
		}

		// Turns this Vec3 into the maximum of itself and another Vec3.

		inline void Max(Vec3& a_Other)
		{
			x = (x > a_Other.x) ? x : a_Other.x;
			y = (y > a_Other.y) ? y : a_Other.y;
			z = (z > a_Other.z) ? z : a_Other.z;
		}

		inline void Clamp(float a_Min, float a_Max)
		{
			if (x < a_Min) { x = a_Min; }
			if (y < a_Min) { y = a_Min; }
			if (z < a_Min) { z = a_Min; }

			if (x > a_Max) { x = a_Max; }
			if (y > a_Max) { y = a_Max; }
			if (z > a_Max) { z = a_Max; }
		}

		// Turns this Vec3 into the reciprocal (1 / x) of itself.

		inline Vec3& Reciprocral()
		{
			x = 1 / x;
			y = 1 / y;
			z = 1 / z;

			return *this;
		}

		// Turns this Vec3 into the cross product of this Vec3 and another Vec3.

		inline Vec3& CrossProduct(const Vec3& a_Other)
		{
			const float oldx = x;
			const float oldy = y;
			const float oldz = z;

			x = (oldy * a_Other.z) - (oldz * a_Other.y);
			y = (oldz * a_Other.x) - (oldx * a_Other.z);
			z = (oldx * a_Other.y) - (oldy * a_Other.x);

			return *this;
		}

		// Turns this Vec3 into the normalized version of itself.

		inline Vec3& Normalize()
		{
			const float length = 1.f / GetLength();

			x *= length;
			y *= length;
			z *= length;

			return *this;
		}

		// Returns the vector in the opposite direction of this Vec3.

		inline Vec3 GetReverse() const
		{
			return (Vec3(-x, -y, -z));
		}

		// Returns the minimum of this Vec3 and another Vec3.

		inline Vec3 GetMin(Vec3& a_Other)
		{
			Vec3 temp(x, y, z);
			temp.Min(a_Other);
			return temp;
		}

		// Returns the maximum of this Vec3 and another Vec3.

		inline Vec3 GetMax(Vec3& a_Other)
		{
			Vec3 temp(x, y, z);
			temp.Max(a_Other);
			return temp;
		}

		

		// Returns the squared length of this Vec3.

		inline float GetSquaredLength() const
		{
			return ((x * x) + (y * y) + (z * z));
		}

		// Returns the length of this Vec3. NOTE: USE GetSquaredLength WHEREVER POSSIBLE DUE TO SPEED

		inline float GetLength() const
		{
			return (Math::Sqrt(GetSquaredLength()));
		}

		// Returns the normalized version of this Vec3. NOTE: SLOW

		inline Vec3 GetNormalized() const
		{
			const float length = 1.f / GetLength();

			return (Vec3(x * length, y * length, z * length));
		}

		// Returns a Vec3 with the reciprocal (1 / x) of this Vec3.

		inline Vec3 GetReciprocal() const
		{
			return (Vec3(1 / x, 1 / y, 1 / z));
		}

		// Returns the dot product between this Vec3 and another Vec3.

		inline float GetDotProduct(const Vec3& a_Other) const
		{
			return (
				(x * a_Other.x) +
				(y * a_Other.y) +
				(z * a_Other.z)
			);
		}

		// Returns the cross product of this Vec3 and another Vec3.

		inline Vec3 GetCrossProduct(const Vec3& a_Other) const
		{
			Vec3 temp;

			temp.x = (y * a_Other.z) - (z * a_Other.y);
			temp.y = (z * a_Other.x) - (x * a_Other.z);
			temp.z = (x * a_Other.y) - (y * a_Other.x);

			return temp;
		}

		// Returns the squared distance between this Vec3 and another Vec3.

		inline float GetSquaredDistanceBetween(const Vec3& a_Other) const
		{
			const float diffX = x - a_Other.x;
			const float diffY = y - a_Other.y;
			const float diffZ = z - a_Other.z;

			return (
				(diffX * diffX) + 
				(diffY * diffY) + 
				(diffZ * diffZ)
			);
		}

		// Returns the distance between this Vec3 and another Vec3. NOTE: USE GetSquaredDistanceBetween WHEREVER POSSIBLE DUE TO SPEED

		inline float GetDistanceBetween(const Vec3& a_Other) const
		{
			return (sqrtf(GetSquaredDistanceBetween(a_Other)));
		}

		// Returns the spherical interpolation between this Vec3 and another Vec3 at a_Percentage (between 0 and 1).

		// TODO: Rip working version from Relativity
		inline Vec3 GetSphericalInterpolationNormalized(const Vec3& a_Other, float a_Percentage)
		{
			/*const float angle = ACOS(GetDotProduct(a_Other));
			const float sinm = SIN((1 - a_Percentage) * angle);
			const float sino = SIN(a_Percentage * angle);
			return (((*this * sinm) + (a_Other * sino)) / SIN(angle));*/

			return Vec3(0);
		}

		inline Vec3 GetReflectedVector(Vec3 a_Normal)
		{
			/*const float dot = 2.f * ((x * a_Normal.x) + (y * a_Normal.y) + (z * a_Normal.z));

			Vec3 result
			(
				x - (dot * a_Normal.x),
				y - (dot * a_Normal.y), 
				z - (dot * a_Normal.z)
			);

			return result;*/

			// 2 * normal.dot(this) * normal - this
			// *this - 2.0f * GetDotProduct(a_Normal) * a_Normal
			return (((a_Normal * GetDotProduct(a_Normal) * 2.f) - *this) * -1);

		}

		// Returns the halfway vector (the vector BETWEEN two vectors)

		inline Vec3 GetHalfWayVector(Vec3& a_Other)
		{
			Vec3 halfway(x + a_Other.x, y + a_Other.y, z + a_Other.z);
			halfway.Normalize();

			return halfway;
		}

		// Storage container for the vector data.

		union 
		{ 
			struct { float x, y, z, w; }; 
			struct { float vector[4]; };
		};

	}; // class Vec3

};

#endif