
#include "sand.h"

#include "gtest/gtest.h"

TEST(IsStringLong, ShortString) {
  int result = Parse("5");
  EXPECT_EQ(result, 5);
}
