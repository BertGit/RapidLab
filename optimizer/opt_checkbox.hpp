#ifndef RapidLab_opt_checkbox_hpp
#define RapidLab_opt_checkbox_hpp

template <size_t _size_p>
int optimizer<_size_p>::check_box(box<_size_p>& b) {
    ++this->num_boxes;

    if (this->func_d) {
        std::array<interval, _size_p> f_d = func_d(b);

        //MONOTONY TEST
        for (size_t i = 0; i < _size_p; i++) {
            if (f_d[i].upper() < 0 || f_d[i].lower() > 0) {
                //derivation over box is monotone -> no local minimum possible
                return 1;
            }
        }
    }

    // //NONCONVEXITY TEST
    // matrix<interval, _size_p, _size_p> f_dd = func_dd(b);
    // for (int i = 0; i < _size_p; i++) {
    //     if (f_dd(i,i).upper() < 0) {
    //         //Function is non-convex over box
    //         return 1;
    //     }
    // }
    //
    // //GAUSS-SEIDEL
    // box<_size_p> c(mid<_size_p>(b));
    // if (gauss_seidel(f_dd, neg<_size_p>(func_d(c)), b.data(), c.data()) == 1) {
    //     //Box has been rejected
    //     return 1;
    // }

    interval t = this->func(b);
    if (t.lower() > this->f_min) {
        //reject box
        return 1;
    }

    return 0;
}

#endif
