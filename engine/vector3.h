/*
 * Vector3.h
 *
 *  Created on: 30/11/2010
 *      Author: marrony.neris
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "math_util.h"

#include <cmath>
#include <algorithm>
#include <limits>

struct Vector3 {
	union {
		float vector[3];

		struct {
			float x, y, z;
		};

		struct {
			float r, g, b;
		};
	};

	INLINE bool operator==(const Vector3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	INLINE bool operator!=(const Vector3& other) const {
		return x != other.x && y != other.y && z != other.z;
	}

	INLINE float length() const {
		return std::sqrt(x*x + y*y + z*z);
	}

	INLINE Vector3 normalize() const {
		float lenght = 1.0 / length();

		Vector3 v = {x*lenght, y*lenght, z*lenght};
		return v;
	}

	INLINE Vector3 operator-() const {
		Vector3 v = {-x, -y, -z};
		return v;
	}

	INLINE Vector3& operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	INLINE Vector3& operator-=(const Vector3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	INLINE Vector3& operator*=(const Vector3& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	INLINE Vector3& operator/=(const Vector3& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	INLINE Vector3& operator+=(float k) {
		x += k;
		y += k;
		z += k;
		return *this;
	}

	INLINE Vector3& operator-=(float k) {
		x -= k;
		y -= k;
		z -= k;
		return *this;
	}

	INLINE Vector3& operator*=(float k) {
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	INLINE Vector3& operator/=(float k) {
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}

	static INLINE Vector3 min() {
		float m = -std::numeric_limits<float>::max();
		Vector3 v = {m, m, m};
		return v;
	}

	static INLINE Vector3 max() {
		float m = +std::numeric_limits<float>::max();
		Vector3 v = {m, m, m};
		return v;
	}
};

namespace vector {
	INLINE Vector3 make(float x, float y, float z) {
		Vector3 v = {x, y, z};
		return v;
	}

	INLINE Vector3 cross(const Vector3& v0, const Vector3& v1) {
		/*
		 * | i    j    j    |   +i*(v0.y*v1.z - v0.z*v1.y)   +i*(v0.y*v1.z - v0.z*v1.y)
		 * | v0.x v0.y v0.z | = -j*(v0.x*v1.z - v0.z*v1.x) = +j*(v0.z*v1.x - v0.x*v1.z)
		 * | v1.x v1.y v1.z |   +k*(v0.x*v1.y - v0.y*v1.x)   +k*(v0.x*v1.y - v0.y*v1.x)
		 */
		float newX = v0.y*v1.z - v0.z*v1.y;
		float newY = v0.z*v1.x - v0.x*v1.z;
		float newZ = v0.x*v1.y - v0.y*v1.x;

		Vector3 v = {newX, newY, newZ};
		return v;
	}

	INLINE float dot(const Vector3& v0, const Vector3& v1) {
		return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
	}

	INLINE Vector3 max(const Vector3& v0, const Vector3& v1) {
		float x = std::max(v0.x, v1.x);
		float y = std::max(v0.y, v1.y);
		float z = std::max(v0.z, v1.z);

		Vector3 v = {x, y, z};
		return v;
	}

	INLINE Vector3 min(const Vector3& v0, const Vector3& v1) {
		float x = std::min(v0.x, v1.x);
		float y = std::min(v0.y, v1.y);
		float z = std::min(v0.z, v1.z);

		Vector3 v = {x, y, z};
		return v;
	}

	INLINE Vector3 abs(const Vector3& v) {
		float x = std::abs(v.x);
		float y = std::abs(v.y);
		float z = std::abs(v.z);

		Vector3 out = {x, y, z};
		return out;
	}
}

INLINE bool operator>(const Vector3& v0, const Vector3& v1) {
	return v0.x > v1.x && v0.y > v1.y && v0.z > v1.z;
}

INLINE bool operator<(const Vector3& v0, const Vector3& v1) {
	return v0.x < v1.x && v0.y < v1.y && v0.z < v1.z;
}

INLINE bool operator>=(const Vector3& v0, const Vector3& v1) {
	return v0.x >= v1.x && v0.y >= v1.y && v0.z >= v1.z;
}

INLINE bool operator<=(const Vector3& v0, const Vector3& v1) {
	return v0.x <= v1.x && v0.y <= v1.y && v0.z <= v1.z;
}

INLINE Vector3 operator+(const Vector3& v, float k) {
	Vector3 out = {v.x+k, v.y+k, v.z+k};
	return out;
}

INLINE Vector3 operator-(const Vector3& v, float k) {
	Vector3 out = {v.x-k, v.y-k, v.z-k};
	return out;
}

INLINE Vector3 operator+(float k, const Vector3& v) {
	Vector3 out = {k+v.x, k+v.y, k+v.z};
	return out;
}

INLINE Vector3 operator-(float k, const Vector3& v) {
	Vector3 out = {k-v.x, k-v.y, k-v.z};
	return out;
}

INLINE Vector3 operator*(const Vector3& v0, const Vector3& v1) {
	Vector3 out = {v0.x*v1.x, v0.y*v1.y, v0.z*v1.z};
	return out;
}

INLINE Vector3 operator*(const Vector3& v, float k) {
	Vector3 out = {v.x*k, v.y*k, v.z*k};
	return out;
}

INLINE Vector3 operator*(float k, const Vector3& v) {
	Vector3 out = {k*v.x, k*v.y, k*v.z};
	return out;
}

INLINE Vector3 operator/(const Vector3& v0, const Vector3& v1) {
	Vector3 out = {v0.x/v1.x, v0.y/v1.y, v0.z/v1.z};
	return out;
}

INLINE Vector3 operator/(const Vector3& v, float k) {
	Vector3 out = {v.x/k, v.y/k, v.z/k};
	return out;
}

INLINE Vector3 operator/(float k, const Vector3& v) {
	Vector3 out = {k/v.x, k/v.y, k/v.z};
	return out;
}

INLINE Vector3 operator+(const Vector3& v0, const Vector3& v1) {
	Vector3 out = {v0.x+v1.x, v0.y+v1.y, v0.z+v1.z};
	return out;
}

INLINE Vector3 operator-(const Vector3& v0, const Vector3& v1) {
	Vector3 out = {v0.x-v1.x, v0.y-v1.y, v0.z-v1.z};
	return out;
}

#endif /* VECTOR3_H_ */
