//
//  box.hpp
//  RapidLab
//
//  Created by Bertram Müller on 30.04.12.
//  Copyright (c) 2012 University of Stuttgart. All rights reserved.
//

#ifndef RapidLab_box_hpp
#define RapidLab_box_hpp

#include "interval.hpp"
#include <array>

namespace rapidlab {

template<int _size>
class box {
private:
    std::array<interval, _size> idata;
    double rank;

public:
    box();
    box(const std::array<interval, _size>& d);
    box(const box& b);

    const std::array<interval, _size>& data() const;
    std::array<interval, _size> &data();
    double get_rank() const;
    void set_rank(double r);

    interval& operator[](int index);
    const interval& operator[](int index) const;
};

template<int _size>
inline box<_size>::box() {}

template<int _size>
inline box<_size>::box(std::array<interval, _size> const& d):idata(d) {}

template<int _size>
inline box<_size>::box(box const &b):idata(b.data()), rank(b.get_rank()) {}

template<int _size>
inline std::array<interval, _size> const &box<_size>::data() const {
    return idata;
}

template<int _size>
inline std::array<interval, _size> &box<_size>::data() {
    return idata;
}

template<int _size>
inline double box<_size>::get_rank() const {
    return rank;
}

template<int _size>
inline void box<_size>::set_rank(double r) {
    rank = r;
}

template<int _size>
inline interval& box<_size>::operator[](int index) {
    return idata[index];
}

template<int _size>
inline const interval& box<_size>::operator[](int index) const {
    return idata[index];
}

template<int _size>
inline int operator<(box<_size> const& a, box<_size> const& b) {
    //operator needed for heap transform of a list
    return (a.get_rank() < b.get_rank());
}

} // namespace rapidlab

#endif
