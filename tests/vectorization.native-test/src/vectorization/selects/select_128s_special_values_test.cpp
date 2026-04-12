#include "vectorization_tests.h"

#include <cmath>

namespace vectorization::test {
  TEST_CLASS(Select128sSpecialValuesTest) {
  public:
    TEST_METHOD(selectPreservesSpecialValuesWithBoolTrue) {
      alignas(16) const std::array special{
          NegativeZero<Float_32>(), NotANumber<Float_32>(), Infinity<Float_32>(), NegativeInfinity<Float_32>()
      };
      alignas(16) const std::array normal{1.0f, 2.0f, 3.0f, 4.0f};
      const auto specialPack = _mm_load_ps(special.data());
      const auto normalPack = _mm_load_ps(normal.data());
      std::array<Float_32, VectorSizes::W> actual{};

      _mm_store_ps(actual.data(), select(true, specialPack, normalPack));

      Assert::AreEqual(0.0f, actual.at(VectorIndices::X), L"-0.0 value changed", LINE_INFO());
      Assert::IsTrue(std::signbit(actual.at(VectorIndices::X)), L"-0.0 sign bit lost in select", LINE_INFO());
      Assert::IsTrue(std::isnan(actual.at(VectorIndices::Y)), L"NaN not preserved in select", LINE_INFO());
      Assert::AreEqual(Infinity<Float_32>(), actual.at(VectorIndices::Z), L"+inf not preserved", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_32>(), actual.at(VectorIndices::W), L"-inf not preserved", LINE_INFO());
    }

    TEST_METHOD(selectPreservesSpecialValuesWithBoolFalse) {
      alignas(16) const std::array normal{1.0f, 2.0f, 3.0f, 4.0f};
      alignas(16) const std::array special{
          NegativeZero<Float_32>(), NotANumber<Float_32>(), Infinity<Float_32>(), NegativeInfinity<Float_32>()
      };
      const auto normalPack = _mm_load_ps(normal.data());
      const auto specialPack = _mm_load_ps(special.data());
      std::array<Float_32, VectorSizes::W> actual{};

      _mm_store_ps(actual.data(), select(false, normalPack, specialPack));

      Assert::IsTrue(std::signbit(actual.at(VectorIndices::X)), L"-0.0 sign bit lost in select", LINE_INFO());
      Assert::IsTrue(std::isnan(actual.at(VectorIndices::Y)), L"NaN not preserved in select", LINE_INFO());
      Assert::AreEqual(Infinity<Float_32>(), actual.at(VectorIndices::Z), L"+inf not preserved", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_32>(), actual.at(VectorIndices::W), L"-inf not preserved", LINE_INFO());
    }

    TEST_METHOD(selectPreservesSpecialValuesWithPackedIntMask) {
      alignas(16) const std::array special{
          NegativeZero<Float_32>(), NotANumber<Float_32>(), Infinity<Float_32>(), NegativeInfinity<Float_32>()
      };
      alignas(16) const std::array normal{1.0f, 2.0f, 3.0f, 4.0f};
      const auto specialPack = _mm_load_ps(special.data());
      const auto normalPack = _mm_load_ps(normal.data());
      std::array<Float_32, VectorSizes::W> actual{};

      _mm_store_ps(actual.data(), select(_mm_set1_epi32(-1), specialPack, normalPack));

      Assert::IsTrue(std::signbit(actual.at(VectorIndices::X)), L"-0.0 sign bit lost", LINE_INFO());
      Assert::IsTrue(std::isnan(actual.at(VectorIndices::Y)), L"NaN not preserved", LINE_INFO());
      Assert::AreEqual(Infinity<Float_32>(), actual.at(VectorIndices::Z), L"+inf not preserved", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_32>(), actual.at(VectorIndices::W), L"-inf not preserved", LINE_INFO());
    }
  };
}
