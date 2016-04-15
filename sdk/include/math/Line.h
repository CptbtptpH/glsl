/**
	\brief 直线，射线，线段的定义
	\file Line.h
	\author whyjld
*/
//------------------------------------------------------------------------------
#include "MyMath.h"
#include "Vector.h"
//------------------------------------------------------------------------------
#ifndef _AE_LINE_h_
#define _AE_LINE_h_ "Line.h"
//------------------------------------------------------------------------------
_AE_BEGIN_
//------------------------------------------------------------------------------
/**
	\brief 直线定义
	方程定义为Line = A + D * t, t = [-INF, +INF]
*/
struct Line
{
	/**
		\brief 直线上一点，射线、线段的起点
	*/
	aeVec4f A;
	/**
		\brief 线上另一点，线段的终点
	*/
	aeVec4f B;
	/**
		\brief 方向
	*/
	aeVec4f D;
	/**
		\brief t是否在范围内
		\param t 方程参数
		\return t与范围的关系
		\retval -1 t在下界外
		\retval  0 t在界限内
		\retval  1 t在上界外
	*/
	static aeINT InRange(aeFLOAT t)
	{
		if(t > -std::numeric_limits<aeFLOAT>::infinity()
		   && t < std::numeric_limits<aeFLOAT>::infinity())
		{
			return 0;
		}
		return 1;
	}
	/**
		\brief t截断到范围
		\param t 方程参数
		\return 截断后的t
	*/
	static aeFLOAT Clamp(aeFLOAT t)
	{
		return t;
	}
};
//------------------------------------------------------------------------------
/**
	\brief 射线定义
	方程定义为Line = A + D * t, t = [0, +INF]
*/
struct Radial : public Line
{
	/**
		\brief t是否在范围内
		\param t 方程参数
		\return t与范围的关系
		\retval -1 t在下界外
		\retval  0 t在界限内
		\retval  1 t在上界外
	*/
	static aeINT InRange(aeFLOAT t)
	{
		if(t < 0.0f)
		{
			return -1;
		}
		if(t < std::numeric_limits<aeFLOAT>::infinity())
		{
			return 0;
		}
        return 1;
	}
	/**
		\brief t截断到范围
		\param t 方程参数
		\return 截断后的t
	*/
	static aeFLOAT Clamp(aeFLOAT t)
	{
		return max(t, 0.0f);
	}
};
//------------------------------------------------------------------------------
/**
	\brief 线段定义
	方程定义为Line = A + D * t, t = [0, 1]
*/
struct Segment : public Radial
{
	/**
		\brief t是否在范围内
		\param t 方程参数
		\return t与范围的关系
		\retval -1 t在下界外
		\retval  0 t在界限内
		\retval  1 t在上界外
	*/
	static aeINT InRange(aeFLOAT t)
	{
		if(t < 0.0f)
		{
			return -1;
		}
		else if(t > 1.0f)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	/**
		\brief t截断到范围
		\param t 方程参数
		\return 截断后的t
	*/
	static aeFLOAT Clamp(aeFLOAT t)
	{
		return ActiveEngine::Clamp(t, 0.0f, 1.0f);
	}
};
//------------------------------------------------------------------------------
/**
	\brief 计算线段
	\param from 线段起点
	\param to   线段终点
	\return 线段
*/
inline Segment MakeSegment(const aeVec4f& from, const aeVec4f& to)
{
	Segment ret;
	ret.A = from;
	ret.B = to;
	ret.D = ret.B - ret.A;

	return ret;
}
//------------------------------------------------------------------------------
/**
	\brief 计算射线
	\param from 射线起点
	\param p    射线上一点
	\return 射线
*/
inline Radial MakeRadial(const aeVec4f& from, const aeVec4f& p)
{
	Radial ret;
	ret.A = from;
	ret.B = p;
	ret.D = p - from;

	return ret;
}
//------------------------------------------------------------------------------
/**
	\brief 计算直线
	\param p0 直线上一点
	\param p1 直线上另一点
	\return 直线
*/
inline Line MakeLine(const aeVec4f& p0, const aeVec4f& p1)
{
	Line ret;
	ret.A = p0;
	ret.B = p1;
	ret.D = p1 - p0;

	return ret;
}
//------------------------------------------------------------------------------
/**
	\brief 计算点到直线的距离的平方
	\param l 射线
	\param p 点
	\return 点到直线的距离的平方
	不判断l是否为有效直线
*/
inline aeFLOAT DistanceSQPointToLine(const Line& l, const aeVec3f& p)
{
	aeVec4f ap = {p.X - l.A.X, p.Y - l.A.Y, p.Z - l.A.Z, 0.0f};
	aeFLOAT e  = Dot(ap, l.D);
	return Dot(ap, ap) - e * e / Dot(l.D, l.D);
}
//------------------------------------------------------------------------------
/**
	\brief 计算点到直线的距离的平方
	\param l 射线
	\param p 点
	\return 点到直线的距离的平方
	不判断l是否为有效直线
*/
inline aeFLOAT DistanceSQPointToLine(const Line& l, const aeVec4f& p)
{
	aeVec4f ap = p - l.A;
	aeFLOAT e  = Dot(ap, l.D);
	return Dot(ap, ap) - e * e / Dot(l.D, l.D);
}
//------------------------------------------------------------------------------
/**
	\brief 计算点到射线的距离的平方
	\param l 射线
	\param p 点
	\return 点到射线的距离的平方
	不判断l是否为有效射线
*/
inline aeFLOAT DistanceSQPointToLine(const Radial& r, const aeVec3f& p)
{
	aeVec4f ap = {p.X - r.A.X, p.Y - r.A.Y, p.Z - r.A.Z, 0.0f};
	aeFLOAT e  = Dot(ap, r.D);
	if(e <= 0.0f)
	{
		return Dot(ap, ap);
	}
	return Dot(ap, ap) - e * e / Dot(r.D, r.D);
}
//------------------------------------------------------------------------------
/**
	\brief 计算点到射线的距离的平方
	\param l 射线
	\param p 点
	\return 点到射线的距离的平方
	不判断l是否为有效射线
*/
inline aeFLOAT DistanceSQPointToLine(const Radial& r, const aeVec4f& p)
{
	aeVec4f ap = p - r.A;
	aeFLOAT e  = Dot(ap, r.D);
	if(e <= 0.0f)
	{
		return Dot(ap, ap);
	}
	return Dot(ap, ap) - e * e / Dot(r.D, r.D);
}
//------------------------------------------------------------------------------
/**
	\brief 计算点到线段的距离的平方
	\param l 线段
	\param p 点
	\return 点到线段的距离的平方
*/
inline aeFLOAT DistanceSQPointToLine(const Segment& s, const aeVec3f& p)
{
	aeVec4f ap = {p.X - s.A.X, p.Y - s.A.Y, p.Z - s.A.Z, 0.0f};
	aeFLOAT e  = Dot(ap, s.D);
	if(e <= 0.0f)
	{
		return Dot(ap, ap);
	}
	else if(e > Dot(s.D, s.D))
	{
		aeVec4f bp;
		bp.X = p.X - s.B.X;
		bp.Y = p.Y - s.B.Y;
		bp.Z = p.Z - s.B.Z;
		bp.W = 0.0f;
		return Dot(bp, bp);
	}
	return Dot(ap, ap) - e * e / Dot(s.D, s.D);
}
//------------------------------------------------------------------------------
/**
	\brief 计算点到线段的距离的平方
	\param l 线段
	\param p 点
	\return 点到线段的距离的平方
*/
inline aeFLOAT DistanceSQPointToLine(const Segment& s, const aeVec4f& p)
{
	aeVec4f ap = p - s.A;
	aeFLOAT e  = Dot(ap, s.D);
	if(e <= 0.0f)
	{
		return Dot(ap, ap);
	}
	else if(e > Dot(s.D, s.D))
	{
		aeVec4f bp = p - s.B;
		return Dot(bp, bp);
	}
	return Dot(ap, ap) - e * e / Dot(s.D, s.D);
}
//------------------------------------------------------------------------------
/**
	\brief 计算线段到线的距离的平方
	\param s 线段
	\param l 线段
	\return 线段到线距离的平方
*/
template<typename _L> inline aeFLOAT DistanceSQSegmentToLine(const Segment& s, const _L& t)
{
	aeVec4f l  = t.A - s.A;
	aeFLOAT a = Dot(t.D, t.D);
	aeFLOAT e = Dot(s.D, s.D);

	if(a <= c_FloatError && e <= c_FloatError)
	{
		return Dot(l, l);
	}
	aeFLOAT u, v;
	aeFLOAT f = Dot(s.D, l);
	if(a <= c_FloatError)
	{
		u = 0.0f;
		v = f / e;
		v = t.Clamp(v);
	}
	else
	{
		aeFLOAT c = Dot(t.D, l);
		if(e < c_FloatError)
		{
			v = 0.0f;
			u = t.Clamp(-c / a);
		}
		else
		{
			aeFLOAT b = Dot(t.D, s.D);
			aeFLOAT denom = a * e - b * b;
			if(denom != 0.0f)
			{
				u = t.Clamp((b * f - c * e) / denom);
			}
			else
			{
				u = 0.0f;
			}
			v = (b * u + f) / e;
			if(v < 0.0f)
			{
				u = t.Clamp(-c / a);
				v = 0.0f;
			}
			else if(v > 1.0f)
			{
				u = t.Clamp((b - c) / a);
				v = 1.0f;
			}
		}
	}
	l += (t.D * u - s.D * v);
	return Dot(l, l);
}
//------------------------------------------------------------------------------
/**
	\brief 点是否在线上
	\param l 线
	\param p 点
	\return p是否在l上
*/
template<typename _L> inline aeBOOL PointOnLine(const _L& l, const aeVec4f& p)
{
	return DistanceSQPointToLine(l, p) < c_FloatError;
}
//------------------------------------------------------------------------------
/**
	\brief 两条直线是否相等
	\param l 参与比较的线
	\param r 参与比较的线
*/
inline aeBOOL operator==(const Line& l, const Line& r)
{
	if(!PointOnLine(l, r.A))
	{
		return false;
	}
	aeFLOAT l0 = Dot(l.D, l.D);
	if(0.0f == l0)
	{
		return false;
	}
	aeFLOAT l1 = Dot(r.D, r.D);
	if(0.0f == l1)
	{
		return false;
	}
	l0 = 1.0f / l0;
	l1 = 1.0f / l1;

	return (l.D.X * l.D.X * l0 == r.D.X * r.D.X * l1)
		   && (l.D.Y * l.D.Y * l0 == r.D.Y * r.D.Y * l1)
		   && (l.D.Z * l.D.Z * l0 == r.D.Z * r.D.Z * l1);
}
//------------------------------------------------------------------------------
_AE_END_
//------------------------------------------------------------------------------
#endif//_AE_LINE_h_
