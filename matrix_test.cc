#include "matrix.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(Matrix, Create) {
  Matrix m(4, 4);
  EXPECT_EQ(0, m.at(0, 0));
  EXPECT_EQ(0, m.at(3, 3));

  IdentityMatrix im(4);
  EXPECT_EQ(1, im.at(0, 0));
  EXPECT_EQ(1, im.at(3, 3));

  VandermondeMatrix vm(4, 4);
  EXPECT_NE(0, vm.at(0, 0));
  EXPECT_NE(0, vm.at(3, 3));
}