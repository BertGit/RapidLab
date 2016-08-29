#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "interval/interval.hpp"
#include "interval/io.hpp"

#include <iomanip>
#include <cfenv>

using namespace rapidlab;
using namespace testing;

class AnInterval : public Test {
public:
    double aTenth_lb, aTenth_ub;
    interval aTenthInterval;

    void SetUp() override final {
        // Initialize double variables with lower and upper floating point bounds
        std::fesetround(FE_DOWNWARD);
        aTenth_lb = 1.0; aTenth_lb /= 10.0;
        std::fesetround(FE_UPWARD);
        aTenth_ub = 1.0; aTenth_ub /= 10.0;
        // Generate interval include 0.1
        aTenthInterval = interval(aTenth_lb, aTenth_ub);
        // Reset to default
        std::fesetround(FE_TONEAREST);
        // Initialize every test by rounding to infinity
        _MM_SET_ROUNDING_MODE(_MM_ROUND_UP);
    }
};

TEST_F(AnInterval, defaultInitializesToZero) {
    interval v;
    EXPECT_THAT(v, Eq(interval(0)));
}

TEST_F(AnInterval, canBeConstructedFromSingleValue) {
    interval v(2);
    EXPECT_THAT(v.lower(), Eq(2));
    EXPECT_THAT(v.upper(), Eq(2));
}

TEST_F(AnInterval, canBeConstructedFromLowerAndUpperBound) {
    interval v(1,2);
    EXPECT_THAT(v.lower(), Eq(1));
    EXPECT_THAT(v.upper(), Eq(2));
}

TEST_F(AnInterval, hasAdditionOperation) {
    interval a(-1,1);
    interval b(2,3);
    interval c = a + b;

    EXPECT_THAT(c.lower(), DoubleEq(1));
    EXPECT_THAT(c.upper(), DoubleEq(4));
}

TEST_F(AnInterval, hasAdditionOperationWithCorrectRounding) {
    interval a(0);
    for (size_t i = 0; i < 10; ++i)
        a += aTenthInterval;

    EXPECT_THAT(a.lower(), Lt(1));
    EXPECT_THAT(a.upper(), Gt(1));
}

TEST_F(AnInterval, hasSubtractionOperation) {
    interval a(-1,1);
    interval b(2,3);
    interval c = a - b;

    EXPECT_THAT(c.lower(), DoubleEq(-4));
    EXPECT_THAT(c.upper(), DoubleEq(-1));
}

TEST_F(AnInterval, hasSubtractionOperationWithCorrectRounding) {
    interval a(1);
    for (size_t i = 0; i < 10; ++i)
        a -= aTenthInterval;

    EXPECT_THAT(a.lower(), Lt(0));
    EXPECT_THAT(a.upper(), Gt(0));
}

TEST_F(AnInterval, hasMultiplicationOperationBothIntervalsPositive) {
    interval a(1,2);
    interval b(2,5);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(2));
    EXPECT_THAT(c.upper(), DoubleEq(10));
}

TEST_F(AnInterval, hasMultiplicationOperationBothIntervalsPositiveCorrectRounding) {
    interval a = 1 + aTenthInterval;
    EXPECT_THAT(a.lower(), DoubleEq(1.1));
    EXPECT_THAT(a.upper(), DoubleEq(1.1));

    a = a * a;

    EXPECT_THAT(a.lower(), Lt(1.21));
    EXPECT_THAT(a.upper(), Gt(1.21));
}

TEST_F(AnInterval, hasMultiplicationOperationBothIntervalsNegative) {
    interval a(-2,-1);
    interval b(-5,-2);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(2));
    EXPECT_THAT(c.upper(), DoubleEq(10));
}

