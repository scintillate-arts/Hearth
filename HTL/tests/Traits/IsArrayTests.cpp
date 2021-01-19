#include <gtest/gtest.h>
#include <htl/Traits/IsArray.hpp>

TEST(IsArrayTests, isArrayWithSize) {
  constexpr auto val = htl::IsArray_V<int[10]>;
  ASSERT_TRUE(val);
}

TEST(IsArrayTests, isArrayWithoutSize) {
  constexpr auto val = htl::IsArray_V<int[]>;
  ASSERT_TRUE(val);
}

TEST(IsArrayTests, isNotArray) {
  constexpr auto val = htl::IsArray_V<int>;
  ASSERT_FALSE(val);
}
