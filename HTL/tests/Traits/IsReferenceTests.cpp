#include <gtest/gtest.h>
#include <htl/Traits/IsReference.hpp>

TEST(IsReferenceTests, allowLValueReference) {
  constexpr auto val1 = htl::IsLValueReference_V<int&>;
  constexpr auto val2 = htl::IsReference_V<int&>;
  ASSERT_TRUE(val1 && val2);
}

TEST(IsReferenceTests, allowRValueReference) {
  constexpr auto val1 = htl::IsRValueReference_V<int&&>;
  constexpr auto val2 = htl::IsReference_V<int&&>;
  ASSERT_TRUE(val1 && val2);
}

TEST(IsReferenceTests, disallowNonReference) {
  constexpr auto val = htl::IsReference_V<int*>;
  ASSERT_FALSE(val);
}