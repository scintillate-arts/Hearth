#include <gtest/gtest.h>
#include <htl/Traits/IsSame.hpp>
#include <htl/Traits/RemoveReference.hpp>

TEST(RemoveReferenceTests, canRemoveLeftValueReference) {
  constexpr auto val = htl::IsSame_V<int, htl::RemoveReference_T<int&>>;
  ASSERT_TRUE(val);
}

TEST(RemoveReferenceTests, canRemoveRightValueReference) {
  constexpr auto val = htl::IsSame_V<int, htl::RemoveReference_T<int&&>>;
  ASSERT_TRUE(val);
}
