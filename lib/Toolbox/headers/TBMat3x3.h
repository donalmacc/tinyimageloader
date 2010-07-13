#ifndef _TBMAT3X3_H_
#define _TBMAT3X3_H_

#include "TBSettings.h"
#include "TBMath.h"
#include "TBVec3.h"

namespace tb
{

	class Mat3x3
	{

	public:

		////////////////////////////////////////////
		//                                        //
		//              CONSTRUCTORS              //
		//                                        //
		////////////////////////////////////////////

		Mat3x3() 
		{
			Identity();
		}

		Mat3x3
		(
			float a_X1, float a_Y1, float a_Z1,
			float a_X2, float a_Y2, float a_Z2,
			float a_X3, float a_Y3, float a_Z3
		)
		{
			values[0] = a_X1;
			values[1] = a_Y1;
			values[2] = a_Z1;

			values[3] = a_X2;
			values[4] = a_Y2;
			values[5] = a_Z2;

			values[6] = a_X3;
			values[7] = a_Y3;
			values[8] = a_Z3;
		}

		Mat3x3 (Mat3x3& a_Other)
		{
			Copy(a_Other);
		}

		////////////////////////////////////////////
		//                                        //
		//                  HOOKS                 //
		//                                        //
		////////////////////////////////////////////

		float GetValue(int a_X, int a_Y)
		{
			return (values[(a_X - 1) + ((a_Y - 1) * 3)]);
		}

		////////////////////////////////////////////
		//                                        //
		//               OPERATORS                //
		//                                        //
		////////////////////////////////////////////

		Mat3x3& operator = (Mat3x3& a_Other)
		{
			Copy(a_Other);
			return *this;
		}

		Vec3 operator * (Vec3& a_Point)
		{
			Vec3 temp (
				(a_Point.x * values[0]) + (a_Point.y * values[1]) + (a_Point.z * values[2]),
				(a_Point.x * values[3]) + (a_Point.y * values[4]) + (a_Point.z * values[5]),
				(a_Point.x * values[6]) + (a_Point.y * values[7]) + (a_Point.z * values[8])
			);

			return (temp);
		}

		Mat3x3 operator * (Mat3x3& a_Other)
		{
			Mat3x3 temp (
				(values[0] * a_Other.values[0]), (values[1] * a_Other.values[1]), (values[2] * a_Other.values[2]), 
				(values[3] * a_Other.values[3]), (values[4] * a_Other.values[4]), (values[5] * a_Other.values[5]), 
				(values[6] * a_Other.values[6]), (values[7] * a_Other.values[7]), (values[8] * a_Other.values[8])
			);

			return (temp);
		}

		Mat3x3 operator += (Mat3x3& a_Other)
		{
			// [ (a11 * b11) + (a12 * b21) + (a13 * b31) ] 
			// [ (a21 * b11) + (a22 * b21) + (a23 * b31) ] 
			// [ (a31 * b11) + (a32 * b21) + (a33 * b31) ] 

			// [ (a11 * b12) + (a12 * b22) + (a13 * b32) ] 
			// [ (a21 * b12) + (a22 * b22) + (a23 * b32) ] 
			// [ (a31 * b12) + (a32 * b22) + (a33 * b32) ] 

			// [ (a11 * b13) + (a12 * b23) + (a13 * b33) ] 
			// [ (a21 * b13) + (a22 * b23) + (a23 * b33) ] 
			// [ (a31 * b13) + (a32 * b23) + (a33 * b33) ] 

			Concatenate(a_Other);

			return *this;
		}

		Mat3x3& Concatenate(Mat3x3& a_Other)
		{
			Mat3x3 result;

			for (int c = 0; c < 3; ++c)
			{
				for (int r = 0; r < 3; ++r)
				{
					result.values[r * 3 + c] = 
						(values[r * 3 + 0] * a_Other.values[c + 0]) +
						(values[r * 3 + 1] * a_Other.values[c + 3]) +
						(values[r * 3 + 2] * a_Other.values[c + 6]);
				}
			}

			Copy(result);

			return *this;
		}

		Mat3x3 operator + (Mat3x3& a_Other)
		{
			Mat3x3 result(*this);
			result += a_Other;
			return result;
		}

		////////////////////////////////////////////
		//                                        //
		//                FUNCTIONS               //
		//                                        //
		////////////////////////////////////////////

		// Clears the values of this Mat3x3.

