#ifndef _TBMAT4X4_H_
#define _TBMAT4X4_H_

#include "TBSettings.h"
#include "TBMath.h"
#include "TBVec3.h"
#include "TBMat3x3.h"

namespace tb
{

	class Mat4x4
	{

	public:

		enum Field
		{
			X1 = 0,  Y1 = 1,  Z1 = 2,  TX = 3,
			X2 = 4,  Y2 = 5,  Z2 = 6,  TY = 7,
			X3 = 8,  Y3 = 9,  Z3 = 10, TZ = 11,
			WX = 12, WY = 13, WZ = 14, CZ = 15,

			SX = 0, 
			         SY = 5, 
			                  SZ = 10
		};

		Mat4x4()
		{
			Identity();
		}

		Mat4x4 (
			float a_X1, float a_Y1, float a_Z1, float a_ScaleX,
			float a_X2, float a_Y2, float a_Z2, float a_ScaleY,
			float a_X3, float a_Y3, float a_Z3, float a_ScaleZ,
			float a_TranslateX, float a_TranslateY, float a_TranslateZ, float a_Corner
		)
		{
			values[X1] = a_X1;
			values[Y1] = a_Y1;
			values[Z1] = a_Z1;
			values[TX] = a_ScaleX;

			values[X2] = a_X2;
			values[Y2] = a_Y2;
			values[Z3] = a_Z2;
			values[TY] = a_ScaleY;

			values[X3] = a_X3;
			values[Y3] = a_Y3;
			values[Z3] = a_Z3;
			values[TZ] = a_ScaleZ;

			values[WX] = a_TranslateX;
			values[WY] = a_TranslateY;
			values[WZ] = a_TranslateZ;
			values[CZ] = a_Corner;
		}

		////////////////////////////////////////////
		//                                        //
		//                  HOOKS                 //
		//                                        //
		////////////////////////////////////////////

		float GetValue(int a_Column, int a_Row)
		{
			if (
				a_Column > 0 && a_Column < 4 &&
				a_Row > 0 && a_Column < 4
				)
			{
				return (values[(a_Column - 1) + ((a_Row - 1) * 3)]);
			}

			return NULL;
		}

		float& operator [] (int a_Index)
		{
			return (values[a_Index]);
		}

		void ToOpenGLMatrix()
		{
			//  [ 0  1  2  3  ]      [ 0  4  8  12 ]
			//  [ 4  5  6  7  ]  __  [ 1  5  9  13 ]
			//  [ 8  9  10 11 ]  __  [ 2  6  10 14 ]
			//  [ 12 13 14 15 ]      [ 3  7  11 15 ]

			//values[X2] = values[Y1];
		}

		float* GetBuffer()
		{
			return values;
		}

		Mat3x3 GetRotation()
		{
			Mat3x3 result (
				values[X1], values[Y1], values[Z1],
				values[X2], values[Y2], values[Z2],
				values[X3], values[Y3], values[Z3]
			);

			return result;
		}

		Vec3 GetTranslation()
		{
			Vec3 result(values[TX], values[TY], values[TZ]);
			return result;
		}

		Vec3 GetScale()
		{
			Vec3 scale;
			scale.x = Vec3(values[X1], values[Y1], values[Z1]).GetLength();
			scale.y = Vec3(values[X2], values[Y2], values[Z2]).GetLength();
			scale.z = Vec3(values[X3], values[Y3], values[Z3]).GetLength();

			return scale;
		}

		////////////////////////////////////////////
		//                                        //
		//                FUNCTIONS               //
		//                                        //
		////////////////////////////////////////////

		Vec3 operator * (Vec3 a_Value)
		{
			return (Transform(a_Value));
		}

		void Clear()
		{
			memset(values, 0, sizeof(values));
		}

		Mat4x4& operator = (Mat4x4& a_Other)
		{
			memcpy(values, a_Other.values, 16 * sizeof(float));

			return *this;
		}

		Mat4x4& Set(Mat4x4& a_Other)
		{
			memcpy(values, a_Other.values, 16 * sizeof(float));

			return *this;
		}

		Mat4x4& Identity()
		{
			/*     */    values[Y1] = values[Z1] = values[TX] =
			values[X2] = /*	     */   values[Z2] = values[TY] =
			values[X3] = values[Y3]   /*      */ = values[TZ] =
			values[12] = values[13] = values[14]   /*      */ = 0.f;

			values[SX] = /*      */   /*      */   /*      */
			/*      */   values[SY] = /*      */   /*      */
			/*      */   /*      */   values[SZ] = /*      */
			/*      */   /*      */   /*      */   values[15] = 1.f;

			return *this;
		}

