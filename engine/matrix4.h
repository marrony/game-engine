/*
 * Matrix4.h
 *
 *  Created on: 30/11/2010
 *      Author: marrony.neris
 */

#ifndef MATRIX4_H_
#define MATRIX4_H_

#include "math_util.h"
#include "vector4.h"
#include "matrix3.h"
#include "quaternion.h"

#include <memory.h>
#include <cmath>

/*
	struct mat4_row_major {
	  vec4 row0;
	  vec4 row1;
	  vec4 row2;
	  vec4 row3;
	};

	This implements o = M*v

	o.x = dot(M.row0, v);
	o.y = dot(M.row1, v);
	o.z = dot(M.row2, v);
	o.w = dot(M.row3, v);

	struct mat4_col_major {
	  vec4 col0;
	  vec4 col1;
	  vec4 col2;
	  vec4 col3;
	};

	M*v expands to...

	o = M.col0 * v.x;
	o += M.col1 * v.y;
	o += M.col2 * v.z;
	o += M.col3 * v.w;

*/

#define MATRIX4_IDENTITY {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1}

/**
 * m00, m01, m02, m03
 * m10, m11, m12, m13
 * m20, m21, m22, m23
 * m30, m31, m32, m33
 */
struct Matrix4 {
	union {
		float matrix[16];

		struct {
			float m00, m10, m20, m30; //first column
			float m01, m11, m21, m31; //second column
			float m02, m12, m22, m32; //third column
			float m03, m13, m23, m33; //forth column
		};

#if defined(__SSE__) && !defined(ANDROID)
		__m128 columns[4];
#endif
	};

	INLINE Matrix4& operator=(const Matrix4& mat) {
		for(int i = 0; i < 16; i++) {
			matrix[i] = mat.matrix[i];
		}
		return *this;
	}

	INLINE bool operator==(const Matrix4& mat) const {
		for(int i = 0; i < 16; i++) {
			if(matrix[i] != mat.matrix[i])
				return false;
		}
		return true;
	}

	INLINE void loadIdentity() {
		static float identity[16] = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1,
		};

