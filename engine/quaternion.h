/*
 * Quaternion.h
 *
 *  Created on: 24/01/2011
 *      Author: marrony.neris
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "vector3.h"

/**
 *
 * (s, x, y, z) = s*1 + x*i + y*j + z*k
 *
 * i^2 = j^2 = k^2 = ijk = -1
 *
 * exemplo
 *
 *   -1 = ijk       =>
 *   -1*k = ijk*k   =>
 *   -k = ijk*(-1)  =>
 *   k = ij
 *
 *     1   i   j   k
 *  1  1   i   j 	 k
 *  i  i  -1   k  -j
 *  j  j  -k  -1   i
 *  k  k   j  -i  -1
 *
 */
class Quaternion {
public:
	union {
		float quat[4];
		struct {
			float s, x, y, z;
		};
	};

	INLINE Quaternion() {
	}

	INLINE Quaternion(float _s, float _x, float _y, float _z) :
		s(_s), x(_x), y(_y), z(_z) {
	}

	INLINE Quaternion(const Vector3& axis, float angle) {
		const float si = std::sin(angle*0.5);
		const float co = std::cos(angle*0.5);

		s = co;
		x = axis.x * si;
		y = axis.y * si;
		z = axis.z * si;
	}

//	INLINE Quaternion(const EulerAngle& angles) {
//		float angle;
//
//		angle = angles.z * 0.5;
//		const float sy = std::sin(angle);
//		const float cy = std::cos(angle);
//
//		angle = angles.y * 0.5;
//		const float sp = std::sin(angle);
//		const float cp = std::cos(angle);
//
//		angle = angles.x * 0.5;
//		const float sr = std::sin(angle);
//		const float cr = std::cos(angle);
//
//		x = sr * cp * cy - cr * sp * sy; // X
//		y = cr * sp * cy + sr * cp * sy; // Y
//		z = cr * cp * sy - sr * sp * cy; // Z
//		s = cr * cp * cy + sr * sp * sy; // W
//	}

	INLINE Quaternion conjugate() const {
		return Quaternion(s, -x, -y, -z);
	}

	/**
	 *      q*
	 * q^1  --
	 *      |q|
	 */
	INLINE Quaternion invert() const {
		float invLength = 1.0 / length();

		float _s =  s * invLength;
		float _x = -x * invLength;
		float _y = -y * invLength;
		float _z = -z * invLength;

		return Quaternion(_s, _x, _y, _z);
	}

	INLINE Quaternion normalize() const {
		float invLength = 1.0 / length();

		float _s = s * invLength;
		float _x = x * invLength;
		float _y = y * invLength;
		float _z = z * invLength;

		return Quaternion(_s, _x, _y, _z);
	}

	INLINE float length() const {
		return std::sqrt(s*s + x*x + y*y + z*z);
	}

	INLINE Quaternion operator-() const {
		return Quaternion(-s, -x, -y, -z);
	}

