#ifndef _TBQUATERNION_H_
#define _TBQUATERNION_H_

#include "TBSettings.h"
#include "TBMath.h"
#include "TBVec3.h"

namespace tb
{

	class Quaternion
	{

	public:

		Quaternion() 
		{
			Identity();
		}

		Quaternion(float a_X, float a_Y, float a_Z, float a_W) 
		{
			x = a_X;
			y = a_Y;
			z = a_Z;
			w = a_W;
		}

		Quaternion(Vec3& a_Vector, float a_W) 
		{
			x = a_Vector.x;
			y = a_Vector.y;
			z = a_Vector.z;
			w = a_W;
		}

		void Identity()
		{
			x = y = z = 0;
			w = 1;
		}

		float GetSquaredLength()
		{
			return (x * x + y * y + z * z + w * w);
		}

		float GetLength()
		{
			return (Math::Sqrt(GetSquaredLength()));
		}

		Quaternion GetNormalized()
		{
			float length = 1 / GetLength();

			return (Quaternion(x * length, y * length, z * length, w * length));
		}

		Mat3x3 GetRotationMatrix()
		{
			/*double sqw = q.w*q.w;
			double sqx = q.x*q.x;
			double sqy = q.y*q.y;
			double sqz = q.z*q.z;

			// invs (inverse square length) is only required if quaternion is not already normalised
			double invs = 1 / (sqx + sqy + sqz + sqw)
				m00 = ( sqx - sqy - sqz + sqw)*invs ; // since sqw + sqx + sqy + sqz =1/invs*invs
			m11 = (-sqx + sqy - sqz + sqw)*invs ;
			m22 = (-sqx - sqy + sqz + sqw)*invs ;

			double tmp1 = q.x*q.y;
			double tmp2 = q.z*q.w;
			m10 = 2.0 * (tmp1 + tmp2)*invs ;
			m01 = 2.0 * (tmp1 - tmp2)*invs ;

			tmp1 = q.x*q.z;
			tmp2 = q.y*q.w;
			m20 = 2.0 * (tmp1 - tmp2)*invs ;
			m02 = 2.0 * (tmp1 + tmp2)*invs ;
			tmp1 = q.y*q.z;
			tmp2 = q.x*q.w;
			m21 = 2.0 * (tmp1 + tmp2)*invs ;
			m12 = 2.0 * (tmp1 - tmp2)*invs ; */

			// yy zz xy zw xz yw
			// xy zw xx zz yz xw
			// xz yw yz xw xx yy

			// xx: 2
			// xy: 2
			// xz: 2
			// xw: 2

			// 
			// yy: 2
			// yz: 2
			// yw: 2 
		
			//
			//
			// zz: 2
			// zw: 2

			/*		const Mat3x3<T> toRotationMatrix() const {
			return Mat3x3<T>(
			1 - 2*_v[Y]*_v[Y] - 2*_v[Z]*_v[Z]		,	2*_v[X]*_v[Y] - 2*_v[Z]*_v[W]			,	2*_v[X]*_v[Z] + 2*_v[Y]*_v[W]		,
			2*_v[X]*_v[Y] + 2*_v[Z]*_v[W]			,	1 - 2*_v[X]*_v[X] - 2*_v[Z]*_v[Z]		, 	2*_v[Y]*_v[Z] - 2*_v[X]*_v[W]		,
			2*_v[X]*_v[Z] - 2*_v[Y]*_v[W]			, 	2*_v[Y]*_v[Z] + 2*_v[X]*_v[W]			, 	1 - 2*_v[X]*_v[X] - 2*_v[Y]*_v[Y]
			);
			}*/
				
			const float xx = 2 * x * x;
			const float xy = 2 * x * y;
			const float xz = 2 * x * z;
			const float xw = 2 * x * w;

			const float yy = 2 * y * y;
			const float yz = 2 * y * z;
			const float yw = 2 * y * w;

			const float zz = 2 * y * z;
			const float zw = 2 * y * w;

			/*1 - 2*y*y - 2*z*z		,	2*x*y - 2*z*w			,	2*x*z + 2*y*w		,
				2*x*y + 2*z*w			,	1 - 2*x*x - 2*z*z		, 	2*y*z - 2*x*w		,
				2*x*z - 2*y*w			, 	2*y*z + 2*x*w			, 	1 - 2*x*x - 2*y*y
			);*/

			/*return Mat3x3
			(
				1 - yy - zz,	xy - zw,		xz + yw,
				xy + zw,		1 - xx - zz,	yz - xw,
				xz - yw, 		yz + xw,		1 - xx - yy
			);*/

			return Mat3x3
			(
				1 - yy - zz,	xy + zw,		xz - yw,
				xy - zw,		1 - xx - zz,	yz + xw,
				xz + yw,		yz - xw,		1 - xx - yy
			);

			// 0  3  6
			// 1  4  7
			// 2  5  8

			// 0  1  2
			// 3  4  5
			// 6  7  8
		}

