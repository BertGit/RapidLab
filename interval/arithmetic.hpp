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
    __m128d x = a.value().vec;

	// Interval spans over zero
    if (_mm_movemask_pd(_mm_cmple_pd((__m128d) {0, 0}, x)) == 3) {
		return interval(-INFINITY, INFINITY);
	}

    x = _mm_shuffle_pd(x, x, 1);

	return interval(_mm_div_pd((__m128d){-1.0, -1.0}, x));
}

} // namespace detail

inline double mid(const interval& a) {
    return .5 * (a.lower() + a.upper());
}

inline double diam(const interval& a) {
    return a.upper() - a.lower();
}

inline double mag(const interval& a) {
    return std::max(a.upper(), -a.lower());
}

inline bool operator==(const interval& a, const interval& b) {
    __m128d vcmp = _mm_cmpeq_pd(a.value().vec, b.value().vec);
    return _mm_movemask_pd(vcmp) == 3;
}

inline bool operator!=(const interval& a, const interval& b) {
    return !(a==b);
}

inline const interval& operator+(const interval& a) {
    return a;
}

inline interval operator-(const interval& a) {
    return interval(_mm_shuffle_pd(a.value().vec, a.value().vec, 1));
}

inline interval& operator+=(interval& a, const interval& b) {
    a.value().vec = _mm_add_pd(a.value().vec, b.value().vec);
    return a;
}

inline interval operator+(const interval& a, const interval& b) {
    interval c(a);
    c += b;
    return c;
}

inline interval& operator-=(interval& a, const interval& b) {
    a += -b;
    return a;
}

inline interval operator-(const interval& a, const interval& b) {
    return a + -b;
}

inline interval& operator*=(interval& a, double b) {
    if (b < 0) {
        a.value().vec = _mm_mul_pd((-a).value().vec, _mm_set1_pd(-b));
    } else {
        a.value().vec = _mm_mul_pd(a.value().vec, _mm_set1_pd(b));
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
    __m128d x = a.value().vec;
    __m128d y = b.value().vec;
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

		a.value().vec = x * _mm_xor_pd(y, c3);
	} else {
    	// Zero overlap
    	t1 = _mm_mul_pd(_mm_castsi128_pd(
                _mm_shuffle_epi32(_mm_castpd_si128(x), 0x4e)),
                _mm_unpacklo_pd(y, y));
    	t2 = _mm_mul_pd(t2, x);

        a.value().vec = _mm_max_pd(t1, t2);
    }
    return a;
}

inline interval operator*(const interval& a, const interval& b) {
    interval c(a);
    c *= b;
    return c;
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

inline interval sqr(const interval& a) {
    __m128d x = a.value().vec;

    // Take high bit value
    __m128d t1 = _mm_castsi128_pd(_mm_shuffle_epi32(_mm_castpd_si128(x), 0xee));

    if(_mm_movemask_pd(_mm_xor_pd(x, t1))) {
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
	m128d t5;
	t5.vec = _mm_max_pd(_mm_mul_pd(t1, t1), _mm_mul_pd(t2, t2));
	__m128d result = _mm_set_pd(t5.d[0], -0.0);
    return interval(result);
}

inline interval sqrt(const interval& a) {
    //TODO: define sqrt also for values < 0
    m128d x;
	x.vec = a.value().vec;

	if (x.d[0] > 0) return interval(_mm_set1_pd(NAN));

	// Two roundings to counteract with multiply...
	x.vec = _mm_mul_pd(x.vec, _mm_set_pd(1.0, -1.0));

	x.vec = _mm_sqrt_pd(x.vec);

	return interval(_mm_xor_pd(x.vec, _mm_set_pd(0.0, -0.0)));
}

inline interval fmod(const interval& a, const interval& b)
{
    double const &yb = (a.lower() < 0) ? b.lower() : b.upper();
    double n = -detail::to_int(-(a.lower() / yb));
    return a - n * b;
}

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

} // namespace rapidlab

#endif