		Mat4x4& Concatenate(Mat4x4& a_Other)
		{
			Mat4x4 result;

			for (int cell = 0; cell < 4; cell++) 
			{
				for (int row = 0; row < 4; row++)
				{
					result.values[row * 4 + cell] = 
						values[row * 4 + 0] * a_Other.values[cell +  0] +
						values[row * 4 + 1] * a_Other.values[cell +  4] +
						values[row * 4 + 2] * a_Other.values[cell +  8] +
						values[row * 4 + 3] * a_Other.values[cell + 12];
				}	
			}

			memcpy(values, result.values, 16 * sizeof(float));

			return *this;
		}

		Mat4x4& AddRotation(Mat3x3 a_Rotation)
		{
			values[X1] *= a_Rotation.GetValue(1, 1);
			values[Y1] *= a_Rotation.GetValue(2, 1);
			values[Z1] *= a_Rotation.GetValue(3, 1);

			values[X2] *= a_Rotation.GetValue(1, 2);
			values[Y2] *= a_Rotation.GetValue(2, 2);
			values[Z2] *= a_Rotation.GetValue(3, 2);

			values[X3] *= a_Rotation.GetValue(1, 3);
			values[Y3] *= a_Rotation.GetValue(2, 3);
			values[Z3] *= a_Rotation.GetValue(3, 3);

			return *this;
		}

		Mat4x4& SetRotation(Mat3x3 a_Rotation)
		{
			values[X1] = a_Rotation.GetValue(1, 1);
			values[Y1] = a_Rotation.GetValue(2, 1);
			values[Z1] = a_Rotation.GetValue(3, 1);

			values[X2] = a_Rotation.GetValue(1, 2);
			values[Y2] = a_Rotation.GetValue(2, 2);
			values[Z2] = a_Rotation.GetValue(3, 2);

			values[X3] = a_Rotation.GetValue(1, 3);
			values[Y3] = a_Rotation.GetValue(2, 3);
			values[Z3] = a_Rotation.GetValue(3, 3);

			return *this;
		}

		Mat4x4& SetScale(Vec3 a_Scale)
		{
			values[SX] *= a_Scale.x;
			values[SY] *= a_Scale.y;
			values[SZ] *= a_Scale.z;

			return *this;
		}	

		Mat4x4& Scale(Vec3 a_Scale)
		{
			values[X1] *= a_Scale.x;
			values[Y1] *= a_Scale.y;
			values[Z1] *= a_Scale.z;

			values[X2] *= a_Scale.x;
			values[Y2] *= a_Scale.y;
			values[Z2] *= a_Scale.z;

			values[X3] *= a_Scale.x;
			values[Y3] *= a_Scale.y;
			values[Z3] *= a_Scale.z;

			values[TX] *= a_Scale.x;
			values[TY] *= a_Scale.y;
			values[TZ] *= a_Scale.z;


			return *this;
		}	

		Mat4x4& SetTranslation(Vec3 a_Translation)
		{
			values[TX] = a_Translation.x;
			values[TY] = a_Translation.y;
			values[TZ] = a_Translation.z;

			return *this;
		}

		Mat4x4& Translate(Vec3 a_Translation)
		{
			values[TX] += a_Translation.x;
			values[TY] += a_Translation.y;
			values[TZ] += a_Translation.z;

			return *this;
		}

		// Adds a rotation on the X axis to this Mat3x3.

		Mat4x4& AddRotationX(float a_Degrees)
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

			//x = values[X1];
			y = values[Y1];
			z = values[Z1];

			//values[X1] =   (x * 1.f) +   (y * 0.f)        + (z * 0.f);
			values[Y1]   = /*(x * 0.f) + */(y * cosAngle)	+ (z * -sinAngle);
			values[Z1]   = /*(x * 0.f) + */(y * sinAngle)	+ (z * cosAngle);

			//x = values[X2];
			y = values[Y2];
			z = values[Z2];

			//values[X2] =   (x * 1.f) +   (y * 0.f)      + (z * 0.f);
			values[Y2]   = /*(y * 0.f) + */(y * cosAngle) + (z * -sinAngle);
			values[Z2]   = /*(z * 0.f) + */(y * sinAngle) + (z * cosAngle);