		Quaternion& Normalize()
		{
			float length = 1 / GetLength();

			x *= length;
			y *= length;
			z *= length;
			w *= length;

			return *this;
		}

		Quaternion& Conjugate()
		{
			x = -x;
			y = -y;
			z = -z;

			return *this;
		}

		Quaternion GetConjugate()
		{
			return (Quaternion(-x, -y, -z, w));
		}

		Quaternion& FromAngleAxis(Vec3& a_Axis, float a_Angle)
		{
			float halfangle = 0.5f * a_Angle;

			float sintheta = Math::Sin(halfangle);
			x = sintheta * a_Axis.x;
			y = sintheta * a_Axis.y;
			z = sintheta * a_Axis.z;
			w = Math::Cos(halfangle);

			return *this;
		}

		Quaternion& operator = (Quaternion a_Other)
		{
			x = a_Other.x;
			y = a_Other.y;
			z = a_Other.z;
			w = a_Other.w;

			return *this;
		}

		Vec3 operator * (Vec3& a_Other)
		{
			// i'm not touching this magic
			// i might break something

			// Ogre 1.6.2 source

			// nVidia SDK implementation
			Vec3 uv, uuv;
			Vec3 qvec(x, y, z);
			uv = qvec.GetCrossProduct(a_Other);
			uuv = qvec.GetCrossProduct(uv);
			uv *= (2.0f * w);
			uuv *= 2.0f;

			return a_Other + uv + uuv;
		}

		Quaternion operator * (Quaternion a_Other)
		{
			Quaternion result (
				(w * a_Other.x) + (x * a_Other.w) + (y * a_Other.z) - (z * a_Other.y),
				(w * a_Other.y) - (x * a_Other.z) + (y * a_Other.w) + (z * a_Other.x),
				(w * a_Other.z) + (x * a_Other.y) - (y * a_Other.x) + (z * a_Other.w),
				(w * a_Other.w) - (x * a_Other.x) - (y * a_Other.y) - (z * a_Other.z)
			);

			return result;
		}

		Quaternion& operator *= (Quaternion a_Other)
		{
			Quaternion old(x, y, z, w);

			x = (old.w * a_Other.x) + (old.x * a_Other.w) + (old.y * a_Other.z) - (old.z * a_Other.y);
			y = (old.w * a_Other.y) - (old.x * a_Other.z) + (old.y * a_Other.w) + (old.z * a_Other.x);
			z = (old.w * a_Other.z) + (old.x * a_Other.y) - (old.y * a_Other.x) + (old.z * a_Other.w);
			w = (old.w * a_Other.w) - (old.x * a_Other.x) - (old.y * a_Other.y) - (old.z * a_Other.z);

			return *this;
		}

		float x, y, z, w;

	}; // class Quaternion

}; // namespace tb

#endif