/*
 * Vector2.h
 *
 *  Created on: 30/11/2010
 *      Author: marrony.neris
 */

#ifndef VECTOR2_H_
#define VECTOR2_H_

#include "math_util.h"

#include <cmath>
#include <algorithm>
#include <limits>

struct Vector2 {
	union {
		float vector[2];

		struct {
			float x, y;
		};

		struct {
			float s, t;
		};
	};

	INLINE Vector2() { }

	INLINE Vector2(const float vector[2]) :
		x(vector[0]), y(vector[1]) { }

	INLINE Vector2(float _x, float _y) :
		x(_x), y(_y) {
	}

	INLINE bool operator==(const Vector2& other) const {
		return x == other.x && y == other.y;
	}

	INLINE bool operator!=(const Vector2& other) const {
		return x != other.x && y != other.y;
	}

	INLINE float length() const {
		return std::sqrt(x*x + y*y);
	}

	INLINE Vector2 normalize() const {
		float lenght = 1.0 / length();

		return Vector2(x*lenght, y*lenght);
	}

	INLINE Vector2 operator-() const {
		return Vector2(-x, -y);
	}

	INLINE Vector2& operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	INLINE Vector2& operator-=(const Vector2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	INLINE Vector2& operator*=(const Vector2& other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	INLINE Vector2& operator/=(const Vector2& other) {
		x /= other.x;
		y /= other.y;
		return *this;
	}

	INLINE Vector2& operator+=(float k) {
		x += k;
		y += k;
		return *this;
	}

	INLINE Vector2& operator-=(float k) {
		x -= k;
		y -= k;
		return *this;
	}

	INLINE Vector2& operator*=(float k) {
		x *= k;
		y *= k;
		return *this;
	}

	INLINE Vector2& operator/=(float k) {
		x /= k;
		y /= k;
		return *this;
	}

	static INLINE Vector2 min() {
		float m = -std::numeric_limits<float>::max();
		return Vector2(m, m);
	}

	static INLINE Vector2 max() {
		float m = +std::numeric_limits<float>::max();
		return Vector2(m, m);
	}
};

namespace vector {
	INLINE float dot(const Vector2& v0, const Vector2& v1) {
		return v0.x*v1.x + v0.y*v1.y;
	}

	INLINE Vector2 max(const Vector2& v0, const Vector2& v1) {
		float x = std::max(v0.x, v1.x);
		float y = std::max(v0.y, v1.y);

		return Vector2(x, y);
	}

	INLINE Vector2 min(const Vector2& v0, const Vector2& v1) {
		float x = std::min(v0.x, v1.x);
		float y = std::min(v0.y, v1.y);

		return Vector2(x, y);
	}

	INLINE Vector2 abs(const Vector2& v) {
		float x = std::abs(v.x);
		float y = std::abs(v.y);

		return Vector2(x, y);
	}
}

INLINE bool operator>(const Vector2& v0, const Vector2& v1) {
	return v0.x > v1.x && v0.y > v1.y;
}

INLINE bool operator<(const Vector2& v0, const Vector2& v1) {
	return v0.x < v1.x && v0.y < v1.y;
}

INLINE bool operator>=(const Vector2& v0, const Vector2& v1) {
	return v0.x >= v1.x && v0.y >= v1.y;
}

INLINE bool operator<=(const Vector2& v0, const Vector2& v1) {
	return v0.x <= v1.x && v0.y <= v1.y;
}

INLINE Vector2 operator+(const Vector2& v, float k) {
	return Vector2(v.x+k, v.y+k);
}

INLINE Vector2 operator-(const Vector2& v, float k) {
	return Vector2(v.x-k, v.y-k);
}

INLINE Vector2 operator+(float k, const Vector2& v) {
	return Vector2(k+v.x, k+v.y);
}

INLINE Vector2 operator-(float k, const Vector2& v) {
	return Vector2(k-v.x, k-v.y);
}

INLINE Vector2 operator*(const Vector2& v0, const Vector2& v1) {
	return Vector2(v0.x*v1.x, v0.y*v1.y);
}

INLINE Vector2 operator*(const Vector2& v, float k) {
	return Vector2(v.x*k, v.y*k);
}

INLINE Vector2 operator*(float k, const Vector2& v) {
	return Vector2(k*v.x, k*v.y);
}

INLINE Vector2 operator/(const Vector2& v0, const Vector2& v1) {
	return Vector2(v0.x/v1.x, v0.y/v1.y);
}

INLINE Vector2 operator/(const Vector2& v, float k) {
	return Vector2(v.x/k, v.y/k);
}

INLINE Vector2 operator/(float k, const Vector2& v) {
	return Vector2(k/v.x, k/v.y);
}

INLINE Vector2 operator+(const Vector2& v0, const Vector2& v1) {
	return Vector2(v0.x+v1.x, v0.y+v1.y);
}

INLINE Vector2 operator-(const Vector2& v0, const Vector2& v1) {
	return Vector2(v0.x-v1.x, v0.y-v1.y);
}

#endif /* VECTOR2_H_ */
