#ifndef RapidLab_io_hpp
#define RapidLab_io_hpp

namespace rapidlab {

#include "interval.hpp"
#include "box.hpp"

#include <iostream>

// output interval
inline std::ostream& operator<<(std::ostream& os, interval const& intval) {
    os << '[' << intval.lower() << ',' << intval.upper() << ']';
    return os;
}

// output box
template<int _size>
inline std::ostream& operator<<(std::ostream& os, box<_size> const& b) {
    for (int i = 0; i < _size; i++) {
        os << b[i] << std::endl;
    }
    return os;
}

} // namespace rapidlab

#endif