		memcpy(this->matrix, identity, sizeof(identity));
	}

	INLINE float at(int row, int column) const {
		return matrix[column*4 + row];
	}

	INLINE float& at(int row, int column) {
		return matrix[column*4 + row];
	}

	/**
	 * m00, m01, m02, m03
	 * m10, m11, m12, m13
	 * m20, m21, m22, m23
	 * m30, m31, m32, m33
	 */
	INLINE Vector4 row(int index) const {
		return Vector4(at(index, 0), at(index, 1), at(index, 2), at(index, 3));
	}

	INLINE void row(int index, const Vector4& r) {
		at(index, 0) = r.x;
		at(index, 1) = r.y;
		at(index, 2) = r.z;
		at(index, 3) = r.w;
	}

	INLINE Vector4 column(int index) const {
		return Vector4(at(0, index), at(1, index), at(2, index), at(3, index));
	}

	INLINE void column(int index, const Vector4& c) {
		at(0, index) = c.x;
		at(1, index) = c.y;
		at(2, index) = c.z;
		at(3, index) = c.w;
	}

	INLINE Matrix4 inverse() const {
		float c30 = -matrix::determinant3x3(m01, m02, m03, m11, m12, m13, m21, m22, m23);
		float c31 = +matrix::determinant3x3(m00, m02, m03, m10, m12, m13, m20, m22, m23);
		float c32 = -matrix::determinant3x3(m00, m01, m03, m10, m11, m13, m20, m21, m23);
		float c33 = +matrix::determinant3x3(m00, m01, m02, m10, m11, m12, m20, m21, m22);

		float determinant = m30*c30 + m31*c31 + m32*c32 + m33*c33;

		if(std::abs(determinant) <= 0.001) {
			//throw engine::Exception("");
		}

		float c20 = +matrix::determinant3x3(m01, m02, m03, m11, m12, m13, m31, m32, m33);
		float c21 = -matrix::determinant3x3(m00, m02, m03, m10, m12, m13, m30, m32, m33);
		float c22 = +matrix::determinant3x3(m00, m01, m03, m10, m11, m13, m30, m31, m33);
		float c23 = -matrix::determinant3x3(m00, m01, m02, m10, m11, m12, m30, m31, m32);

		float c10 = -matrix::determinant3x3(m01, m02, m03, m21, m22, m23, m31, m32, m33);
		float c11 = +matrix::determinant3x3(m00, m02, m03, m20, m22, m23, m30, m32, m33);
		float c12 = -matrix::determinant3x3(m00, m01, m03, m20, m21, m23, m30, m31, m33);
		float c13 = +matrix::determinant3x3(m00, m01, m02, m20, m21, m22, m30, m31, m32);

		float c00 = +matrix::determinant3x3(m11, m12, m13, m21, m22, m23, m31, m32, m33);
		float c01 = -matrix::determinant3x3(m10, m12, m13, m20, m22, m23, m30, m32, m33);
		float c02 = +matrix::determinant3x3(m10, m11, m13, m20, m21, m23, m30, m31, m33);
		float c03 = -matrix::determinant3x3(m10, m11, m12, m20, m21, m22, m30, m31, m32);

		float invDet = 1.0f / determinant;

		Matrix4 inv;

		inv.m00 = c00*invDet;  inv.m01 = c10*invDet;  inv.m02 = c20*invDet;  inv.m03 = c30*invDet;
		inv.m10 = c01*invDet;  inv.m11 = c11*invDet;  inv.m12 = c21*invDet;  inv.m13 = c31*invDet;
		inv.m20 = c02*invDet;  inv.m21 = c12*invDet;  inv.m22 = c22*invDet;  inv.m23 = c32*invDet;
		inv.m30 = c03*invDet;  inv.m31 = c13*invDet;  inv.m32 = c23*invDet;  inv.m33 = c33*invDet;

		return inv;
	}

	INLINE Matrix4 transpose() const {
		Matrix4 transp;

		transp.m00 = m00;  transp.m01 = m10;  transp.m02 = m20;  transp.m03 = m30;
		transp.m10 = m01;  transp.m11 = m11;  transp.m12 = m21;  transp.m13 = m31;
		transp.m20 = m02;  transp.m21 = m12;  transp.m22 = m22;  transp.m23 = m32;
		transp.m30 = m03;  transp.m31 = m13;  transp.m32 = m23;  transp.m33 = m33;

		return transp;
	}

	/**
	 * m00, m01, m02, m03
	 * m10, m11, m12, m13
	 * m20, m21, m22, m23
	 * m30, m31, m32, m33
	 */
	INLINE Matrix3 upperLeft() const {
		Matrix3 mat3;

		mat3.m00 = m00;  mat3.m01 = m01;  mat3.m02 = m02;
		mat3.m10 = m10;  mat3.m11 = m11;  mat3.m12 = m12;
		mat3.m20 = m20;  mat3.m21 = m21;  mat3.m22 = m22;

		return mat3;
	}

	INLINE Matrix3 normalMatrix() const {
		return upperLeft().inverse().transpose();
	}

	//http://www.gamedev.net/topic/441695-transform-matrix-decomposition/
	INLINE void decompose(Quaternion& rotation, Vector3& translation, Vector3& scale) {
		translation.x = m03;
		translation.y = m13;
		translation.z = m23;

		scale.x = std::sqrt(m00*m00 + m01*m01 + m02*m02);
		scale.y = std::sqrt(m10*m10 + m11*m11 + m12*m12);
		scale.z = std::sqrt(m20*m20 + m21*m21 + m22*m22);

		Matrix3 rot;

		rot.m00 = m00/scale.x;   rot.m01 = m01/scale.x;   rot.m02 = m02/scale.x;
		rot.m10 = m10/scale.y;   rot.m11 = m11/scale.y;   rot.m12 = m12/scale.y;
		rot.m20 = m20/scale.z;   rot.m21 = m21/scale.z;   rot.m22 = m22/scale.z;

		rotation = Quaternion::fromMatrix(rot);
	}

	static Matrix4 rotationMatrix(const Quaternion& orientation) {
		Matrix4 mat;

		const float xx = 2.0 * orientation.x * orientation.x;
		const float yy = 2.0 * orientation.y * orientation.y;
		const float zz = 2.0 * orientation.z * orientation.z;
		const float xy = 2.0 * orientation.x * orientation.y;
		const float xz = 2.0 * orientation.x * orientation.z;
		const float yz = 2.0 * orientation.y * orientation.z;
		const float sx = 2.0 * orientation.s * orientation.x;
		const float sy = 2.0 * orientation.s * orientation.y;
		const float sz = 2.0 * orientation.s * orientation.z;

		mat.m00 = 1 - yy - zz;  mat.m01 =     xy - sz;  mat.m02 =     xz + sy;  mat.m03 = 0;
		mat.m10 =     xy + sz;  mat.m11 = 1 - xx - zz;  mat.m12 =     yz - sx;  mat.m13 = 0;
		mat.m20 =     xz - sy;  mat.m21 =     yz + sx;  mat.m22 = 1 - xx - yy;  mat.m23 = 0;
		mat.m30 = 0;            mat.m31 = 0;            mat.m32 = 0;            mat.m33 = 1;

		return mat;
	}

