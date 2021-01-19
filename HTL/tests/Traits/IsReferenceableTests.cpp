#include <gtest/gtest.h>
#include <htl/Traits/IsReferenceable.hpp>
#include <htl/Traits/IsSame.hpp>

TEST(IsReferenceableTests, isReferenceableType) {
  constexpr auto val = htl::IsReferenceable_V<int>;
  ASSERT_TRUE(val);
}

TEST(IsReferenceableTests, functionIsReferenceableType) {
  constexpr auto val = htl::IsReferenceable_V<void()>;
  ASSERT_TRUE(val);
}

TEST(IsReferenceableTests, pointerIsReferenceableType) {
  constexpr auto val = htl::IsReferenceable_V<int*>;
  ASSERT_TRUE(val);
}

TEST(IsReferenceableTests, arrayIsReferenceableType) {
  constexpr auto val = htl::IsReferenceable_V<int[]>;
  ASSERT_TRUE(val);
}

TEST(IsReferenceableTests, isNotReferenceableType) {
  constexpr auto val = htl::IsReferenceable_V<void>;
  ASSERT_FALSE(val);
}
