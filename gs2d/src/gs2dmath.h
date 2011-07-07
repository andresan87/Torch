/*-----------------------------------------------------------------------

 gameSpace2d (C) Copyright 2008-2011 Andre Santee
 http://www.asantee.net/gamespace/
 http://groups.google.com/group/gamespacelib

    This file is part of gameSpace2d.

    gameSpace2d is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    gameSpace2d is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with gameSpace2d. If not, see
    <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------*/

#ifndef GS2D_MATH_H_
#define GS2D_MATH_H_

#include "gs2dtypes.h"
#include "MersenneTwister.h"

namespace gs2d {
namespace math {

namespace constant {
const float PI = 3.141592654f;
const float PIb = 1.570796327f;
} // namespace constant

struct PODVector2 { float x, y; };
struct PODVector3 { float x, y, z; };

struct Vector2 : public PODVector2
{
	Vector2()
	{
		x = y = 0.0f;
	}

	Vector2(const Vector2 &v2)
	{
		x = v2.x;
		y = v2.y;
	}

	Vector2(float fx, float fy)
	{
		x=fx; y=fy;
	}

	inline float Length() const
	{
		return sqrtf(x*x + y*y);
	}

	inline float SquaredLength() const
	{
		return (x*x + y*y);
	}

	inline Vector2 operator * (float num) const
	{
		return Vector2(x*num, y*num);
	}

	inline Vector2 operator * (const Vector2& v2) const
	{
		return Vector2(x*v2.x, y*v2.y);
	}

	inline Vector2 operator / (float num) const
	{
		return Vector2(x/num, y/num);
	}

	inline Vector2 operator / (const Vector2& v2) const
	{
		return Vector2(x/v2.x, y/v2.y);
	}

	inline Vector2 operator - (float num) const
	{
		return Vector2(x-num, y-num);
	}

	inline Vector2 operator - (const Vector2& v2) const
	{
		return Vector2(x-v2.x, y-v2.y);
	}

	inline Vector2 operator + (float num) const
	{
		return Vector2(x+num, y+num);
	}

	inline Vector2 operator + (const Vector2& v2) const
	{
		return Vector2(x+v2.x, y+v2.y);
	}

	inline bool operator == (const Vector2& v2) const
	{
		return (x==v2.x && y==v2.y);
	}

	inline bool operator != (const Vector2& v2) const
	{
		return (x!=v2.x || y!=v2.y);
	}

	inline Vector2& operator += (const Vector2& v2)
	{
		x+=v2.x;
		y+=v2.y;
		return *this;
	}

	inline Vector2& operator -= (const Vector2& v2)
	{
		x-=v2.x;
		y-=v2.y;
		return *this;
	}

	inline Vector2& operator *= (float f)
	{
		x*=f;
		y*=f;
		return *this;
	}

	inline Vector2& operator /= (float f)
	{
		x/=f;
		y/=f;
		return *this;
	}
};

struct Vector3 : public PODVector3
{
	Vector3()
	{
		x = y = z = 0.0f;
	}

	Vector3(const Vector3 &v3)
	{
		x = v3.x;
		y = v3.y;
		z = v3.z;
	}

	Vector3(float fx, float fy, float fz)
	{
		x=fx; y=fy; z=fz;
	}

	Vector3(const Vector2 &v2, float fz)
	{
		x=v2.x; y=v2.y; z=fz;
	}

	inline float Length() const
	{
		return sqrtf(x*x + y*y + z*z);
	}

	inline float SquaredLength() const
	{
		return (x*x + y*y + z*z);
	}

	inline Vector3 operator * (float num) const
	{
		return Vector3(x*num, y*num, z*num);
	}

	inline Vector3 operator * (const Vector3& v3) const
	{
		return Vector3(x*v3.x, y*v3.y, z*v3.z);
	}

	inline Vector3 operator / (float num) const
	{
		return Vector3(x/num, y/num, z/num);
	}

	inline Vector3 operator / (const Vector3& v3) const
	{
		return Vector3(x/v3.x, y/v3.y, z/v3.z);
	}

