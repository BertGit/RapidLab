#ifndef RapidLab_opt_bisection_hpp
#define RapidLab_opt_bisection_hpp

template <size_t _size_p>
std::array<box<_size_p>, 2>
optimizer<_size_p>::bisection(const box<_size_p>& b) const {

    size_t split_element = 0;
    std::array<double, _size_p> w = diam(b);

    if (this->options.bi_mode == bisection_mode::MAX_DIAM) {
        split_element = std::distance(
            w.begin(), std::max_element(w.begin(), w.end()));
    }

    std::array<box<_size_p>, 2> q = {b,b};
    //Bisect box interval at split_element
    double m = mid(b[split_element]);
    (q[0])[split_element].set_upper(m);
    (q[1])[split_element].set_lower(m);

    return q;
}

#endif
