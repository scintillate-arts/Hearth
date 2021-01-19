#include <gtest/gtest.h>
#include <htl/Traits/Conditional.hpp>

TEST(ConditionalTests, canChooseTrueType) {
  using Type = htl::Conditional_T<true, char*, int>;
  Type pChar = nullptr;
}

TEST(ConditionalTests, canChooseFalseType) {
  using Type = htl::Conditional_T<false, char*, int>;
  Type i = 0;
}