	inline Vector3 operator - (float num) const
	{
		return Vector3(x-num, y-num, z-num);
	}

	inline Vector3 operator - (const Vector3& v3) const
	{
		return Vector3(x-v3.x, y-v3.y, z-v3.z);
	}

	inline Vector3 operator + (float num) const
	{
		return Vector3(x+num, y+num, z+num);
	}

	inline Vector3 operator + (const Vector3& v3) const
	{
		return Vector3(x+v3.x, y+v3.y, z+v3.z);
	}

	inline bool operator == (const Vector3& v3) const
	{
		return (x==v3.x && y==v3.y && z==v3.z);
	}

	inline bool operator != (const Vector3& v3) const
	{
		return (x!=v3.x || y!=v3.y || z!=v3.z);
	}

	inline Vector3& operator += (const Vector3& v3)
	{
		x+=v3.x;
		y+=v3.y;
		z+=v3.z;
		return *this;
	}

	inline Vector3& operator -= (const Vector3& v3)
	{
		x-=v3.x;
		y-=v3.y;
		z-=v3.z;
		return *this;
	}

	inline Vector3& operator *= (float f)
	{
		x*=f;
		y*=f;
		z*=f;
		return *this;
	}

	inline Vector3& operator /= (float f)
	{
		x/=f;
		y/=f;
		z/=f;
		return *this;
	}
};

struct Vector4
{
	Vector4()
	{
		x = y = z = w = 0.0f;
	}

	Vector4(const Vector4 &v4)
	{
		x = v4.x;
		y = v4.y;
		z = v4.z;
		w = v4.w;
	}

	Vector4(float fx, float fy, float fz, float fw)
	{
		x=fx; y=fy; z=fz; w=fw;
	}

	Vector4(const Vector2& v2, float fz, float fw)
	{
		x=v2.x; y=v2.y; z=fz; w=fw;
	}

	Vector4(const Vector2& v0, const Vector2& v1)
	{
		x=v0.x; y=v0.y; z=v1.x; w=v1.y;
	}

	Vector4(Vector3 v3, float fw)
	{
		x=v3.x; y=v3.y; z=v3.z; w=fw;
	}

	inline Vector4 operator * (float num) const
	{
		return Vector4(x*num, y*num, z*num, w*num);
	}

	inline Vector4 operator * (const Vector4& v4) const
	{
		return Vector4(x*v4.x, y*v4.y, z*v4.z, w*v4.w);
	}

	inline Vector4 operator / (float num) const
	{
		return Vector4(x/num, y/num, z/num, w/num);
	}

	inline Vector4 operator / (const Vector4& v4) const
	{
		return Vector4(x/v4.x, y/v4.y, z/v4.z, w/v4.w);
	}

	inline Vector4 operator - (float num) const
	{
		return Vector4(x-num, y-num, z-num, w-num);
	}

	inline Vector4 operator - (const Vector4& v4) const
	{
		return Vector4(x-v4.x, y-v4.y, z-v4.z, w-v4.w);
	}

	inline Vector4 operator + (float num) const
	{
		return Vector4(x+num, y+num, z+num, w+num);
	}

	inline Vector4 operator + (const Vector4& v4) const
	{
		return Vector4(x+v4.x, y+v4.y, z+v4.z, w+v4.w);
	}

	inline bool operator == (const Vector4& v4) const
	{
		return (x==v4.x && y==v4.y && z==v4.z && w==v4.w);
	}

	inline bool operator != (const Vector4& v4) const
	{
		return (x!=v4.x || y!=v4.y || z!=v4.z || w!=v4.w);
	}

	inline Vector4& operator += (const Vector4& v4)
	{
		x+=v4.x;
		y+=v4.y;
		z+=v4.z;
		w+=v4.w;
		return *this;
	}

	inline Vector4& operator -= (const Vector4& v4)
	{
		x-=v4.x;
		y-=v4.y;
		z-=v4.z;
		w-=v4.w;
		return *this;
	}

