/**
	\file Matrix.h
	\brief 数学矩阵定义
	\author 王延樑
*/
//------------------------------------------------------------------------------
#ifndef _AE_MATRIX_H_
#define _AE_MATRIX_H_ "Matrix.h"
//------------------------------------------------------------------------------
#include "MyMath.h"
#include "Vector.h"
#include "Quaternion.h"
//------------------------------------------------------------------------------
_AE_BEGIN_
//------------------------------------------------------------------------------
/**
	\brief 4x4矩阵
	\param aeFLOAT 数据类型
	矩阵是列优先储存
*/
class aeMat4f
{
public:
	enum
	{
		width  = 4,
		height = 4,
	};

	/**
		\biref 列数组
	*/
	struct
	{
		aeVec4f C[width];
	};

	/**
		\brief 构造4x4矩阵
	*/
	aeMat4f()
	{
		Identity();
	}

	/**
		\brief 复制4x4矩阵
	*/
	aeMat4f(const aeFLOAT* src)
	{
		aeVecOperate<aeFLOAT, aeMat4f::width * aeMat4f::height>::Assign(get(), src);
	}

	/**
		\brief 获得矩阵数据指针
		\return 矩阵数据指针
	*/
	aeFLOAT* get()
	{
		return C[0].D;
	}

	/**
		\brief 获得矩阵数据指针
		\return 矩阵数据指针
	*/
	const aeFLOAT* get() const
	{
		return C[0].D;
	}

	/**
		\brief 初始化矩阵
	*/
	aeVOID Identity()
	{
		Element(0, 0) = 1.0f;
		Element(0, 1) = 0.0f;
		Element(0, 2) = 0.0f;
		Element(0, 3) = 0.0f;

		Element(1, 0) = 0.0f;
		Element(1, 1) = 1.0f;
		Element(1, 2) = 0.0f;
		Element(1, 3) = 0.0f;

		Element(2, 0) = 0.0f;
		Element(2, 1) = 0.0f;
		Element(2, 2) = 1.0f;
		Element(2, 3) = 0.0f;

		Element(3, 0) = 0.0f;
		Element(3, 1) = 0.0f;
		Element(3, 2) = 0.0f;
		Element(3, 3) = 1.0f;
	}

	/**
		\brief 缩放
		\param x X轴方向缩放量
		\param y Y轴方向缩放量
		\param z Z轴方向缩放量
	*/
	aeVOID Scale(aeFLOAT x, aeFLOAT y, aeFLOAT z)
	{
		C[0] *= x;
		C[1] *= y;
		C[2] *= z;
	}

	/**
		\brief 缩放
		\param s 三轴缩放量
	*/
	aeVOID Scale(const aeFLOAT* s)
	{
		C[0] *= s[0];
		C[1] *= s[1];
		C[2] *= s[2];
	}
	/**
	\brief 构造截头锥体 透视投影矩阵
	\param s
	*/
	aeVOID  Frustum(aeFLOAT left, aeFLOAT right, aeFLOAT bottom, aeFLOAT top, aeFLOAT zNear, aeFLOAT zFar)
	{
		Identity();

		C[0].X = zNear / right;
		C[1].Y = zNear / top;
		C[2].Z = -(zFar + zNear) / (zFar - zNear);
		C[2].W = -1; 
		C[3].Z = -2.0f*(zNear*zFar) / (zFar - zNear);
	}
	/**
	\brief 构造透视投影矩阵
	\param s  
	*/
	aeVOID Perspective(aeFLOAT fov, aeFLOAT aspect, aeFLOAT near, aeFLOAT far)
	{
		Identity();
		fov = (fov / 180.0f)*c_PI;
		C[0].X = 1.0f /(aspect * tanf(fov / 2));
		C[1].Y = 1.0f / tanf(fov / 2);
		C[2].Z = -(far + near) / (far - near);
		C[2].W = -1;// -2.0f*(far*near) / (far - near);
		C[3].Z = -2.0f*(far*near) / (far - near);
	}
	 
	/**
		\brief 平移
		\param x X轴方向平移量
		\param y Y轴方向平移量
		\param z Z轴方向平移量
	*/
	aeVOID Translate(aeFLOAT x, aeFLOAT y, aeFLOAT z)
	{
		C[3] += C[0] * x + C[1] * y + C[2] * z;
	}

	/**
		\brief 平移
		\param t 三轴方向平移量
	*/
	aeVOID Translate(const aeFLOAT* t)
	{
		C[3] += C[0] * t[0] + C[1] * t[1] + C[2] * t[2];
	}

	/**
		\brief 设置位置
		\param x X轴方向位置
		\param y Y轴方向位置
		\param z Z轴方向位置
	*/
	aeVOID Position(aeFLOAT x, aeFLOAT y, aeFLOAT z)
	{
		C[3].X = x;
		C[3].Y = y;
		C[3].Z = z;
	}

	/**
		\brief 设置位置
		\param p 三轴位置
	*/
	aeVOID Position(const aeFLOAT* p)
	{
		aeVecOperate<aeFLOAT, 3>::Assign(C[3].D, p);
	}

