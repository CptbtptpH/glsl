/**
	\file Quaternion.h
	\brief 四元数定义
	\author 王延樑
*/
//------------------------------------------------------------------------------
#ifndef _AE_QUATERNION_H_
#define _AE_QUATERNION_H_ "Quaternion.h"
//------------------------------------------------------------------------------
#include "Vector.h"
//------------------------------------------------------------------------------
_AE_BEGIN_
//------------------------------------------------------------------------------
class aeQuatf
{
public:
	enum { size = 4 };

	union
	{
		struct
		{
			/**
				\brief W分量
			*/
			aeFLOAT W;
			union
			{
				/**
					\brief 向量部分
				*/
				aeVec3f V;
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
			};
		};
		/**
			\brief 向量数组
		*/
		aeFLOAT D[1 + aeVec3f::size];
	};

	/**
		\brief 构造四元数
	*/
	aeQuatf()
		: W((aeFLOAT)1.0), V({ 0.0f, 0.0f, 0.0f })
	{
	}

	/**
		\brief 构造四元数
		\param w 标量部分
		\param x, y, z 向量部分
	*/
	aeQuatf(aeFLOAT w, aeFLOAT x, aeFLOAT y, aeFLOAT z)
		: W(w), V({ x, y, z })
	{
	}
	/**
		\brief 构造四元数
		\param v 标量部分w, 向量部分(x, y, z)
	*/
	aeQuatf(const aeFLOAT* v)
		: W(v[0]), V({ v[1], v[2], v[3] })
	{
	}
	/**
		\brief 构造四元数
		\param w 标量部分
		\param v 向量部分
	*/
	aeQuatf(aeFLOAT w, const aeVec3f& v)
	 : W(w), V(v)
	{
	}

	/**
		\brief 初始化四元数
	*/
	aeVOID Identity()
	{
		V.X = 0.0f;
		V.Y = 0.0f;
		V.Z = 0.0f;
		W   = 1.0f;
	}

	/**
		\brief 归一化四元数
		\return 归一化是否成功
	*/
	aeBOOL Normalize()
	{
		aeFLOAT len = sqrt(aeVecOperate<aeFLOAT, size>::Dot(this->D, this->D));
		if(FloatEqual(len, 0.0f))
		{
			return false;
		}
		aeVecOperate<aeFLOAT, size>::SelfDiv(this->D, len);

		return true;
	}

	/**
		\brief 从欧拉角转换成四元数
		\param heading 方位角，绕z轴旋转
		\param pitch 俯仰角，绕x轴旋转
		\param rotate 侧倾角，绕y轴旋转
	*/
	aeVOID FromEuler(aeFLOAT heading, aeFLOAT pitch, aeFLOAT rotate)
	{
		aeFLOAT sh = sin(heading * 0.5f);
		aeFLOAT ch = cos(heading * 0.5f);
		aeFLOAT sp = sin(pitch   * 0.5f);
		aeFLOAT cp = cos(pitch   * 0.5f);
		aeFLOAT sr = sin(rotate  * 0.5f);
		aeFLOAT cr = cos(rotate  * 0.5f);

		W   = cp * cr * ch + sp * sr * sh;
		V.X = sp * cr * ch + cp * sr * sh;
		V.Y = cp * sr * ch - sp * cr * sh;
		V.Z = cp * cr * sh - sp * sr * ch;
	}

	/**
		\brief 根据源和目标向量计算旋转四元数
		\param from 源向量
		\param to   目标向量
	*/
	aeVOID FromTo(const aeVec3f& from, const aeVec3f& to)
	{
		aeVec3f n    = Cross(from, to);
		aeFLOAT nl   = n.Length();
		aeFLOAT cosa = ActiveEngine::Dot(from, to);
		if (nl < c_FloatError)
		{
			if (cosa > 0.0f)
			{
				W   = 1.0f;
				V.X = 0.0f;
				V.Y = 0.0f;
				V.Z = 0.0f;
			}
			else
			{
				W = 0.0f;
				if (from.X * from.X < c_FloatError && from.Y * from.Y < c_FloatError)
				{
					V.X = 1.0f;
					V.Y = 0.0f;
					V.Z = 0.0f;
				}
				else
				{
					aeVec3f ta = { 0.0f, 0.0f, 1.0f };
					V = Cross(from, ta);
					V.Normalize();
				}
			}
		}
		else
		{
			aeVec3f l = Cross(n, from);
			if (ActiveEngine::Dot(l, to) < 0.0f)
			{
				n /= -nl;
			}
			else
			{
				n /= nl;
			}
			cosa /= (from.Length() * to.Length());

			aeFLOAT sa = (aeFLOAT)sqrt((1.0f - cosa) * 0.5f);
			W = (aeFLOAT)sqrt((1.0f + cosa) * 0.5f);
			V.X = sa * n.X;
			V.Y = sa * n.Y;
			V.Z = sa * n.Z;
		}
	}

