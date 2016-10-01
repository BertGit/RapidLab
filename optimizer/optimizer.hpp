#ifndef RapidLab_optimizer_hpp
#define RapidLab_optimizer_hpp

#include "interval/core.hpp"
#include "interval/box.hpp"
#include "interval/eigen_support.hpp"

#include <array>
#include <chrono>
#include <functional>
#include <vector>

namespace rapidlab {

enum class bisection_mode {
    MAX_DIAM,
    MAX_SMEAR_DIAM
};

struct options_t {
    double epsilon = 1e-3;
    bisection_mode bi_mode = bisection_mode::MAX_DIAM;
};

template <size_t _size_p>
class optimizer {
public:
    using func_t = std::function<interval(const box<_size_p>& b)>;
    using func_d_t = std::function<std::array<interval, _size_p>(const box<_size_p>& b)>;
    using func_dd_t = std::function<Eigen::Matrix<interval, _size_p, _size_p>(const box<_size_p>& b)>;

    optimizer(const func_t& func, options_t opt = options_t())
    : func(func), options(opt) {}

    void set_first_derivative(func_d_t f) { func_d = f; }
    void set_second_derivative(func_dd_t f) { func_dd = f; }

    box<_size_p> solve(const box<_size_p>& box0);

    int64_t box_count() const { return num_boxes; }
    double minimum() const { return f_min; }
    double time() const {return calc_time; }

private:
    func_t func;
    func_d_t func_d;
    func_dd_t func_dd;
    options_t options;
    box<_size_p> box0;

    double f_min = INFINITY;
    int64_t num_boxes = 0;
    double calc_time = 0; // in seconds

    std::array<box<_size_p>, 2> bisection(const box<_size_p>& b) const;
    int check_box(box<_size_p>& b);
    int gauss_seidel(
        const Eigen::Matrix<interval, _size_p, _size_p>& A,
        const std::array<double, _size_p>& b,
        box<_size_p> &x,
        const std::array<double, _size_p>& x_tilda) const;
};

#include "opt_checkbox.hpp"
#include "opt_bisection.hpp"
#include "opt_algorithm.hpp"
#include "opt_gaussseidel.hpp"

} // namespace rapidlab

#endif
