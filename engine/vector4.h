/*
 * Vector4.h
 *
 *  Created on: 01/12/2010
 *      Author: marrony
 */

#ifndef VECTOR4_H_
#define VECTOR4_H_

#include "math_util.h"

#include <cmath>
#include <algorithm>
#include <limits>

struct Vector4 {
	union {
		float vector[4];

		struct {
			float x, y, z, w;
		};

		struct {
			float r, g, b, a;
		};
	};

	INLINE Vector4() { }

	INLINE Vector4(const float vector[4]) :
		x(vector[0]), y(vector[1]), z(vector[2]), w(vector[3]) { }

	INLINE Vector4(float _x, float _y, float _z, float _w) :
		x(_x), y(_y), z(_z), w(_w) { }

	INLINE bool operator==(const Vector4& other) const {
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	INLINE bool operator!=(const Vector4& other) const {
		return x != other.x && y != other.y && z != other.z && w != other.w;
	}

	INLINE float length() const {
		return std::sqrt(x*x + y*y + z*z + w*w);
	}

	INLINE Vector4 normalize() const {
		float lenght = 1.0 / length();

		return Vector4(x*lenght, y*lenght, z*lenght, w*lenght);
	}

	INLINE Vector4 operator-() const {
		return Vector4(-x, -y, -z, -w);
	}

	INLINE Vector4& operator+=(const Vector4& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	INLINE Vector4& operator-=(const Vector4& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	INLINE Vector4& operator*=(const Vector4& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	INLINE Vector4& operator/=(const Vector4& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	INLINE Vector4& operator+=(float k) {
		x += k;
		y += k;
		z += k;
		w += k;
		return *this;
	}

	INLINE Vector4& operator-=(float k) {
		x -= k;
		y -= k;
		z -= k;
		w -= k;
		return *this;
	}

	INLINE Vector4& operator*=(float k) {
		x *= k;
		y *= k;
		z *= k;
		w *= k;
		return *this;
	}

	INLINE Vector4& operator/=(float k) {
		x /= k;
		y /= k;
		z /= k;
		w /= k;
		return *this;
	}

	static INLINE Vector4 min() {
		float m = -std::numeric_limits<float>::max();
		return Vector4(m, m, m, m);
	}

	static INLINE Vector4 max() {
		float m = +std::numeric_limits<float>::max();
		return Vector4(m, m, m, m);
	}
};

namespace vector {
	INLINE float dot(const Vector4& v0, const Vector4& v1) {
		return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z + v0.w*v1.w;
	}

	INLINE Vector4 max(const Vector4& v0, const Vector4& v1) {
		float x = std::max(v0.x, v1.x);
		float y = std::max(v0.y, v1.y);
		float z = std::max(v0.z, v1.z);
		float w = std::max(v0.w, v1.w);

		return Vector4(x, y, z, w);
	}

	INLINE Vector4 min(const Vector4& v0, const Vector4& v1) {
		float x = std::min(v0.x, v1.x);
		float y = std::min(v0.y, v1.y);
		float z = std::min(v0.z, v1.z);
		float w = std::min(v0.w, v1.w);

		return Vector4(x, y, z, w);
	}

	INLINE Vector4 abs(const Vector4& v) {
		float x = std::abs(v.x);
		float y = std::abs(v.y);
		float z = std::abs(v.z);
		float w = std::abs(v.w);

		return Vector4(x, y, z, w);
	}
}

INLINE bool operator>(const Vector4& v0, const Vector4& v1) {
	return v0.x > v1.x && v0.y > v1.y && v0.z > v1.z && v0.w > v1.w;
}

INLINE bool operator<(const Vector4& v0, const Vector4& v1) {
	return v0.x < v1.x && v0.y < v1.y && v0.z < v1.z && v0.w < v1.w;
}

INLINE bool operator>=(const Vector4& v0, const Vector4& v1) {
	return v0.x >= v1.x && v0.y >= v1.y && v0.z >= v1.z && v0.w >= v1.w;
}

INLINE bool operator<=(const Vector4& v0, const Vector4& v1) {
	return v0.x <= v1.x && v0.y <= v1.y && v0.z <= v1.z && v0.w <= v1.w;
}

INLINE Vector4 operator+(const Vector4& v, float k) {
	return Vector4(v.x+k, v.y+k, v.z+k, v.w+k);
}

INLINE Vector4 operator-(const Vector4& v, float k) {
	return Vector4(v.x-k, v.y-k, v.z-k, v.w-k);
}

INLINE Vector4 operator+(float k, const Vector4& v) {
	return Vector4(k+v.x, k+v.y, k+v.z, k+v.w);
}

INLINE Vector4 operator-(float k, const Vector4& v) {
	return Vector4(k-v.x, k-v.y, k-v.z, k-v.w);
}

INLINE Vector4 operator*(const Vector4& v0, const Vector4& v1) {
	return Vector4(v0.x*v1.x, v0.y*v1.y, v0.z*v1.z, v0.w*v1.w);
}

INLINE Vector4 operator*(const Vector4& v, float k) {
	return Vector4(v.x*k, v.y*k, v.z*k, v.w*k);
}

INLINE Vector4 operator*(float k, const Vector4& v) {
	return Vector4(k*v.x, k*v.y, k*v.z, k*v.w);
}

INLINE Vector4 operator/(const Vector4& v0, const Vector4& v1) {
	return Vector4(v0.x/v1.x, v0.y/v1.y, v0.z/v1.z, v0.w/v1.w);
}

INLINE Vector4 operator/(const Vector4& v, float k) {
	return Vector4(v.x/k, v.y/k, v.z/k, v.w/k);
}

INLINE Vector4 operator/(float k, const Vector4& v) {
	return Vector4(k/v.x, k/v.y, k/v.z, k/v.w);
}

INLINE Vector4 operator+(const Vector4& v0, const Vector4& v1) {
	return Vector4(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z, v0.w+v1.w);
}

INLINE Vector4 operator-(const Vector4& v0, const Vector4& v1) {
	return Vector4(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z, v0.w-v1.w);
}

#endif /* VECTOR4_H_ */
