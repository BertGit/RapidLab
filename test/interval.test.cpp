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
