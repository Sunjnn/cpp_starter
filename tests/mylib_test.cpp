#include <gtest/gtest.h>

#include "mylib.hpp"

using mylib::greet;

TEST(MylibTest, Greet) {
  EXPECT_EQ(greet("World"), "Hello, World!");
  EXPECT_EQ(greet("CMake"), "Hello, CMake!");
}
