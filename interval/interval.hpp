#ifndef RapidLab_interval_hpp
#define RapidLab_interval_hpp

#include <emmintrin.h>
#include <iostream>

namespace rapidlab {

union m128d {
	__m128d vec;
	double d[2];
};

class interval
{
private:
    m128d ival;
public:
    interval();
    interval(double a);
    interval(double a, double b);
    interval(const __m128d& v);
    interval(const interval& r);

    double lower() const;
    double upper() const;
    void set_lower(double d);
    void set_upper(double d);
    const m128d& value() const;
	m128d& value();

    friend inline std::ostream& operator<<(
            std::ostream& os, const interval& intval);
};

inline interval::interval() {}

inline interval::interval(double a) {
	ival.vec = _mm_set_pd(a,-a);
}

inline interval::interval(double a, double b) {
	ival.vec = _mm_set_pd(b, -a);
}

inline interval::interval(const __m128d& v) {
	ival.vec = v;
}

inline interval::interval(const interval& r) {
	ival = r.value();
}

inline double interval::lower() const {
	return -ival.d[0];
}

inline double interval::upper() const {
    return ival.d[1];
}

inline void interval::set_lower(double d) {
    ival.d[0] = -d;
}

inline void interval::set_upper(double d) {
    ival.d[1] = d;
}

inline const m128d& interval::value() const {
    return ival;
}

inline m128d& interval::value() {
    return ival;
}

} // namespace rapidlab

#endif
