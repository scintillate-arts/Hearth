#include <gtest/gtest.h>
#include <htl/Memory/View.hpp>

TEST(ViewTests, canCreateViewFromProperType) {
  htl::View<int> intRef;
  htl::View<int*> intPRef;
  htl::View<int[]> intARef;
}

/*
// If SFINAE ever stops working on the object, uncomment this.
TEST(ViewTests, cannotCreateViewFromImproperType) {
  htl::View<void()> fnRef;
  htl::View<int&> intLVRef;
  htl::View<int&&> intRVRef;
}
*/

TEST(ViewTests, canCastViewToStoredType) {
  int i = 1;
  htl::View<int> intRef{ &i };
  const int* iPtr = static_cast<const int*>(intRef);
  ASSERT_EQ(*iPtr, 1);
}

TEST(ViewTests, canAccessViewMembers) {
  struct SomeType { int x; };
  SomeType st { .x = 1 };
  htl::View<SomeType> stRef{ &st };
  ASSERT_EQ(stRef->x, 1);
}

TEST(ViewTests, canDeViewTheView) {
  int i = 1;
  htl::View<int> intRef{ &i };
  ASSERT_EQ(*intRef, 1);
}

TEST(ViewTests, identicalViewsAreEqual) {
  htl::View<int> intRef1{ reinterpret_cast<int*>(0xFF) };
  htl::View<int> intRef2{ reinterpret_cast<int*>(0xFF) };
  ASSERT_TRUE(intRef1 == intRef2);
}

TEST(ViewTests, nonidenticalViewsAreNotEqual) {
  htl::View<int> intRef1{ reinterpret_cast<int*>(0x01) };
  htl::View<int> intRef2{ reinterpret_cast<int*>(0x0F) };
  ASSERT_TRUE(intRef1 != intRef2);
}

TEST(ViewTests, ViewsCanCompareLess) {
  htl::View<int> intRef1{ reinterpret_cast<int*>(0x00) };
  htl::View<int> intRef2{ reinterpret_cast<int*>(0x0F) };
  ASSERT_TRUE(intRef1 < intRef2);
}

TEST(ViewTests, ViewsCanCompareGreater) {
  htl::View<int> intRef1{ reinterpret_cast<int*>(0x0F) };
  htl::View<int> intRef2{ reinterpret_cast<int*>(0x00) };
  ASSERT_TRUE(intRef1 > intRef2);
}

TEST(ViewTests, ViewsCanCompareLessThanOrEqual) {
  htl::View<int> intRef1{ reinterpret_cast<int*>(0x0F) };
  htl::View<int> intRef2{ reinterpret_cast<int*>(0x0F) };
  ASSERT_TRUE(intRef1 <= intRef2);
}

TEST(ViewTests, ViewsCanCompareGreaterThanOrEqual) {
  htl::View<int> intRef1{ reinterpret_cast<int*>(0x0F) };
  htl::View<int> intRef2{ reinterpret_cast<int*>(0x0F) };
  ASSERT_TRUE(intRef1 >= intRef2);
}