TEST_F(AnInterval, hasMultiplicationOperationBothIntervalsNegativeCorrectRounding) {
    interval a = 1 + aTenthInterval;
    a = -a;
    EXPECT_THAT(a.lower(), DoubleEq(-1.1));
    EXPECT_THAT(a.upper(), DoubleEq(-1.1));

    a = a * a;

    EXPECT_THAT(a.lower(), Lt(1.21));
    EXPECT_THAT(a.upper(), Gt(1.21));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalNegativeOnePositive) {
    interval a(-2,-1);
    interval b(3,6);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-12));
    EXPECT_THAT(c.upper(), DoubleEq(-3));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalNegativeOnePositiveCorrectRounding) {
    interval a = 1 + aTenthInterval;
    interval b = -1 - aTenthInterval;

    interval c = a * b;

    EXPECT_THAT(c.lower(), Lt(-1.21));
    EXPECT_THAT(c.upper(), Gt(-1.21));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalPositiveOneNegative) {
    interval a(3,6);
    interval b(-2,-1);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-12));
    EXPECT_THAT(c.upper(), DoubleEq(-3));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalPositiveOneNegativeCorrectRounding) {
    interval a = -1 - aTenthInterval;
    interval b = 1 + aTenthInterval;

    interval c = a * b;

    EXPECT_THAT(c.lower(), Lt(-1.21));
    EXPECT_THAT(c.upper(), Gt(-1.21));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalSpansZeroOnePositive) {
    interval a(-2,1);
    interval b(2,3);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-6));
    EXPECT_THAT(c.upper(), DoubleEq(3));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalSpansZeroOnePositiveCorrectRounding) {
    interval a(-(1 + aTenthInterval.upper()), 1 + aTenthInterval.upper());
    interval b = 1 + aTenthInterval;

    interval c = a * b;

    EXPECT_THAT(c.lower(), Lt(-1.21));
    EXPECT_THAT(c.upper(), Gt(1.21));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalPositiveOneSpansZero) {
    interval a(2,3);
    interval b(-2,1);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-6));
    EXPECT_THAT(c.upper(), DoubleEq(3));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalPositiveOneSpansZeroCorrectRounding) {
    interval a = 1 + aTenthInterval;
    interval b(-(1 + aTenthInterval.upper()), 1 + aTenthInterval.upper());

    interval c = a * b;

    EXPECT_THAT(c.lower(), Lt(-1.21));
    EXPECT_THAT(c.upper(), Gt(1.21));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalSpansZeroOneNegative) {
    interval a(-2,1);
    interval b(-3,-2);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-3));
    EXPECT_THAT(c.upper(), DoubleEq(6));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalSpansZeroOneNegativeCorrectRounding) {
    interval a(-(1 + aTenthInterval.upper()), 1 + aTenthInterval.upper());
    interval b = -interval(1 + aTenthInterval);

    interval c = a * b;

    EXPECT_THAT(c.lower(), Lt(-1.21));
    EXPECT_THAT(c.upper(), Gt(1.21));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalNegativeOneSpansZero) {
    interval a(-3,-2);
    interval b(-2,1);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-3));
    EXPECT_THAT(c.upper(), DoubleEq(6));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalNegativeOneSpansZeroCorrectRounding) {
    interval a = -interval(1 + aTenthInterval);
    interval b(-(1 + aTenthInterval.upper()), 1 + aTenthInterval.upper());

    interval c = a * b;

    EXPECT_THAT(c.lower(), Lt(-1.21));
    EXPECT_THAT(c.upper(), Gt(1.21));
}

TEST_F(AnInterval, hasMultiplicationOperationBothIntervalsSpanZero) {
    interval a(-5,2);
    interval b(-3,4);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-20));
    EXPECT_THAT(c.upper(), DoubleEq(15));
}

TEST_F(AnInterval, hasMultiplicationOperationBothIntervalsSpanZero2) {
    interval a(-5,7);
    interval b(-3,4);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-21));
    EXPECT_THAT(c.upper(), DoubleEq(28));
}

TEST_F(AnInterval, hasMultiplicationOperationBothIntervalsSpanZeroCorrectRounding) {
    interval a(-(1 + aTenthInterval.upper()), 1 + aTenthInterval.upper());
    interval b(-(1 + aTenthInterval.upper()), 1 + aTenthInterval.upper());

    interval c = a * b;

    EXPECT_THAT(c.lower(), Lt(-1.21));
    EXPECT_THAT(c.upper(), Gt(1.21));
}

TEST_F(AnInterval, hasDivisionOperationResultingInInfinityForDenominatorSpanningZero) {
    interval a(1);
    interval b(-1,1);

    interval c = a / b;

    EXPECT_THAT(c.lower(), DoubleEq(-INFINITY));
    EXPECT_THAT(c.upper(), DoubleEq(INFINITY));
}

TEST_F(AnInterval, hasDivisionOperationNumeratorPositiveDenominatorPositive) {
    interval a(1,3);
    interval b(2,4);

    interval c = a / b;

    EXPECT_THAT(c.lower(), DoubleEq(0.25));
    EXPECT_THAT(c.upper(), DoubleEq(1.5));
}

TEST_F(AnInterval, hasDivisionOperationNumeratorPositiveDenominatorPositiveCorrectRounding) {
    interval a(1);
    interval b = aTenthInterval;

    interval c = a / b;

    EXPECT_THAT(c.lower(), Lt(10));
    EXPECT_THAT(c.upper(), Gt(10));
}