	/**
		\brief 旋转向量
		\param from 源向量
		\param to   目标向量
	*/
	aeVOID FromTo(const aeVec3f& from, const aeVec3f& to)
	{
		aeFLOAT _1_ftl = 1.0f / (from.Length() * to.Length());

		aeVec3f n = Cross(from, to);
		aeVec3f l = Cross(n, from);
		aeFLOAT nl = n.Length();
		aeFLOAT sina = nl * _1_ftl;
		aeFLOAT cosa = Dot(from, to) * _1_ftl;
		aeFLOAT _1_cos = 1.0f - cosa;
		if (Dot(l, to) < 0.0f)
		{
			n *= -1.0f;
		}
		aeFLOAT sax = _1_ftl * n.X;//sina * n.X
		aeFLOAT say = _1_ftl * n.Y;
		aeFLOAT saz = _1_ftl * n.Z;
		if (nl > c_FloatError)
		{
			n /= nl;
		}
		aeFLOAT xx = n.X * n.X;
		aeFLOAT yy = n.Y * n.Y;
		aeFLOAT zz = n.Z * n.Z;
		aeFLOAT xy = n.X * n.Y;
		aeFLOAT yz = n.Y * n.Z;
		aeFLOAT xz = n.X * n.Z;


		C[0].X = _1_cos * xx + cosa; C[0].Y = _1_cos * xy + saz;  C[0].Z = _1_cos * xz - say;  C[0].W = 0.0f;
		C[1].X = _1_cos * xy - saz;  C[1].Y = _1_cos * yy + cosa; C[1].Z = _1_cos * yz + sax;  C[1].W = 0.0f;
		C[2].X = _1_cos * xz + say;  C[2].Y = _1_cos * yz - sax;  C[2].Z = _1_cos * zz + cosa; C[2].W = 0.0f;
		C[3].X = 0.0f;               C[3].Y = 0.0f;               C[3].Z = 0.0f;               C[3].W = 1.0f;
	}

	/**
		\brief 旋转向量
		\param from 源向量
		\param to   目标向量
	*/
	aeVOID FromTo(const aeVec4f& from, const aeVec4f& to)
	{
		aeFLOAT _1_ftl = 1.0f / (from.Length() * to.Length());

		aeVec4f n = Cross(from, to);
		aeVec4f l = Cross(n, from);
		aeFLOAT nl = n.Length();
		aeFLOAT sina = nl * _1_ftl;
		aeFLOAT cosa = Dot(from, to) * _1_ftl;
		aeFLOAT _1_cos = 1.0f - cosa;
		if(Dot(l, to) < 0.0f)
		{
			n *= -1.0f;
		}
		aeFLOAT sax = _1_ftl * n.X;//sina * n.X
		aeFLOAT say = _1_ftl * n.Y;
		aeFLOAT saz = _1_ftl * n.Z;
		if(nl > c_FloatError)
		{
			n /= nl;
		}
		aeFLOAT xx = n.X * n.X;
		aeFLOAT yy = n.Y * n.Y;
		aeFLOAT zz = n.Z * n.Z;
		aeFLOAT xy = n.X * n.Y;
		aeFLOAT yz = n.Y * n.Z;
		aeFLOAT xz = n.X * n.Z;


		C[0].X = _1_cos * xx + cosa; C[0].Y = _1_cos * xy + saz;  C[0].Z = _1_cos * xz - say;  C[0].W = 0.0f;
		C[1].X = _1_cos * xy - saz;  C[1].Y = _1_cos * yy + cosa; C[1].Z = _1_cos * yz + sax;  C[1].W = 0.0f;
		C[2].X = _1_cos * xz + say;  C[2].Y = _1_cos * yz - sax;  C[2].Z = _1_cos * zz + cosa; C[2].W = 0.0f;
		C[3].X = 0.0f;               C[3].Y = 0.0f;               C[3].Z = 0.0f;               C[3].W = 1.0f;
	}
	/**
		\brief 绕轴旋转
		\param angle 旋转弧度
		\param x, y, z 旋转轴
	*/
	aeVOID Rotate(aeFLOAT angle, aeFLOAT x, aeFLOAT y, aeFLOAT z)
	{
		aeMat4f mat;

		//是否是特殊情况
		aeBOOL optimized = false;;
		
		const aeFLOAT s = sin(angle);
		const aeFLOAT c = cos(angle);

#define M(row, col) mat.Element(row, col)

		if(0.0f == x)
		{
			if(0.0f == y)
			{
				if(0.0f != z)
				{
					optimized = true;
					// 只处理绕Z轴旋转
					M(0, 0) = c;
					M(1, 1) = c;
					if (z < 0.0f)
					{
						M(0, 1) = s;
						M(1, 0) = -s;
					}
					else
					{
						M(0, 1) = -s;
						M(1, 0) = s;
					}
				}
			}
			else if (0.0f == z)
			{
				optimized = true;
				// 只处理绕Y轴旋转
				M(0, 0) = c;
				M(2, 2) = c;
				if (y < 0.0f)
				{
					M(0, 2) = -s;
					M(2, 0) = s;
				}
				else
				{
					M(0, 2) = s;
					M(2, 0) = -s;
				}
			}
		}
		else if (0.0f == y)
		{
			if (0.0f == z)
			{
				optimized = true;
				// 只处理绕X轴旋转
				M(1,1) = c;
				M(2,2) = c;
				if (x < 0.0f)
				{
					M(1, 2) = s;
					M(2, 1) = -s;
				}
				else
				{
					M(1, 2) = -s;
					M(2, 1) = s;
				}
			}
		}
		
		if (!optimized)//不是特殊情况
		{
			aeFLOAT ll = x * x + y * y + z * z;
			if(0.0f == ll)
			{
				//向量无效，不需要旋转
				return;
			}
			ll = 1.0f / ll;

			const aeFLOAT xx = x * x * ll;
			const aeFLOAT yy = y * y * ll;
			const aeFLOAT zz = z * z * ll;
			const aeFLOAT xy = x * y * ll;
			const aeFLOAT yz = y * z * ll;
			const aeFLOAT zx = z * x * ll;
			const aeFLOAT xs = x * s * ll;
			const aeFLOAT ys = y * s * ll;
			const aeFLOAT zs = z * s * ll;
			
			const aeFLOAT one_c = 1.0f - c;
			
			M(0,0) = (one_c * xx) + c;
			M(0,1) = (one_c * xy) - zs;
			M(0,2) = (one_c * zx) + ys;
			
			M(1,0) = (one_c * xy) + zs;
			M(1,1) = (one_c * yy) + c;
			M(1,2) = (one_c * yz) - xs;
			
			M(2,0) = (one_c * zx) - ys;
			M(2,1) = (one_c * yz) + xs;
			M(2,2) = (one_c * zz) + c;
		}
#undef M

		(*this) *= mat;
	}

