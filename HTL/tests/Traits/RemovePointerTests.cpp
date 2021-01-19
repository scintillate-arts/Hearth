#include <gtest/gtest.h>
#include <htl/Traits/IsSame.hpp>
#include <htl/Traits/RemovePointer.hpp>

TEST(RemovePointerTests, canRemoveOnePointer) {
  constexpr auto val = htl::IsSame_V<int, htl::RemovePointer_T<int*>>;
  ASSERT_TRUE(val);
}

TEST(RemovePointerTests, canRemoveConstPointer) {
  constexpr auto val = htl::IsSame_V<int, htl::RemovePointer_T<int* const>>;
  ASSERT_TRUE(val);
}

TEST(RemovePointerTests, canRemoveVolatilePointer) {
  constexpr auto val = htl::IsSame_V<int, htl::RemovePointer_T<int* volatile>>;
  ASSERT_TRUE(val);
}

TEST(RemovePointerTests, canRemoveConstVolatilePointer) {
  constexpr auto val = htl::IsSame_V<int, htl::RemovePointer_T<int* const volatile>>;
  ASSERT_TRUE(val);
}

TEST(RemovePointerTests, canRemoveMultiplePointers) {
  constexpr auto val = htl::IsSame_V<int, htl::RemoveAllPointers_T<int****>>;
  ASSERT_TRUE(val);
}

TEST(RemovePointerTests, canRemoveComplexMultiplePointers) {
  constexpr auto val = htl::IsSame_V<int, htl::RemoveAllPointers_T<int* const* volatile* const volatile*>>;
  ASSERT_TRUE(val);
}
