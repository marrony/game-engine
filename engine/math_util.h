/*
 * MathUtil.h
 *
 *  Created on: 23/05/2012
 *      Author: marrony.neris
 */

#ifndef MATHUTIL_H_
#define MATHUTIL_H_

#include <cmath>

#if defined(__SSE__) && !defined(ANDROID)
#include <xmmintrin.h> //(for SSE)
#include <emmintrin.h> //(for SSE2)
//#include <pmmintrin.h> //(for SSE3)
//#include <smmintrin.h> //(for SSE4)
#endif

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifdef __GNUC__
#define INLINE __attribute__((always_inline)) inline
#else
#define INLINE inline
#endif

static INLINE float deg2rad(float deg) {
	return deg * M_PI / 180.0;
}

static INLINE float rad2deg(float rad) {
	return rad * 180.0 / M_PI;
}

#endif /* MATHUTIL_H_ */