	/**
		\brief 从欧拉角转换成矩阵
		\param heading 方位角，绕z轴旋转
		\param pitch 俯仰角，绕x轴旋转
		\param rotate 侧倾角，绕y轴旋转
	*/
	aeVOID FromEuler(aeFLOAT heading, aeFLOAT pitch, aeFLOAT rotate)
	{
		const aeFLOAT sh = sin(heading);
		const aeFLOAT ch = cos(heading);
		const aeFLOAT sp = sin(pitch);
		const aeFLOAT cp = cos(pitch);
		const aeFLOAT sr = sin(rotate);
		const aeFLOAT cr = cos(rotate);

		Element(0, 0) =  ch * cr + sh * sp * sr;
		Element(1, 0) =  sh * cp;
		Element(2, 0) = -ch * sr + sh * sp * cr;
		Element(3, 0) =  0.0f;

		Element(0, 1) = -sh * cr + ch * sp * sr;
		Element(1, 1) =  ch * cp;
		Element(2, 1) =  sh * sr + ch * sp * cr;
		Element(3, 1) = 0.0f;

		Element(0, 2) =  cp * sr;
		Element(1, 2) = -sp;
		Element(2, 2) =  cp * cr;
		Element(3, 2) = 0.0f;

		Element(0, 3) = 0.0f;
		Element(1, 3) = 0.0f;
		Element(2, 3) = 0.0f;
		Element(3, 3) = 1.0f;
	}

	/**
		\brief 从四元数转换成矩阵
		\param quat 四元数
	*/
	aeVOID FromQuaternion(const aeQuatf& quat)
	{
		const aeFLOAT xx = quat.V.X * quat.V.X;
		const aeFLOAT yy = quat.V.Y * quat.V.Y;
		const aeFLOAT zz = quat.V.Z * quat.V.Z;
		const aeFLOAT wx = quat.W   * quat.V.X;
		const aeFLOAT wy = quat.W   * quat.V.Y;
		const aeFLOAT wz = quat.W   * quat.V.Z;
		const aeFLOAT xy = quat.V.X * quat.V.Y;
		const aeFLOAT xz = quat.V.X * quat.V.Z;
		const aeFLOAT yz = quat.V.Y * quat.V.Z;

		Element(0, 0) = 1.0f - 2.0f * (yy + zz);
		Element(1, 0) = 2.0f * (xy + wz);
		Element(2, 0) = 2.0f * (xz - wy);
		Element(3, 0) = 0.0f;

		Element(0, 1) = 2.0f * (xy - wz);
		Element(1, 1) = 1.0f - 2.0f * (xx + zz);
		Element(2, 1) = 2.0f * (yz + wx);
		Element(3, 1) = 0.0f;

		Element(0, 2) = 2.0f * (xz + wy);
		Element(1, 2) = 2.0f * (yz - wx);
		Element(2, 2) = 1.0f - 2.0f * (xx + yy);
		Element(3, 2) = 0.0f;

		Element(0, 3) = 0.0f;
		Element(1, 3) = 0.0f;
		Element(2, 3) = 0.0f;
		Element(3, 3) = 1.0f;
	}