			//x = values[X3];
			y = values[Y3];
			z = values[Z3];


			//values[X3] =   (x * 1.f) +   (y * 0.f)      + (z * 0.f);
			values[Y3]   = /*(x * 0.f) + */(y * cosAngle) + (z * -sinAngle);
			values[Z3]   = /*(x * 0.f) + */(y * sinAngle) + (z * cosAngle);

			return *this;
		}

		// Sets this Mat3x3 to a rotation by a_Degrees on the X axis.

		Mat4x4& SetRotationX(float a_Degrees)
		{
			Clear();
			_RotateAxis(0, a_Degrees, true);

			return *this;
		}

		Mat4x4& SetRotationY(float a_Degrees)
		{
			Clear();
			_RotateAxis(1, a_Degrees, true);

			return *this;
		}

		Mat4x4& SetRotationZ(float a_Degrees)
		{
			Clear();
			_RotateAxis(2, a_Degrees, true);

			return *this;
		}

		Mat4x4& Invert()
		{
			//        / [ a11 a12 a13 a14 ] \      [ a11 a21 a31 a41 ]
			//        | [ a21 a22 a23 a24 ] |  ==  [ a12 a22 a32 a42 ]
			// Invert | [ a31 a32 a33 a34 ] |  ==  [ a13 a23 a33 a43 ]
			//        \ [ a41 a42 a43 a44 ] /      [ a14 a24 a34 a44 ]

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

		Vec3 Transform(Vec3 a_Vec)
		{
			Vec3 result;

			result.x = (a_Vec.x * values[X1]) + (a_Vec.y * values[Y1]) + (a_Vec.z * values[Z1]) + values[TX];
			result.y = (a_Vec.x * values[X2]) + (a_Vec.y * values[Y2]) + (a_Vec.z * values[Z2]) + values[TY];
			result.z = (a_Vec.x * values[X3]) + (a_Vec.y * values[Y3]) + (a_Vec.z * values[Z3]) + values[TZ];

			return result;

			/*Vec3 result;

			result.x = (a_Vec.x * values[X1]) + (a_Vec.y * values[Y1]) + (a_Vec.z * values[Z1]) + (1.f * values[WX]) + values[TX];
			result.y = (a_Vec.x * values[X2]) + (a_Vec.y * values[Y2]) + (a_Vec.z * values[Z2]) + (1.f * values[WY]) + values[TY];
			result.z = (a_Vec.x * values[X3]) + (a_Vec.y * values[Y3]) + (a_Vec.z * values[Z3]) + (1.f * values[WZ]) + values[TZ];

			return result;*/
		}

	private:

		float values[16];

		/*union
		{
			struct { float values[16]; };
			struct 
			{
				float s11, s21, s31, tx;
				float s12, s22, s32, ty;
				float s13, s23, s33, tz;
				float s14, s24, s34, s44;
			};
		};*/

		void _RotateAxis(int a_Axis, float a_Degrees, bool a_Clear = false)
		{
			a_Degrees = Math::DegToRad(a_Degrees);

			const float cosAngle = Math::Cos(a_Degrees);
			const float sinAngle = Math::Sin(a_Degrees);

			if (a_Axis == 0)
			{
				// [ 1  0  0  ]
				// [ 0  cx sx ]
				// [ 0 -sx cx ]

				if (a_Clear) { values[SX] = 1.f; }

				values[Y2] += cosAngle;
				values[Z2] += sinAngle;

				values[Y3] -= sinAngle;
				values[Z3] += cosAngle;

				return;
			}

			if (a_Axis == 1)
			{
				// [ cy  0 -sy ]
				// [ 0   1   0 ]
				// [ sy  0  cy ]

				values[X1] += cosAngle;
				values[Z1] -= sinAngle;

				if (a_Clear) { values[SY] = 1.f; }

				values[X3] += sinAngle;
				values[Z3] += cosAngle;

				return;
			}

			// [ cz  sz  0 ]
			// [-sz  cz  0 ]
			// [ 0   0   1 ]

			values[X1] += cosAngle;
			values[Y1] += sinAngle;

			values[X2] -= sinAngle;
			values[Y2] += cosAngle;

			if (a_Clear) { values[SZ] = 1.f; }

			return;
		}

	}; // class Mat4x4

}; // namespace tb

#endif