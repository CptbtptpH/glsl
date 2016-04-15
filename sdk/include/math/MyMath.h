/**
	\file Math.h
	\brief 数学函数定义
	\author 王延樑
*/
//------------------------------------------------------------------------------
#ifndef _AE_MATH_H_
#define _AE_MATH_H_ 
//------------------------------------------------------------------------------

#include <math.h>
#include <stdlib.h>
#include <limits>
#include "../public/DataType.h"
 
//------------------------------------------------------------------------------
#ifdef max
#undef max
#undef min
#endif//max
//------------------------------------------------------------------------------
//定义基本常量
/**
	\brief 圆周率
*/
const aeFLOAT c_PI = 3.1415926535f;
//------------------------------------------------------------------------------
/**
	\brief 浮点误差
*/
const aeFLOAT c_FloatError = 0.0001f;
//------------------------------------------------------------------------------
_AE_BEGIN_
//------------------------------------------------------------------------------
/**
	\brief 判断是否是无穷
*/
inline aeBOOL IsINF(aeFLOAT x)
{
	return ((0x7f800000 == ((aeUINT32&)x))//IEEE754的Inf
	        || (0xff800000 == ((aeUINT32&)x)));//IEEE754的-Inf
}
//------------------------------------------------------------------------------
/**
	\brief 判断是否是无穷
*/
inline aeBOOL IsINF(aeDOUBLE x)
{
	return false;
}
//------------------------------------------------------------------------------
/**
	\brief 比较浮点数是否相等\n这个函数比较两个浮点数的差是否小于指定的最小误差
	\param x, y 参与比较的浮点数
	\return 是否相等
*/
inline aeBOOL FloatEqual(aeFLOAT x, aeFLOAT y)
{
	aeFLOAT d = x - y;
	((aeUINT32&)d) &= ((1UL << 31) - 1UL);
	return d < c_FloatError;
}
//------------------------------------------------------------------------------
/**
	\brief 取得两个值中较大一个
	\param l 第一个值
	\param r 第二个值
	\return 较大的值
*/
template<typename _T> _T max(_T l, _T r)
{
	return (l > r) ? l : r;
}
//------------------------------------------------------------------------------
/**
	\brief 取得两个值中较小一个
	\param l 第一个值
	\param r 第二个值
	\return 较小的值
*/
template<typename _T> _T min(_T l, _T r)
{
	return (l < r) ? l : r;
}
//------------------------------------------------------------------------------
/**
	\brief 截断数据
	\param v 要截断的数据
	\param l 下界
	\param h 上界
	\return 截断后的数据
*/
template<typename _T> _T Clamp(_T v, _T l, _T h)
{
	return max(min(h, v), l);
}
//------------------------------------------------------------------------------
/**
	\brief 交换两个值
	\param v1 第一个值
	\param v2 第二个值
*/
template<typename _T> aeVOID Swap(_T& v1, _T& v2)
{
	_T t = v1;
	v1   = v2;
	v2   = t;
}
//------------------------------------------------------------------------------
/**
	\brief 截断数据
	\param v 要截断的数据
*/
//------------------------------------------------------------------------------
/**
	\brief 计算平方根
*/
inline aeFLOAT FastSqrt(aeFLOAT x)
{
	return sqrt(x);
}
//------------------------------------------------------------------------------
/**
	\brief 计算平方根，从Quake3抄的
*/
//template<> inline aeFLOAT FastSqrt<aeFLOAT>(aeFLOAT x)
//{
//	union
//	{
//		int intPart;
//		float floatPart;
//	} convertor;
//	union
//	{
//		int intPart;
//		float floatPart;
//	} convertor2;
//	convertor.floatPart  = x;
//	convertor2.floatPart = x;
//	convertor.intPart    = 0x1FBCF800 + (convertor.intPart >> 1);
//	convertor2.intPart   = 0x5f3759df - (convertor2.intPart >> 1);
//	return 0.5f * (convertor.floatPart + (x * convertor2.floatPart));
//}
//------------------------------------------------------------------------------
/**
	\brief 计算平方根的倒数
*/
inline aeFLOAT InvSqrt(aeFLOAT x)
{
	return 1.0f / sqrt(x);
}
//------------------------------------------------------------------------------
/**
	\brief 计算平方根的倒数，从Quake3抄的
*/
//template<> inline aeFLOAT InvSqrt<aeFLOAT>(aeFLOAT x)
//{
//	aeFLOAT  h = 0.5f * x;
//	aeINT32& i = (aeINT32&)x;
//	i = 0x5f3759dfL - (i >> 1);
//	x = x * (1.5f - h * x * x);
//	return x;
//}
//------------------------------------------------------------------------------
/**
	\brief 计算平方根的倒数，从Quake3抄的
*/
//template<> inline aeDOUBLE InvSqrt<aeDOUBLE>(aeDOUBLE x)
//{
//	aeDOUBLE h = 0.5f * x;
//	aeINT64& i = (aeINT64&)x;
//	i = 0x5fe6ec85e7de30daLL - (i >> 1);
//	x = x * (1.5f - h * x * x);
//	return x;
//}
//------------------------------------------------------------------------------
/**
	\brief 平方函数
	\param v 参数
	\return v的平方
*/
inline aeFLOAT sqr(aeFLOAT v)
{
	return v * v;
}
//------------------------------------------------------------------------------
/**
	\brief 获得一个浮点随机数
	\return [0.0f-1.0f]的浮点随机数
*/
inline aeFLOAT frand()
{
	return ((aeFLOAT)rand()) / ((aeFLOAT)RAND_MAX);
}
//------------------------------------------------------------------------------
_AE_END_
//------------------------------------------------------------------------------
#endif//_AE_MATH_H_
