#include <gtest/gtest.h>
#include <htl/Traits/BooleanType.hpp>

TEST(BooleanTypeTests, TypeAndValueChecks) {
  // We're checking that we can create variables with their type and assign their respective values.
  htl::TrueType::ValueType  blt = htl::TrueType::value;
  htl::FalseType::ValueType blf = htl::FalseType::value;
  ASSERT_TRUE(blt);
  ASSERT_FALSE(blf);
}