//	static Matrix4 rotationMatrix(const EulerAngle& axis) {
//		Matrix4 mat;
//
//		float sr, sp, sy, cr, cp, cy;
//
//		sy = std::sin(axis.z);
//		cy = std::cos(axis.z);
//
//		sp = std::sin(axis.y);
//		cp = std::cos(axis.y);
//
//		sr = std::sin(axis.x);
//		cr = std::cos(axis.x);
//
//		// matrix = (Z * Y) * X
//		 mat.m00 = cp * cy; mat.m01 = sr * sp * cy + cr * -sy; mat.m02 = cr * sp * cy + -sr * -sy; mat.m03 = 0;
//		 mat.m10 = cp * sy; mat.m11 = sr * sp * sy + cr * cy;  mat.m12 = cr * sp * sy + -sr * cy;  mat.m13 = 0;
//		 mat.m20 = -sp;     mat.m21 = sr * cp;                 mat.m22 = cr * cp;                  mat.m23 = 0;
//		 mat.m30 = 0;       mat.m31 = 0;                       mat.m32 = 0;                        mat.m33 = 1;
//
//		return mat;
//	}

	static INLINE Matrix4 translateMatrix(const Vector3& t) {
		Matrix4 mat = MATRIX4_IDENTITY;

		mat.m03 = t.x;
		mat.m13 = t.y;
		mat.m23 = t.z;

		return mat;
	}

	static INLINE Matrix4 scaleMatrix(const Vector3& s) {
		Matrix4 mat = MATRIX4_IDENTITY;

		mat.m00 = s.x;
		mat.m11 = s.y;
		mat.m22 = s.z;

		return mat;
	}

	/**
	 * m00, m01, m02, m03
	 * m10, m11, m12, m13
	 * m20, m21, m22, m23
	 * m30, m31, m32, m33
	 */
	static INLINE Matrix4 transformationMatrix(const Quaternion& orientation, const Vector3& translation, const Vector3& scale = Vector3(1, 1, 1)) {
		Matrix4 mat = rotationMatrix(orientation);

		mat.m00 *= scale.x;  mat.m01 *= scale.x;  mat.m02 *= scale.x;  mat.m03 = translation.x;
		mat.m10 *= scale.y;  mat.m11 *= scale.y;  mat.m12 *= scale.y;  mat.m13 = translation.y;
		mat.m20 *= scale.z;  mat.m21 *= scale.z;  mat.m22 *= scale.z;  mat.m23 = translation.z;

		return mat;
	}

