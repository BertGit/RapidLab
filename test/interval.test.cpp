#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "interval/interval.hpp"
#include "interval/io.hpp"

using namespace rapidlab;
using namespace testing;

class AnInterval : public Test
{
     virtual void SetUp() {
         // Initialize every test by rounding to infinity
         _MM_SET_ROUNDING_MODE(_MM_ROUND_UP);
     }
};

TEST_F(AnInterval, defaultInitializedToZero)
{
    interval v;
    EXPECT_THAT(v, Eq(interval(0)));
}

TEST_F(AnInterval, canBeConstructedFromSingleValue)
{
    interval v(2);
    EXPECT_THAT(v.lower(), Eq(2));
    EXPECT_THAT(v.upper(), Eq(2));
}

TEST_F(AnInterval, canBeConstructedFromLowerAndUpperBound)
{
    interval v(1,2);
    EXPECT_THAT(v.lower(), Eq(1));
    EXPECT_THAT(v.upper(), Eq(2));
}

TEST_F(AnInterval, hasAdditionOperation)
{
    interval a(-1,1);
    interval b(2,3);
    interval c = a + b;

    EXPECT_THAT(c.lower(), DoubleEq(1));
    EXPECT_THAT(c.upper(), DoubleEq(4));
}

TEST_F(AnInterval, hasAdditionOperationWithCorrectRounding)
{
    interval a(1e-3,1e-3);
    interval b(0,0);
    for (size_t i = 0; i < 1e3; ++i)
        b += a;

    // Bounds include all possible values
    EXPECT_THAT(b.lower(), Lt(1));
    EXPECT_THAT(b.upper(), Gt(1));
}

TEST_F(AnInterval, hasSubtractionOperation)
{
    interval a(-1,1);
    interval b(2,3);
    interval c = a - b;

    EXPECT_THAT(c.lower(), DoubleEq(-4));
    EXPECT_THAT(c.upper(), DoubleEq(-1));
}

TEST_F(AnInterval, hasSubtractionOperationWithCorrectRounding)
{
    interval a(1e-3,1e-3);
    interval b(2,2);
    for (size_t i = 0; i < 1e3; ++i)
        b -= a;

    // Bounds include all possible values
    EXPECT_THAT(b.lower(), Lt(1));
    EXPECT_THAT(b.upper(), Gt(1));
}

TEST_F(AnInterval, hasMultiplicationOperationBothIntervalsPositive)
{
    interval a(1,2);
    interval b(2,5);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(2));
    EXPECT_THAT(c.upper(), DoubleEq(10));
}

TEST_F(AnInterval, hasMultiplicationOperationBothIntervalsNegative)
{
    interval a(-2,-1);
    interval b(-5,-2);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(2));
    EXPECT_THAT(c.upper(), DoubleEq(10));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalNegativeOnePositive)
{
    interval a(-2,-1);
    interval b(3,6);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-12));
    EXPECT_THAT(c.upper(), DoubleEq(-3));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalPositiveOneNegative)
{
    interval a(3,6);
    interval b(-2,-1);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-12));
    EXPECT_THAT(c.upper(), DoubleEq(-3));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalSpansZeroOnePositive)
{
    interval a(-2,1);
    interval b(2,3);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-6));
    EXPECT_THAT(c.upper(), DoubleEq(3));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalPositiveOneSpansZero)
{
    interval a(2,3);
    interval b(-2,1);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-6));
    EXPECT_THAT(c.upper(), DoubleEq(3));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalSpansZeroOneNegative)
{
    interval a(-2,1);
    interval b(-3,-2);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-3));
    EXPECT_THAT(c.upper(), DoubleEq(6));
}

TEST_F(AnInterval, hasMultiplicationOperationOneIntervalNegativeOneSpansZero)
{
    interval a(-3,-2);
    interval b(-2,1);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-3));
    EXPECT_THAT(c.upper(), DoubleEq(6));
}

TEST_F(AnInterval, hasMultiplicationOperationBothIntervalsSpanZero)
{
    interval a(-5,2);
    interval b(-3,4);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-20));
    EXPECT_THAT(c.upper(), DoubleEq(15));
}

TEST_F(AnInterval, hasMultiplicationOperationBothIntervalsSpanZero2)
{
    interval a(-5,6);
    interval b(-3,2);
    interval c = a * b;

    EXPECT_THAT(c.lower(), DoubleEq(-18));
    EXPECT_THAT(c.upper(), DoubleEq(15));
}
