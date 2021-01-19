#include <gtest/gtest.h>
#include <htl/Traits/EnableIf.hpp>

/*
// If SFINAE ever stops working for this, uncomment and compile the test.
TEST(EnableIfTests, SFINAE_sanity_check) {
  htl::EnableIf<false, int>::Type i = 0;
}
*/

TEST(EnableIfTests, canCreateVariableFromType) {
  htl::EnableIf<true, int>::Type i = 69;
  ASSERT_EQ(i, 69);
}
