#include <gtest/gtest.h>
#include <htl/Memory/Reference.hpp>

TEST(ReferenceTests, canCreateReferenceFromProperType) {
  htl::Reference<int> intRef;
  htl::Reference<int*> intPRef;
  htl::Reference<int[]> intARef;
}

/*
// If SFINAE ever stops working on the object, uncomment this.
TEST(ReferenceTests, cannotCreateReferenceFromImproperType) {
  htl::Reference<void()> fnRef;
  htl::Reference<int&> intLVRef;
  htl::Reference<int&&> intRVRef;
}
*/

TEST(ReferenceTests, canCastReferenceToStoredType) {
  int i = 1;
  htl::Reference<int> intRef{ &i };
  int* iPtr = static_cast<int*>(intRef);
  ASSERT_EQ(*iPtr, 1);
}

TEST(ReferenceTests, canAccessReferenceMembers) {
  struct SomeType { int x; };
  SomeType st { .x = 1 };
  htl::Reference<SomeType> stRef{ &st };
  ASSERT_EQ(stRef->x, 1);
}

TEST(ReferenceTests, canDereferenceTheReference) {
  int i = 1;
  htl::Reference<int> intRef{ &i };
  ASSERT_EQ(*intRef, 1);
}

TEST(ReferenceTests, identicalReferencesAreEqual) {
  htl::Reference<int> intRef1{ reinterpret_cast<int*>(0xFF) };
  htl::Reference<int> intRef2{ reinterpret_cast<int*>(0xFF) };
  ASSERT_TRUE(intRef1 == intRef2);
}

TEST(ReferenceTests, nonidenticalReferencesAreNotEqual) {
  htl::Reference<int> intRef1{ reinterpret_cast<int*>(0x01) };
  htl::Reference<int> intRef2{ reinterpret_cast<int*>(0x0F) };
  ASSERT_TRUE(intRef1 != intRef2);
}

TEST(ReferenceTests, referencesCanCompareLess) {
  htl::Reference<int> intRef1{ reinterpret_cast<int*>(0x00) };
  htl::Reference<int> intRef2{ reinterpret_cast<int*>(0x0F) };
  ASSERT_TRUE(intRef1 < intRef2);
}

TEST(ReferenceTests, referencesCanCompareGreater) {
  htl::Reference<int> intRef1{ reinterpret_cast<int*>(0x0F) };
  htl::Reference<int> intRef2{ reinterpret_cast<int*>(0x00) };
  ASSERT_TRUE(intRef1 > intRef2);
}

TEST(ReferenceTests, referencesCanCompareLessThanOrEqual) {
  htl::Reference<int> intRef1{ reinterpret_cast<int*>(0x0F) };
  htl::Reference<int> intRef2{ reinterpret_cast<int*>(0x0F) };
  ASSERT_TRUE(intRef1 <= intRef2);
}

TEST(ReferenceTests, referencesCanCompareGreaterThanOrEqual) {
  htl::Reference<int> intRef1{ reinterpret_cast<int*>(0x0F) };
  htl::Reference<int> intRef2{ reinterpret_cast<int*>(0x0F) };
  ASSERT_TRUE(intRef1 >= intRef2);
}