	/**
		\brief 从四元数转换成欧拉角
		\param heading 方位角，绕z轴旋转
		\param pitch 俯仰角，绕x轴旋转
		\param rotate 侧倾角，绕y轴旋转
	*/
	aeVOID ToEuler(aeFLOAT& heading, aeFLOAT& pitch, aeFLOAT& rotate)
	{
		aeFLOAT sp = -2.0f * (V.Y * V.Z - W * V.X);
		if(FloatEqual(sp, 1.0f) || FloatEqual(sp, -1.0f))
		{
			pitch   = c_PI * 0.5f * sp;
			heading = atan2(-V.X * V.Z + W * V.Y, (0.5f) - V.Y * V.Y - V.Z * V.Z);
			rotate  = 0.0f;
		}
		else
		{
			pitch   = asin(sp);
			rotate  = atan2(V.X * V.Z + W * V.Y, 0.5f - V.X * V.X - V.Y * V.Y);
			heading = atan2(V.X * V.Y + W * V.Z, 0.5f - V.X * V.X - V.Z * V.Z);
		}
	}

	/**
		\brief 设定旋转轴和角度
		\param axis 旋转轴
		\param angle 旋转角(单位弧度)
	*/
	aeVOID setRotate(const aeVec3f& axis, aeFLOAT angle)
	{
		aeFLOAT sa = sin(angle * 0.5f) / axis.Length();
		W   = cos(angle * 0.5f);
		V.X = sa * axis.X;
		V.Y = sa * axis.Y;
		V.Z = sa * axis.Z;
	}

	/**
		\brief 设定旋转角度
		\param angle 旋转角(单位弧度)
	*/
	aeVOID setAngle(aeFLOAT angle)
	{
		if(W >= 1.0f)
		{
			V *= (aeFLOAT)sin(angle * 0.5f);
		}
		else
		{
			V *= (aeFLOAT)sin(angle * 0.5f) / FastSqrt(1 - W * W);
		}
		W =  (aeFLOAT)cos(angle * 0.5f);
	}

	/**
		\brief 获得旋转角(单位弧度)
		\return 旋转角
	*/
	aeFLOAT getAngle() const
	{
		return acos(W) * 2.0f;
	}

	/**
		\brief 获得旋转轴
		\return 旋转轴
	*/
	aeVec3f getAxis() const
	{
		if(W >= 1.0f)
		{
			return V;
		}
		return V * InvSqrt(1.0f - W * W);
	}

	/**
		\brief 求模
	*/
	aeFLOAT Module() const
	{
		return sqrt(aeVecOperate<aeFLOAT, size>::Dot(this->D, this->D));
	}

	/**
		\brief 求共轭四元数
		\return 共轭四元数
	*/
	aeQuatf Conjugate() const
	{
		return aeQuatf(W, -V);
	}

	/**
		\brief 四元数求逆
		\return 逆四元数
	*/
	aeQuatf Inverse() const
	{
		aeFLOAT _1_to_M = ((aeFLOAT)1) / Module();
		return aeQuatf(W * _1_to_M, V.X * -_1_to_M, V.Y * -_1_to_M, V.Z * -_1_to_M);
	}

	/**
		\brief 四元数乘数
		\param r 乘数
		\return 外积
	*/
	aeQuatf operator*(aeFLOAT r) const
	{
		aeFLOAT ret[size];
		aeVecOperate<aeFLOAT, size>::Mul(ret, this->D, r);
		return aeQuatf(ret);
	}

