#ifndef RapidLab_arithmetic_hpp
#define RapidLab_arithmetic_hpp

#include "interval.hpp"
#include "constants.hpp"

#include <cmath>

namespace rapidlab {

namespace detail {

inline __m128d c_swap(__m128d a, __m128d cond) {
    __m128d t = _mm_xor_pd((__m128d)_mm_shuffle_epi32((__m128i) a, 0x4e), a);
    cond = _mm_and_pd(cond, t);
    return _mm_xor_pd(cond, a);
}

inline double to_int(double r) {
    return rint(r);
}

inline interval recip(const interval& a) {
    __m128d x = a.value();

	// Interval spans over zero
    if (_mm_movemask_pd(_mm_cmple_pd((__m128d) {0, 0}, x)) == 3) {
		return interval(-INFINITY, INFINITY);
	}

    x = _mm_shuffle_pd(x, x, 1);

	return interval(_mm_div_pd((__m128d){-1.0, -1.0}, x));
}

} // namespace detail

////////////////////
// OPERATOR EQUAL //
////////////////////
inline bool operator==(const interval& a, const interval& b) {
    __m128d vcmp = _mm_cmpeq_pd(a.value(), b.value());
    return _mm_movemask_pd(vcmp) == 3;
}

inline bool operator!=(const interval& a, const interval& b) {
    return !(a==b);
}

//////////////////////
// UNARY PLUS MINUS //
//////////////////////
inline const interval& operator+(const interval& a) {
    return a;
}

inline interval operator-(const interval& a) {
    return interval(_mm_shuffle_pd(a.value(), a.value(), 1));
}

///////////////////
// OPERATOR PLUS //
///////////////////
inline interval& operator+=(interval& a, const interval& b) {
    a.value() = _mm_add_pd(a.value(), b.value());
    return a;
}

inline interval operator+(const interval& a, const interval& b) {
    interval c(a);
    c += b;
    return c;
}

////////////////////
// OPERATOR MINUS //
////////////////////
inline interval& operator-=(interval& a, const interval& b) {
    a += -b;
    return a;
}

inline interval operator-(const interval& a, const interval& b) {
    return a + -b;
}

/////////////////////////////
// OPERATOR MULTIPLICATION //
/////////////////////////////
inline interval& operator*=(interval& a, double b) {
    if (b < 0) {
        a.value() = _mm_mul_pd((-a).value(), _mm_set1_pd(-b));
    } else {
        a.value() = _mm_mul_pd(a.value(), _mm_set1_pd(b));
    }
    return a;
}

inline interval operator*(const interval& a, double b) {
    interval c(a);
    c *= b;
    return c;
}

inline interval operator*(double a, const interval& b) {
    return b * a;
}

inline interval& operator*=(interval& a, const interval& b) {
    __m128d x = a.value();
    __m128d y = b.value();
    __m128d t1 = _mm_castsi128_pd(_mm_shuffle_epi32(_mm_castpd_si128(x), 0xee));
	__m128d t2 = _mm_castsi128_pd(_mm_shuffle_epi32(_mm_castpd_si128(y), 0xee));

	__m128d t3 = _mm_xor_pd(x, t1);
	__m128d t4 = _mm_xor_pd(y, t2);

	if (_mm_movemask_pd(_mm_and_pd(t3, t4))) {
		__m128d c = _mm_set1_pd(0.0);
		__m128d c1 = _mm_cmple_pd(t2, c);
		__m128d c2 = _mm_cmple_pd(t1, c);
		__m128d c3 = _mm_set_pd(0.0, -0.0);

		x = detail::c_swap(_mm_xor_pd(x, c3), c1);
		y = detail::c_swap(_mm_xor_pd(y, c3), c2);

		a.value() = x * _mm_xor_pd(y, c3);
	} else {
    	// Zero overlap
    	t1 = _mm_mul_pd(_mm_castsi128_pd(
                _mm_shuffle_epi32(_mm_castpd_si128(x), 0x4e)),
                _mm_unpacklo_pd(y, y));
    	t2 = _mm_mul_pd(t2, x);

        a.value() = _mm_max_pd(t1, t2);
    }
    return a;
}

inline interval operator*(const interval& a, const interval& b) {
    interval c(a);
    c *= b;
    return c;
}

///////////////////////
// OPERATOR DIVISION //
///////////////////////
inline interval& operator/=(interval& a, double b) {
    if (b == 0) {
        a.value() = _mm_set1_pd(INFINITY);
    } else if (b < 0) {
        a.value() = _mm_div_pd((-a).value(), _mm_set1_pd(-b));
    } else {
        a.value() = _mm_div_pd(a.value(), _mm_set1_pd(b));
    }
    return a;
}

inline interval operator/(const interval& a, double b) {
    interval c(a);
    c /= b;
    return c;
}

inline interval operator/(double a, const interval& b) {
    return a * detail::recip(b);
}

inline interval& operator/=(interval& a, const interval& b) {
	a = a * detail::recip(b);
    return a;
}

inline interval operator/(const interval& a, const interval& b) {
	interval c(a);
    c /= b;
    return c;
}

//////////////////
// SQRT AND SQR //
//////////////////
inline interval sqrt(const interval& a) {
	if (a.value()[0] > 0) return interval(_mm_set1_pd(NAN));

    __m128d x = a.value();
    // Two roundings to counteract with multiply
    x *= (__m128d) {-0x1.ffffffffffff8p-1, 1.0};
	x = _mm_sqrt_pd(x);

	return interval(_mm_xor_pd(x, _mm_set_pd(0.0, -0.0)));
}

inline interval sqr(const interval& a) {
    __m128d x = a.value();

    // Take high bit value
    __m128d t1 = _mm_castsi128_pd(_mm_shuffle_epi32(_mm_castpd_si128(x), 0xee));

    if (_mm_movemask_pd(_mm_xor_pd(x, t1))) {
        // Interval not containing zero
        __m128d c = _mm_set1_pd(0.0);
        // Compare: is sup(x) <= 0?
        __m128d c1 = _mm_cmple_pd(t1, c);
        __m128d c3 = _mm_set_pd(0.0, -0.0);
        // Swap for sup(x) <= 0
        x = detail::c_swap(_mm_xor_pd(x, c3), c1);

        return interval(_mm_mul_pd(x, _mm_xor_pd(x, c3)));
    }
    // Interval contains zero
    __m128d t2 = _mm_unpacklo_pd(x, x);
	__m128d t5 = _mm_max_pd(_mm_mul_pd(t1, t1), _mm_mul_pd(t2, t2));
	__m128d result = _mm_set_pd(t5[0], -0.0);
    return interval(result);
}

inline interval abs(const interval& a) {
    if (a.lower() > 0) {
        return a;
    } else if (a.upper() <= 0) {
        return -a;
    }
    return interval(0, std::max(-a.lower(), a.upper()));
}

///////////////////////////
// FLOATING POINT MODULO //
///////////////////////////
inline interval fmod(const interval& a, const interval& b)
{
    double const &yb = (a.lower() < 0) ? b.lower() : b.upper();
    double n = -detail::to_int(-(a.lower() / yb));
    return a - n * b;
}

//////////////////
// TRIGONOMETRY //
//////////////////
inline interval cos(const interval& a) {
    // Get lower bound within [0, pi]
    const interval pi2 = pi_twice();
    interval tmp = fmod(a, pi2);
    if (diam(tmp) >= pi2.lower()) return interval(-1, 1); // full period
    if (tmp.lower() >= pi_upper()) return -cos(tmp - pi());
    double l = tmp.lower();
    double u = tmp.upper();

    // Separate into monotone subintervals
    if (u <= pi_lower()) {
        return interval(std::cos(u), std::cos(l));
    } else if (u <= pi2.lower()) {
        return interval(-1, std::cos(std::min(pi2.lower() - u, l)));
    } else {
        return interval(-1, 1);
    }
}

inline interval sin(const interval& a) {
    interval r = cos(a - pi_half());
    return r;
}

/////////////////////////////
// EIGEN TYPE REQUIREMENTS //
/////////////////////////////
inline const interval& conj(const interval& x)  { return x; }
inline const interval& real(const interval& x)  { return x; }
inline interval imag(const interval&)    { return 0.; }
inline interval abs2(const interval& x)  { return sqr(x); }

} // namespace rapidlab

#endif
