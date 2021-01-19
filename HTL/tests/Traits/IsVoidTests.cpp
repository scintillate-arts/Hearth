#include <gtest/gtest.h>
#include <htl/Traits/IsSame.hpp>
#include <htl/Traits/IsVoid.hpp>

TEST(IsVoidTests, isVoidType) {
  constexpr auto val = htl::IsVoid_V<void>;
  ASSERT_TRUE(val);
}

TEST(IsVoidTests, isNotVoidType) {
  constexpr auto val = htl::IsVoid_V<int>;
  ASSERT_FALSE(val);
}
