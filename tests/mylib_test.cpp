#include "mylib.hpp"

#include <gtest/gtest.h>

TEST(MylibTest, Greet) {
  EXPECT_EQ(mylib::greet("World"), "Hello, World!");
  EXPECT_EQ(mylib::greet("CMake"), "Hello, CMake!");
}
