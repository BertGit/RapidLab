#ifndef RapidLab_interval_hpp
#define RapidLab_interval_hpp

#include <emmintrin.h>
#include <iostream>
#include <cassert>

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
    interval(__m128d v);
    interval(const interval& r);

    double lower() const;
    double upper() const;
    void set_lower(double d);
    void set_upper(double d);
    const m128d& value() const;
    m128d& value();

    friend std::ostream& operator<<(std::ostream& os, const interval& intval);
};

inline interval::interval() {
    ival.vec = _mm_set_pd(0,-0);
}

inline interval::interval(double a) {
    ival.vec = _mm_set_pd(a,-a);
}

inline interval::interval(double a, double b) {
    assert(a <= b && "lower bound cannot be greater than upper bound");
    ival.vec = _mm_set_pd(b, -a);
}

inline interval::interval(__m128d v) {
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
    assert(d <= upper() && "lower bound cannot be greater than upper bound");
    ival.d[0] = -d;
}

inline void interval::set_upper(double d) {
    assert(d >= lower() && "upper bound cannot be less than lower bound");
    ival.d[1] = d;
}

inline const m128d& interval::value() const {
    return ival;
}

inline m128d& interval::value() {
    return ival;
}

inline std::ostream& operator<<(std::ostream& os, const interval& a) {
    os << '[' << a.lower() << ',' << a.upper() << ']';
    return os;
}

} // namespace rapidlab

#endif
