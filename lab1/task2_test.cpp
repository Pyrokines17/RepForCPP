#include "task2.h"
#include "gtest/gtest.h"

namespace {

// TODO: переименовать тесты, написать реализации

TEST(SayTest, say_hello) {
    using say::say_hello;

    EXPECT_EQ(say_hello("world"), "Hello, world!");
    EXPECT_EQ(say_hello("friend"), "Hello, friend!");
    EXPECT_EQ(say_hello("bro"), "Hello, bro!");
    EXPECT_EQ(say_hello("mom"), "Hello, mom!");
}

}