		void Clear()
		{
			memset(values, 0, sizeof(values));
		}

		// Sets this Mat3x3 to the identity matrix.

		void Identity()
		{
			/*     */   values[1] = values[2] =
			values[3] =	/*	   */   values[5] =
			values[6] =	values[7]   /*     */ = 0.f;

			values[0] = /*     */   /*     */
			/*     */   values[4] = /*     */
			/*     */   /*     */   values[8] = 1.f;
		}

		// Inverts this Mat3x3.

		Mat3x3& Invert()
		{
			//        / [ a11 a12 a13 ] \    [ a11 a21 a31 ]
			// Invert | [ a21 a22 a23 ] | =  [ a12 a22 a32 ]
			//        \ [ a31 a32 a33 ] /    [ a13 a23 a33 ]

			float store;

			store = values[1];
			values[1] = values[3];
			values[3] = store;

			store = values[2];
			values[2] = values[6];
			values[6] = store;

			store = values[5];
			values[5] = values[7];
			values[7] = store;

			return *this;
		}

		// Returns the inverted version of this Mat3x3.

		Mat3x3 GetInverted()
		{
			Mat3x3 temp(*this);
			temp.Invert();
			return temp;
		}

		Mat3x3 Transpose()
		{
			return (
				Mat3x3 (
					values[0 * 3 + 0], values[1 * 3 + 0], values[2 * 3 + 0],
					values[0 * 3 + 1], values[1 * 3 + 1], values[2 * 3 + 1],
					values[0 * 3 + 2], values[1 * 3 + 2], values[2 * 3 + 2]
				)
			);
		}

		Mat3x3& Scale(Vec3 a_Scale)
		{
			values[0] *= a_Scale.x; 
			values[4] *= a_Scale.y;
			values[8] *= a_Scale.z;

			return *this;
		}

		// Copies the values of another Mat3x3 into this Mat3x3.

		Mat3x3& Copy(Mat3x3& a_Other)
		{
			values[0] = a_Other.values[0];
			values[1] = a_Other.values[1];
			values[2] = a_Other.values[2];

			values[3] = a_Other.values[3];
			values[4] = a_Other.values[4];
			values[5] = a_Other.values[5];

			values[6] = a_Other.values[6];
			values[7] = a_Other.values[7];
			values[8] = a_Other.values[8];

			return *this;
		}

		////////////////////////////////////////////
		//                                        //
		//                 ROTATION               //
		//                                        //
		////////////////////////////////////////////

		// Adds a rotation on the X axis to this Mat3x3.

		Mat3x3& AddRotationX(float a_Degrees)
		{
			a_Degrees = Math::DegToRad(a_Degrees);

			const float cosAngle = Math::Cos(a_Degrees);
			const float sinAngle = Math::Sin(a_Degrees);

			// [ (a11 * b11) + (a12 * b21) + (a13 * b31) ] 
			// [ (a21 * b11) + (a22 * b21) + (a23 * b31) ] 
			// [ (a31 * b11) + (a32 * b21) + (a33 * b31) ] 

			// [ (a11 * b12) + (a12 * b22) + (a13 * b32) ] 
			// [ (a21 * b12) + (a22 * b22) + (a23 * b32) ] 
			// [ (a31 * b12) + (a32 * b22) + (a33 * b32) ] 

			// [ (a11 * b13) + (a12 * b23) + (a13 * b33) ] 
			// [ (a21 * b13) + (a22 * b23) + (a23 * b33) ] 
			// [ (a31 * b13) + (a32 * b23) + (a33 * b33) ] 

			float /*x, */y, z;

			//x = values[0];
			y   = values[1];
			z   = values[2];

			//values[0] =   (x * 1.f) +   (y * 0.f)      + (z * 0.f);
			values[1]   = /*(x * 0.f) + */(y * cosAngle) + (z * -sinAngle);
			values[2]   = /*(x * 0.f) + */(y * sinAngle) + (z * cosAngle);

			//x = values[3];
			y = values[4];
			z = values[5];

			//values[3] = (x * 1.f)   +   (y * 0.f)      + (z * 0.f);
			values[4]   = /*(y * 0.f) + */(y * cosAngle) + (z * -sinAngle);
			values[5]   = /*(z * 0.f) + */(y * sinAngle) + (z * cosAngle);

			//x = values[6];
			y   = values[7];
			z   = values[8];


			//values[6] = (x * 1.f)   +   (y * 0.f)      + (z * 0.f);
			values[7]   = /*(x * 0.f) + */(y * cosAngle) + (z * -sinAngle);
			values[8]   = /*(x * 0.f) + */(y * sinAngle) + (z * cosAngle);

			return *this;
		}