TEST_F(AnInterval, hasDivisionOperationNumeratorPositiveDenominatorNegative) {
    interval a(1,3);
    interval b(-4,-2);

    interval c = a / b;

    EXPECT_THAT(c.lower(), DoubleEq(-1.5));
    EXPECT_THAT(c.upper(), DoubleEq(-0.25));
}

TEST_F(AnInterval, hasDivisionOperationNumeratorPositiveDenominatorNegativeCorrectRounding) {
    interval a(1);
    interval b = -aTenthInterval;

    interval c = a / b;

    EXPECT_THAT(c.lower(), Lt(-10));
    EXPECT_THAT(c.upper(), Gt(-10));
}

TEST_F(AnInterval, hasDivisionOperationNumeratorNegativeDenominatorPositive) {
    interval a(-4,-3);
    interval b(4,6);

    interval c = a / b;

    EXPECT_THAT(c.lower(), DoubleEq(-1));
    EXPECT_THAT(c.upper(), DoubleEq(-0.5));
}

TEST_F(AnInterval, hasDivisionOperationNumeratorNegativeDenominatorPositiveCorrectRounding) {
    interval a(-1);
    interval b = aTenthInterval;

    interval c = a / b;

    EXPECT_THAT(c.lower(), Lt(-10));
    EXPECT_THAT(c.upper(), Gt(-10));
}

TEST_F(AnInterval, hasDivisionOperationNumeratorSpansZeroDenominatorPositive) {
    interval a(-4,2);
    interval b(4,6);

    interval c = a / b;

    EXPECT_THAT(c.lower(), DoubleEq(-1));
    EXPECT_THAT(c.upper(), DoubleEq(0.5));
}

TEST_F(AnInterval, hasDivisionOperationNumeratorSpansZeroDenominatorPositiveCorrectRounding) {
    interval a(-1,1);
    interval b = aTenthInterval;

    interval c = a / b;

    EXPECT_THAT(c.lower(), Lt(-10));
    EXPECT_THAT(c.upper(), Gt(10));
}

TEST_F(AnInterval, hasDivisionOperationNumeratorSpansZeroDenominatorNegative) {
    interval a(-4,2);
    interval b(-8,-4);

    interval c = a / b;

    EXPECT_THAT(c.lower(), DoubleEq(-0.5));
    EXPECT_THAT(c.upper(), DoubleEq(1));
}

TEST_F(AnInterval, hasDivisionOperationNumeratorSpansZeroDenominatorNegativeCorrectRounding) {
    interval a(-1,1);
    interval b = -aTenthInterval;

    interval c = a / b;

    EXPECT_THAT(c.lower(), Lt(-10));
    EXPECT_THAT(c.upper(), Gt(10));
}

TEST_F(AnInterval, hasSquareRootOperationResultingInNanForIntervalSpanningZero) {
    interval a(-1,1);
    interval c = sqrt(a);

    EXPECT_THAT(std::isnan(c.lower()), Eq(true));
    EXPECT_THAT(std::isnan(c.upper()), Eq(true));

    interval b(-2,-1);
    interval d = sqrt(b);

    EXPECT_THAT(std::isnan(d.lower()), Eq(true));
    EXPECT_THAT(std::isnan(d.upper()), Eq(true));
}

TEST_F(AnInterval, hasSquareRootOperationForPositiveIntervals) {
    interval a(4,36);
    interval c = sqrt(a);

    EXPECT_THAT(c.lower(), DoubleEq(2));
    EXPECT_THAT(c.upper(), DoubleEq(6));
}

TEST_F(AnInterval, hasSquareRootOperationForPositiveIntervalsCorrectRounding) {
    interval a = aTenthInterval / 10;
    interval c = sqrt(a);

    double doubleResult = std::sqrt(0.01);

    EXPECT_THAT(c.lower(), Lt(doubleResult));
    EXPECT_THAT(c.upper(), Gt(doubleResult));
}

// By comparing square results to multiplication results,
// rounding correctness is implicitly checked
TEST_F(AnInterval, hasSquareOperationForPositiveIntervals) {
    interval a(0.1,4.1);
    interval c = sqr(a);

    EXPECT_THAT(c, Eq(a*a));
}

TEST_F(AnInterval, hasSquareOperationForNegativeIntervals) {
    interval a(-4.1,-0.1);
    interval c = sqr(a);

    EXPECT_THAT(c, Eq(a*a));
}

TEST_F(AnInterval, hasSquareOperationForIntervalSpanningZero) {
    interval a(-4.1,0.1);
    interval c = sqr(a);

    interval d = a * a;
    d.set_lower(0.0);
    EXPECT_THAT(c, Eq(d));
}
