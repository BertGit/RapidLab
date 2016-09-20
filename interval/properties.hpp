#ifndef RapidLab_attributes_hpp
#define RapidLab_attributes_hpp

#include "interval.hpp"

namespace rapidlab {

inline double mid(const interval& a) {
    return .5 * (a.lower() + a.upper());
}

inline double diam(const interval& a) {
    return a.upper() - a.lower();
}

inline double mag(const interval& a) {
    return std::max(a.upper(), -a.lower());
}

inline int zero_in(const interval& a) {
    return _mm_movemask_pd(_mm_cmple_pd((__m128d){0, 0}, a.value().vec)) == 3;
}

inline bool contains(const interval& a, const interval& b) {
    return a.lower() <= b.lower() && a.upper() >= b.upper();
}

} // namespace rapidlab

#endif
