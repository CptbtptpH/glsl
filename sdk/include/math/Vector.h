/**
	\file Vector.h
	\brief 数学向量定义
	\author 王延樑
*/
//------------------------------------------------------------------------------
#ifndef _AE_VECTOR_H_
#define _AE_VECTOR_H_ "Vector.h"
//------------------------------------------------------------------------------
#include "MyMath.h"
//------------------------------------------------------------------------------
_AE_BEGIN_
//------------------------------------------------------------------------------
/**
	\brief 向量操作模版
	\param T 向量数据类型
	\param n 向量维数
*/
template<typename T, int n>
class aeVecOperate
{
public:
	/**
		\brief 向量相等比较
		\param l 参与比较的向量
		\param r 参与比较的向量
		\return l与r是否相等
	*/
	static inline aeBOOL Equal(const T* l, const T* r)
	{
		return (*l == *r) && aeVecOperate<T, n - 1>::Equal(l + 1, r + 1);
	}
	/**
		\brief 向量不等比较
		\param l 参与比较的向量
		\param r 参与比较的向量
		\return l与r是否不等
	*/
	static inline aeBOOL NotEqual(const T* l, const T* r)
	{
		return (*l != *r) || aeVecOperate<T, n - 1>::NotEqual(l + 1, r + 1);
	}
	/**
		\brief 向量赋值
		\param l 目标向量
		\param r 源向量
	*/
	static inline aeVOID Assign(T* l, const T* r)
	{
		*l = *r;
		aeVecOperate<T, n - 1>::Assign(++l, ++r);
	}
	/**
		\brief 向量赋值
		\param l 目标向量
		\param r 常量 
	*/
	static inline aeVOID Assign(T* l, T r)
	{
		*l = r;
		aeVecOperate<T, n - 1>::Assign(++l, r);
	}
	/**
		\brief 向量自加
		\param l 被加数
		\param r 加数
	*/
	static inline aeVOID SelfAdd(T* l, const T* r)
	{
		*l += *r;
		aeVecOperate<T, n - 1>::SelfAdd(++l, ++r);
	}
	/**
		\brief 向量自减
		\param l 被减数
		\param r 减数
	*/
	static inline aeVOID SelfSub(T* l, const T* r)
	{
		*l -= *r;
		aeVecOperate<T, n - 1>::SelfSub(++l, ++r);
	}
	/**
		\brief 向量自乘数
		\param v 向量
		\param num 乘数
	*/
	static inline aeVOID SelfMul(T* v, T num)
	{
		*v *= num;
		aeVecOperate<T, n - 1>::SelfMul(++v, num);
	}
	/**
		\brief 向量自除数
		\param v 向量
		\param num 除数
	*/
	static inline aeVOID SelfDiv(T* v, T num)
	{
		T _1_to_n = (T)1 / num;
		SelfMul(v, _1_to_n);
	}
	/**
		\brief 向量加法
		\param ret 和
		\param l 被加数
		\param r 加数
	*/
	static inline aeVOID Add(T* ret, const T* l, const T* r)
	{
		*ret = *l + *r;
		aeVecOperate<T, n - 1>::Add(++ret, ++l, ++r);
	}
	/**
		\brief 向量减法
		\param ret 差
		\param l 被减数
		\param r 减数
	*/
	static inline aeVOID Sub(T* ret, const T* l, const T* r)
	{
		*ret = *l - *r;
		aeVecOperate<T, n - 1>::Sub(++ret, ++l, ++r);
	}
	/**
		\brief 向量乘数
		\param r 积
		\param v 向量
		\param num 乘数
	*/
	static inline aeVOID Mul(T* r, const T* v, T num)
	{
		*r = (*v) * num;
		aeVecOperate<T, n - 1>::Mul(++r, ++v, num);
	}
	/**
		\brief 向量除数
		\param r 商
		\param v 向量
		\param num 除数
	*/
	static inline aeVOID Div(T* r, const T* v, T num)
	{
		T _1_to_n = (T)1 / num;
		Mul(r, v, _1_to_n);
	}
	/**
		\brief 向量内积
		\param l 被乘数
		\param r 乘数
		\return 内积
	*/
	static inline T Dot(const T* l, const T* r)
	{
		return (*l) * (*r) + aeVecOperate<T, n - 1>::Dot(l + 1, r + 1);
	}
};
//------------------------------------------------------------------------------
/**
	\brief 向量操作模版
	\param T 向量数据类型
*/
template<typename T>
class aeVecOperate<T, 1>
{
public:
	/**
		\brief 向量相等比较
		\param l 参与比较的向量
		\param r 参与比较的向量
		\return l与r是否相等
	*/
	static inline aeBOOL Equal(const T* l, const T* r)
	{
		return *l == *r;
	}
	/**
		\brief 向量不等比较
		\param l 参与比较的向量
		\param r 参与比较的向量
		\return l与r是否不等
	*/
	static inline aeBOOL NotEqual(const T* l, const T* r)
	{
		return *l != *r;
	}
	/**
		\brief 向量赋值
		\param l 目标向量
		\param r 源向量
	*/
	static inline aeVOID Assign(T* l, const T* r)
	{
		*l = *r;
	}
	/**
		\brief 向量赋值
		\param l 目标向量
		\param r 常量 
	*/
	static inline aeVOID Assign(T* l, T r)
	{
		*l = r;
	}
	/**
		\brief 向量自加
		\param l 被加数
		\param r 加数
	*/
	static inline aeVOID SelfAdd(T* l, const T* r)
	{
		*l += *r;
	}
	/**
		\brief 向量自减
		\param l 被减数
		\param r 减数
	*/
	static inline aeVOID SelfSub(T* l, const T* r)
	{
		*l -= *r;
	}
	/**
		\brief 向量自乘数
		\param v 向量
		\param num 乘数
	*/
	static inline aeVOID SelfMul(T* v, T num)
	{
		*v *= num;
	}
	/**
		\brief 向量自除数
		\param v 向量
		\param num 除数
	*/
	static inline aeVOID SelfDiv(T* v, T num)
	{
		T _1_to_n = (T)1 / num;
		SelfMul(v, _1_to_n);
	}
	/**
		\brief 向量加法
		\param ret 和
		\param l 被加数
		\param r 加数
	*/
	static inline aeVOID Add(T* ret, const T* l, const T* r)
	{
		*ret = *l + *r;
	}
	/**
		\brief 向量减法
		\param ret 差
		\param l 被减数
		\param r 减数
	*/
	static inline aeVOID Sub(T* ret, const T* l, const T* r)
	{
		*ret = *l - *r;
	}
	/**
		\brief 向量乘数
		\param r 积
		\param v 向量
		\param num 乘数
	*/
	static inline aeVOID Mul(T* r, const T* v, T num)
	{
		*r = (*v) * num;
	}
	/**
		\brief 向量除数
		\param r 商
		\param v 向量
		\param num 除数
	*/
	static inline aeVOID Div(T* r, const T* v, T num)
	{
		T _1_to_n = (T)1 / num;
		Mul(r, v, _1_to_n);
	}
	/**
		\brief 向量内积
		\param l 被乘数
		\param r 乘数
		\return 内积
	*/
	static inline T Dot(const T* l, const T* r)
	{
		return (*l) * (*r);
	}
};
//------------------------------------------------------------------------------
/**
	\brief 向量操作模版4f
*/
template<>
class aeVecOperate<aeFLOAT, 4>
{
public:
	/**
		\brief 向量相等比较
		\param l 参与比较的向量
		\param r 参与比较的向量
		\return l与r是否相等
	*/
	static inline aeBOOL Equal(const aeFLOAT* l, const aeFLOAT* r)
	{
#if 0//_MSC_VER
		if(g_SystemInfo.SSE)
		{
			aeUINT32 _ret;
			__asm
			{
				mov    eax,  l
				movups xmm0, [eax]
				mov    ebx,  r
				movups xmm1, [ebx]
				cmpps  xmm0, xmm1, 0
				movaps xmm1, xmm0
				shufps xmm1, xmm1, 31h
				andps  xmm0, xmm1
				movaps xmm1, xmm0
				shufps xmm1, xmm1, 2h
				andps  xmm0, xmm1
				movss  _ret, xmm0
			}
			return (_ret == 0xffffffff);
		}
		else
		{
#endif//_MSC_VER
			return (*l == *r) && aeVecOperate<aeFLOAT, 3>::Equal(l + 1, r + 1);
#if 0//_MSC_VER
		}
#endif//MSC_VER
	}
	/**
		\brief 向量不等比较
		\param l 参与比较的向量
		\param r 参与比较的向量
		\return l与r是否不等
	*/
	static inline aeBOOL NotEqual(const aeFLOAT* l, const aeFLOAT* r)
	{
#if 0//_MSC_VER
		if(g_SystemInfo.SSE)
		{
			aeUINT32 _ret;
			__asm
			{
				mov eax, l
				movups xmm0, [eax]
				mov ebx, r
				movups xmm1, [ebx]
				cmpps xmm0, xmm1, 4
				movaps xmm1, xmm0
				shufps xmm1, xmm1, 31h
				andps  xmm0, xmm1
				movaps xmm1, xmm0
				shufps xmm1, xmm1, 2h
				andps  xmm0, xmm1
				movss  _ret, xmm0
			}
			return (_ret == 0xffffffff);
		}
		else
		{
#endif//_MSC_VER
			return (*l != *r) || aeVecOperate<aeFLOAT, 3>::NotEqual(l + 1, r + 1);
#if 0//_MSC_VER
		}
#endif//_MSC_VER
	}
	/**
		\brief 向量赋值
		\param l 目标向量
		\param r 源向量
	*/
	static inline aeVOID Assign(aeFLOAT* l, const aeFLOAT* r)
	{
		*l = *r;
		aeVecOperate<aeFLOAT, 3>::Assign(++l, ++r);
	}
	/**
		\brief 向量赋值
		\param l 目标向量
		\param r 常量 
	*/
	static inline aeVOID Assign(aeFLOAT* l, aeFLOAT r)
	{
		*l = r;
		aeVecOperate<aeFLOAT, 3>::Assign(++l, r);
	}
	/**
		\brief 向量自加
		\param l 被加数
		\param r 加数
	*/
	static inline aeVOID SelfAdd(aeFLOAT* l, const aeFLOAT* r)
	{
#if 0//_MSC_VER
		if(g_SystemInfo.SSE)
		{
			__asm
			{
				mov    eax,   l
				movups xmm0,  [eax]
				mov    ebx,   r
				movups xmm1,  [ebx]
				addps  xmm0,  xmm1
				movups [eax], xmm0
			};
		}
		else
		{
#endif//_MSC_VER
			*l += *r;
			aeVecOperate<aeFLOAT, 3>::SelfAdd(++l, ++r);
#if 0//_MSC_VER
		}
#endif//_MSC_VER
	}
	/**
		\brief 向量自减
		\param l 被减数
		\param r 减数
	*/
	static inline aeVOID SelfSub(aeFLOAT* l, const aeFLOAT* r)
	{
#if 0//_MSC_VER
		if(g_SystemInfo.SSE)
		{
			__asm
			{
				mov    eax,   l
				movups xmm0,  [eax]
				mov    ebx,   r
				movups xmm1,  [ebx]
				subps  xmm0,  xmm1
				movups [eax], xmm0
			};
		}
		else
		{
#endif//_MSC_VER
			*l -= *r;
			aeVecOperate<aeFLOAT, 3>::SelfSub(++l, ++r);
#if 0//_MSC_VER
		}
#endif//_MSC_VER
	}
	/**
		\brief 向量自乘数
		\param v 向量
		\param num 乘数
	*/
	static inline aeVOID SelfMul(aeFLOAT* v, aeFLOAT num)
	{
#if 0//_MSC_VER
		if(g_SystemInfo.SSE)
		{
			__asm
			{
				mov    eax,   v
				movups xmm0,  [eax]
				lea    ebx,   num
				movups xmm1,  [ebx]
				shufps xmm1,  xmm1, 0h
				mulps  xmm0,  xmm1
				movups [eax], xmm0
			};
		}
		else
		{
#endif//_MSC_VER
			*v *= num;
			aeVecOperate<aeFLOAT, 3>::SelfMul(++v, num);
#if 0//_MSC_VER
		}
#endif//_MSC_VER
	}
	/**
		\brief 向量自除数
		\param v 向量
		\param num 除数
	*/
	static inline aeVOID SelfDiv(aeFLOAT* v, aeFLOAT num)
	{
		aeFLOAT _1_to_n = 1.0f / num;
		SelfMul(v, _1_to_n);
	}
	/**
		\brief 向量加法
		\param _ret 和
		\param l 被加数
		\param r 加数
	*/
	static inline aeVOID Add(aeFLOAT* _ret, const aeFLOAT* l, const aeFLOAT* r)
	{
#if 0//_MSC_VER
		if(g_SystemInfo.SSE)
		{
			__asm
			{
				mov    eax,   l
				movups xmm0,  [eax]
				mov    ebx,   r
				movups xmm1,  [ebx]
				mov    eax,   _ret
				addps  xmm0,  xmm1
				movups [eax], xmm0
			};
		}
		else
		{
#endif//_MSC_VER
			*_ret = *l + *r;
			aeVecOperate<aeFLOAT, 3>::Add(++_ret, ++l, ++r);
#if 0//_MSC_VER
		}
#endif//_MSC_VER
	}
	/**
		\brief 向量减法
		\param _ret 差
		\param l 被减数
		\param r 减数
	*/
	static inline aeVOID Sub(aeFLOAT* _ret, const aeFLOAT* l, const aeFLOAT* r)
	{
#if 0//_MSC_VER
		if(g_SystemInfo.SSE)
		{
			__asm
			{
				mov    eax,   l
				movups xmm0,  [eax]
				mov    ebx,   r
				movups xmm1,  [ebx]
				mov    eax,   _ret
				subps  xmm0,  xmm1
				movups [eax], xmm0
			};
		}
		else
		{
#endif//_MSC_VER
			*_ret = *l - *r;
			aeVecOperate<aeFLOAT, 3>::Sub(++_ret, ++l, ++r);
#if 0//_MSC_VER
		}
#endif//_MSC_VER
	}
	/**
		\brief 向量乘数
		\param r 积
		\param v 向量
		\param num 乘数
	*/
	static inline aeVOID Mul(aeFLOAT* r, const aeFLOAT* v, aeFLOAT num)
	{
#if 0//_MSC_VER
		if(g_SystemInfo.SSE)
		{
			__asm
			{
				lea    ebx,   num
				movss  xmm1,  [ebx]
				mov    eax,   v
				shufps xmm1,  xmm1, 0h
				movups xmm0,  [eax]
				mulps  xmm0,  xmm1
				mov    eax,   r
				movups [eax], xmm0
			};
		}
		else
		{
#endif//_MSC_VER
			*r = (*v) * num;
			aeVecOperate<aeFLOAT, 3>::Mul(++r, ++v, num);
#if 0//_MSC_VER
		}
#endif//_MSC_VER
	}
	/**
		\brief 向量除数
		\param r 商
		\param v 向量
		\param num 除数
	*/
	static inline aeVOID Div(aeFLOAT* r, const aeFLOAT* v, aeFLOAT num)
	{
		aeFLOAT _1_to_n = 1.0f / num;
		Mul(r, v, _1_to_n);
	}
	/**
		\brief 向量内积
		\param l 被乘数
		\param r 乘数
		\return 内积
	*/
	static inline aeFLOAT Dot(const aeFLOAT* l, const aeFLOAT* r)
	{
#if 0//_MSC_VER
		if(g_SystemInfo.SSE)
		{
			aeFLOAT _ret;
			__asm
			{
				mov eax, l
				movups xmm0, [eax]
				mov ebx, r
				movups xmm1, [ebx]
				mulps  xmm0, xmm1
				lea ecx, _ret
				shufps xmm1, xmm0, 31h
				addps xmm0, xmm1
				shufps xmm1, xmm0, 2h
				addss xmm0, xmm1
				movss dword ptr[ecx], xmm0
			}
			return _ret;
		}
		else
		{
#endif//MSC_VER
			return (*l) * (*r) + aeVecOperate<aeFLOAT, 3>::Dot(l + 1, r + 1);
#if 0//_MSC_VER
		}
#endif//MSC_VER
	}
};
//------------------------------------------------------------------------------
/**
\brief 2维向量模版 knife 2016
\param T 数据类型
*/
class aeVec2f
{
public:
	enum { size = 2};