	INLINE Quaternion& operator+=(const Quaternion& other) {
		s += other.s;
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	INLINE Quaternion& operator*=(const Quaternion& q2) {
		const Quaternion q1(*this);

		s = q1.s*q2.s - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
		x = q1.s*q2.x + q1.x*q2.s + q1.y*q2.z - q1.z*q2.y;
		y = q1.s*q2.y - q1.x*q2.z + q1.y*q2.s + q1.z*q2.x;
		z = q1.s*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.s;

		return *this;
	}

	INLINE Quaternion& operator*=(float k) {
		s *= k;
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	INLINE Quaternion& operator/=(float k) {
		float invK = 1.0 / k;
		s *= invK;
		x *= invK;
		y *= invK;
		z *= invK;
		return *this;
	}

	/**
	 * (s1, x1, y1, z1)*(s2, x2, y2, z2) =>
	 *
	 * (s1 + x1*i + y1*j + z1*k) * (s2 + x2*i + y2*j + z2*k) =>
	 *
	 * s1*s2   +   s1*x2*i +   s1*y2*j +   s1*z2*k +
	 * x1*i*s2 + x1*i*x2*i + x1*i*y2*j + x1*i*z2*k +
	 * y1*j*s2 + y1*j*x2*i + y1*j*y2*j + y1*j*z2*k +
	 * z1*k*s2 + z1*k*x2*i + z1*k*y2*j + z1*k*z2*k           =>
	 *
	 * s1*s2   +   s1*x2*i  +   s1*y2*j  + s1*z2*k    +
	 * x1*s2*i + x1*x2*(-1) + x1*y2*k    + x1*z2*(-j) +
	 * y1*s2*j + y1*x2*(-k) + y1*y2*(-1) + y1*z2*i    +
	 * z1*s2*k + z1*x2*j    + z1*y2*(-i) + z1*z2*(-1)
	 *
	 *
	 * [s1, v1]*[s2, v2]
	 *
	 * [s1*s2 - v1.v2, v1 x v2 + v1*s2 + v2*s1]
	 */
	static INLINE void concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result) {
		float _s = q1.s*q2.s - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
		float _x = q1.s*q2.x + q1.x*q2.s + q1.y*q2.z - q1.z*q2.y;
		float _y = q1.s*q2.y - q1.x*q2.z + q1.y*q2.s + q1.z*q2.x;
		float _z = q1.s*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.s;

		result.x = _x;
		result.y = _y;
		result.z = _z;
		result.s = _s;
	}

	//http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	template<typename Matrix>
	static INLINE Quaternion fromMatrix(const Matrix& m) {
		float tr = m.m00 + m.m11 + m.m22;

		float qs, qx, qy, qz;

		if(tr > 0) {
			float S = std::sqrt(tr + 1.0) * 2; // S=4*qw
			qs = 0.25 * S;
			qx = (m.m21 - m.m12) / S;
			qy = (m.m02 - m.m20) / S;
			qz = (m.m10 - m.m01) / S;
		} else if((m.m00 > m.m11) & (m.m00 > m.m22)) {
			float S = std::sqrt(1.0 + m.m00 - m.m11 - m.m22) * 2; // S=4*qx
			qs = (m.m21 - m.m12) / S;
			qx = 0.25 * S;
			qy = (m.m01 + m.m10) / S;
			qz = (m.m02 + m.m20) / S;
		} else if(m.m11 > m.m22) {
			float S = std::sqrt(1.0 + m.m11 - m.m00 - m.m22) * 2; // S=4*qy
			qs = (m.m02 - m.m20) / S;
			qx = (m.m01 + m.m10) / S;
			qy = 0.25 * S;
			qz = (m.m12 + m.m21) / S;
		} else {
			float S = std::sqrt(1.0 + m.m22 - m.m00 - m.m11) * 2; // S=4*qz
			qs = (m.m10 - m.m01) / S;
			qx = (m.m02 + m.m20) / S;
			qy = (m.m12 + m.m21) / S;
			qz = 0.25 * S;
		}

		return Quaternion(qs, qx, qy, qz);
	}
};

INLINE float dot(const Quaternion& q1, const Quaternion& q2) {
	return q1.s*q2.s + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
}

INLINE Quaternion exp(const Quaternion& q, float exp) {
	if ((1.0 - std::abs(q.s)) > 0.001) {
		float alpha = std::acos(q.s);
		float alpha2 = alpha * exp;
		float mult = std::sin(alpha2) / std::sin(alpha);

		return Quaternion(std::cos(alpha), q.x * mult, q.y * mult, q.z * mult);
	} else {
		return q;
	}
}

/**
 * q0 * (q0^-1 * q1)^t
 */
INLINE Quaternion slerp(const Quaternion& q0, const Quaternion& q2, float t) {
	Quaternion q1 = q2;

	float cosOmega = dot(q0, q1);

	if(cosOmega < 0) {
		q1.s = -q1.s;
		q1.x = -q1.x;
		q1.y = -q1.y;
		q1.z = -q1.z;
		cosOmega = -cosOmega;
	}

	float k0, k1;
	if(cosOmega > (1.0 - 0.001)) {
		k0 = 1 - t;
		k1 = t;
	} else {
		//cos(theta)^2 + sin(theta)^2 = 1
		//sin(theta)^2 = 1 - cos(theta)^2
		//sin(theta) = sqrt(1 - cos(theta)^2)

		float sinOmega = std::sqrt(1 - cosOmega * cosOmega);
		float omega = std::atan2(sinOmega, cosOmega);
		float sinOmegaInv = 1.0 / sinOmega;

		k0 = std::sin((1.0 - t) * omega) * sinOmegaInv;
		k1 = std::sin(t * omega) * sinOmegaInv;
	}

	return Quaternion(
			q0.s * k0 + q1.s * k1,
			q0.x * k0 + q1.x * k1,
			q0.y * k0 + q1.y * k1,
			q0.z * k0 + q1.z * k1
	);
}

INLINE Quaternion operator+(const Quaternion& q1, const Quaternion& q2) {
	float _s = q1.s + q2.s;
	float _x = q1.x + q2.x;
	float _y = q1.y + q2.y;
	float _z = q1.z + q2.z;

	return Quaternion(_s, _x, _y, _z);
}

INLINE Quaternion operator*(const Quaternion& q1, float k) {
	float _s = q1.s * k;
	float _x = q1.x * k;
	float _y = q1.y * k;
	float _z = q1.z * k;

	return Quaternion(_s, _x, _y, _z);
}

INLINE Quaternion operator*(float k, const Quaternion& q1) {
	float _s = q1.s * k;
	float _x = q1.x * k;
	float _y = q1.y * k;
	float _z = q1.z * k;

	return Quaternion(_s, _x, _y, _z);
}

INLINE Quaternion operator/(const Quaternion& q1, float k) {
	float invK = 1.0 / k;
	float _s = q1.s * invK;
	float _x = q1.x * invK;
	float _y = q1.y * invK;
	float _z = q1.z * invK;

	return Quaternion(_s, _x, _y, _z);
}

INLINE Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
	float _s = q1.s*q2.s - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
	float _x = q1.s*q2.x + q1.x*q2.s + q1.y*q2.z - q1.z*q2.y;
	float _y = q1.s*q2.y - q1.x*q2.z + q1.y*q2.s + q1.z*q2.x;
	float _z = q1.s*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.s;

	return Quaternion(_s, _x, _y, _z);
}

INLINE Vector3 operator*(const Quaternion& q1, const Vector3& v) {
	Quaternion q0(0, v.x, v.y, v.z);

	Quaternion vt = q1 * q0 * q1.invert();

	return Vector3::make(vt.x, vt.y, vt.z);
}

#endif /* QUATERNION_H_ */