	inline Vector4& operator *= (float f)
	{
		x*=f;
		y*=f;
		z*=f;
		w*=f;
		return *this;
	}

	inline Vector4& operator /= (float f)
	{
		x/=f;
		y/=f;
		z/=f;
		w/=f;
		return *this;
	}

	inline void SetColor(const GS_COLOR& color)
	{
		x = (static_cast<float>(color.r)/255.0f);
		y = (static_cast<float>(color.g)/255.0f);
		z = (static_cast<float>(color.b)/255.0f);
		w = (static_cast<float>(color.a)/255.0f);
	}

	float x,y,z,w;
};

struct Vector2i
{
	Vector2i()
	{
		x=y=0;
	}

	Vector2i(const int nx, const int ny)
	{
		x = nx;
		y = ny;
	}

	Vector2i(const Vector2i &v2)
	{
		x = v2.x;
		y = v2.y;
	}

	inline Vector2i operator * (int num) const
	{
		return Vector2i(x*num, y*num);
	}

	inline Vector2i operator * (const Vector2i& v2) const
	{
		return Vector2i(x*v2.x, y*v2.y);
	}

	inline Vector2i operator / (int num) const
	{
		return Vector2i(x/num, y/num);
	}

	inline Vector2i operator / (const Vector2i& v2) const
	{
		return Vector2i(x/v2.x, y/v2.y);
	}

	inline Vector2i operator - (int num) const
	{
		return Vector2i(x-num, y-num);
	}

	inline Vector2i operator - (const Vector2i& v2) const
	{
		return Vector2i(x-v2.x, y-v2.y);
	}

	inline Vector2i operator + (int num) const
	{
		return Vector2i(x+num, y+num);
	}

	inline Vector2i operator + (const Vector2i& v2) const
	{
		return Vector2i(x+v2.x, y+v2.y);
	}

	inline bool operator == (const Vector2i& v2) const
	{
		return (x==v2.x && y==v2.y);
	}

	inline bool operator != (const Vector2i& v2) const
	{
		return (x!=v2.x || y!=v2.y);
	}

	int x,y;
};

struct Rect2D
{
	Rect2D() : pos(0,0), size(0,0)
	{
	}

	Rect2D(const Vector2i &pos, const Vector2i &size)
	{
		this->pos = pos;
		this->size = size;
	}

	Rect2D(const int posX, const int posY, const int sizeX, const int sizeY)
	{
		this->pos.x = posX;
		this->pos.y = posY;
		this->size.x = sizeX;
		this->size.y = sizeY;
	}

	Vector2i pos, size;
};

struct Rect2Df
{
	Rect2Df() : pos(0,0), size(0,0)
	{
	}

	Rect2Df(const Vector2 &pos, const Vector2 &size)
	{
		this->pos = pos;
		this->size = size;
	}

	Rect2Df(const float posX, const float posY, const float sizeX, const float sizeY)
	{
		this->pos.x = posX;
		this->pos.y = posY;
		this->size.x = sizeX;
		this->size.y = sizeY;
	}
	Vector2 pos, size;
};

struct Vertex2D
{
	Vector2 pos;
	float z, rhw;
	GS_COLOR color;
	Vector2 tex;
};

struct Matrix4x4
{
	Matrix4x4(const Matrix4x4 &m)
	{
		for (unsigned int t=0; t<16; t++)
		{
			e[t] = m.e[t];
		}
	}

	Matrix4x4()
	{
		Identity();
	}

	inline void Identity()
	{
		m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
		m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
		m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	}

	Matrix4x4(const float _11, const float _12, const float _13, const float _14,
			const float _21, const float _22, const float _23, const float _24,
			const float _31, const float _32, const float _33, const float _34,
			const float _41, const float _42, const float _43, const float _44)
	{
		mat.a11=_11; mat.a12=_12; mat.a13=_13; mat.a14=_14;
		mat.a21=_21; mat.a22=_22; mat.a23=_23; mat.a24=_24;
		mat.a31=_31; mat.a32=_32; mat.a33=_33; mat.a34=_34;
		mat.a41=_41; mat.a42=_42; mat.a43=_43; mat.a44=_44;
	}

