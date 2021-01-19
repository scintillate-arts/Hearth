#include <gtest/gtest.h>
#include <htl/Traits/AddPointer.hpp>
#include <htl/Traits/IsSame.hpp>

TEST(AddPointerTests, canAddPointerToTypeWithoutReference) {
  constexpr auto val = htl::IsSame_V<int*, htl::AddPointer_T<int>>;
  ASSERT_TRUE(val);
}

TEST(AddPointerTests, canAddPointerToTypeWithReference) {
  constexpr auto val = htl::IsSame_V<int*, htl::AddPointer_T<int&>>;
  ASSERT_TRUE(val);
}
