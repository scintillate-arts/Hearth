#include <gtest/gtest.h>
#include <htl/Traits/IsSame.hpp>
#include <htl/Traits/RemoveVolatile.hpp>

TEST(RemoveVolatileTests, canRemoveVolatileProperly) {
  constexpr auto val = htl::IsSame_V<int, htl::RemoveVolatile_T<volatile int>>;
  ASSERT_TRUE(val);
}
