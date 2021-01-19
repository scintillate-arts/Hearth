#include <gtest/gtest.h>
#include <htl/Traits/Disjunction.hpp>

TEST(DisjunctionTests, canChooseFirstBool) {
  constexpr auto val = htl::Disjunction_V<htl::TrueType, htl::FalseType>;
  ASSERT_TRUE(val);
}

TEST(DisjunctionTests, canChooseSecondBool) {
  constexpr auto val = htl::Disjunction_V<htl::FalseType, htl::TrueType>;
  ASSERT_TRUE(val);
}

TEST(DisjunctionTests, canChooseThirdBool) {
  constexpr auto val = htl::Disjunction_V<htl::FalseType, htl::FalseType, htl::TrueType>;
  ASSERT_TRUE(val);
}
