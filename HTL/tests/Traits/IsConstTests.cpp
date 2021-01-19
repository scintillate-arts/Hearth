#include <gtest/gtest.h>
#include <htl/Traits/IsConst.hpp>

TEST(IsConstTests, isConstTypeWithoutPointer) {
  constexpr auto val = htl::IsConst_V<const int>;
  ASSERT_TRUE(val);
}

TEST(IsConstTests, isConstTypeWithPointer) {
  constexpr auto val = htl::IsConst_V<int* const>;
  ASSERT_TRUE(val);
}

TEST(IsConstTests, isNotConstTypeWithoutPointer) {
  constexpr auto val = htl::IsConst_V<int>;
  ASSERT_FALSE(val);
}
