/**
	\file Defines.h
	\brief 公用标志定义
	\author 王延樑
*/
//------------------------------------------------------------------------------
#ifndef _AE_DEFINES_H_
#define _AE_DEFINES_H_ "Defines.h"
//------------------------------------------------------------------------------
//定义NameSpace
#define _AE_BEGIN_                                                             \
namespace ActiveEngine                                                         \
{

#define _AE_END_                                                               \
};
//------------------------------------------------------------------------------
#if defined(_DEBUG)
#	define _AE_DEBUG 1
#endif//_DEBUG
//------------------------------------------------------------------------------
//定义调用方式
#if defined(_MSC_VER)
#	define _USE_STDCALL 1
#elif defined(__GNUC__)
#	if defined(__i386__) || defined(__i686__)
#		define _USE_STDCALL 1
#	elif defined(__x86_64__)
#		define _USE_STDCALL 0
#	elif defined(__arm__)
#		define _USE_STDCALL 0
#	else
#		error Not supported platform.
#	endif
#else
#	error Not supported compiler.
#endif

#if _USE_STDCALL
#if defined(_MSC_VER) || defined(__BORLANDC__)
#	define EXPCALL __stdcall
#elif defined(__GNUC__)
#	define EXPCALL __attribute__ (( fastcall))
#endif
#else
#	define EXPCALL
#endif
//定义导出方式
#if defined(_WIN32)
#	if !defined(_WIN64)
#		define AEEXPORT __declspec(dllexport)
#		define AEIMPORT __declspec(dllimport)
#	else
#		define AEEXPORT
#		define AEIMPORT
#	endif
#elif defined(__linux__)
#	define AEEXPORT __attribute__ ((visibility("default")))
#	define AEIMPORT
#else
#   define AEEXPORT
#   define AEIMPORT
#endif
//------------------------------------------------------------------------------
#define DEF_ID(x)                                                              \
enum                                                                           \
{                                                                              \
	x  = ActiveEngine::x,                                                      \
	ID = x,                                                                    \
};
//------------------------------------------------------------------------------
_AE_BEGIN_
//------------------------------------------------------------------------------
/**
	\brief 设置符号
	\param v 要设置符号的变量
	\param s 包含符号的变量
*/
template<typename value_type, typename mask_type>
inline value_type SetSign(value_type v, value_type s)
{
	const mask_type mask = (((mask_type)1) << (sizeof(mask_type) * ((mask_type)8) - ((mask_type)1))) - ((mask_type)1);
	((mask_type&)v) = ((((mask_type&)v) & mask) | (((mask_type&)s) & (~mask)));
	return v;
}
//------------------------------------------------------------------------------
/**
	\brief 设置符号
	\param v 要设置符号的变量
	\param s 包含符号的变量
*/
inline float SetSign(float v, float s)
{
	return SetSign<float, unsigned long>(v, s);
}
//------------------------------------------------------------------------------
/**
	\brief 设置符号
	\param v 要设置符号的变量
	\param s 包含符号的变量
*/
inline double SetSign(double v, double s)
{
	if(v * s < 0.0)
	{
		return -v;
	}
	return v;
	//return SetSign<double, unsigned long long>(v, s);
}
//------------------------------------------------------------------------------
/**
	\brief 安全删除指针
	\param x 要删除的指针
*/
template <class t>
inline void SafeDelete(t& x)
{
	if(((t)0) != x)
	{
		delete x;
		x = ((t)0);
	}
}
//------------------------------------------------------------------------------
/**
	\brief 安全增加引用
	\param x 要增加引用的接口
*/
template <class t>
inline void SafeIncr(t x)
{
	if(((t)0) != x)
	{
		x->IncrRef();
	}
}
//------------------------------------------------------------------------------
/**
	\brief 安全增加引用
	\param x 要增加引用的接口
*/
template <class t>
inline void SafeAdd(t x)
{
	if(((t)0) != x)
	{
		x->AddRef();
	}
}
//------------------------------------------------------------------------------
/**
	\brief 安全释放接口
	\param x 要释放的接口
*/
template <class t>
inline void SafeRelease(t& x)
{
	if(((t)0) != x)
	{
		x->Release();
		x = ((t)0);
	}
}
//------------------------------------------------------------------------------
template<typename T>
struct is_ptr
{
	enum 
	{
		Value = false,
	};
};
//------------------------------------------------------------------------------
template<typename T>
struct is_ptr<T*>
{
	enum 
	{
		Value = true,
	};
};
//------------------------------------------------------------------------------
template<typename T>
struct is_ptr<const T*>
{
	enum 
	{
		Value = true,
	};
};
//------------------------------------------------------------------------------
_AE_END_
//------------------------------------------------------------------------------
#endif//_AE_DEFINES_H_
