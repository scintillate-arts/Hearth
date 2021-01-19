#include <gtest/gtest.h>
#include <htl/Traits/IsSame.hpp>
#include <htl/Traits/RemoveExtent.hpp>

TEST(RemoveExtentTests, removeOneExtentWithSize) {
  constexpr auto val = htl::IsSame_V<int, htl::RemoveExtent_T<int[10]>>;
  ASSERT_TRUE(val);
}

TEST(RemoveExtentTests, removeOneExtentWithoutSize) {
  constexpr auto val = htl::IsSame_V<int, htl::RemoveExtent_T<int[]>>;
  ASSERT_TRUE(val);
}

TEST(RemoveExtentTests, removeMultipleExtentComplex) {
  constexpr auto val = htl::IsSame_V<int, htl::RemoveAllExtents_T<int[][64][10]>>;
  ASSERT_TRUE(val);
}
