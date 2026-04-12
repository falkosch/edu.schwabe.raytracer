#include "vectorization_tests.h"

#include <cmath>

namespace vectorization::test {
  TEST_CLASS(Select128dSpecialValuesTest) {
  public:
    TEST_METHOD(selectPreservesNaNAndNegativeInfinityWithBoolTrue) {
      alignas(16) const std::array special{NotANumber<Float_64>(), NegativeInfinity<Float_64>()};
      alignas(16) const std::array normal{1.0, 2.0};
      const auto specialPack = _mm_load_pd(special.data());
      const auto normalPack = _mm_load_pd(normal.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      _mm_store_pd(actual.data(), select(true, specialPack, normalPack));
      Assert::IsTrue(std::isnan(actual.at(VectorIndices::X)), L"NaN not preserved in select", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_64>(), actual.at(VectorIndices::Y), L"-inf not preserved", LINE_INFO());
    }

    TEST_METHOD(selectPreservesNegativeZeroAndInfinityWithBoolFalse) {
      alignas(16) const std::array normal{1.0, 2.0};
      alignas(16) const std::array special{NegativeZero<Float_64>(), Infinity<Float_64>()};
      const auto normalPack = _mm_load_pd(normal.data());
      const auto specialPack = _mm_load_pd(special.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      _mm_store_pd(actual.data(), select(false, normalPack, specialPack));

      Assert::AreEqual(0.0, actual.at(VectorIndices::X), L"-0.0 value changed", LINE_INFO());
      Assert::IsTrue(std::signbit(actual.at(VectorIndices::X)), L"-0.0 sign bit lost in select", LINE_INFO());
      Assert::AreEqual(Infinity<Float_64>(), actual.at(VectorIndices::Y), L"+inf not preserved", LINE_INFO());
    }

    TEST_METHOD(selectPreservesSpecialValuesWithPackedIntMask) {
      alignas(16) const std::array special{NotANumber<Float_64>(), NegativeZero<Float_64>()};
      alignas(16) const std::array normal{1.0, 2.0};
      const auto specialPack = _mm_load_pd(special.data());
      const auto normalPack = _mm_load_pd(normal.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      _mm_store_pd(actual.data(), select(_mm_set1_epi64x(-1), specialPack, normalPack));

      Assert::IsTrue(std::isnan(actual.at(VectorIndices::X)), L"NaN not preserved", LINE_INFO());
      Assert::AreEqual(0.0, actual.at(VectorIndices::Y), L"-0.0 value changed", LINE_INFO());
      Assert::IsTrue(std::signbit(actual.at(VectorIndices::Y)), L"-0.0 sign bit lost", LINE_INFO());
    }
  };
}
