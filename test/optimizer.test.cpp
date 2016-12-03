#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "interval/core.hpp"

#include "optimizer/optimizer.hpp"
#include "interval/eigen_support.hpp"

#include <iomanip>

using namespace rapidlab;
using namespace testing;

class AnOptimizer : public Test {
public:
    void SetUp() override final {
        // Initialize every test by rounding to infinity
        _MM_SET_ROUNDING_MODE(_MM_ROUND_UP);
    }
};

////////////////////////////
// OPTIMIZATION FUNCTIONS //
////////////////////////////

// f(x) = (x-2.5)^2
interval convex1d(const box<1>& b) {
    return sqr(b[0] - 2.5) + 1.2;
}

// Rosenbrock functions
interval rosenbrock2d(const box<2>& b) {
    return 100 * sqr(b[1] - sqr(b[0])) + sqr(b[0] - 1);
}
std::array<interval, 2> rosenbrock2d_d(const box<2>& b) {
    std::array<interval, 2> s;
    s[1] = 200 * (b[1] - sqr(b[0]));
    s[0] = (1 - s[1]) * 2 * b[0] - 2;
    return s;
}
Eigen::Matrix<interval, 2, 2> rosenbrock2d_dd(const box<2>& b) {
    Eigen::Matrix<interval, 2, 2> s;
    s(0,0) = -400 * b[0] * -2 * b[0] + -400 * (b[1] - sqr(b[0])) + 2;
    s(1,1) = 200;
    s(0,1) = -400 * b[0];
    s(1,0) = s(0,1);
    return s;
}

interval bukin_no6(const box<2>& b) {
    return 100 * sqrt(abs(b[1] - 0.01 * sqr(b[0]))) + 0.01 * abs(b[0] + 10);
}

TEST_F(AnOptimizer, canSolveOneDimensionalEquationWithDefaultSettings) {
    // Default epsilon is 1e-3
    optimizer<1> opt(convex1d);
    box<1> b({interval(0,5)});
    box<1> s = opt.solve(b);

    EXPECT_THAT(opt.minimum(), DoubleEq(1.2));
    EXPECT_THAT(contains(s[0], 2.5), Eq(true));
    EXPECT_THAT(diam(s[0]), Lt(1e-3));

    std::cout << "CalcTime: " << opt.time() << "\n";
    std::cout << "Boxes: " << opt.box_count() << "\n";
}

TEST_F(AnOptimizer, canSolveOneDimensionalEquationWithHigherPrecision) {
    options_t o;
    o.epsilon = 1e-6;
    optimizer<1> opt(convex1d, o);
    box<1> b({interval(0,5)});
    box<1> s = opt.solve(b);

    EXPECT_THAT(opt.minimum(), DoubleEq(1.2));
    EXPECT_THAT(contains(s[0], 2.5), Eq(true));
    EXPECT_THAT(diam(s[0]), Lt(1e-6));

    std::cout << "CalcTime: " << opt.time() << "\n";
    std::cout << "Boxes: " << opt.box_count() << "\n";
}

TEST_F(AnOptimizer, canSolveRosenbrockFunctionIn2D) {
    options_t o;
    o.epsilon = 1e-6;
    optimizer<2> opt(rosenbrock2d, o);
    box<2> b({interval(0,2), interval(-1,1.5)});
    box<2> s = opt.solve(b);

    interval tolerance(-1e-5,1e-5);
    EXPECT_THAT(contains(0.0 + tolerance, opt.minimum()), Eq(true));
    EXPECT_THAT(contains(s[0] + tolerance, 1.0), Eq(true));
    EXPECT_THAT(contains(s[1] + tolerance, 1.0), Eq(true));
    EXPECT_THAT(diam(s[0]), Lt(1e-6));
    EXPECT_THAT(diam(s[1]), Lt(1e-6));

    std::cout << "CalcTime: " << opt.time() << "\n";
    std::cout << "Boxes: " << opt.box_count() << "\n";
}

TEST_F(AnOptimizer, canSolveRosenbrockFunctionIn2DProvidingFirstDerivative) {
    options_t o;
    o.epsilon = 1e-6;
    optimizer<2> opt(rosenbrock2d, o);
    opt.set_first_derivative(rosenbrock2d_d);

    box<2> b({interval(-5,5), interval(-5,5)});
    box<2> s = opt.solve(b);

    interval tolerance(-1e-5,1e-5);
    EXPECT_THAT(contains(0.0 + tolerance, opt.minimum()), Eq(true));
    EXPECT_THAT(contains(s[0] + tolerance, 1.0), Eq(true));
    EXPECT_THAT(contains(s[1] + tolerance, 1.0), Eq(true));
    EXPECT_THAT(diam(s[0]), Lt(1e-6));
    EXPECT_THAT(diam(s[1]), Lt(1e-6));

    std::cout << "CalcTime: " << opt.time() << "\n";
    std::cout << "Boxes: " << opt.box_count() << "\n";
}

TEST_F(AnOptimizer, canSolveRosenbrockFunctionIn2DProvidingFirstAndSecondDerivative) {
    options_t o;
    o.epsilon = 1e-6;
    optimizer<2> opt(rosenbrock2d, o);
    opt.set_first_derivative(rosenbrock2d_d);
    opt.set_second_derivative(rosenbrock2d_dd);

    box<2> b({interval(-5,5), interval(-5,5)});
    box<2> s = opt.solve(b);

    interval tolerance(-1e-5,1e-5);
    EXPECT_THAT(contains(0.0 + tolerance, opt.minimum()), Eq(true));
    EXPECT_THAT(contains(s[0] + tolerance, 1.0), Eq(true));
    EXPECT_THAT(contains(s[1] + tolerance, 1.0), Eq(true));
    EXPECT_THAT(diam(s[0]), Lt(1e-6));
    EXPECT_THAT(diam(s[1]), Lt(1e-6));

    std::cout << "CalcTime: " << opt.time() << "\n";
    std::cout << "Boxes: " << opt.box_count() << "\n";
}
