#include <gtest/gtest.h>
#include <htl/Traits/IsVolatile.hpp>

TEST(IsVolatileTests, isVolatileTypeWithoutPointer) {
  constexpr auto val = htl::IsVolatile_V<volatile int>;
  ASSERT_TRUE(val);
}

TEST(IsVolatileTests, isVolatileTypeWithPointer) {
  constexpr auto val = htl::IsVolatile_V<int* volatile>;
  ASSERT_TRUE(val);
}

TEST(IsVolatileTests, isNotVolatileTypeWithoutPointer) {
  constexpr auto val = htl::IsVolatile_V<int>;
  ASSERT_FALSE(val);
}
