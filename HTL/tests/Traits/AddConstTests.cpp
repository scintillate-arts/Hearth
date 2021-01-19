#include <gtest/gtest.h>
#include <htl/Traits/AddConst.hpp>
#include <htl/Traits/IsSame.hpp>

TEST(AddConstTests, canAddConstToType) {
  constexpr auto val = htl::IsSame_V<const int, htl::AddConst_T<int>>;
  ASSERT_TRUE(val);
}
