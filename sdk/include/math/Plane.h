/**
	\brief 平面定义
	\file line.h
	\author whyjld
*/
//------------------------------------------------------------------------------
#include "MyMath.h"
#include "Line.h"
//------------------------------------------------------------------------------
#ifndef _AE_PLANE_h_
#define _AE_PLANE_h_ "Plane.h"
//------------------------------------------------------------------------------
_AE_BEGIN_
//------------------------------------------------------------------------------
/**
	\brief 平面定义
	方程定义为
	Plane: dot(N, P) - D = 0;
	P为平面上一点
*/
struct Plane
{
	/**
		\brief 平面法线
	*/
	aeVec3f N;
	/**
		\brief 距离
	*/
	aeFLOAT D;
};
//------------------------------------------------------------------------------
/**
	\brief 构造一个平面
	\param A 平面上一点
	\param B 平面上一点
	\param C 平面上一点
	\return 平面
*/
inline Plane MakePlane(const aeVec4f& A, const aeVec4f& B, const aeVec4f& C)
{
	Plane ret;
	ret.N = Cross(MakeVec3((B - A).D), MakeVec3((C - A).D));
	if(!ret.N.Normalize())
	{
		ret.N.X = std::numeric_limits<aeFLOAT>::quiet_NaN();
		ret.N.Y = std::numeric_limits<aeFLOAT>::quiet_NaN();
		ret.N.Z = std::numeric_limits<aeFLOAT>::quiet_NaN();
		ret.D   = std::numeric_limits<aeFLOAT>::quiet_NaN();

		return ret;
	}
	ret.D = Dot(A, ret.N);
	return ret;
}
//------------------------------------------------------------------------------
/**
	\brief 构造一个平面
	\param A 平面上一点
	\param B 平面上一点
	\param C 平面上一点
	\return 平面
*/
inline Plane MakePlane(const aeVec3f& A, const aeVec3f& B, const aeVec3f& C)
{
	Plane ret;
	ret.N = Cross(B - A, C - A);
	if(!ret.N.Normalize())
	{
		ret.N.X = std::numeric_limits<aeFLOAT>::quiet_NaN();
		ret.N.Y = std::numeric_limits<aeFLOAT>::quiet_NaN();
		ret.N.Z = std::numeric_limits<aeFLOAT>::quiet_NaN();
		ret.D   = std::numeric_limits<aeFLOAT>::quiet_NaN();

		return ret;
	}
	ret.D = Dot(A, ret.N);
	return ret;
}
//------------------------------------------------------------------------------
/**
	\brief 构造一个平面
	\param N 平面的法线
	\param A 平面上一点
	\return 平面
*/
inline Plane MakePlane(const aeVec3f& N, const aeVec3f& A)
{
	Plane ret;
	ret.N = N;
	if(!ret.N.Normalize())
	{
		ret.N.X = std::numeric_limits<aeFLOAT>::quiet_NaN();
		ret.N.Y = std::numeric_limits<aeFLOAT>::quiet_NaN();
		ret.N.Z = std::numeric_limits<aeFLOAT>::quiet_NaN();
		ret.D   = std::numeric_limits<aeFLOAT>::quiet_NaN();

		return ret;
	}
	ret.D = Dot(A, ret.N);
	return ret;
}
//------------------------------------------------------------------------------
/**
	\brief 构造一个平面
	\param N 平面的法线
	\param A 平面上一点
	\return 平面
*/
inline Plane MakePlane(const aeVec3f& N, const aeVec4f& A)
{
	Plane ret;
	ret.N = N;
	if(!ret.N.Normalize())
	{
		ret.N.X = std::numeric_limits<aeFLOAT>::quiet_NaN();
		ret.N.Y = std::numeric_limits<aeFLOAT>::quiet_NaN();
		ret.N.Z = std::numeric_limits<aeFLOAT>::quiet_NaN();
		ret.D   = std::numeric_limits<aeFLOAT>::quiet_NaN();

		return ret;
	}
	ret.D = aeVecOperate<aeFLOAT, 3>::Dot(A.D, ret.N.D);
	return ret;
}
//------------------------------------------------------------------------------
/**
	\brief 求线和直线的交点
*/
inline aeVec4f operator&(const Plane& p, const Segment& s)
{
	aeFLOAT a = Dot(p.N, s.A) - p.D;
	aeFLOAT b = Dot(p.N, s.B) - p.D;

	if (a * b > 0)
	{
		return aeVec4f({
			std::numeric_limits<aeFLOAT>::quiet_NaN(),
			std::numeric_limits<aeFLOAT>::quiet_NaN(),
			std::numeric_limits<aeFLOAT>::quiet_NaN(),
			std::numeric_limits<aeFLOAT>::quiet_NaN(), });
	}

	aeFLOAT t = (a / (a - b));
	return ((s.B - s.A) * t + s.A);
}
//------------------------------------------------------------------------------
_AE_END_
//------------------------------------------------------------------------------
#endif//_AE_PLANE_h_
