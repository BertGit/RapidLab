#ifndef RapidLab_arithmetic_hpp
#define RapidLab_arithmetic_hpp

#include "interval.hpp"
#include "constants.hpp"

#include <cmath>

namespace rapidlab {

namespace detail {

inline __m128d c_swap(const __m128d& x, const __m128d& cond) {
    __m128d t = _mm_and_pd(_mm_castsi128_pd(_mm_shuffle_epi32(
            _mm_castpd_si128(x), 0x4e)), cond);

    __m128d t2 = _mm_andnot_pd(cond, x);

    return _mm_or_pd(t2, t);
}

inline double to_int(double r) {
    return rint(r);
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

inline const interval& operator+(const interval& x) {
    return x;
}

inline interval operator-(const interval& x) {
    return interval(_mm_shuffle_pd(x.value().vec, x.value().vec, 1));
}

inline interval& operator+=(interval& x, const interval& y) {
    x.value().vec = _mm_add_pd(x.value().vec, y.value().vec);
    return x;
}

inline interval operator+(const interval& x, const interval& y) {
    interval z(x);
    z += y;
    return z;
}

inline interval& operator-=(interval& x, const interval& y) {
    x += -y;
    return x;
}

inline interval operator-(const interval& x, const interval& y) {
    return x + -y;
}

inline interval operator*(const interval& x, double y) {
    if (y < 0) {
        return interval(_mm_mul_pd((-x).value().vec, _mm_set1_pd(-y)));
    }
    return interval(_mm_mul_pd(x.value().vec, _mm_set1_pd(y)));
}

inline interval operator*(const interval& a, const interval& b) {
    __m128d x = a.value().vec;
    __m128d y = b.value().vec;
    __m128d t1 = _mm_castsi128_pd(_mm_shuffle_epi32(_mm_castpd_si128(x), 0xee));
	__m128d t2 = _mm_castsi128_pd(_mm_shuffle_epi32(_mm_castpd_si128(y), 0xee));

	__m128d t3 = _mm_xor_pd(x, t1);
	__m128d t4 = _mm_xor_pd(y, t2);

	if (_mm_movemask_pd(_mm_and_pd(t3, t4)))
	{
		__m128d c = _mm_set1_pd(0.0);
		__m128d c1 = _mm_cmple_pd(t2, c);
		__m128d c2 = _mm_cmple_pd(t1, c);
		__m128d c3 = _mm_set_pd(0.0, -0.0);

		x = detail::c_swap(_mm_xor_pd(x, c3), c1);
		y = detail::c_swap(_mm_xor_pd(y, c3), c2);

		return interval(x * _mm_xor_pd(y, c3));
	}

	// Zero overlap
	t1 = _mm_mul_pd(_mm_castsi128_pd(
            _mm_shuffle_epi32(_mm_castpd_si128(x), 0x4e)),
            _mm_unpacklo_pd(y, y));
	t2 = _mm_mul_pd(t2, x);

	return interval(_mm_max_pd(t1, t2));
}

inline interval square(const interval& a) {
    __m128d x = a.value().vec;

    // Take high bit value
    __m128d t1 = _mm_castsi128_pd(_mm_shuffle_epi32(_mm_castpd_si128(x), 0xee));

    if(_mm_movemask_pd(_mm_xor_pd(x, t1))) {
        // Interval not containing zero
        __m128d c = _mm_set1_pd(0.0);
        //compare: is sup(x) <= 0?
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

inline interval recip(const interval& a) {
    m128d x;
	x.vec = a.value().vec;

    double rmin, rmax;

	// Possible divide by zero
	if ((x.d[1] >= 0) && (-x.d[0] <= 0)) {
		return interval(-INFINITY, INFINITY);
	}

	//_MM_SET_ROUNDING_MODE(_MM_ROUND_UP);
	rmax = 1.0/x.d[0];

	//_MM_SET_ROUNDING_MODE(_MM_ROUND_DOWN);
	rmin = 1.0/x.d[1];

	return interval(rmin, -rmax);
}

inline interval operator/(const interval& a, const interval& b) {
	return a * recip(b);
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
