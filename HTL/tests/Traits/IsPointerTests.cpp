#include <gtest/gtest.h>
#include <htl/Traits/IsPointer.hpp>

struct SomeType { };
TEST(IsPointerTests, allowRawPointers) {
  constexpr auto val = htl::IsPointer_V<int*>;
  ASSERT_TRUE(val);
}

TEST(IsPointerTests, allowFunctionPointers) {
  constexpr auto val = htl::IsPointer_V<void(*)()>;
  ASSERT_TRUE(val);
}

TEST(IsPointerTests, disallowMemberPointers) {
  constexpr auto val = htl::IsPointer_V<int SomeType::*>;
  ASSERT_FALSE(val);
}

TEST(IsPointerTests, disallowMemberFunctionPointers) {
  constexpr auto val = htl::IsPointer_V<void(SomeType::*)()>;
  ASSERT_FALSE(val);
}

TEST(IsPointerTests, disallowNonPointers) {
  constexpr auto val = htl::IsPointer_V<int>;
  ASSERT_FALSE(val);
}