	float GetDeterminant()
	{
		return 
		mat.a14 * mat.a23 * mat.a32 * mat.a41-mat.a13 * mat.a24 * mat.a32 * mat.a41-mat.a14 * mat.a22 * mat.a33 * mat.a41+mat.a12 * mat.a24 * mat.a33 * mat.a41+
		mat.a13 * mat.a22 * mat.a34 * mat.a41-mat.a12 * mat.a23 * mat.a34 * mat.a41-mat.a14 * mat.a23 * mat.a31 * mat.a42+mat.a13 * mat.a24 * mat.a31 * mat.a42+
		mat.a14 * mat.a21 * mat.a33 * mat.a42-mat.a11 * mat.a24 * mat.a33 * mat.a42-mat.a13 * mat.a21 * mat.a34 * mat.a42+mat.a11 * mat.a23 * mat.a34 * mat.a42+
		mat.a14 * mat.a22 * mat.a31 * mat.a43-mat.a12 * mat.a24 * mat.a31 * mat.a43-mat.a14 * mat.a21 * mat.a32 * mat.a43+mat.a11 * mat.a24 * mat.a32 * mat.a43+
		mat.a12 * mat.a21 * mat.a34 * mat.a43-mat.a11 * mat.a22 * mat.a34 * mat.a43-mat.a13 * mat.a22 * mat.a31 * mat.a44+mat.a12 * mat.a23 * mat.a31 * mat.a44+
		mat.a13 * mat.a21 * mat.a32 * mat.a44-mat.a11 * mat.a23 * mat.a32 * mat.a44-mat.a12 * mat.a21 * mat.a33 * mat.a44+mat.a11 * mat.a22 * mat.a33 * mat.a44;
	}

	struct Matrix
	{
		float a11, a12, a13, a14;
		float a21, a22, a23, a24;
		float a31, a32, a33, a34;
		float a41, a42, a43, a44;
	};

