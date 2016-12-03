#ifndef RapidLab_constants_hpp
#define RapidLab_constants_hpp

namespace rapidlab {

static const double pi_d_l = (3373259426.0 + 273688.0 / (1<<21)) / (1<<30);
static const double pi_d_u = (3373259426.0 + 273689.0 / (1<<21)) / (1<<30);

inline double pi_twice_lower() { return pi_d_l * 2; }
inline double pi_twice_upper() { return pi_d_u * 2; }
inline double pi_half_lower() { return pi_d_l / 2; }
inline double pi_half_upper() { return pi_d_u / 2; }
inline double pi_lower() { return pi_d_l; }
inline double pi_upper() { return pi_d_u; }

inline interval pi_twice() {
    return interval(pi_twice_lower(), pi_twice_upper());
}

inline interval pi() {
    return interval(pi_lower(), pi_upper());
}

inline interval pi_half() {
    return interval(pi_half_lower(), pi_half_upper());
}

} // namespace rapidlab

#endif
