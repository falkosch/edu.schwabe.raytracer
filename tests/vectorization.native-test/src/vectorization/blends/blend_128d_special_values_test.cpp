#include "vectorization_tests.h"

#include <cmath>

namespace vectorization::test {
  TEST_CLASS(Blend128dSpecialValuesTest) {
  public:
    TEST_METHOD(blendPreservesNegativeZero) {
      alignas(16) const std::array special{NegativeZero<Float_64>(), 1.0};
      alignas(16) const std::array normal{10.0, 20.0};
      const auto specialPack = _mm_load_pd(special.data());
      const auto normalPack = _mm_load_pd(normal.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      _mm_store_pd(actual.data(), blend<false, true>(specialPack, normalPack));
      Assert::AreEqual(0.0, actual.at(VectorIndices::X), L"-0.0 value changed", LINE_INFO());
      Assert::IsTrue(std::signbit(actual.at(VectorIndices::X)), L"-0.0 sign bit lost in blend", LINE_INFO());
    }

    TEST_METHOD(blendPreservesNaN) {
      alignas(16) const std::array normal{1.0, 2.0};
      alignas(16) const std::array withNaN{NotANumber<Float_64>(), 20.0};
      const auto normalPack = _mm_load_pd(normal.data());
      const auto nanPack = _mm_load_pd(withNaN.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      _mm_store_pd(actual.data(), blend<true, false>(normalPack, nanPack));
      Assert::IsTrue(std::isnan(actual.at(VectorIndices::X)), L"NaN not preserved in blend", LINE_INFO());
      Assert::AreEqual(2.0, actual.at(VectorIndices::Y), L"normal value changed", LINE_INFO());
    }

    TEST_METHOD(blendPreservesInfinity) {
      alignas(16) const std::array normal{1.0, 2.0};
      alignas(16) const std::array withInf{Infinity<Float_64>(), NegativeInfinity<Float_64>()};
      const auto normalPack = _mm_load_pd(normal.data());
      const auto infPack = _mm_load_pd(withInf.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      _mm_store_pd(actual.data(), blend<true, true>(normalPack, infPack));
      Assert::AreEqual(Infinity<Float_64>(), actual.at(VectorIndices::X), L"+inf not preserved", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_64>(), actual.at(VectorIndices::Y), L"-inf not preserved", LINE_INFO());
    }

    TEST_METHOD(blendMasked128dPreservesSpecialValues) {
      alignas(16) const std::array normal{1.0, 2.0};
      alignas(16) const std::array special{NotANumber<Float_64>(), NegativeInfinity<Float_64>()};
      const auto normalPack = _mm_load_pd(normal.data());
      const auto specialPack = _mm_load_pd(special.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      _mm_store_pd(actual.data(), blendMasked(normalPack, specialPack, MaskAll<PackedFloat2_128>()));
      Assert::IsTrue(std::isnan(actual.at(VectorIndices::X)), L"NaN not preserved", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_64>(), actual.at(VectorIndices::Y), L"-inf not preserved", LINE_INFO());
    }
  };
}