	union
	{
		struct
		{
			/**
			\brief X分量
			*/
			aeFLOAT X;
			/**
			\brief Y分量
			*/
			aeFLOAT Y;
		
		};
		/**
		\brief 向量数组
		*/
		aeFLOAT D[size];
	};
};
//------------------------------------------------------------------------------
/**
	\brief 3维向量模版
	\param T 数据类型
*/
class aeVec3f
{
public:
	enum { size = 3 };
	union
	{
		struct
		{
			/**
				\brief X分量
			*/
			aeFLOAT X;
			/**
				\brief Y分量
			*/
			aeFLOAT Y;
			/**
				\brief Z分量
			*/
			aeFLOAT Z;
		};
		/**
			\brief 向量数组
		*/
		aeFLOAT D[size];
	};
	/**
		\brief 获得向量长度
		\return 向量长度
	*/
	aeFLOAT Length() const
	{
		return sqrt(aeVecOperate<aeFLOAT, size>::Dot(this->D, this->D));
	}
	/**
		\brief 归一化向量
		\return 归一化是否成功
	*/
	aeBOOL Normalize()
	{
		aeFLOAT _1_to_len = InvSqrt(aeVecOperate<aeFLOAT, size>::Dot(this->D, this->D));
		if(IsINF(_1_to_len))
		{
			return false;
		}
		aeVecOperate<aeFLOAT, size>::SelfMul(this->D, _1_to_len);

		return true;
	}
	/**
		\brief 向量相等运算符
		\param r 比较对象
		\return 是否相等
	*/
	aeBOOL operator==(const aeVec3f& r) const
	{
		return aeVecOperate<aeFLOAT, size>::Equal(this->D, r.D);
	}
	/**
		\brief 向量不相等运算符
		\param r 比较对象
		\return 是否不相等
	*/
	aeBOOL operator!=(const aeVec3f& r) const
	{
		return aeVecOperate<aeFLOAT, size>::NotEqual(this->D, r.D);
	}
	/**
		\brief 逆向量
		\return 逆向量
	*/
	aeVec3f operator-() const
	{
		return (*this) * ((aeFLOAT)-1);
	}
	/**
		\brief 向量加
		\param r 加数
		\return 和
	*/
	aeVec3f operator+(const aeVec3f& r) const
	{
		aeVec3f ret;
		aeVecOperate<aeFLOAT, size>::Add(ret.D, this->D, r.D);
		return ret;
	}
	/**
		\brief 向量减
		\param r 减数
		\return 差
	*/
	aeVec3f operator-(const aeVec3f& r) const
	{
		aeVec3f ret;
		aeVecOperate<aeFLOAT, size>::Sub(ret.D, this->D, r.D);
		return ret;
	}
	/**
		\brief 向量乘以数
		\param r 乘数
		\return 积
	*/
	aeVec3f operator*(aeFLOAT r) const
	{
		aeVec3f ret;
		aeVecOperate<aeFLOAT, size>::Mul(ret.D, this->D, r);
		return ret;
	}
	/**
		\brief 向量除以数
		\param r 除数
		\return 商
	*/
	aeVec3f operator/(aeFLOAT r) const
	{
		aeVec3f ret;
		aeVecOperate<aeFLOAT, size>::Div(ret.D, this->D, r);
		return ret;
	}
	/**
		\brief 向量自加
		\param r 加数
		\return 和
	*/
	const aeVec3f& operator+=(const aeVec3f& r)
	{
		aeVecOperate<aeFLOAT, size>::SelfAdd(this->D, r.D);
		return *this;
	}
	/**
		\brief 向量自减
		\param r 减数
		\return 商
	*/
	const aeVec3f& operator-=(const aeVec3f& r)
	{
		aeVecOperate<aeFLOAT, size>::SelfSub(this->D, r.D);
		return *this;
	}
	/**
		\brief 向量自乘数
		\param r 乘数
		\return 积
	*/
	const aeVec3f& operator*=(const aeFLOAT r)
	{
		aeVecOperate<aeFLOAT, size>::SelfMul(this->D, r);
		return *this;
	}
	/**
		\brief 向量自除数
		\param r 除数
		\return 商
	*/
	const aeVec3f& operator/=(const aeFLOAT r)
	{
		aeVecOperate<aeFLOAT, size>::SelfDiv(this->D, r);
		return *this;
	}
};
//------------------------------------------------------------------------------
/**
	\brief 4维向量
	\param T 数据类型
*/
class aeVec4f
{
public:
	enum { size = 4 };
	union
	{
		struct
		{
			/**
				\brief X分量
			*/
			aeFLOAT X;
			/**
				\brief Y分量
			*/
			aeFLOAT Y;
			/**
				\brief Z分量
			*/
			aeFLOAT Z;
			/**
				\brief W分量
			*/
			aeFLOAT W;
		};
		/**
			\brief 向量数组
		*/
		aeFLOAT D[size];
	};
	/**
		\brief 获得向量长度
		\return 向量长度
	*/
	aeFLOAT Length() const
	{
		return sqrt(aeVecOperate<aeFLOAT, size>::Dot(this->D, this->D));
	}
	/**
		\brief 归一化向量
		\return 归一化是否成功
	*/
	aeBOOL Normalize()
	{
		aeFLOAT _1_to_len = InvSqrt(aeVecOperate<aeFLOAT, size>::Dot(this->D, this->D));
		if(IsINF(_1_to_len))
		{
			return false;
		}
		aeVecOperate<aeFLOAT, size>::SelfMul(this->D, _1_to_len);

		return true;
	}
	/**
		\brief 向量赋值运算符
		\param v 值
		\return 赋值后的值
	*/
	aeVec4f operator=(const aeVec3f& v)
	{
		aeVecOperate<aeFLOAT, 3>::Assign(this->D, v.D);
		return *this;
	}
	/**
		\brief 向量相等运算符
		\param r 比较对象
		\return 是否相等
	*/
	aeBOOL operator==(const aeVec4f& r) const
	{
		return aeVecOperate<aeFLOAT, size>::Equal(this->D, r.D);
	}
	/**
		\brief 向量不相等运算符
		\param r 比较对象
		\return 是否不相等
	*/
	aeBOOL operator!=(const aeVec4f& r) const
	{
		return aeVecOperate<aeFLOAT, size>::NotEqual(this->D, r.D);
	}
	/**
		\brief 逆向量
		\return 逆向量
	*/
	aeVec4f operator-() const
	{
		return (*this) * ((aeFLOAT)-1);
	}
	/**
		\brief 向量加
		\param r 加数
		\return 和
	*/
	aeVec4f operator+(const aeVec4f& r) const
	{
		aeVec4f ret;
		aeVecOperate<aeFLOAT, size>::Add(ret.D, this->D, r.D);
		return ret;
	}
	/**
		\brief 向量减
		\param r 减数
		\return 差
	*/
	aeVec4f operator-(const aeVec4f& r) const
	{
		aeVec4f ret;
		aeVecOperate<aeFLOAT, size>::Sub(ret.D, this->D, r.D);
		return ret;
	}
	/**
		\brief 向量乘以数
		\param r 乘数
		\return 积
	*/
	aeVec4f operator*(aeFLOAT r) const
	{
		aeVec4f ret;
		aeVecOperate<aeFLOAT, size>::Mul(ret.D, this->D, r);
		return ret;
	}
	/**
		\brief 向量除以数
		\param r 除数
		\return 商
	*/
	aeVec4f operator/(aeFLOAT r) const
	{
		aeVec4f ret;
		aeVecOperate<aeFLOAT, size>::Div(ret.D, this->D, r);
		return ret;
	}
	/**
		\brief 向量自加
		\param r 加数
		\return 和
	*/
	const aeVec4f& operator+=(const aeVec4f& r)
	{
		aeVecOperate<aeFLOAT, size>::SelfAdd(this->D, r.D);
		return *this;
	}
	/**
		\brief 向量自减
		\param r 减数
		\return 商
	*/
	const aeVec4f& operator-=(const aeVec4f& r)
	{
		aeVecOperate<aeFLOAT, size>::SelfSub(this->D, r.D);
		return *this;
	}
	/**
		\brief 向量自乘数
		\param r 乘数
		\return 积
	*/
	const aeVec4f& operator*=(const aeFLOAT r)
	{
		aeVecOperate<aeFLOAT, size>::SelfMul(this->D, r);
		return *this;
	}
	/**
		\brief 向量自除数
		\param r 除数
		\return 商
	*/
	const aeVec4f& operator/=(const aeFLOAT r)
	{
		aeVecOperate<aeFLOAT, size>::SelfDiv(this->D, r);
		return *this;
	}
	/**
		\brief 向量点乘
		\param r 乘数向量
		\return 点积
	*/
	aeFLOAT Dot(const aeVec4f& r) const
	{
		return aeVecOperate<aeFLOAT, size>::Dot(this->D, r.D);
	}
};
//------------------------------------------------------------------------------
/**
	\brief 构造3D向量
	\param d 数据数组
	\return 构造的3D向量
*/
inline aeVec3f MakeVec3(const aeFLOAT* d)
{
	aeVec3f ret;
	ret.X = d[0];
	ret.Y = d[1];
	ret.Z = d[2];
	return ret;
}
//------------------------------------------------------------------------------
/**
	\brief 构造4D向量
	\param v3 3D向量
	\return 构造的4D向量
*/
inline aeVec4f MakeVec4(const aeVec3f& v3)
{
	aeVec4f ret;
	ret.X = v3.X;
	ret.Y = v3.Y;
	ret.Z = v3.Z;
	ret.W = 1.0;
	return ret;
}
//------------------------------------------------------------------------------
/**
	\brief 构造4D向量
	\param d 数据数组
	\param s 数据数量
	\return 构造的4D向量
*/
inline aeVec4f MakeVec4(const aeFLOAT* d, aeUINT32 s)
{
	aeVec4f ret;
	switch(s)
	{
		case 0:
			ret.X = 0.0f;
			ret.Y = 0.0f;
			ret.Z = 0.0f;
			ret.W = 1.0f;
			break;
		case 1:
			ret.X = d[0];
			ret.Y = 0.0f;
			ret.Z = 0.0f;
			ret.W = 1.0f;
			break;
		case 2:
			ret.X = d[0];
			ret.Y = d[1];
			ret.Z = 0.0f;
			ret.W = 1.0f;
			break;
		case 3:
			ret.X = d[0];
			ret.Y = d[1];
			ret.Z = d[2];
			ret.W = 1.0f;
			break;
		default:
			ret.X = d[0];
			ret.Y = d[1];
			ret.Z = d[2];
			ret.W = d[3];
			break;
	}
	return ret;
}
//------------------------------------------------------------------------------
/**
	\brief 构造4D向量
	\param d 数据数组
	\param s 数据数量
	\return 构造的4D向量
*/
inline aeVec4f MakeVec4(const aeFLOAT* d)
{
	aeVec4f ret;
	ret.X = d[0];
	ret.Y = d[1];
	ret.Z = d[2];
	ret.W = d[3];
	return ret;
}
//------------------------------------------------------------------------------
/**
	\brief 向量内积
*/
inline aeFLOAT Dot(const aeVec3f& l, const aeVec3f& r)
{
	return aeVecOperate<aeFLOAT, 3>::Dot(l.D, r.D);
}
//------------------------------------------------------------------------------
/**
	\brief 向量内积
*/
inline aeFLOAT Dot(const aeVec3f& v3, const aeVec4f& v4)
{
	return aeVecOperate<aeFLOAT, 3>::Dot(v3.D, v4.D);
}
//------------------------------------------------------------------------------
/**
	\brief 向量内积
*/
inline aeFLOAT Dot(const aeVec4f& v4, const aeVec3f& v3)
{
	return aeVecOperate<aeFLOAT, 3>::Dot(v4.D, v3.D);
}
//------------------------------------------------------------------------------
/**
	\brief 向量内积
*/
inline aeFLOAT Dot(const aeVec4f& l, const aeVec4f& r)
{
	return aeVecOperate<aeFLOAT, 4>::Dot(l.D, r.D);
}
//------------------------------------------------------------------------------
/**
	\brief 向量外积
*/
inline aeVec3f Cross(const aeVec3f& l, const aeVec3f& r)
{
	return aeVec3f({l.Y * r.Z - l.Z * r.Y,
	                l.Z * r.X - l.X * r.Z,
	                l.X * r.Y - l.Y * r.X});
}
//------------------------------------------------------------------------------
/**
	\brief 向量外积
*/
inline aeVec4f Cross(const aeVec4f& l, const aeVec4f& r)
{
	return aeVec4f({l.Y * r.Z - l.Z * r.Y,
	                l.Z * r.X - l.X * r.Z,
	                l.X * r.Y - l.Y * r.X,
	                0.0f});
}
//------------------------------------------------------------------------------
_AE_END_
//------------------------------------------------------------------------------
#endif//_AE_VECTOR_H_
