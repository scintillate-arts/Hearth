#include <gtest/gtest.h>
#include <htl/Traits/IsObject.hpp>

TEST(IsObjectTests, primitiveIsObject) {
  constexpr auto val = htl::IsObject_V<int>;
  ASSERT_TRUE(val);
}

TEST(IsObjectTests, userDefinedObjectIsObject) {
  struct SomeTest { };
  constexpr auto val = htl::IsObject_V<SomeTest>;
  ASSERT_TRUE(val);
}

TEST(IsObjectTests, pointerIsObject) {
  constexpr auto val = htl::IsObject_V<int*>;
  ASSERT_TRUE(val);
}

TEST(IsObjectTests, arrayIsObject) {
  constexpr auto val = htl::IsObject_V<int[]>;
  ASSERT_TRUE(val);
}

TEST(IsObjectTests, functionIsNotObject) {
  constexpr auto val = htl::IsObject_V<void()>;
  ASSERT_FALSE(val);
}

TEST(IsObjectTests, referenceIsNotObject) {
  constexpr auto val1 = htl::IsObject_V<int&>;
  constexpr auto val2 = htl::IsObject_V<int&&>;
  ASSERT_FALSE(val1 || val2);
}

TEST(IsObjectTests, voidIsNotObject) {
  constexpr auto val = htl::IsObject_V<void>;
  ASSERT_FALSE(val);
}
