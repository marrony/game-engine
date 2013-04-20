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

	INLINE Vector3() { }

	INLINE Vector3(const float vector[3]) :
		x(vector[0]), y(vector[1]), z(vector[2]) { }

	INLINE Vector3(float _x, float _y, float _z) :
		x(_x), y(_y), z(_z) {
	}

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

		return Vector3(x*lenght, y*lenght, z*lenght);
	}

	INLINE Vector3 operator-() const {
		return Vector3(-x, -y, -z);
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
		return Vector3(m, m, m);
	}

	static INLINE Vector3 max() {
		float m = +std::numeric_limits<float>::max();
		return Vector3(m, m, m);
	}
};

namespace vector {
	INLINE Vector3 cross(const Vector3& v0, const Vector3& v1) {
		/*
		 * | i    j    j    |   +i*(v0.y*v1.z - v0.z*v1.y)   +i*(v0.y*v1.z - v0.z*v1.y)
		 * | v0.x v0.y v0.z | = -j*(v0.x*v1.z - v0.z*v1.x) = +j*(v0.z*v1.x - v0.x*v1.z)
		 * | v1.x v1.y v1.z |   +k*(v0.x*v1.y - v0.y*v1.x)   +k*(v0.x*v1.y - v0.y*v1.x)
		 */
		float newX = v0.y*v1.z - v0.z*v1.y;
		float newY = v0.z*v1.x - v0.x*v1.z;
		float newZ = v0.x*v1.y - v0.y*v1.x;

		return Vector3(newX, newY, newZ);
	}

	INLINE float dot(const Vector3& v0, const Vector3& v1) {
		return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
	}

	INLINE Vector3 max(const Vector3& v0, const Vector3& v1) {
		float x = std::max(v0.x, v1.x);
		float y = std::max(v0.y, v1.y);
		float z = std::max(v0.z, v1.z);

		return Vector3(x, y, z);
	}

	INLINE Vector3 min(const Vector3& v0, const Vector3& v1) {
		float x = std::min(v0.x, v1.x);
		float y = std::min(v0.y, v1.y);
		float z = std::min(v0.z, v1.z);

		return Vector3(x, y, z);
	}

	INLINE Vector3 abs(const Vector3& v) {
		float x = std::abs(v.x);
		float y = std::abs(v.y);
		float z = std::abs(v.z);

		return Vector3(x, y, z);
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
	return Vector3(v.x+k, v.y+k, v.z+k);
}

INLINE Vector3 operator-(const Vector3& v, float k) {
	return Vector3(v.x-k, v.y-k, v.z-k);
}

INLINE Vector3 operator+(float k, const Vector3& v) {
	return Vector3(k+v.x, k+v.y, k+v.z);
}

INLINE Vector3 operator-(float k, const Vector3& v) {
	return Vector3(k-v.x, k-v.y, k-v.z);
}

INLINE Vector3 operator*(const Vector3& v0, const Vector3& v1) {
	return Vector3(v0.x*v1.x, v0.y*v1.y, v0.z*v1.z);
}

INLINE Vector3 operator*(const Vector3& v, float k) {
	return Vector3(v.x*k, v.y*k, v.z*k);
}

INLINE Vector3 operator*(float k, const Vector3& v) {
	return Vector3(k*v.x, k*v.y, k*v.z);
}

INLINE Vector3 operator/(const Vector3& v0, const Vector3& v1) {
	return Vector3(v0.x/v1.x, v0.y/v1.y, v0.z/v1.z);
}

INLINE Vector3 operator/(const Vector3& v, float k) {
	return Vector3(v.x/k, v.y/k, v.z/k);
}

INLINE Vector3 operator/(float k, const Vector3& v) {
	return Vector3(k/v.x, k/v.y, k/v.z);
}

INLINE Vector3 operator+(const Vector3& v0, const Vector3& v1) {
	return Vector3(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z);
}

INLINE Vector3 operator-(const Vector3& v0, const Vector3& v1) {
	return Vector3(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z);
}

#endif /* VECTOR3_H_ */
