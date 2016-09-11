//
//  utility.hpp
//  RapidLab
//
//  Created by Bertram Müller on 30.04.12.
//  Copyright (c) 2012 University of Stuttgart. All rights reserved.
//

#ifndef RapidLab_utility_hpp
#define RapidLab_utility_hpp

#include "interval.hpp"
#include "box.hpp"
// #include "../matrix/matrix.hpp"
#ifdef _MSC_VER
	#define INFINITY (DBL_MAX+DBL_MAX)
	#define NAN (INFINITY-INFINITY)
	#define isnan _isnan
	#define isinf !finite
	#define rint(x) (int)(x+0.5)
	#include <array>
#else
	#include <tr1/array>
#endif

#include <math.h>
#include <emmintrin.h>

namespace rapidlab {

using namespace std;
using namespace tr1;



//template<int _size>
//inline array<double, _size> mid(box<_size> const& b) {
//    array<double, _size> r;
//    for (int i = 0; i < _size; i++) {
//        r[i] = mid(b[i]);
//    }
//    return r;
//}

template<int _size>
inline array<interval, _size> mid(box<_size> const& b) {
    array<interval, _size> r;
    for (int i = 0; i < _size; i++) {
        r[i] = interval(mid(b[i]));
    }
    return r;
}

template<int _size>
inline array<double, _size> mid_double(box<_size> const& b) {
    array<double, _size> r;
    for (int i = 0; i < _size; i++) {
        r[i] = mid(b[i]);
    }
    return r;
}

template<int _size>
inline array<double, _size> diam(box<_size> const& b) {
    array<double, _size> r;
    for (int i = 0; i < _size; i++) {
        r[i] = diam(b[i]);
    }
    return r;
}

template <int _size>
inline int max_element(array<double, _size> const& arr) {
    double max_val = arr[0];
    int max_el = 0;
    for (int i = 1; i < _size; i++) {
        if (max_val < arr[i]) {
            max_val = arr[i];
            max_el = i;
        }
    }
    return max_el;
}

inline interval intersect(interval const& a, interval const& b) {
    __m128d x = a.value().vec;
    __m128d y = b.value().vec;

    m128d r;
	r.vec = _mm_min_pd(x, y);

	if (-r.d[0] <= r.d[1])   return interval(r.vec);
    else                return interval(NAN);
}

inline double max(double a, double b) {
    return (a > b) ? a : b;
}

// template<int rows, int cols>
// inline matrix<double, rows, cols> mid(matrix<interval, rows, cols> const& m) {
//     array<double, rows * cols> r;
//     array<interval, rows * cols> m_data = m.data();
//
//     for (int i = 0; i < rows*cols; i++) {
//         r[i] = mid(m_data[i]);
//     }
//
//     return matrix<double, rows, cols>(r);
// }

template<int _size>
inline array<interval, _size> neg(array<interval, _size> const &arr) {
    array<interval, _size> r;
    for (int i = 0; i < _size; i++) {
        r[i] = -arr[i];
    }
    return r;
}

} // namespace rapidlab

#endif
