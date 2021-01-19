#include <gtest/gtest.h>
#include <htl/Traits/AddConstVolatile.hpp>
#include <htl/Traits/IsSame.hpp>

TEST(AddConstVolatileTests, canAddConstVolatileToType) {
  constexpr auto val = htl::IsSame_V<const volatile int, htl::AddConstVolatile_T<int>>;
  ASSERT_TRUE(val);
}
