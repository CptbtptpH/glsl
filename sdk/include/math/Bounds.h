/**
	\brief 包围盒数据
	\author whyjld
	\file Bounds.h
*/
//------------------------------------------------------------------------------
#include "MyMath.h"
#include "Line.h"
#include "Plane.h"
//------------------------------------------------------------------------------
#ifndef _AE_BOUNDS_h_
#define _AE_BOUNDS_h_ "Bounds.h"
//------------------------------------------------------------------------------
#if defined(min)
#undef min
#endif
#if defined(max)
#undef max
#endif
//------------------------------------------------------------------------------
_AE_BEGIN_
//------------------------------------------------------------------------------
/**
	\brief 实体包围球
*/
struct BoundsSphere
{
	/**
		\brief 包围球球心
	*/
	aeVec4f Center;
	/**
		\brief 包围球半径
	*/
	aeFLOAT Radius;
};
//------------------------------------------------------------------------------
/**
	\brief 实体包围胶囊
*/
struct BoundsCapsule
{
	/**
		\brief 圆柱顶面圆心坐标
	*/
	aeVec4f TopCenter;
	/**
		\brief 圆柱底面圆心坐标
	*/
	aeVec4f BottomCenter;
	/**
		\brief 端面半径
	*/
	aeFLOAT Radius;
};
//------------------------------------------------------------------------------
/**
	\brief 轴对齐包围盒
*/
struct BoundsAABB
{
	union
	{
		/**
			\brief 最小角
		*/
		aeVec3f Min;
		struct  
		{
			/**
				\brief 包围盒最小X坐标
			*/
			aeFLOAT MinX;
			/**
				\brief 包围盒最小Y坐标
			*/
			aeFLOAT MinY;
			/**
				\brief 包围盒最小Z坐标
			*/
			aeFLOAT MinZ;
		};
	};
	union
	{
		/**
			\brief 最大角
		*/
		aeVec3f Max;
		struct  
		{
			/**
				\brief 包围盒最大X坐标
			*/
			aeFLOAT MaxX;
			/**
				\brief 包围盒最大Y坐标
			*/
			aeFLOAT MaxY;
			/**
				\brief 包围盒最大Z坐标
			*/
			aeFLOAT MaxZ;
		};
	};
};
//------------------------------------------------------------------------------
/**
	\brief 包围盒三角形
*/
struct BoundsTriangle
{
	/**
		\brief 三角形所在平面
	*/
	Plane p;
	/**
		\brief BC边平面,已经除以A到BC的距离
	*/
	Plane edgePlaneBC;
	/**
		\brief CA边平面,已经除以B到CA的距离
	*/
	Plane edgePlaneCA;
};
//------------------------------------------------------------------------------
/**
	\brief 求点是否在三角形内
	\param p 点
	\param t 包围三角形
	\param u 距BC边的距离
	\param v 距AC边的距离
	p必须出于t定义的平面内
*/
inline aeBOOL PointInTriangle(const aeVec4f& p, const BoundsTriangle& t, aeFLOAT& u, aeFLOAT& v)
{
	u = Dot(t.edgePlaneBC.N, p) - t.edgePlaneBC.D;
	if(u > 1.0f || u < 0.0f)
	{
		return false;
	}
	v = Dot(t.edgePlaneCA.N, p) - t.edgePlaneCA.D;
	if(v < 0.0f)
	{
		return false;
	}
	return (u + v <= 1.0f);
}
//------------------------------------------------------------------------------
/**
	\brief 求线与包围球是否相交
	\param l 线
	\param b 包围球
	\return 是否相交
*/
template<typename _L>
inline aeBOOL IntersectLinesToBoundBall(const _L& l, const BoundsSphere& b)
{
	return (b.Radius * b.Radius > DistanceSQPointToLines(l, b.Center));
}
//------------------------------------------------------------------------------
/**
	\brief 求射线与包围圆柱是否相交
	\param r 射线
	\param c 包围圆柱
	\return 是否相交
*/
template<typename _L>
inline aeBOOL IntersectedLinesToBoundCylinder(const _L& r, const BoundsCapsule& c)
{
	return (c.Radius * c.Radius > DistanceSQSegmentToLines(MakeSegment(c.BottomCenter, c.TopCenter), r));
}
//------------------------------------------------------------------------------
/**
	\brief 求线段与某轴范围的关系
	\param a 起点
	\param b 终点
	\param d 方向
	\param min 范围最小值
	\param max 范围最大值
	\param minT 最小的T
	\param maxT 最大的T
	\return 线段是否在范围内
*/
inline aeBOOL SegmentInRange(aeFLOAT a, aeFLOAT b, aeFLOAT d,
							 aeFLOAT min, aeFLOAT max,
							 aeFLOAT& minT, aeFLOAT& maxT)
{
	if(d > 0.0f)
	{
		if(a > max || b < min)
		{
			return false;
		}
		aeFLOAT _1_to_d = 1.0f / d;
		minT = (min - a) * _1_to_d;
		maxT = (max - a) * _1_to_d;
		return true;
	}
	else if(d < 0.0f)
	{
		if(a < min || b > max)
		{
			return false;
		}
		aeFLOAT _1_to_d = 1.0f / d;
		minT = (max - a) * _1_to_d;
		maxT = (min - a) * _1_to_d;
		return true;
	}
	else
	{
		minT = -std::numeric_limits<aeFLOAT>::infinity();
		maxT =  std::numeric_limits<aeFLOAT>::infinity();
		return (min <= a && a <= max);
	}
}
//------------------------------------------------------------------------------
/**
	\brief 求射线与某轴范围的关系
	\param a 起点
	\param d 方向
	\param min 范围最小值
	\param max 范围最大值
	\param minT 最小的T
	\param maxT 最大的T
	\return 射线是否在范围内
*/
inline aeBOOL RadialInRange(aeFLOAT a, aeFLOAT d,
							aeFLOAT min, aeFLOAT max,
							aeFLOAT& minT, aeFLOAT& maxT)
{
	if(d > 0.0f)
	{
		if(a > max)
		{
			return false;
		}
		aeFLOAT _1_to_d = 1.0f / d;
		minT = (min - a) * _1_to_d;
		maxT = (max - a) * _1_to_d;
		return true;
	}
	else if(d < 0.0f)
	{
		if(a < min)
		{
			return false;
		}
		aeFLOAT _1_to_d = 1.0f / d;
		minT = (max - a) * _1_to_d;
		maxT = (min - a) * _1_to_d;
		return true;
	}
	else
	{
		minT = -std::numeric_limits<aeFLOAT>::infinity();
		maxT =  std::numeric_limits<aeFLOAT>::infinity();
		return (min <= a && a <= max);
	}
}
//------------------------------------------------------------------------------
/**
	\brief 求直线与某轴范围的关系
	\param a 起点
	\param d 方向
	\param min 范围最小值
	\param max 范围最大值
	\param minT 最小的T
	\param maxT 最大的T
	\return 直线是否在范围内
*/
inline aeBOOL LineInRange(aeFLOAT a, aeFLOAT d,
						  aeFLOAT min, aeFLOAT max,
						  aeFLOAT& minT, aeFLOAT& maxT)
{
	if(d > 0.0f)
	{
		aeFLOAT _1_to_d = 1.0f / d;
		minT = (min - a) * _1_to_d;
		maxT = (max - a) * _1_to_d;
		return true;
	}
	else if(d < 0.0f)
	{
		aeFLOAT _1_to_d = 1.0f / d;
		minT = (max - a) * _1_to_d;
		maxT = (min - a) * _1_to_d;
		return true;
	}
	else
	{
		minT = -std::numeric_limits<aeFLOAT>::infinity();
		maxT =  std::numeric_limits<aeFLOAT>::infinity();
		return (min <= a && a <= max);
	}
}
//------------------------------------------------------------------------------
/**
	\brief 求线与某轴范围的关系
	\param a 起点
	\param b 终点
	\param d 方向
	\param min 范围最小值
	\param max 范围最大值
	\param minT 最小的T
	\param maxT 最大的T
	\return 线是否在范围内
*/
template<typename _L>
inline aeBOOL LinesInRange(aeFLOAT a, aeFLOAT b, aeFLOAT d,
                           aeFLOAT min, aeFLOAT max,
						   aeFLOAT& minT, aeFLOAT& maxT)
{
}
//------------------------------------------------------------------------------
/**
	\brief 求线与某轴范围的关系
	\param a 起点
	\param b 终点
	\param d 方向
	\param min 范围最小值
	\param max 范围最大值
	\param minT 最小的T
	\param maxT 最大的T
	\return 线是否在范围内
*/
template<>
inline aeBOOL LinesInRange<Segment>(aeFLOAT a, aeFLOAT b, aeFLOAT d,
							        aeFLOAT min, aeFLOAT max,
									aeFLOAT& minT, aeFLOAT& maxT)
{
	return SegmentInRange(a, b, d, min, max, minT, maxT);
}
//------------------------------------------------------------------------------
/**
	\brief 求线与某轴范围的关系
	\param a 起点
	\param b 终点
	\param d 方向
	\param min 范围最小值
	\param max 范围最大值
	\param minT 最小的T
	\param maxT 最大的T
	\return 线是否在范围内
*/
template<>
inline aeBOOL LinesInRange<Radial>(aeFLOAT a, aeFLOAT b, aeFLOAT d,
							       aeFLOAT min, aeFLOAT max,
								   aeFLOAT& minT, aeFLOAT& maxT)
{
	return RadialInRange(a, d, min, max, minT, maxT);
}
//------------------------------------------------------------------------------
/**
	\brief 求线与某轴范围的关系
	\param a 起点
	\param b 终点
	\param d 方向
	\param min 范围最小值
	\param max 范围最大值
	\param minT 最小的T
	\param maxT 最大的T
	\return 线是否在范围内
*/
template<>
inline aeBOOL LinesInRange<Line>(aeFLOAT a, aeFLOAT b, aeFLOAT d,
							     aeFLOAT min, aeFLOAT max,
								 aeFLOAT& minT, aeFLOAT& maxT)
{
	return LineInRange(a, d, min, max, minT, maxT);
}
//------------------------------------------------------------------------------
/**
	\brief 求点和AABB的关系
	\param p 点
	\param b AABB包围盒
	\return 点和AABB的关系
	\retval -1 点在AABB内
	\retval  0 点在AABB上
	\retval  1 点在AABB外
*/
template<typename _P>
inline aeINT PointWithAABB(const _P& p, const BoundsAABB& b)
{
	aeINT ret = -1;
	if(p.X < b.MinX || p.Y > b.MaxX)
	{
		return 1;
	}
	else if(p.X == b.MinX || p.Y == b.MaxX)
	{
		ret = 0;
	}
	if(p.Y < b.MinY || p.Y > b.MaxY)
	{
		return 1;
	}
	else if(p.Y == b.MinY || p.Y == b.MaxY)
	{
		ret = 0;
	}
	if(p.Z < b.MinZ || p.Z > b.MaxZ)
	{
		return 1;
	}
	else if(p.Z == b.MinZ || p.Z == b.MaxZ)
	{
		ret = 0;
	}
	return ret;
}
//------------------------------------------------------------------------------
/**
	\brief 求线与球是否相交
	\param r 线
	\param b 包围球
	\param minT 最小参数t
	\param maxT 最大参数t
	\return 是否相交
*/
template<typename _L> inline aeBOOL LinesInBoundSphere(const _L& l, const BoundsSphere& b, aeFLOAT& minT, aeFLOAT& maxT)
{
	aeVec4f e = l.A - b.Center;
	aeFLOAT d2 = Dot(l.D, l.D);
	aeFLOAT de = Dot(l.D, e);
	aeFLOAT e2 = Dot(e, e);
	aeFLOAT dlt = de * de - d2 * (e2 - b.Radius * b.Radius);
	if(dlt < 0)
	{
		return false;
	}
	else if(dlt == 0.0f)
	{
		minT = -de / d2;
		maxT = std::numeric_limits<aeFLOAT>::quiet_NaN();
	}
	else
	{
		dlt = sqrt(dlt);
		d2  = 1.0f / d2;
		minT = (-de - dlt) * d2;
		maxT = (-de + dlt) * d2;
	}
	return true;
}
//------------------------------------------------------------------------------
/**
	\brief 求线与AABB是否相交
	\param r 线
	\param b AABB
	\return 是否相交
*/
template<typename _L> inline aeBOOL IntersectedLinesToBoundAABB(const _L& l, const BoundsAABB& b)
{
	aeFLOAT min  = std::numeric_limits<aeFLOAT>::min();
	aeFLOAT max  = std::numeric_limits<aeFLOAT>::max();
	aeFLOAT tmin = std::numeric_limits<aeFLOAT>::quiet_NaN();
	aeFLOAT tmax = std::numeric_limits<aeFLOAT>::quiet_NaN();
	if(!LinesInRange<_L>(l.A.X, l.B.X, l.D.X, b.MinX, b.MaxX, tmin, tmax))
	{
		return false;
	}
	if(tmin == tmin)
	{
		min = tmin;
	}
	if(tmax == tmax)
	{
		max = tmax;
	}
	if(!LinesInRange<_L>(l.A.Y, l.B.Y, l.D.Y, b.MinY, b.MaxY, tmin, tmax))
	{
		return false;
	}
	if(tmin == tmin)
	{
		if(tmin > min)
		{
			min = tmin;
		}
	}
	if(tmax == tmax)
	{
		if(tmax < max)
		{
			max = tmax;
		}
	}
	if(!LinesInRange<_L>(l.A.Z, l.B.Z, l.D.Z, b.MinZ, b.MaxZ, tmin, tmax))
	{
		return false;
	}
	if(tmin == tmin)
	{
		if(tmin > min)
		{
			min = tmin;
		}
	}
	if(tmax == tmax)
	{
		if(tmax < max)
		{
			max = tmax;
		}
	}
	return min <= max && (_L::InRange(min) == 0 || _L::InRange(max) == 0);
}
//------------------------------------------------------------------------------
/**
	\brief 求射线与三角形的交点
	\param r 射线
	\param t 三角形
	\param p 碰撞点
	\param u 到BC边的距离
	\param v 到AC边的距离
	\return 交点对应的线段参数
	\retval NaN 没有交点
	考虑到实际应用中的情况，这里认为线平行于三角形则没有交点
*/
inline aeFLOAT IntersectedPointToBoundTriangle(const Radial& r, const BoundsTriangle& tri, aeVec4f& p, aeFLOAT& u, aeFLOAT& v)
{
	float distp = tri.p.D - Dot(tri.p.N, r.A);
	float dirtp = Dot(tri.p.N, r.D);
	if(dirtp == 0.0f)
	{
		return std::numeric_limits<aeFLOAT>::quiet_NaN();
	}
	if(distp * dirtp < 0.0f)
	{
		return std::numeric_limits<aeFLOAT>::quiet_NaN();
	}

	aeFLOAT ret = distp / dirtp;
	p = r.A + r.D * ret;
	if(PointInTriangle(p, tri, u, v))
	{
		return ret;
	}
	else
	{
		return std::numeric_limits<aeFLOAT>::quiet_NaN();
	}
}
//------------------------------------------------------------------------------
/**
	\brief 求线段与三角形的交点
	\param s 射线
	\param t 三角形
	\param p 碰撞点
	\param u 到BC边的距离
	\param v 到AC边的距离
	\return 交点对应的线段参数
	\retval NaN 没有交点
	考虑到实际应用中的情况，这里认为线平行于三角形则没有交点
*/
inline aeFLOAT IntersectedPointToBoundTriangle(const Segment& s, const BoundsTriangle& tri, aeVec4f& p, aeFLOAT& u, aeFLOAT& v)
{
	float distp = Dot(tri.p.N, s.A) - tri.p.D;
	float distq = Dot(tri.p.N, s.B) - tri.p.D;
	if(distp * distq > 0.0f)
	{
		return std::numeric_limits<aeFLOAT>::quiet_NaN();
	}
	if(distp == distq)
	{
		return std::numeric_limits<aeFLOAT>::quiet_NaN();
	}

	aeFLOAT ret = distp / (distp - distq);
	p = s.A + s.D * ret;
	if(PointInTriangle(p, tri, u, v))
	{
		return ret;
	}
	else
	{
		return std::numeric_limits<aeFLOAT>::quiet_NaN();
	}
}
//------------------------------------------------------------------------------
_AE_END_
//------------------------------------------------------------------------------
#endif//_AE_BOUNDS_h_