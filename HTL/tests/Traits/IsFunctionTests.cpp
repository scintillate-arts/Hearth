#include <gtest/gtest.h>
#include <htl/Traits/IsFunction.hpp>

TEST(IsFunctionTests, allowFunctionType) {
  constexpr auto val = htl::IsFunction_V<void()>;
  ASSERT_TRUE(val);
}

TEST(IsFunctionTests, disallowFunctionPointer) {
  constexpr auto val = htl::IsFunction_V<void(*)()>;
  ASSERT_FALSE(val);
}

TEST(IsFunctionTests, disallowMemberFunctionPointer) {
  struct SomeType { };
  constexpr auto val = htl::IsFunction_V<void(SomeType::*)()>;
  ASSERT_FALSE(val);
}

TEST(IsFunctionTests, disallowRawPointer) {
  constexpr auto val = htl::IsFunction_V<int*>;
  ASSERT_FALSE(val);
}

TEST(IsFunctionTests, disallowNonFunction) {
  constexpr auto val = htl::IsFunction_V<int>;
  ASSERT_FALSE(val);
}
