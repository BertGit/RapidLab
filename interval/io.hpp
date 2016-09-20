#ifndef RapidLab_io_hpp
#define RapidLab_io_hpp

#include <iostream>

namespace rapidlab {

class interval;
// output interval

template<size_t _size> class box;
// output box
template<size_t _size>
std::ostream& operator<<(std::ostream& os, const box<_size>& b) {
    for (int i = 0; i < _size; i++) {
        os << b[i] << "\n";
    }
    return os;
}

} // namespace rapidlab

#endif
