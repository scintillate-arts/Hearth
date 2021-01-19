#include <gtest/gtest.h>
#include <htl/Traits/IsSame.hpp>
#include <htl/Traits/RemoveConstVolatile.hpp>

TEST(RemoveConstVolatileTests, canRemoveConstProperly) {
  constexpr auto val = htl::IsSame_V<int, htl::RemoveConstVolatile_T<const int>>;
  ASSERT_TRUE(val);
}

TEST(RemoveConstVolatileTests, canRemoveVolatileProperly) {
  constexpr auto val = htl::IsSame_V<int, htl::RemoveConstVolatile_T<volatile int>>;
  ASSERT_TRUE(val);
}

TEST(RemoveConstVolatileTests, canRemoveBothProperly) {
  constexpr auto val = htl::IsSame_V<int, htl::RemoveConstVolatile_T<const volatile int>>;
  ASSERT_TRUE(val);
}