//	static INLINE Matrix4 transformationMatrix(const EulerAngle& axis, const Vector3& translation, const Vector3& scale = Vector3(1, 1, 1)) {
//		Matrix4 mat = rotationMatrix(axis);
//
//		mat.m00 *= scale.x;  mat.m01 *= scale.x;  mat.m02 *= scale.x;  mat.m03 = translation.x;
//		mat.m10 *= scale.y;  mat.m11 *= scale.y;  mat.m12 *= scale.y;  mat.m13 = translation.y;
//		mat.m20 *= scale.z;  mat.m21 *= scale.z;  mat.m22 *= scale.z;  mat.m23 = translation.z;
//
//		return mat;
//	}

	static INLINE Matrix4 orthoMatrix(float left, float right, float bottom, float top, float znear, float zfar) {
		float difx = right-left;
		float dify = top-bottom;
		float difz = zfar-znear;

		float tx = -(right + left) / difx;
		float ty = -(top + bottom) / dify;
		float tz = -(zfar + znear) / difz;

		Matrix4 matrix;

		matrix.m00 = 2.0 / difx;  matrix.m01 = 0;           matrix.m02 = 0;           matrix.m03 = tx;
		matrix.m10 = 0;           matrix.m11 = 2.0 / dify;  matrix.m12 = 0;           matrix.m13 = ty;
		matrix.m20 = 0;           matrix.m21 = 0;           matrix.m22 = -2.0 / difz; matrix.m23 = tz;
		matrix.m30 = 0;           matrix.m31 = 0;           matrix.m32 = 0;           matrix.m33 = 1;

		return matrix;
	}

	static INLINE Matrix4 orthoMatrix(float left, float right, float bottom, float top) {
		return orthoMatrix(left, right, bottom, top, -1, +1);
	}

	static INLINE Matrix4 perspectiveMatrix(float left, float right, float bottom, float top, float znear, float zfar) {
		const float difx = right - left;
		const float dify = top - bottom;
		const float difz = zfar - znear;
		const float znear2 = 2.0 * znear;

		const float a = (right + left) / difx;
		const float b = (top + bottom) / dify;
		const float c = (zfar + znear) / difz;
		const float d = (-2.0 * zfar * znear) / difz;

		Matrix4 matrix;

		matrix.m00 = znear2 / difx;  matrix.m01 = 0;              matrix.m02 = a;   matrix.m03 = 0;
		matrix.m10 = 0;              matrix.m11 = znear2 / dify;  matrix.m12 = b;   matrix.m13 = 0;
		matrix.m20 = 0;              matrix.m21 = 0;              matrix.m22 = c;   matrix.m23 = d;
		matrix.m30 = 0;              matrix.m31 = 0;              matrix.m32 = -1;  matrix.m33 = 0;

		return matrix;
	}

	static INLINE Matrix4 perspectiveMatrix(float fov, float aspect, float znear, float zfar) {
		float f = 1.0 / std::tan(deg2rad(fov / 2.0));

		float multZ = znear * zfar;
		float sumZ = znear + zfar;
		float diffZ = znear - zfar;

		Matrix4 matrix;

		matrix.m00 = f / aspect;  matrix.m01 = 0;  matrix.m02 = 0;             matrix.m03 = 0;
		matrix.m10 = 0;           matrix.m11 = f;  matrix.m12 = 0;             matrix.m13 = 0;
		matrix.m20 = 0;           matrix.m21 = 0;  matrix.m22 = sumZ / diffZ;  matrix.m23 = 2*multZ/diffZ;
		matrix.m30 = 0;           matrix.m31 = 0;  matrix.m32 = -1;            matrix.m33 = 0;

		return matrix;
	}

	static INLINE Matrix4 lookAtMatrix(const Vector3& eye, const Vector3& direction) {
		Matrix4 matrix;

		Vector3 s, u, f;
		Vector3 up(0, 1, 0);

		f = direction.normalize();
		if(1.0 - std::abs(f.y) <= 0.005) {
			s.x = 1.0;
			s.y = 0.0;
			s.z = 0.0;
		} else {
			s = vector::cross(f, up);
		}

		u = vector::cross(s, f);

		f = -f;

		float x = vector::dot(s, -eye);
		float y = vector::dot(u, -eye);
		float z = vector::dot(f, -eye);

		matrix.m00 = s.x;  matrix.m01 = s.y;  matrix.m02 = s.z;  matrix.m03 = x;
		matrix.m10 = u.x;  matrix.m11 = u.y;  matrix.m12 = u.z;  matrix.m13 = y;
		matrix.m20 = f.x;  matrix.m21 = f.y;  matrix.m22 = f.z;  matrix.m23 = z;
		matrix.m30 = 0;    matrix.m31 = 0;    matrix.m32 = 0;    matrix.m33 = 1;

		return matrix;
	}
};

INLINE Matrix4 operator/(const Matrix4& mat, float k) {
	Matrix4 div;

	float invK = 1.0 / k;

	for(int i = 0; i < 16; i++) {
		div.matrix[i] = mat.matrix[i] * invK;
	}

	return div;
}

/**
 * m00, m01, m02, m03
 * m10, m11, m12, m13
 * m20, m21, m22, m23
 * m30, m31, m32, m33
 */
