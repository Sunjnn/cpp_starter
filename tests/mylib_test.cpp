#include "mylib.hpp"

#include <gtest/gtest.h>

using mylib::greet;

TEST(MylibTest, Greet) {
  EXPECT_EQ(greet("World"), "Hello, World!");
  EXPECT_EQ(greet("CMake"), "Hello, CMake!");
}
