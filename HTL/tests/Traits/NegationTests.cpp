#include <gtest/gtest.h>
#include <htl/Traits/Negation.hpp>

TEST(NegationTests, canFlipValue) {
  constexpr auto val = htl::Negation_V<htl::TrueType>;
  ASSERT_FALSE(val);
}