		// Sets this Mat3x3 to a rotation by a_Degrees on the X axis.

		Mat3x3& setRotationX(float a_Degrees)
		{
			Clear();
			_rotateAxis(0, a_Degrees, true);

			return *this;
		}

		// Returns a Mat3x3 that is this Mat3x3 rotated by a_Degrees on the X axis.

		Mat3x3 getRotatedX(float a_Degrees)
		{
			Mat3x3 result (
				values[0], values[1], values[2], 
				values[3], values[4], values[5], 
				values[6], values[7], values[8] 
			);
			result.AddRotationX(a_Degrees);
			return result;
		}


		// Adds a rotation on the Y axis to this Mat3x3.

		Mat3x3& AddRotationY(float a_Degrees)
		{
			a_Degrees = Math::DegToRad(a_Degrees);

			const float cosAngle = Math::Cos(a_Degrees);
			const float sinAngle = Math::Sin(a_Degrees);

			// [ (a11 * b11) + (a12 * b21) + (a13 * b31) ] 
			// [ (a21 * b11) + (a22 * b21) + (a23 * b31) ] 
			// [ (a31 * b11) + (a32 * b21) + (a33 * b31) ] 

			// [ (a11 * b12) + (a12 * b22) + (a13 * b32) ] 
			// [ (a21 * b12) + (a22 * b22) + (a23 * b32) ] 
			// [ (a31 * b12) + (a32 * b22) + (a33 * b32) ] 

			// [ (a11 * b13) + (a12 * b23) + (a13 * b33) ] 
			// [ (a21 * b13) + (a22 * b23) + (a23 * b33) ] 
			// [ (a31 * b13) + (a32 * b23) + (a33 * b33) ] 

			float x, /*y,*/ z;

			x   = values[0];
			//y = values[1];
			z   = values[2];

			values[0]   =   (x * cosAngle)/* + (y * 0.f)*/ + (z * -sinAngle);
			//values[1] = (x * 0.f)	         + (y * 1.f)   + (z * 0);
			values[2]   =   (x * sinAngle)/* + (y * 0.f)*/ + (z * cosAngle);

			x   = values[3];
			//y = values[4];
			z   = values[5];

			values[3]   =   (x * cosAngle)/* + (y * 0.f)*/ + (z * -sinAngle);
			//values[4] = (x * 0.f)          + (y * 1.f)   + (z * 0);
			values[5]   =   (x * sinAngle)/* + (y * 0.f)*/ + (z * cosAngle);

			x   = values[6];
			//y = values[7];
			z   = values[8];

			values[6]   =   (x * cosAngle)/* + (y * 0.f)*/ + (z * -sinAngle);
			//values[7] = (x * 0.f)          + (y * 1.f)   + (z * 0);
			values[8]   =   (x * sinAngle)/* + (y * 0.f)*/ + (z * cosAngle);

			return *this;
		}

		// Sets this Mat3x3 to a rotation by a_Degrees on the Y axis.

		Mat3x3& SetRotationY(float a_Degrees)
		{
			Clear();
			_rotateAxis(1, a_Degrees, true);
			return *this;
		}

		// Returns a Mat3x3 that is this Mat3x3 rotated by a_Degrees on the Y axis.

		Mat3x3 GetRotatedY(float a_Degrees)
		{
			Mat3x3 result (
				values[0], values[1], values[2], 
				values[3], values[4], values[5], 
				values[6], values[7], values[8] 
			);
			result.AddRotationY(a_Degrees);
			return result;
		}


		// Adds a rotation on the Z axis to this Mat3x3.

