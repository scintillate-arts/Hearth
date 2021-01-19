#include <gtest/gtest.h>
#include <htl/Traits/AddReference.hpp>
#include <htl/Traits/IsSame.hpp>

TEST(AddReferenceTests, canAddLValueReference) {
  constexpr auto val = htl::IsSame_V<int&, htl::AddLValueReference_T<int>>;
  ASSERT_TRUE(val);
}

TEST(AddReferenceTests, canAddRValueReference) {
  constexpr auto val = htl::IsSame_V<int&&, htl::AddRValueReference_T<int>>;
  ASSERT_TRUE(val);
}
