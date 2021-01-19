#include <gtest/gtest.h>
#include <htl/Traits/Decay.hpp>
#include <htl/Traits/IsSame.hpp>

TEST(DecayTests, canDecayArrayToPointer) {
  constexpr auto val = htl::IsSame_V<int*, htl::Decay_T<int[]>>;
  ASSERT_TRUE(val);
}

TEST(DecayTests, canDecayFunctionToPointer) {
  constexpr auto val = htl::IsSame_V<void(*)(), htl::Decay_T<void()>>;
  ASSERT_TRUE(val);
}

TEST(DecayTests, canDecayLValueReferenceToRawType) {
  constexpr auto val = htl::IsSame_V<int, htl::Decay_T<int&>>;
  ASSERT_TRUE(val);
}

TEST(DecayTests, canDecayRValueReferenceToRawType) {
  constexpr auto val = htl::IsSame_V<int, htl::Decay_T<int&&>>;
  ASSERT_TRUE(val);
}
