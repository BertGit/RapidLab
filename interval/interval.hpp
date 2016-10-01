#ifndef RapidLab_interval_hpp
#define RapidLab_interval_hpp

#include <emmintrin.h>
#include <iostream>
#include <cassert>

namespace rapidlab {

class interval
{
private:
    __m128d v;
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
    const __m128d& value() const;
    __m128d& value();

    friend std::ostream& operator<<(std::ostream& os, const interval& intval);
};

inline interval::interval() {
    this->v = _mm_set_pd(0,-0);
}

inline interval::interval(double a) {
    this->v = _mm_set_pd(a,-a);
}

inline interval::interval(double a, double b) {
    assert(a <= b && "lower bound cannot be greater than upper bound");
    this->v = _mm_set_pd(b, -a);
}

inline interval::interval(__m128d v) {
    this->v = v;
}

inline interval::interval(const interval& r) {
    this->v = r.value();
}

inline double interval::lower() const {
    return -this->v[0];
}

inline double interval::upper() const {
    return this->v[1];
}

inline void interval::set_lower(double d) {
    assert(d <= upper() && "lower bound cannot be greater than upper bound");
    this->v[0] = -d;
}

inline void interval::set_upper(double d) {
    assert(d >= lower() && "upper bound cannot be less than lower bound");
    this->v[1] = d;
}

inline const __m128d& interval::value() const {
    return this->v;
}

inline __m128d& interval::value() {
    return this->v;
}

inline std::ostream& operator<<(std::ostream& os, const interval& a) {
    os << '[' << a.lower() << ',' << a.upper() << ']';
    return os;
}

} // namespace rapidlab

#endif