		Mat3x3& AddRotationZ(float a_Degrees)
		{
			a_Degrees = Math::DegToRad(a_Degrees);

			const float cosAngle = Math::Cos(a_Degrees);
			const float sinAngle = Math::Sin(a_Degrees);

			// [ (a11 * b11) + (a12 * b21) + (a13 * b31) ] 
			// [ (a21 * b11) + (a22 * b21) + (a23 * b31) ] 
			// [ (a31 * b11) + (a32 * b21) + (a33 * b31) ] 

			// [ (a11 * b12) + (a12 * b22) + (a13 * b32) ] 
			// [ (a21 * b12) + (a22 * b22) + (a23 * b32) ] 
			// [ (a31 * b12) + (a32 * b22) + (a33 * b32) ] 

			// [ (a11 * b13) + (a12 * b23) + (a13 * b33) ] 
			// [ (a21 * b13) + (a22 * b23) + (a23 * b33) ] 
			// [ (a31 * b13) + (a32 * b23) + (a33 * b33) ] 

			float x, y/*, z*/;

			x   = values[0];
			y   = values[1];
			//z = values[2];

			values[0]   = (x * cosAngle) + (y * -sinAngle)/* + (z * 0.f)*/;
			values[1]   = (x * sinAngle) + (y * cosAngle)/*  + (z * 0.f)*/;
			//values[2] = (x * 0.f)		 + (y * 0.f)	     + (z * 1.f);

			x   = values[3];
			y   = values[4];
			//z = values[5];

			values[3]   = (x * cosAngle) + (y * -sinAngle)/* + (z * 0.f)*/;
			values[4]   = (x * sinAngle) + (y * cosAngle)/*  + (z * 0.f)*/;
			//values[5] = (x * 0.f)		 + (y * 0.f)         + (z * 1.f);

			x   = values[6];
			y   = values[7];
			//z = values[8];

			values[6]   = (x * cosAngle) + (y * -sinAngle)/* + (z * 0.f)*/;
			values[7]   = (x * sinAngle) + (y * cosAngle)/*	 + (z * 0.f)*/;
			//values[8] = (x * 0.f)      + (y * 0.f)		 + (z * 1.f);

			return *this;
		}

		// Sets this Mat3x3 to a rotation by a_Degrees on the Z axis.

		Mat3x3& SetRotationZ(float a_Degrees)
		{
			Clear();
			_rotateAxis(2, a_Degrees, true);
			return *this;
		}

		// Returns a Mat3x3 that is this Mat3x3 rotated by a_Degrees on the Z axis.

		Mat3x3 GetRotatedZ(float a_Degrees)
		{
			Mat3x3 result (
				values[0], values[1], values[2], 
				values[3], values[4], values[5], 
				values[6], values[7], values[8] 
			);
			result.AddRotationZ(a_Degrees);
			return result;
		}

		// Adds a rotation (X, Y, Z) to this Mat3x3.

		Mat3x3& AddRotation(Vec3 a_Rotation)
		{
			AddRotationX(a_Rotation.x);
			AddRotationY(a_Rotation.y);
			AddRotationZ(a_Rotation.z);
			return *this;
		}

		// Sets this Mat3x3 to a rotation (X, Y, Z).

		Mat3x3& SetRotation(Vec3 a_Rotation)
		{
			Identity();
			AddRotationX(a_Rotation.x);
			AddRotationY(a_Rotation.y);
			AddRotationZ(a_Rotation.z);
			return *this;
		}

		Mat3x3& SetRotationToDirection(Vec3 a_Direction)
		{
			// TODO: Implement this
			return *this;
		}

		float values[9];

	private:

		void _rotateAxis(int a_Axis, float a_Degrees, bool a_Clear = false)
		{
			a_Degrees = Math::DegToRad(a_Degrees);

			const float cosAngle = Math::Cos(a_Degrees);
			const float sinAngle = Math::Sin(a_Degrees);

			if (a_Axis == 0)
			{
				// [ 1  0  0  ]
				// [ 0  cx sx ]
				// [ 0 -sx cx ]

				if (a_Clear) { values[0] = 1.f; }

				values[4] += cosAngle;
				values[5] += sinAngle;

				values[7] -= sinAngle;
				values[8] += cosAngle;

				return;
			}

			if (a_Axis == 1)
			{
				// [ cy  0 -sy ]
				// [ 0   1   0 ]
				// [ sy  0  cy ]

				values[0] += cosAngle;
				values[2] -= sinAngle;

				if (a_Clear) { values[4] = 1.f; }

				values[6] += sinAngle;
				values[8] += cosAngle;

				return;
			}

			// [ cz  sz  0 ]
			// [-sz  cz  0 ]
			// [ 0   0   1 ]

			values[0] += cosAngle;
			values[1] += sinAngle;

			values[3] -= sinAngle;
			values[4] += cosAngle;

			if (a_Clear) { values[8] = 1.f; }

			return;
		}


	}; // class Mat3x3

}; // namespace tb

#endif