	/**
		\brief 四元数乘数
		\param r 乘数
		\return 积
	*/
	aeQuatf operator*=(aeFLOAT r)
	{
		aeVecOperate<aeFLOAT, size>::SelfMul(this->D, r);
		return *this;
	}

	/**
		\brief 四元数叉乘
		\param q 乘数
		\return 外积
	*/
	aeQuatf operator*=(const aeQuatf& q)
	{
		aeFLOAT dot = ActiveEngine::Dot(V, q.V);
		V = q.V * W + V * q.W + Cross(V, q.V);
		W = W * q.W - dot;
		return *this;
	}

	/**
		\brief 四元数叉乘
		\param q 乘数
		\return 外积
	*/
	aeQuatf operator*(const aeQuatf& q) const
	{
		return aeQuatf(W * q.W - ActiveEngine::Dot(V, q.V), q.V * W + V * q.W + Cross(V, q.V));
	}

	/**
		\brief 四元数乘向量
		\param v 乘数
		\return 向量
	*/
	aeQuatf operator*(const aeVec3f& v) const
	{
		return aeQuatf(-ActiveEngine::Dot(V, v), v * W + Cross(V, v));
	}

	/**
		\brief 四元数点乘
		\param r 乘数
		\return 点积
	*/
	aeFLOAT operator^(const aeQuatf& r) const
	{
		return aeVecOperate<aeFLOAT, 4>::Dot(D, r.D);
	}

	/**
		\brief 四元数差
		\param r 减数
		\return 差
	*/
	aeQuatf operator-(const aeQuatf& r) const
	{
		return r.Inverse() * (*this);
	}

	/**
		\brief 四元数点乘
		\param r 乘数
		\return 点积
	*/
	aeFLOAT Dot(const aeQuatf& r) const
	{
		return aeVecOperate<aeFLOAT, 4>::Dot(D, r.D);
	}

	/**
		\brief 四元数相等比较
		\param r 右侧四元数
		\return 两个四元数是否相等
	*/
	bool operator==(const aeQuatf& r) const
	{
		return aeVecOperate<aeFLOAT, 4>::Equal(D, r.D);
	}

	/**
		\brief 四元数不等比较
		\param r 右侧四元数
		\return 两个四元数是否不等
	*/
	bool operator!=(const aeQuatf& r) const
	{
		return aeVecOperate<aeFLOAT, 4>::NotEqual(D, r.D);
	}
};
//------------------------------------------------------------------------------
inline aeQuatf Slerp(const aeQuatf& from, const aeQuatf& to, aeFLOAT t)
{
	aeFLOAT s = from ^ to;
	aeFLOAT cs;
	if(s < 0.0f)
	{
		cs = -s;
	}
	else
	{
		cs = s;
	}

	aeFLOAT k;
	if(cs < 1.0f - c_FloatError)
	{
		aeFLOAT sn = sqrt(1.0f - cs * cs);
		aeFLOAT o  = atan(sn / cs);
		aeFLOAT _1_to_sn = 1.0f / sn;

		k = sin((1.0f - t) * o) * _1_to_sn;
		t = sin(t * o)          * _1_to_sn;
	}
	else
	{
		k = 1.0f - t;
	}
	if(s < 0.0f)
	{
		t *= -1.0f;
	}
#if 0//_MSC_VER
	if(g_SystemInfo.SSE)
	{
		aeFLOAT d[4];

		__asm
		{
			movups xmm0, from
			movss  xmm1, k
			movups xmm2, to
			movss  xmm3, t
			shufps xmm1, xmm1, 0
			shufps xmm3, xmm3, 0
			mulps  xmm0, xmm1
			mulps  xmm2, xmm3
			addps  xmm0, xmm2
			movups dword ptr[d], xmm0
		};
	}
	else
	{
#endif//_MSC_VER
		return aeQuatf(from.W   * k + to.W   * t,
		                             from.V.X * k + to.V.X * t,
									 from.V.Y * k + to.V.Y * t,
									 from.V.Z * k + to.V.Z * t);
#if 0//_MSC_VER
	}
#endif//_MSC_VER
}
//------------------------------------------------------------------------------
_AE_END_
//------------------------------------------------------------------------------
#endif//_AE_QUATERNION_H_
