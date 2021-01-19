#include <gtest/gtest.h>
#include <htl/Traits/IsSame.hpp>
#include <htl/Traits/RemoveConst.hpp>

TEST(RemoveConstTests, canRemoveConstProperly) {
  constexpr auto val = htl::IsSame_V<int, htl::RemoveConst_T<const int>>;
  ASSERT_TRUE(val);
}
