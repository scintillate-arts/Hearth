#include <gtest/gtest.h>
#include <htl/Traits/AddVolatile.hpp>
#include <htl/Traits/IsSame.hpp>

TEST(AddVolatileTests, canAddVolatileToType) {
  constexpr auto val = htl::IsSame_V<volatile int, htl::AddVolatile_T<int>>;
  ASSERT_TRUE(val);
}