	/**
		\brief 矩阵求逆
		\return 逆矩阵
	*/
	aeMat4f Inverse() const
	{
		aeMat4f minv;

		aeFLOAT r1[8], r2[8], r3[8], r4[8];
		aeFLOAT *s[4], *tmprow;

		s[0] = &r1[0];
		s[1] = &r2[0];
		s[2] = &r3[0];
		s[3] = &r4[0];

		int i,j,p,jj;
		for(i = 0;i < 4;i++)
		{
			for(j = 0;j < 4;j++)
			{
				s[i][j] = Element(i, j);
				if(i == j)
				{
					s[i][j + 4] = 1.0f;
				}
				else
				{
					s[i][j + 4] = 0.0f;
				}
			}
		}
		aeFLOAT scp[4];
		for(i = 0;i < 4;++i)
		{
			scp[i] = aeFLOAT(fabs(s[i][0]));
			for(j = 1;j < 4;++j)
			{
				if(aeFLOAT(fabs(s[i][j])) > scp[i])
				{
					scp[i] = aeFLOAT(fabs(s[i][j]));
				}
			}
			if(scp[i] == 0.0f)
			{
				return minv; // singular matrix!
			}
		}

		int pivot_to;
		aeFLOAT scp_max;
		for(i = 0;i < 4;++i)
		{
			// select pivot row
			pivot_to = i;
			scp_max = aeFLOAT(fabs(s[i][i] / scp[i]));
			// find out which row should be on top
			for(p = i + 1;p < 4;++p)
			{
				if (aeFLOAT(fabs(s[p][i] / scp[p])) > scp_max)
				{
					scp_max  = aeFLOAT(fabs(s[p][i] / scp[p]));
					pivot_to = p;
				}
			}
			// Pivot if necessary
			if(pivot_to != i)
			{
				tmprow = s[i];
				s[i]   = s[pivot_to];
				s[pivot_to] = tmprow;
				aeFLOAT tmpscp;
				tmpscp = scp[i];
				scp[i] = scp[pivot_to];
				scp[pivot_to] = tmpscp;
			}

			aeFLOAT mji;
			// perform gaussian elimination
			for(j = i + 1;j < 4;j++)
			{
				mji = s[j][i] / s[i][i];
				s[j][i] = 0.0;
				for(jj = i + 1;jj < 8;++jj)
				{
					s[j][jj] -= mji*s[i][jj];
				}
			}
		}
		if(s[3][3] == 0.0f)
		{
			return minv; // singular matrix!
		}

		//
		// Now we have an upper triangular matrix.
		//
		//  x x x x | y y y y
		//  0 x x x | y y y y 
		//  0 0 x x | y y y y
		//  0 0 0 x | y y y y
		//
		//  we'll back substitute to get the inverse
		//
		//  1 0 0 0 | z z z z
		//  0 1 0 0 | z z z z
		//  0 0 1 0 | z z z z
		//  0 0 0 1 | z z z z 
		//

		aeFLOAT mij;
		for(i = 3;i > 0;--i)
		{
			for(j = i - 1;j > -1;--j)
			{
				mij = s[j][i] / s[i][i];
				for(jj = j + 1;jj < 8;++jj)
				{
					s[j][jj] -= mij * s[i][jj];
				}
			}
		}

		for(i = 0;i < 4;++i)
		{
			for(j = 0;j < 4;j++)
			{
				minv.Element(i,j) = s[i][j + 4] / s[i][i];
			}
		}

		return minv;
	}
	/**
		\brief 计算缩放
		\param scale 三轴缩放
	*/
	aeVOID ToScale(aeFLOAT scale[3]) const
	{
		scale[0] = sqrt(C[0].X * C[0].X + C[0].Y * C[0].Y + C[0].Z * C[0].Z);
		scale[1] = sqrt(C[1].X * C[1].X + C[1].Y * C[1].Y + C[1].Z * C[1].Z);
		scale[2] = sqrt(C[2].X * C[2].X + C[2].Y * C[2].Y + C[2].Z * C[2].Z);
	}
	/**
		\brief 计算旋转和三轴缩放
		\param quat  旋转
		\param scale 三轴缩放
	*/
	aeVOID ToQuaternion(aeQuatf& quat, aeFLOAT scale[3]) const
	{
		ToScale(scale);
		aeFLOAT s1 = C[0].X / scale[0];
		aeFLOAT s2 = C[1].Y / scale[1];
		aeFLOAT s3 = C[2].Z / scale[2];

		aeFLOAT tq[4];

		tq[0] = 1 + s1 + s2 + s3;
		tq[1] = 1 + s1 - s2 - s3;
		tq[2] = 1 - s1 + s2 - s3;
		tq[3] = 1 - s1 - s2 + s3;

		aeUINT j = 0;
		for(aeUINT i = 1;i < 4;++i)
		{
			j = (tq[i] > tq[j]) ? i : j;
		}

		switch(j)
		{
			case 0:
				quat.W   = tq[0];
				quat.V.X = C[1].Z - C[2].Y;//mat->_23-mat->_32;
				quat.V.Y = C[2].X - C[0].Z;//mat->_31-mat->_13;
				quat.V.Z = C[0].Y - C[1].X;//mat->_12-mat->_21;
				break;
			case 1:
				quat.W   = C[1].Z - C[2].Y;//mat->_23-mat->_32;
				quat.V.X = tq[1];
				quat.V.Y = C[0].Y + C[1].X;//mat->_12+mat->_21;
				quat.V.Z = C[2].X + C[0].Z;//mat->_31+mat->_13;
				break;
			case 2:
				quat.W   = C[2].X - C[0].Z;//mat->_31-mat->_13;
				quat.V.X = C[0].Y + C[1].X;//mat->_12+mat->_21;
				quat.V.Y = tq[2];
				quat.V.Z = C[1].Z + C[2].Y;//mat->_23+mat->_32;
				break;
			case 3:
				quat.W   = C[0].Y - C[1].X;//mat->_12-mat->_21;
				quat.V.X = C[2].X + C[0].Z;//mat->_31+mat->_13;
				quat.V.Y = C[1].Z + C[2].Y;//mat->_23+mat->_32;
				quat.V.Z = tq[3];
				break;
			default:
				break;
		}

		float s = sqrt(0.25f / tq[j]);
		quat.W   *= s;
		quat.V.X *= s;
		quat.V.Y *= s;
		quat.V.Z *= s;
	}
	/**
		\brief 获得位置
		\param p
	*/
	aeVOID ToPosition(aeFLOAT* p) const
	{
		aeVecOperate<aeFLOAT, 3>::Assign(p, C[3].D);
	}
	/**
		\brief 计算转置矩阵
		\param m 用于计算的源矩阵
		\return 转置矩阵
	*/
	aeMat4f Transpose() const
	{
		aeMat4f mtrans;

		for(int i = 0;i < 4;i++)
		{
			for(int j = 0;j < 4;++j)
			{
				mtrans.Element(i, j) = Element(j, i);
			}
		}
		return mtrans;
	}

