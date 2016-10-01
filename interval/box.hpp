#ifndef RapidLab_box_hpp
#define RapidLab_box_hpp

#include "interval.hpp"

namespace rapidlab {

template<size_t _size> class box;

template<size_t _size>
std::ostream& operator<<(std::ostream& os, const box<_size>& b) {
    for (size_t i = 0; i < _size; ++i) {
        os << b[i] << "\n";
    }
    return os;
}

template<size_t _size>
class box {
private:
    std::array<interval, _size> data;
    double rank;

public:
    using iterator = typename std::array<interval, _size>::iterator;
    using const_iterator = typename std::array<interval, _size>::const_iterator;

    box() {}
    box(const std::array<interval, _size>& d) : data(d) {}
    box(const std::array<double, _size>& d) {
        std::copy(d.begin(), d.end(), data.begin());
    }

    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }

    interval& operator[](size_t index) { return data[index]; }
    const interval& operator[](size_t index) const { return data[index]; }

    double get_rank() const { return rank; }
    void set_rank(double r) { rank = r; }

    friend std::ostream& operator<<<>(std::ostream& os, const box<_size>& b);
};

template<size_t _size>
inline int operator<(const box<_size>& a, const box<_size>& b) {
    //operator needed for heap transform of a list
    return (a.get_rank() < b.get_rank());
}

template<size_t _size>
inline std::array<double, _size> mid(const box<_size>& b) {
    std::array<double, _size> a;
    for (size_t i = 0; i < _size; ++i) {
        a[i] = mid(b[i]);
    }
    return a;
}

template<size_t _size>
inline std::array<double, _size> diam(const box<_size>& b) {
    std::array<double, _size> a;
    for (size_t i = 0; i < _size; ++i) {
        a[i] = diam(b[i]);
    }
    return a;
}

} // namespace rapidlab

#endif
