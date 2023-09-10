#include "task1.h"
#include "gtest/gtest.h"

namespace {

// TODO: переименовать тесты, написать реализации

TEST(SumTest, plus) {
    using sum::plus;

    EXPECT_EQ(plus(2, 3), 5);
    EXPECT_EQ(plus(2, 4), 6);
    EXPECT_EQ(plus(2, 5), 7);
    EXPECT_EQ(plus(2, 6), 8);
}

}