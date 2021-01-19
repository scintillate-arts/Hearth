#include <gtest/gtest.h>
#include <htl/Traits/IsMemberPointer.hpp>

struct SomeType { };
TEST(IsMemberPointerTests, allowMemberPointers) {
  constexpr auto val = htl::IsMemberPointer_V<int SomeType::*>;
  ASSERT_TRUE(val);
}

TEST(IsMemberPointerTests, disallowRawPointers) {
  constexpr auto val = htl::IsMemberPointer_V<int*>;
  ASSERT_FALSE(val);
}

TEST(IsMemberPointerTests, disallowFunctionPointers) {
  constexpr auto val = htl::IsMemberPointer_V<void(*)()>;
  ASSERT_FALSE(val);
}

TEST(IsMemberPointerTests, disallowMemberFunctionPointers) {
  constexpr auto val = htl::IsMemberPointer_V<void(SomeType::*)()>;
  ASSERT_FALSE(val);
}

TEST(IsMemberPointerTests, disallowNonMemberPointers) {
  constexpr auto val = htl::IsMemberPointer_V<int>;
  ASSERT_FALSE(val);
}