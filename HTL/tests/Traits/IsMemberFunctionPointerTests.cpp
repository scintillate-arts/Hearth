#include <gtest/gtest.h>
#include <htl/Traits/IsMemberFunctionPointer.hpp>

struct SomeType { };
TEST(IsMemberFunctionPointerTests, allowMemberFunctionPointer) {
  constexpr auto val = htl::IsMemberFunctionPointer_V<void(SomeType::*)()>;
  ASSERT_TRUE(val);
}

TEST(IsMemberFunctionPointerTests, disallowRawPointers) {
  constexpr auto val = htl::IsMemberFunctionPointer_V<int*>;
  ASSERT_FALSE(val);
}

TEST(IsMemberFunctionPointerTests, disallowFunctionPointers) {
  constexpr auto val = htl::IsMemberFunctionPointer_V<void(*)()>;
  ASSERT_FALSE(val);
}

TEST(IsMemberFunctionPointerTests, disallowMemberPointers) {
  constexpr auto val = htl::IsMemberFunctionPointer_V<int SomeType::*>;
  ASSERT_FALSE(val);
}

TEST(IsMemberFunctionPointerTests, disallowNonMemberFunctionPointers) {
  constexpr auto val = htl::IsMemberFunctionPointer_V<int>;
  ASSERT_FALSE(val);
}
