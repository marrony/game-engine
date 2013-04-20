/*
 * Matrix3.h
 *
 *  Created on: 30/11/2010
 *      Author: marrony.neris
 */

#ifndef MATRIX3_H_
#define MATRIX3_H_

#include <memory.h>
#include <cmath>

#include "vector3.h"
#include "math_util.h"

namespace matrix {
	/**
	 * m00, m01
	 * m10, m11
	 */
	INLINE float determinant2x2(float m00, float m01, float m10, float m11) {
		return m00*m11 - m01*m10;
	}

	/**
	 * m00, m01, m02
	 * m10, m11, m12
	 * m20, m21, m22
	 */
	INLINE float determinant3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) {
		float c00 = +determinant2x2(m11, m12, m21, m22);
		float c01 = -determinant2x2(m10, m12, m20, m22);
		float c02 = +determinant2x2(m10, m11, m20, m21);

		return m00*c00 + m01*c01 + m02*c02;
	}
}

struct Matrix3 {
	union {
		float matrix[9];

		struct {
			float m00, m10, m20;
			float m01, m11, m21;
			float m02, m12, m22;
		};
	};

	INLINE Matrix3() {
	}

	INLINE void loadIdentity() {
		static float identity[9] = {
				1, 0, 0,
				0, 1, 0,
				0, 0, 1,
		};

		memcpy(this->matrix, identity, sizeof(identity));
	}

	/**
	 * m00, m01, m02
	 * m10, m11, m12
	 * m20, m21, m22
	 */
	INLINE Matrix3 inverse() const {
		float c00 = +matrix::determinant2x2(m11, m12, m21, m22);
		float c01 = -matrix::determinant2x2(m10, m12, m20, m22);
		float c02 = +matrix::determinant2x2(m10, m11, m20, m21);

		float determinant = m00*c00 + m01*c01 + m02*c02;

		if(std::abs(determinant) < 0.001) {
			//throw engine::Exception("");
		}

		float c10 = -matrix::determinant2x2(m01, m02, m21, m22);
		float c11 = +matrix::determinant2x2(m00, m02, m20, m22);
		float c12 = -matrix::determinant2x2(m00, m01, m20, m21);

		float c20 = +matrix::determinant2x2(m01, m02, m11, m12);
		float c21 = -matrix::determinant2x2(m00, m02, m10, m12);
		float c22 = +matrix::determinant2x2(m00, m01, m10, m11);

		float invDet = 1.0f / determinant;

		Matrix3 inv;

		inv.m00 = c00*invDet;  inv.m01 = c10*invDet;  inv.m02 = c20*invDet;
		inv.m10 = c01*invDet;  inv.m11 = c11*invDet;  inv.m12 = c21*invDet;
		inv.m20 = c02*invDet;  inv.m21 = c12*invDet;  inv.m22 = c22*invDet;

		return inv;
	}

	INLINE Matrix3 transpose() const {
		Matrix3 transp;

		transp.m00 = m00;  transp.m01 = m10;  transp.m02 = m20;
		transp.m10 = m01;  transp.m11 = m11;  transp.m12 = m21;
		transp.m20 = m02;  transp.m21 = m12;  transp.m22 = m22;

		return transp;
	}

	INLINE Matrix3 orthoNormalize() const {
		Matrix3 ret;

		Vector3 col0 = Vector3(m00, m10, m20).normalize();
		Vector3 col1 = Vector3(m01, m11, m21).normalize();
		Vector3 col2 = Vector3(m02, m12, m22).normalize();

		ret.m00 = col0.x;
		ret.m10 = col0.y;
		ret.m20 = col0.z;

		ret.m01 = col1.x;
		ret.m11 = col1.y;
		ret.m21 = col1.z;

		ret.m02 = col2.x;
		ret.m12 = col2.y;
		ret.m22 = col2.z;

		return ret;
	}
};

INLINE Matrix3 operator/(const Matrix3& mat, float k) {
	Matrix3 div;

	float invK = 1.0 / k;

	for(int i = 0; i < 9; i++) {
		div.matrix[i] = mat.matrix[i] * invK;
	}

	return div;
}

INLINE Vector3 operator*(const Matrix3& matrix, const Vector3& vector) {
	Vector3 result;

	result.x = matrix.m00*vector.x + matrix.m01*vector.y + matrix.m02*vector.z;
	result.y = matrix.m10*vector.x + matrix.m11*vector.y + matrix.m12*vector.z;
	result.z = matrix.m20*vector.x + matrix.m21*vector.y + matrix.m22*vector.z;

	return result;
}

INLINE Matrix3 operator*(const Matrix3& a, const Matrix3& b) {
	Matrix3 mat;

	mat.m00 = a.m00*b.m00 + a.m01*b.m10 + a.m02*b.m20;
	mat.m01 = a.m00*b.m01 + a.m01*b.m11 + a.m02*b.m21;
	mat.m02 = a.m00*b.m02 + a.m01*b.m12 + a.m02*b.m22;

	mat.m10 = a.m10*b.m00 + a.m11*b.m10 + a.m12*b.m20;
	mat.m11 = a.m10*b.m01 + a.m11*b.m11 + a.m12*b.m21;
	mat.m12 = a.m10*b.m02 + a.m11*b.m12 + a.m12*b.m22;

	mat.m20 = a.m20*b.m00 + a.m21*b.m10 + a.m22*b.m20;
	mat.m21 = a.m20*b.m01 + a.m21*b.m11 + a.m22*b.m21;
	mat.m22 = a.m20*b.m02 + a.m21*b.m12 + a.m22*b.m22;

	return mat;
}

#endif /* MATRIX3_H_ */
