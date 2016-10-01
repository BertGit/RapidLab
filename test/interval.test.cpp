#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "interval/core.hpp"
#include <boost/timer.hpp>

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

class APositiveInterval : public AnInterval {};
class ANegativeInterval : public AnInterval {};
class AZeroSpanInterval : public AnInterval {};

////////////////////
// INITIALIZATION //
////////////////////
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

//////////////
// ADDITION //
//////////////
TEST_F(AnInterval, canBeAddedToAScalarAndViceVersa) {
    interval a(-1,2);
    double b = 3;
    interval c = a + b;
    interval d = b + a;

    EXPECT_THAT(c.lower(), DoubleEq(2));
    EXPECT_THAT(c.upper(), DoubleEq(5));
    EXPECT_THAT(d, Eq(c));
}

TEST_F(AnInterval, canBeAddedToAnInterval) {
    interval a(-1,1);
    interval b(2,3);
    interval c = a + b;
    interval d = b + a;

    EXPECT_THAT(c.lower(), DoubleEq(1));
    EXPECT_THAT(c.upper(), DoubleEq(4));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(AnInterval, canBeAddedToAnIntervalHavingCorrectRounding) {
    interval a(0);
    for (size_t i = 0; i < 10; ++i)
        a += aTenthInterval;

    EXPECT_THAT(a.lower(), Lt(1));
    EXPECT_THAT(a.upper(), Gt(1));
}

/////////////////
// SUBTRACTION //
/////////////////
TEST_F(AnInterval, canBeSubtractedFromAScalarAndViceVersa) {
    interval a(-1,1);
    double b = 3;
    interval c = a - b;
    interval d = b - a;

    EXPECT_THAT(c.lower(), DoubleEq(-4));
    EXPECT_THAT(c.upper(), DoubleEq(-2));
    EXPECT_THAT(d.lower(), DoubleEq(2));
    EXPECT_THAT(d.upper(), DoubleEq(4));
}

TEST_F(AnInterval, canBeSubtractedFromAnInterval) {
    interval a(-1,1);
    interval b(2,3);
    interval c = a - b;
    interval d = b - a;

    EXPECT_THAT(c.lower(), DoubleEq(-4));
    EXPECT_THAT(c.upper(), DoubleEq(-1));
    EXPECT_THAT(d.lower(), DoubleEq(1));
    EXPECT_THAT(d.upper(), DoubleEq(4));
}

TEST_F(AnInterval, canBeSubtracedFromAnIntervalHavingCorrectRounding) {
    interval a(1);
    for (size_t i = 0; i < 10; ++i)
        a -= aTenthInterval;

    EXPECT_THAT(a.lower(), Lt(0));
    EXPECT_THAT(a.upper(), Gt(0));
}

////////////////////
// MULTIPLICATION //
////////////////////
TEST_F(APositiveInterval, canBeMultipliedWithAPositiveScalarAndViceVersa) {
    interval a(2,3);
    double b = 2;
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(4));
    EXPECT_THAT(c.upper(), DoubleEq(6));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(APositiveInterval, canBeMultipliedWithANegativeScalarAndViceVersa) {
    interval a(2,3);
    double b = -2;
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(-6));
    EXPECT_THAT(c.upper(), DoubleEq(-4));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(ANegativeInterval, canBeMultipliedWithAPositiveScalarAndViceVersa) {
    interval a(-3,-2);
    double b = 2;
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(-6));
    EXPECT_THAT(c.upper(), DoubleEq(-4));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(ANegativeInterval, canBeMultipliedWithANegativeScalarAndViceVersa) {
    interval a(-3,-2);
    double b = -2;
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(4));
    EXPECT_THAT(c.upper(), DoubleEq(6));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(AZeroSpanInterval, canBeMultipliedWithAPositiveScalarAndViceVersa) {
    interval a(-3,2);
    double b = 2;
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(-6));
    EXPECT_THAT(c.upper(), DoubleEq(4));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(AZeroSpanInterval, canBeMultipliedWithANegativeScalarAndViceVersa) {
    interval a(-3,2);
    double b = -2;
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(-4));
    EXPECT_THAT(c.upper(), DoubleEq(6));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(APositiveInterval, canBeMultipliedWithAPositiveInterval) {
    interval a(1,2);
    interval b(2,5);
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(2));
    EXPECT_THAT(c.upper(), DoubleEq(10));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(APositiveInterval, canBeMultipliedWithAPositiveIntervalHavingCorrectRounding) {
    interval a = 1 + aTenthInterval;
    EXPECT_THAT(a.lower(), DoubleEq(1.1));
    EXPECT_THAT(a.upper(), DoubleEq(1.1));

    a = a * a;

    EXPECT_THAT(a.lower(), Lt(1.21));
    EXPECT_THAT(a.upper(), Gt(1.21));
}

TEST_F(ANegativeInterval, canBeMultipliedWithANegativeInterval) {
    interval a(-2,-1);
    interval b(-5,-2);
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(2));
    EXPECT_THAT(c.upper(), DoubleEq(10));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(ANegativeInterval, canBeMultipliedWithANegativeIntervalHavingCorrectRounding) {
    interval a = 1 + aTenthInterval;
    a = -a;
    EXPECT_THAT(a.lower(), DoubleEq(-1.1));
    EXPECT_THAT(a.upper(), DoubleEq(-1.1));

    a = a * a;

    EXPECT_THAT(a.lower(), Lt(1.21));
    EXPECT_THAT(a.upper(), Gt(1.21));
}

TEST_F(APositiveInterval, canBeMultipliedWithANegativeInterval) {
    interval a(-2,-1);
    interval b(3,6);
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(-12));
    EXPECT_THAT(c.upper(), DoubleEq(-3));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(APositiveInterval, canBeMultipliedWithANegativeIntervalHavingCorrectRounding) {
    interval a = 1 + aTenthInterval;
    interval b = -1 - aTenthInterval;

    interval c = a * b;

    EXPECT_THAT(c.lower(), Lt(-1.21));
    EXPECT_THAT(c.upper(), Gt(-1.21));
}

TEST_F(AZeroSpanInterval, canBeMultipliedWithAPositiveInterval) {
    interval a(-2,1);
    interval b(2,3);
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(-6));
    EXPECT_THAT(c.upper(), DoubleEq(3));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(AZeroSpanInterval, canBeMultipliedWithAPositiveIntervalHavingCorrectRounding) {
    interval a(-(1 + aTenthInterval.upper()), 1 + aTenthInterval.upper());
    interval b = 1 + aTenthInterval;

    interval c = a * b;

    EXPECT_THAT(c.lower(), Lt(-1.21));
    EXPECT_THAT(c.upper(), Gt(1.21));
}

TEST_F(AZeroSpanInterval, canBeMultipliedWithANegativeInterval) {
    interval a(-2,1);
    interval b(-3,-2);
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(-3));
    EXPECT_THAT(c.upper(), DoubleEq(6));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(AZeroSpanInterval, canBeMultipliedWithANegativeIntervalHavingCorrectRounding) {
    interval a(-(1 + aTenthInterval.upper()), 1 + aTenthInterval.upper());
    interval b = -interval(1 + aTenthInterval);

    interval c = a * b;

    EXPECT_THAT(c.lower(), Lt(-1.21));
    EXPECT_THAT(c.upper(), Gt(1.21));
}

TEST_F(AZeroSpanInterval, canBeMultipliedWithAZeroSpanInterval) {
    interval a(-5,2);
    interval b(-3,4);
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(-20));
    EXPECT_THAT(c.upper(), DoubleEq(15));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(AZeroSpanInterval, canBeMultipliedWithAZeroSpanInterval2) {
    interval a(-5,7);
    interval b(-3,4);
    interval c = a * b;
    interval d = b * a;

    EXPECT_THAT(c.lower(), DoubleEq(-21));
    EXPECT_THAT(c.upper(), DoubleEq(28));
    EXPECT_THAT(c, Eq(d));
}

TEST_F(AZeroSpanInterval, canBeMultipliedWithAZeroSpanIntervalHavingCorrectRounding) {
    interval a(-(1 + aTenthInterval.upper()), 1 + aTenthInterval.upper());
    interval b(-(1 + aTenthInterval.upper()), 1 + aTenthInterval.upper());
    interval c = a * b;

    EXPECT_THAT(c.lower(), Lt(-1.21));
    EXPECT_THAT(c.upper(), Gt(1.21));
}

//////////////
// DIVISION //
//////////////
TEST_F(AnInterval, canBeDividedByAZeroScalarResultingInInfinity) {
    interval a(2,3);
    double b = 0;
    interval c = a / b;

    EXPECT_THAT(c.lower(), DoubleEq(-INFINITY));
    EXPECT_THAT(c.upper(), DoubleEq(INFINITY));
}

// Division is implemented in terms of multiplication with a reciprocal
// Checking for reciprocal correctness will suffice
TEST_F(APositiveInterval, hasCorrectReciprocal) {
    interval a(2,4);

    interval c = 1.0 / a;

    EXPECT_THAT(c.lower(), DoubleEq(0.25));
    EXPECT_THAT(c.upper(), DoubleEq(0.5));
}

TEST_F(APositiveInterval, hasCorrectReciprocalHavingCorrectRounding) {
    interval a = aTenthInterval;

    interval c = 1.1 / a;

    EXPECT_THAT(c.lower(), Lt(11));
    EXPECT_THAT(c.upper(), Gt(11));
}

TEST_F(ANegativeInterval, hasCorrectReciprocal) {
    interval a(-4,-2);

    interval c = 1.0 / a;

    EXPECT_THAT(c.lower(), DoubleEq(-0.5));
    EXPECT_THAT(c.upper(), DoubleEq(-0.25));
}

TEST_F(ANegativeInterval, hasCorrectReciprocalHavingCorrectRounding) {
    interval a = -aTenthInterval;

    interval c = 1.1 / a;

    EXPECT_THAT(c.lower(), Lt(-11));
    EXPECT_THAT(c.upper(), Gt(-11));
}

/////////////////////
// SQUARE and SQRT //
/////////////////////
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

TEST_F(AnInterval, hasSquareRootOperationForPositiveIntervalsHavingCorrectRounding) {
    interval a = aTenthInterval / 10;
    interval c = sqrt(a);

    EXPECT_THAT(c.lower(), Lt(0.1));
    EXPECT_THAT(c.upper(), Gt(0.1));
}

// By comparing square results to multiplication results,
// rounding correctness is implicitly checked
TEST_F(APositiveInterval, canBeSquared) {
    interval a(0.1,4.1);
    interval c = sqr(a);

    EXPECT_THAT(c, Eq(a*a));
}

TEST_F(ANegativeInterval, canBeSquared) {
    interval a(-4.1,-0.1);
    interval c = sqr(a);

    EXPECT_THAT(c, Eq(a*a));
}

TEST_F(AZeroSpanInterval, canBeSquared) {
    interval a(-4.1,0.1);
    interval c = sqr(a);

    interval d = a * a;
    d.set_lower(0.0);
    EXPECT_THAT(c, Eq(d));
}

/////////
// ABS //
/////////
// Rounding implicitly checked
TEST_F(APositiveInterval, hasAbsolute) {
    interval a(0.1,4.1);
    interval c = abs(a);

    EXPECT_THAT(c, Eq(a));
}

TEST_F(ANegativeInterval, hasAbsolute) {
    interval a(-4.1,-0.1);
    interval c = abs(a);

    EXPECT_THAT(c, Eq(-a));
}

TEST_F(AZeroSpanInterval, hasAbsolute) {
    interval a(-4.1,0.1);
    interval c = abs(a);

    interval d = -a;
    d.set_lower(0.0);
    EXPECT_THAT(c, Eq(d));
}

////////////////
// PROPERTIES //
////////////////
TEST_F(AnInterval, hasMidPoint) {
    interval a(2,4);
    EXPECT_THAT(mid(a), DoubleEq(3));
}

TEST_F(AnInterval, hasDiameter) {
    interval a(-1,4);
    EXPECT_THAT(diam(a), DoubleEq(5));
}

TEST_F(AnInterval, hasMagnitude) {
    interval a(-1,4);
    interval b(-5,4);
    EXPECT_THAT(mag(a), DoubleEq(4));
    EXPECT_THAT(mag(b), DoubleEq(5));
}

TEST_F(AnInterval, hasCheckWhetherItSpansZero) {
    interval a(-1,4);
    interval b(-5,-4);
    interval c(2,3);
    EXPECT_THAT(zero_in(a), Eq(true));
    EXPECT_THAT(zero_in(b), Eq(false));
    EXPECT_THAT(zero_in(c), Eq(false));
}

TEST_F(AnInterval, hasIntersection) {
    interval a(2,4);
    interval b(-5,3);
    interval c(-8, -1);
    interval d = intersect(a,b);
    interval e = intersect(b,c);
    interval f = intersect(a,c);
    EXPECT_THAT(d, Eq(interval(2,3)));
    EXPECT_THAT(e, Eq(interval(-5,-1)));
    EXPECT_THAT(std::isnan(f.lower()), Eq(true));
    EXPECT_THAT(std::isnan(f.upper()), Eq(true));
}
