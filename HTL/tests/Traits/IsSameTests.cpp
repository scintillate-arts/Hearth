#include <gtest/gtest.h>
#include <htl/Traits/IsSame.hpp>

TEST(IsSameTests, checkTwoTypesAreTheSame) {
  constexpr auto val = htl::IsSame_V<int, int>;
  ASSERT_TRUE(val);
}

TEST(IsSameTests, checkTwoTypesAreNotTheSame) {
  constexpr auto val = htl::IsSame_V<int, float>;
  ASSERT_FALSE(val);
}