INLINE Matrix4 operator*(const Matrix4& a, const Matrix4& b) {
	Matrix4 mat;

#if defined(__SSE__) && !defined(ANDROID)
	mat.columns[0] = _mm_mul_ps(a.columns[0], _mm_set1_ps(b.m00));
	mat.columns[0] = _mm_add_ps(mat.columns[0], _mm_mul_ps(a.columns[1], _mm_set1_ps(b.m10)));
	mat.columns[0] = _mm_add_ps(mat.columns[0], _mm_mul_ps(a.columns[2], _mm_set1_ps(b.m20)));
	mat.columns[0] = _mm_add_ps(mat.columns[0], _mm_mul_ps(a.columns[3], _mm_set1_ps(b.m30)));

	mat.columns[1] = _mm_mul_ps(a.columns[0], _mm_set1_ps(b.m01));
	mat.columns[1] = _mm_add_ps(mat.columns[1], _mm_mul_ps(a.columns[1], _mm_set1_ps(b.m11)));
	mat.columns[1] = _mm_add_ps(mat.columns[1], _mm_mul_ps(a.columns[2], _mm_set1_ps(b.m21)));
	mat.columns[1] = _mm_add_ps(mat.columns[1], _mm_mul_ps(a.columns[3], _mm_set1_ps(b.m31)));

	mat.columns[2] = _mm_mul_ps(a.columns[0], _mm_set1_ps(b.m02));
	mat.columns[2] = _mm_add_ps(mat.columns[2], _mm_mul_ps(a.columns[1], _mm_set1_ps(b.m12)));
	mat.columns[2] = _mm_add_ps(mat.columns[2], _mm_mul_ps(a.columns[2], _mm_set1_ps(b.m22)));
	mat.columns[2] = _mm_add_ps(mat.columns[2], _mm_mul_ps(a.columns[3], _mm_set1_ps(b.m32)));

	mat.columns[3] = _mm_mul_ps(a.columns[0], _mm_set1_ps(b.m03));
	mat.columns[3] = _mm_add_ps(mat.columns[3], _mm_mul_ps(a.columns[1], _mm_set1_ps(b.m13)));
	mat.columns[3] = _mm_add_ps(mat.columns[3], _mm_mul_ps(a.columns[2], _mm_set1_ps(b.m23)));
	mat.columns[3] = _mm_add_ps(mat.columns[3], _mm_mul_ps(a.columns[3], _mm_set1_ps(b.m33)));
#else
	// col0 =  col0          col1          col2          col3
	mat.m00 = a.m00*b.m00 + a.m01*b.m10 + a.m02*b.m20 + a.m03*b.m30;
	mat.m10 = a.m10*b.m00 + a.m11*b.m10 + a.m12*b.m20 + a.m13*b.m30;
	mat.m20 = a.m20*b.m00 + a.m21*b.m10 + a.m22*b.m20 + a.m23*b.m30;
	mat.m30 = a.m30*b.m00 + a.m31*b.m10 + a.m32*b.m20 + a.m33*b.m30;

	// col1 =  col0          col1          col2          col3
	mat.m01 = a.m00*b.m01 + a.m01*b.m11 + a.m02*b.m21 + a.m03*b.m31;
	mat.m11 = a.m10*b.m01 + a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31;
	mat.m21 = a.m20*b.m01 + a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31;
	mat.m31 = a.m30*b.m01 + a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31;

	// col2 =  col0          col1          col2          col3
	mat.m02 = a.m00*b.m02 + a.m01*b.m12 + a.m02*b.m22 + a.m03*b.m32;
	mat.m12 = a.m10*b.m02 + a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32;
	mat.m22 = a.m20*b.m02 + a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32;
	mat.m32 = a.m30*b.m02 + a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32;

	// col3 =  col0          col1          col2          col3
	mat.m03 = a.m00*b.m03 + a.m01*b.m13 + a.m02*b.m23 + a.m03*b.m33;
	mat.m13 = a.m10*b.m03 + a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33;
	mat.m23 = a.m20*b.m03 + a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33;
	mat.m33 = a.m30*b.m03 + a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33;
#endif

	return mat;
}

INLINE Vector3 operator*(const Matrix4& m, const Vector3& v) {
	Vector3 result;

	result.x = m.m00*v.x + m.m01*v.y + m.m02*v.z + m.m03;
	result.y = m.m10*v.x + m.m11*v.y + m.m12*v.z + m.m13;
	result.z = m.m20*v.x + m.m21*v.y + m.m22*v.z + m.m23;

	return result;
}

INLINE Vector4 operator*(const Matrix4& m, const Vector4& v) {
	Vector4 result;

	result.x = m.m00*v.x + m.m01*v.y + m.m02*v.z + m.m03*v.w;
	result.y = m.m10*v.x + m.m11*v.y + m.m12*v.z + m.m13*v.w;
	result.z = m.m20*v.x + m.m21*v.y + m.m22*v.z + m.m23*v.w;
	result.w = m.m30*v.x + m.m31*v.y + m.m32*v.z + m.m33*v.w;

	return result;
}

INLINE Vector3 operator|(const Matrix4& m, const Vector3& v) {
	Vector4 result = m * Vector4(v.x, v.y, v.z, 1);

	float inv = 1.0 / result.w;

	return Vector3(result.x * inv, result.y * inv, result.z * inv);
}

#endif /* MATRIX4_H_ */
