#include <gtest/gtest.h>
#include <htl/Traits/Conjunction.hpp>

TEST(ConjunctionTests, canChooseSingleBool) {
  constexpr auto val = htl::Conjunction_V<htl::TrueType>;
  ASSERT_TRUE(val);
}

TEST(ConjunctionTests, canLogicallyAndTwoValues) {
  constexpr auto val = htl::Conjunction_V<htl::TrueType, htl::TrueType>;
  ASSERT_TRUE(val);
}

TEST(ConjunctionTests, canLogicallyAndThreeValues) {
  constexpr auto val = htl::Conjunction_V<htl::TrueType, htl::TrueType, htl::TrueType>;
  ASSERT_TRUE(val);
}

TEST(ConjunctionTests, canLogicallyChooseFalse) {
  constexpr auto val = htl::Conjunction_V<htl::TrueType, htl::FalseType>;
  ASSERT_FALSE(val);
}