	/**
		\brief 取得指定的元素
		\param row 行
		\param col 列
		\return 指定位置的元素
	*/
	aeFLOAT Element(int row, int col) const
	{
		return C[col].D[row];
	}
	/**
		\brief 取得指定的元素
		\param row 行
		\param col 列
		\return 指定位置的元素
	*/
	aeFLOAT& Element(int row, int col)
	{
		return C[col].D[row];
	}
	/**
		\brief 计算矩阵自乘
		\param l 左侧矩阵
		\param r 右侧矩阵
		\return 积矩阵
	*/
	inline const aeMat4f& operator*=(const aeMat4f& r)
	{
#if 0//_MSC_VER
		if (g_SystemInfo.SSE)
		{
			__asm
			{
				mov    eax, l
					mov    ebx, r

					movups xmm0, [eax + 0]  //保存left矩阵
					movups xmm1, [eax + 16]
					movups xmm2, [eax + 32]
					movups xmm3, [eax + 48]

					//处理第一列
					movups xmm4, [ebx]       //取得right第一列

					movaps xmm5, xmm4
					shufps xmm5, xmm5, 0x00  //r[0, 0]
					mulps  xmm5, xmm0        //l(0) = l(0) * r(0, 0)

					movaps xmm6, xmm4
					shufps xmm6, xmm6, 0x55  //r(1, 0)
					mulps  xmm6, xmm1        //l = l(1) * r(1, 0)

					addps  xmm5, xmm6        //l(0) += l

					movaps xmm6, xmm4
					shufps xmm6, xmm6, 0xAA
					mulps  xmm6, xmm2

					addps  xmm5, xmm6

					movaps xmm6, xmm4
					shufps xmm6, xmm6, 0xFF
					mulps  xmm6, xmm3

					addps  xmm5, xmm6

					movups[eax + 0], xmm5

					//处理第二列
					movups xmm4, [ebx + 16]

					movaps xmm5, xmm4
					shufps xmm5, xmm5, 0x00
					mulps  xmm5, xmm0

					movaps xmm6, xmm4
					shufps xmm6, xmm6, 0x55
					mulps  xmm6, xmm1

					addps  xmm5, xmm6

					movaps xmm6, xmm4
					shufps xmm6, xmm6, 0xAA
					mulps  xmm6, xmm2

					addps  xmm5, xmm6

					movaps xmm6, xmm4
					shufps xmm6, xmm6, 0xFF
					mulps  xmm6, xmm3

					addps  xmm5, xmm6

					movups[eax + 16], xmm5

					//处理第三列
					movups xmm4, [ebx + 32]

					movaps xmm5, xmm4
					shufps xmm5, xmm5, 0x00
					mulps  xmm5, xmm0

					movaps xmm6, xmm4
					shufps xmm6, xmm6, 0x55
					mulps  xmm6, xmm1

					addps  xmm5, xmm6

					movaps xmm6, xmm4
					shufps xmm6, xmm6, 0xAA
					mulps  xmm6, xmm2

					addps  xmm5, xmm6

					movaps xmm6, xmm4
					shufps xmm6, xmm6, 0xFF
					mulps  xmm6, xmm3

					addps  xmm5, xmm6

					movups[eax + 32], xmm5

					//处理第四列
					movups xmm4, [ebx + 48]

					movaps xmm5, xmm4
					shufps xmm5, xmm5, 0x00
					mulps  xmm5, xmm0

					movaps xmm6, xmm4
					shufps xmm6, xmm6, 0x55
					mulps  xmm6, xmm1

					addps  xmm5, xmm6

					movaps xmm6, xmm4
					shufps xmm6, xmm6, 0xAA
					mulps  xmm6, xmm2

					addps  xmm5, xmm6

					movaps xmm6, xmm4
					shufps xmm6, xmm6, 0xFF
					mulps  xmm6, xmm3

					addps  xmm5, xmm6

					movups[eax + 48], xmm5
			};
		}
		else
		{
#endif//MSC_VER
			aeMat4f mat4 = *this;

			//P(i, j) = A(i, 0) * B(0, j) + A(i, 1) * B(1, j) + A(i, 2) * B(2, j) + A(i, 3) * B(3, j);
			C[0] = mat4.C[0] * r.Element(0, 0) + mat4.C[1] * r.Element(1, 0) + mat4.C[2] * r.Element(2, 0) + mat4.C[3] * r.Element(3, 0);
			C[1] = mat4.C[0] * r.Element(0, 1) + mat4.C[1] * r.Element(1, 1) + mat4.C[2] * r.Element(2, 1) + mat4.C[3] * r.Element(3, 1);
			C[2] = mat4.C[0] * r.Element(0, 2) + mat4.C[1] * r.Element(1, 2) + mat4.C[2] * r.Element(2, 2) + mat4.C[3] * r.Element(3, 2);
			C[3] = mat4.C[0] * r.Element(0, 3) + mat4.C[1] * r.Element(1, 3) + mat4.C[2] * r.Element(2, 3) + mat4.C[3] * r.Element(3, 3);

			return *this;
#if 0//_MSC_VER
		}
#endif//MSC_VER
	}
};
//------------------------------------------------------------------------------
/**
	\brief 矩阵乘法
	\param l 左侧矩阵
	\param r 右侧矩阵
	\return 积矩阵
*/
inline aeMat4f operator*(const aeMat4f& l, const aeMat4f& r)
{
#if 0//_MSC_VER
	if(g_SystemInfo.SSE)
	{
		aeFLOAT d[16];

		__asm
		{
			mov    eax,  l
			mov    ebx,  r

			movups xmm0, [eax +  0]  //保存left矩阵
			movups xmm1, [eax + 16]
			movups xmm2, [eax + 32]
			movups xmm3, [eax + 48]

			lea    eax,  d

			//处理第一列
			movups xmm4, [ebx]       //取得right第一列

			movaps xmm5, xmm4
			shufps xmm5, xmm5, 0x00  //r[0, 0]
			mulps  xmm5, xmm0        //l(0) = l(0) * r(0, 0)

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0x55  //r(1, 0)
			mulps  xmm6, xmm1        //l = l(1) * r(1, 0)

			addps  xmm5, xmm6        //l(0) += l

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0xAA
			mulps  xmm6, xmm2

			addps  xmm5, xmm6

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0xFF
			mulps  xmm6, xmm3

			addps  xmm5, xmm6

			movups [eax + 0], xmm5

			//处理第二列
			movups xmm4, [ebx + 16]

			movaps xmm5, xmm4
			shufps xmm5, xmm5, 0x00
			mulps  xmm5, xmm0

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0x55
			mulps  xmm6, xmm1

			addps  xmm5, xmm6

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0xAA
			mulps  xmm6, xmm2

			addps  xmm5, xmm6

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0xFF
			mulps  xmm6, xmm3

			addps  xmm5, xmm6

			movups [eax + 16], xmm5

			//处理第三列
			movups xmm4, [ebx + 32]

			movaps xmm5, xmm4
			shufps xmm5, xmm5, 0x00
			mulps  xmm5, xmm0

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0x55
			mulps  xmm6, xmm1

			addps  xmm5, xmm6

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0xAA
			mulps  xmm6, xmm2

			addps  xmm5, xmm6

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0xFF
			mulps  xmm6, xmm3

			addps  xmm5, xmm6

			movups [eax + 32], xmm5

			//处理第四列
			movups xmm4, [ebx + 48]

			movaps xmm5, xmm4
			shufps xmm5, xmm5, 0x00
			mulps  xmm5, xmm0

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0x55
			mulps  xmm6, xmm1

			addps  xmm5, xmm6

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0xAA
			mulps  xmm6, xmm2

			addps  xmm5, xmm6

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0xFF
			mulps  xmm6, xmm3

			addps  xmm5, xmm6

			movups [eax + 48], xmm5
		};

		return aeMat4f(d);
	}
	else
	{
#endif//MSC_VER
		aeMat4f mat4;
		
		//P(i, j) = A(i, 0) * B(0, j) + A(i, 1) * B(1, j) + A(i, 2) * B(2, j) + A(i, 3) * B(3, j);
		mat4.C[0] = l.C[0] * r.Element(0, 0) + l.C[1] * r.Element(1, 0) + l.C[2] * r.Element(2, 0) + l.C[3] * r.Element(3, 0);
		mat4.C[1] = l.C[0] * r.Element(0, 1) + l.C[1] * r.Element(1, 1) + l.C[2] * r.Element(2, 1) + l.C[3] * r.Element(3, 1);
		mat4.C[2] = l.C[0] * r.Element(0, 2) + l.C[1] * r.Element(1, 2) + l.C[2] * r.Element(2, 2) + l.C[3] * r.Element(3, 2);
		mat4.C[3] = l.C[0] * r.Element(0, 3) + l.C[1] * r.Element(1, 3) + l.C[2] * r.Element(2, 3) + l.C[3] * r.Element(3, 3);

		return mat4;
#if 0//_MSC_VER
	}
#endif//MSC_VER
}
//------------------------------------------------------------------------------
/**
	\brief 方阵乘以向量
	\param m 4阶方阵
	\param v 3维向量
	\return 结果向量
*/
inline aeVec3f operator*(const aeMat4f& m, const aeVec3f& v)
{
#if 0//_MSC_VER
	if(g_SystemInfo.SSE)
	{
		aeFLOAT d[4];
		__asm
		{
			mov    eax,  m
			mov    ebx,  v

			movups xmm0, [eax +  0]
			movups xmm1, [eax + 16]
			movups xmm2, [eax + 32]

			lea    eax,  d

			movss  xmm4, [ebx]
			shufps xmm4, xmm4, 0
			movss  xmm5, [ebx + 4]
			shufps xmm5, xmm5, 0
			movss  xmm6, [ebx + 8]
			shufps xmm6, xmm6, 0

			mulps  xmm4, xmm0
			mulps  xmm5, xmm1
			mulps  xmm6, xmm2

			addps  xmm4, xmm5
			addps  xmm4, xmm6

			movups [eax], xmm4
		};
		return d;
	}
	else
	{
#endif//MSC_VER
		return aeVec3f({m.C[0].X * v.X + m.C[1].X * v.Y + m.C[2].X * v.Z,
		                m.C[0].Y * v.X + m.C[1].Y * v.Y + m.C[2].Y * v.Z,
						m.C[0].Z * v.X + m.C[1].Z * v.Y + m.C[2].Z * v.Z});
#if 0//_MSC_VER
	}
#endif//MSC_VER
}
//------------------------------------------------------------------------------
/**
	\brief 方阵乘以向量
	\param m 4阶方阵
	\param v 4维向量
	\return 结果向量
*/
inline aeVec4f operator*(const aeMat4f& m, const aeVec4f& v)
{
#if 0//_MSC_VER
	if(g_SystemInfo.SSE)
	{
		aeFLOAT d[4];
		__asm
		{
			mov    eax,  m
			mov    ebx,  v

			movups xmm0, [eax +  0]
			movups xmm1, [eax + 16]
			movups xmm2, [eax + 32]
			movups xmm3, [eax + 48]

			lea    eax,  d

			movups xmm4, [ebx]

			movaps xmm5, xmm4
			shufps xmm5, xmm5, 0x00
			mulps  xmm5, xmm0

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0x55
			mulps  xmm6, xmm1

			addps  xmm5, xmm6

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0xaa
			mulps  xmm6, xmm2

			addps  xmm5, xmm6

			movaps xmm6, xmm4
			shufps xmm6, xmm6, 0xff
			mulps  xmm6, xmm3

			addps  xmm5, xmm6

			movups [eax], xmm5
		};
		return aeVec4f(d);
	}
	else
	{
#endif//MSC_VER
		return aeVec4f({m.C[0].X * v.X + m.C[1].X * v.Y + m.C[2].X * v.Z + m.C[3].X * v.W,
		                m.C[0].Y * v.X + m.C[1].Y * v.Y + m.C[2].Y * v.Z + m.C[3].Y * v.W,
						m.C[0].Z * v.X + m.C[1].Z * v.Y + m.C[2].Z * v.Z + m.C[3].Z * v.W,
						m.C[0].W * v.X + m.C[1].W * v.Y + m.C[2].W * v.Z + m.C[3].W * v.W});
#if 0//_MSC_VER
	}
#endif//MSC_VER
}
//------------------------------------------------------------------------------
/**
	\brief 矩阵比较
	\param l 左矩阵
	\param r 右矩阵
	\return 是否相等
*/
inline aeBOOL operator==(const aeMat4f& l, const aeMat4f& r)
{
#if 0//_MSC_VER
	if(g_SystemInfo.SSE)
	{
		aeUINT32 _ret;
		__asm
		{
			mov    eax,  l
			mov    ebx,  r

			movups xmm0, [eax +  0]
			movups xmm1, [eax + 16]
			movups xmm2, [eax + 32]
			movups xmm3, [eax + 48]

			movups xmm4, [ebx +  0]
			movups xmm5, [ebx + 16]
			movups xmm6, [ebx + 32]
			movups xmm7, [ebx + 48]

			cmpps  xmm0, xmm4, 0
			cmpps  xmm1, xmm5, 0
			cmpps  xmm2, xmm6, 0
			cmpps  xmm3, xmm7, 0

			andps  xmm0, xmm1
			andps  xmm2, xmm3

			andps  xmm0, xmm2

			movaps xmm1, xmm0
			shufps xmm1, xmm1, 31h
			andps  xmm0, xmm1
			movaps xmm1, xmm0
			shufps xmm1, xmm1, 2h
			andps  xmm0, xmm1
			movss  _ret, xmm0
		};
		return (0xffffffff == _ret);
	}
	else
	{
#endif//MSC_VER
		return aeVecOperate<aeFLOAT, 16>::Equal(l.get(), r.get());
#if 0//_MSC_VER
	}
#endif//MSC_VER
}
//------------------------------------------------------------------------------
/**
	\brief 矩阵比较
	\param l 左矩阵
	\param r 右矩阵
	\return 是否不相等
*/
inline aeBOOL operator!=(const aeMat4f& l, const aeMat4f& r)
{
#if 0//_MSC_VER
	if(g_SystemInfo.SSE)
	{
		aeUINT32 _ret;
		__asm
		{
			mov    eax,  l
			mov    ebx,  r

			movups xmm0, [eax +  0]
			movups xmm1, [eax + 16]
			movups xmm2, [eax + 32]
			movups xmm3, [eax + 48]

			movups xmm4, [ebx +  0]
			movups xmm5, [ebx + 16]
			movups xmm6, [ebx + 32]
			movups xmm7, [ebx + 48]

			cmpps  xmm0, xmm4, 4
			cmpps  xmm1, xmm5, 4
			cmpps  xmm2, xmm6, 4
			cmpps  xmm3, xmm7, 4

			andps  xmm0, xmm1
			andps  xmm2, xmm3

			andps  xmm0, xmm2

			movaps xmm1, xmm0
			shufps xmm1, xmm1, 31h
			andps  xmm0, xmm1
			movaps xmm1, xmm0
			shufps xmm1, xmm1, 2h
			andps  xmm0, xmm1
			movss  _ret, xmm0
		};
		return (0xffffffff == _ret);
	}
	else
	{
#endif//MSC_VER
		return aeVecOperate<aeFLOAT, 16>::NotEqual(l.get(), r.get());
#if 0//_MSC_VER
	}
#endif//MSC_VER
}
//------------------------------------------------------------------------------
/**
	\brief 构造透视投影矩阵
	\param mat 构造的矩阵
	\param a 横纵比
	\param h 垂直方向视角(弧度)
	\param n 近截面
	\param f 远截面
	\return 矩阵指针
*/
inline aeMat4f* PerspectiveMatrix(aeMat4f* mat, aeFLOAT a, aeFLOAT h, aeFLOAT n, aeFLOAT f)
{
	aeFLOAT tg = tan(h * 0.5f);
	mat->Element(0, 0) =  1.0f / (a * tg);
	mat->Element(0, 1) =  0.0f;
	mat->Element(0, 2) =  0.0f;
	mat->Element(0, 3) =  0.0f;

	mat->Element(1, 0) =  0.0f;
	mat->Element(1, 1) =  1.0f / tg;
	mat->Element(1, 2) =  0.0f;
	mat->Element(1, 3) =  0.0f;

	mat->Element(2, 0) =  0.0f;
	mat->Element(2, 1) =  0.0f;
	if (f < std::numeric_limits<aeFLOAT>::infinity())
	{
		aeFLOAT _1_to_deltaZ = -1.0f / (f - n);
		mat->Element(2, 2) = (f + n) * _1_to_deltaZ;
		mat->Element(2, 3) = 2.0f * f * n * _1_to_deltaZ;
	}
	else
	{
		mat->Element(2, 2) = -1.0f;
		mat->Element(2, 3) = -2.0f * n;
	}

	mat->Element(3, 0) =  0.0f;
	mat->Element(3, 1) =  0.0f;
	mat->Element(3, 2) = -1.0f;
	mat->Element(3, 3) =  0.0f;

	return mat;
}
//------------------------------------------------------------------------------
/**
	\brief 构造透视投影矩阵
	\param mat 构造的矩阵
	\param ut 视线方向上方可视角度正弦值
	\param dt 视线方向下方可视角度正弦值
	\param lt 视线方向左方可视角度正弦值
	\param rt 视线方向右方可视角度正弦值
	\param n 近截面
	\param f 远截面
	\return 矩阵指针
*/
inline aeMat4f* PerspectiveMatrix(aeMat4f* mat, aeFLOAT ut, aeFLOAT dt, aeFLOAT lt, aeFLOAT rt, aeFLOAT n, aeFLOAT f)
{
	mat->Element(0, 0) =  2.0f * n / (lt + rt);
	mat->Element(0, 1) =  0.0f;
	mat->Element(0, 2) = (rt - lt)* mat->Element(0, 0) * 0.5f;
	mat->Element(0, 3) =  0.0f;

	mat->Element(1, 0) =  0.0f;
	mat->Element(1, 1) =  2.0f * n/ (ut + dt);
	mat->Element(1, 2) =  (ut - dt) * mat->Element(1, 1) * 0.5f;
	mat->Element(1, 3) =  0.0f;

	mat->Element(2,0) =  0.0f;
	mat->Element(2,1) =  0.0f;
	if (f < std::numeric_limits<aeFLOAT>::infinity())
	{
		aeFLOAT _1_to_deltaZ = -1.0f / (f - n);
		mat->Element(2, 2) = (f + n) * _1_to_deltaZ;
		mat->Element(2, 3) = 2.0f * f * n * _1_to_deltaZ;
	}
	else
	{
		mat->Element(2, 2) = -1.0f;
		mat->Element(2, 3) = -2.0f * n;
	}

	mat->Element(3, 0) =  0.0f;
	mat->Element(3, 1) =  0.0f;
	mat->Element(3, 2) = -1.0f;
	mat->Element(3, 3) =  0.0f;

	return mat;
}
//------------------------------------------------------------------------------
/**
	\brief 构造平行投影矩阵
	\param mat 构造的矩阵
	\param w 宽度
	\param h 高度
	\param n 近截面
	\param f 远截面
	\return 矩阵指针
*/
inline aeMat4f* OrthoMatrix(aeMat4f* mat, aeFLOAT w, aeFLOAT h, aeFLOAT n, aeFLOAT f)
{
	mat->Element(0,0) = 2.0f / w;
	mat->Element(0,1) = 0.0f;
	mat->Element(0,2) = 0.0f;
	mat->Element(0,3) = 0.0f;

	mat->Element(1,0) = 0.0f;
	mat->Element(1,1) = (aeFLOAT)( 2.0 / h);
	mat->Element(1,2) = 0.0f;
	mat->Element(1,3) = 0.0f;

	mat->Element(2,0) = 0.0f;
	mat->Element(2,1) = 0.0f;
	if (f < std::numeric_limits<aeFLOAT>::infinity())
	{
		aeFLOAT _1_to_deltaZ = -1.0f / (f - n);
		mat->Element(2, 2) = (aeFLOAT)(2.0 * _1_to_deltaZ);
		mat->Element(2, 3) = (f + n) * _1_to_deltaZ;
	}
	else
	{
		mat->Element(2, 2) = 0.0f;
		mat->Element(2, 3) = -1.0f;
	}

	mat->Element(3,0) = 0.0f;
	mat->Element(3,1) = 0.0f;
	mat->Element(3,2) = 0.0f;
	mat->Element(3,3) = 1.0f;

	return mat;
}
//------------------------------------------------------------------------------
/**
	\brief 构造视矩阵
	\param mat 构造的矩阵
	\param dir 视线方向
	\param up  上方向
	\param pos 视点位置
	\return 矩阵指针
*/
inline aeMat4f* ViewMatrix(aeMat4f* mat, aeVec3f const * dir, aeVec3f const * up, aeVec3f const * side, aeVec3f const * pos)
{
	mat->Element(0, 0) = side->X;
	mat->Element(0, 1) = side->Y;
	mat->Element(0, 2) = side->Z;
	mat->Element(0, 3) = 0.0f;

	mat->Element(1, 0) = up->X;
	mat->Element(1, 1) = up->Y;
	mat->Element(1, 2) = up->Z;
	mat->Element(1, 3) = 0.0f;

	mat->Element(2, 0) = -dir->X;
	mat->Element(2, 1) = -dir->Y;
	mat->Element(2, 2) = -dir->Z;
	mat->Element(2, 3) = 0.0f;

	mat->Element(3, 0) = 0.0f;
	mat->Element(3, 1) = 0.0f;
	mat->Element(3, 2) = 0.0f;
	mat->Element(3, 3) = 1.0f;

	mat->Translate(-pos->X, -pos->Y, -pos->Z);

	return mat;
}
//------------------------------------------------------------------------------
/**
	\brief 构造视矩阵
	\param mat 构造的矩阵
	\param dir 视线方向
	\param up  上方向
	\param pos 视点位置
	\return 矩阵指针
*/
inline aeMat4f* ViewMatrix(aeMat4f* mat, aeVec3f dir, aeVec3f up, const aeVec3f& pos)
{
	if(!dir.Normalize())
	{
		return NULL;
	}
	if(!up.Normalize())
	{
		return NULL;
	}
	aeVec3f side = Cross(dir, up);
	if(!side.Normalize())
	{
		return NULL;
	}
	up = Cross(side, dir);

	return ViewMatrix(mat, &dir, &up, &side, &pos);
}
//------------------------------------------------------------------------------
/**
	\brief 构造视矩阵
	\param mat 构造的矩阵
	\param wm  视点的世界变换矩阵
	\return 矩阵指针
*/
inline aeMat4f* ViewMatrix(aeMat4f* mat, aeMat4f const * wm)
{
	aeVec3f dir  = {wm->C[1].X, wm->C[1].Y, wm->C[1].Z};
	aeVec3f up   = {wm->C[2].X, wm->C[2].Y, wm->C[2].Z};
	aeVec3f side = {wm->C[0].X, wm->C[0].Y, wm->C[0].Z};
	aeVec3f pos  = {wm->C[3].X, wm->C[3].Y, wm->C[3].Z};

	return ViewMatrix(mat, &dir, &up, &side, &pos);
}
//------------------------------------------------------------------------------
/**
	\brief 构造视矩阵
	\param mat    构造的矩阵
	\param target 观察目标
	\param up     上方向
	\param pos    观察位置
	\return 矩阵指针
*/
inline aeMat4f* LookAt(aeMat4f* mat, aeVec3f const * target, aeVec3f const * up, aeVec3f const * pos)
{
	aeVec3f dir = *target - *pos;
	if(!dir.Normalize())
	{
		return NULL;
	}
	aeVec3f side = Cross(dir, *up);
	if(!side.Normalize())
	{
		return NULL;
	}
	aeVec3f _up = Cross(side, dir);
	return ViewMatrix(mat, &dir, &_up, &side, pos);
}
//------------------------------------------------------------------------------
_AE_END_
//------------------------------------------------------------------------------
#endif//_AE_MATRIX_H_