	union
	{
		Matrix mat;
		float m[4][4];
		float e[16];
	};
};

inline float GetAngle(const Vector2 &v2)
{
	const float r = atan2f(v2.x, v2.y);
	return (r<0) ? r+(2*constant::PI) : r;
}

inline float RadianToDegree(const float angle)
{
	return (angle/(constant::PI*2))*360.0f;
}

inline float DegreeToRadian(const float angle)
{
	return angle*(constant::PI/180.0f);
}

inline float DP2(const Vector2 &a, const Vector2 &b)
{
	return ( a.x*b.x + a.y*b.y );
}

inline float DP3(const Vector3 &a, const Vector3 &b)
{
	return ( a.x*b.x + a.y*b.y + a.z*b.z );
}

inline float DP4(const Vector4 &a, const Vector4 &b)
{
	return ( a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w);
}

inline Vector3 cross(const Vector3 &v0, const Vector3 &v1)
{
	return Vector3((v0.y*v1.z) - (v0.z*v1.y), (v0.z*v1.x) - (v0.x*v1.z), (v0.x*v1.y) - (v0.y*v1.x));
}

inline Vector3 Normalize(const Vector3 &v0)
{
	const float a = sqrtf(DP3(v0, v0));
	if (a <= 0.0f)
		return Vector3(0,0,0);
	return Vector3(v0.x/a, v0.y/a, v0.z/a); 
}

inline Vector2 Normalize(const Vector2 &v0)
{
	const float a = sqrtf(DP2(v0, v0));
	if (a <= 0.0f)
		return Vector2(0,0);
	return Vector2(v0.x/a, v0.y/a); 
}

inline float SquaredDistance(const Vector3 &P0, const Vector3 &P1)
{
	const Vector3 diff = P1 - P0;
	return DP3(diff, diff);
}

inline float SquaredDistance(const Vector2 &P0, const Vector2 &P1)
{
	const Vector2 diff = P1 - P0;
	return DP2(diff, diff);
}

inline float Distance(const Vector3 &P0, const Vector3 &P1)
{
	const Vector3 diff = P1 - P0;
	return sqrtf(DP3(diff, diff));
}

inline float SquaredDist(const Vector2 &P0, const Vector2 &P1)
{
	const Vector2 diff = P1 - P0;
	return DP2(diff, diff);
}

inline float Distance(const Vector2 &P0, const Vector2 &P1)
{
	const Vector2 diff = P1 - P0;
	return sqrtf(DP2(diff, diff));
}

inline Matrix4x4 Scale(const float x, const float y, const float z)
{
	return Matrix4x4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
}

inline Matrix4x4 Translate(const float x, const float y, const float z)
{
	return Matrix4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	);
}

inline Matrix4x4 RotateX(const float theta)
{
	const float sin=sinf(theta), cos=cosf(theta);
	return Matrix4x4(
		1, 0,   0,   0,
		0, cos, sin, 0,
		0,-sin, cos, 0,
		0, 0,   0,   1
	);
}

inline Matrix4x4 RotateY(const float theta)
{
	const float sin=sinf(theta), cos=cosf(theta);
	return Matrix4x4(
		cos, 0,-sin, 0,
		0,   1,   0, 0,
		sin, 0, cos, 0,
		0,   0,   0, 1
	);
}

inline Matrix4x4 RotateZ(const float theta)
{
	const float sin=sinf(theta), cos=cosf(theta);
	return Matrix4x4(
		cos, sin, 0, 0,
		-sin,cos, 0, 0,
		0,   0,   1, 0,
		0,   0,   0, 1
	);
}

inline Matrix4x4 Multiply(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
	Matrix4x4 r;
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			r.m[i][j] = 0;
			for (int k = 0; k < 4; k++)
			{
				r.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return r;
}

inline Vector3 Multiply(const Vector3 &v3, const Matrix4x4 &mat)
{
	return Vector3(
		v3.x * mat.mat.a11 +
		v3.y * mat.mat.a12 +
		v3.z * mat.mat.a13,

		v3.x * mat.mat.a21 +
		v3.y * mat.mat.a22 +
		v3.z * mat.mat.a23,

		v3.x * mat.mat.a31 +
		v3.y * mat.mat.a32 +
		v3.z * mat.mat.a33);
}

inline Vector4 Multiply(const Vector4 &v4, const Matrix4x4 &mat)
{
	return Vector4(
		v4.x * mat.mat.a11 +
		v4.y * mat.mat.a12 +
		v4.z * mat.mat.a13 +
		v4.w * mat.mat.a14,

		v4.x * mat.mat.a21 +
		v4.y * mat.mat.a22 +
		v4.z * mat.mat.a23 +
		v4.w * mat.mat.a24,

		v4.x * mat.mat.a31 +
		v4.y * mat.mat.a32 +
		v4.z * mat.mat.a33 +
		v4.w * mat.mat.a34,

		v4.x * mat.mat.a41 +
		v4.y * mat.mat.a42 +
		v4.z * mat.mat.a43 +
		v4.w * mat.mat.a44);
}

inline Vector2 Multiply(const Vector2 &v2, const Matrix4x4 &mat)
{
	return Vector2(
		v2.x * mat.mat.a11 +
		v2.y * mat.mat.a12,

		v2.x * mat.mat.a21 +
		v2.y * mat.mat.a22);
}

inline void Orthogonal(Matrix4x4 &out, const float w, const float h, const float zn, const float zf)
{
	out.Identity();
	out.mat.a11 = 2.0f/w;
	out.mat.a22 = 2.0f/h;
	out.mat.a33 = 1.0f/(zf-zn);
	out.mat.a43 = zn/(zn-zf);
}

inline Matrix4x4 Rotate(const float x, const float y, const float z)
{
	Matrix4x4 r, temp;
	r = RotateX(x);
	temp = RotateY(y);
	r = Multiply(r, temp);
	temp = RotateZ(z);
	return Multiply(r, temp);
}

template <class T>
inline T Max(const T &a, const T &b)
{
	return (a>b) ? a : b;
}

template <class T>
inline T Min(const T &a, const T &b)
{
	return (a<b) ? a : b;
}

template <class T>
inline T Abs(const T &n)
{
	return (n<0) ? -n : n;
}

template <class T>
inline T Clamp(const T &n, const T &min, const T &max)
{
	return (n < min) ? min : (n > max) ? max : n;
}

template <class T>
inline T Square(const T &a)
{
	return a*a;
}

template <class T>
inline T Sign(const T &n)
{
	return (n < static_cast<T>(0)) ? static_cast<T>(-1) : (n > static_cast<T>(0)) ? static_cast<T>(1) : static_cast<T>(0);
}

inline GS_COLOR ConvertToDW(Vector4 v4)
{
	v4.w = Min(1.0f, v4.w);
	if (v4.x > 1.0f || v4.y > 1.0f || v4.z > 1.0f)
	{
		Vector3 v3(v4.x, v4.y, v4.z);
		v3 = Normalize(v3);
		v4.x = v3.x;
		v4.y = v3.y;
		v4.z = v3.z;
	}
	return GS_COLOR((GS_BYTE)(v4.w*255.0f), (GS_BYTE)(v4.x*255.0f), (GS_BYTE)(v4.y*255.0f), (GS_BYTE)(v4.z*255.0f));
}

inline GS_COLOR ConvertToDW(Vector3 v3)
{
	if (v3.x > 1.0f || v3.y > 1.0f || v3.z > 1.0f)
	{
		v3 = Normalize(v3);
	}
	return GS_COLOR(255, (GS_BYTE)(v3.x*255.0f), (GS_BYTE)(v3.y*255.0f), (GS_BYTE)(v3.z*255.0f));
}

inline unsigned long ARGB(const GS_BYTE a, const GS_BYTE r, const GS_BYTE g, const GS_BYTE b)
{
	return ((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff));
}

class Randomizer
{
	static MTRand m_mt;
public:

	static inline int Int(const int maxValue)
	{
		return static_cast<int>(m_mt.randInt(static_cast<MTRand::uint32>(maxValue)));
	}

	static inline float Float(const float maxValue)
	{
		return static_cast<float>(m_mt.rand(static_cast<double>(maxValue)));
	}

	static inline int Int(const int minValue, const int maxValue)
	{
		const int min = Min(minValue, maxValue);
		const int max = Max(minValue, maxValue);
		const int diff = max-min;
		return Int(diff)+min;
	}

	static inline float Float(const float minValue, const float maxValue)
	{
		const float min = Min(minValue, maxValue);
		const float max = Max(minValue, maxValue);
		const float diff = max-min;
		return Float(diff)+min;
	}

	static void Seed(const unsigned int seed)
	{
		m_mt.seed(static_cast<MTRand::uint32>(seed));
	}
};

typedef Vector4 Plane;
inline Plane ComputePlane(const Vector3& pos, const Vector3& normal)
{
	return (Plane(Normalize(normal), -((normal.x * pos.x) + (normal.y * pos.y) + (normal.z * pos.z))));
}

inline Plane ComputePlane(const Vector2& pos, const Vector2& normal)
{
	return (Plane(Normalize(normal), 0, -((normal.x * pos.x) + (normal.y * pos.y))));
}

inline float PlaneDotVector(const Plane& plane, const Vector3& vec)
{
	return (plane.x * vec.x) + (plane.y * vec.y) + (plane.z * vec.z) + (plane.w);
}

inline float PlaneDotVector(const Plane& plane, const Vector2& vec)
{
	return (plane.x * vec.x) + (plane.y * vec.y) + (plane.w);
}

namespace constant {
const Vector2 HALF_VECTOR2(0.5f, 0.5f);
const Vector2 ZERO_VECTOR2(0.5f, 0.5f);
const Vector3 HALF_VECTOR3(0.5f, 0.5f, 0.5f);
const Vector3 ZERO_VECTOR3(0.5f, 0.5f, 0.5f);
} // namespace constant

} // namespace math
} // namespace gs2d

#endif