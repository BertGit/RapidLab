#ifndef RapidLab_opt_algorithm_hpp
#define RapidLab_opt_algorithm_hpp

#include <algorithm>

template <size_t _size_p>
box<_size_p> optimizer<_size_p>::solve(const box<_size_p>& box0) {
    using namespace std::chrono;
    auto start_time = high_resolution_clock::now();

    this->num_boxes = 0;

    //initialize lists
    std::vector<box<_size_p>> list;
    list.push_back(box0);

    //current box from list
    box<_size_p> b;
    //solution box
    box<_size_p> solution;

    while (list.size() > 0) {
        //pop box from list
        b = list.back();
        list.pop_back();

        //decrease box size or reject
        const bool is_rejected = check_box(b);
        if (is_rejected) {
            continue;
        }

        auto within_tolerance = [&](const interval& ival) {
            return diam(ival) <= this->options.epsilon;
        };
        const bool is_within_tolerance =
            std::all_of(b.begin(), b.end(), within_tolerance);

        //scalar result at interval mid point
        std::array<double, _size_p> m = mid<_size_p>(b);
        interval f_center = this->func(m);

        if (is_within_tolerance) {
            if (this->f_min >= f_center.upper()) {
                solution = m;
                this->f_min = f_center.upper();
            }
        } else {
            //update minimum bound
            if (this->f_min > f_center.upper()) {
                solution = m;
                this->f_min = f_center.upper();
            }
            //bisect current box
            std::array<box<_size_p>, 2> bisected_boxes;
            bisected_boxes = bisection(b);
            //add boxes to list
            list.emplace_back(bisected_boxes[0]);
            list.emplace_back(bisected_boxes[1]);
        }
    }

    auto end_time = high_resolution_clock::now();
    this->calc_time = duration_cast<microseconds>(
        end_time - start_time).count() / 1e6;

    return solution;
}

#endif
