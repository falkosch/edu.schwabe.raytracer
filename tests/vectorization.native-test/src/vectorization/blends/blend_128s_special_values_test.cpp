#include "vectorization_tests.h"

#include <cmath>

namespace vectorization::test {
  TEST_CLASS(Blend128sSpecialValuesTest) {
  public:
    TEST_METHOD(blendPreservesNegativeZero) {
      alignas(16) const std::array special{NegativeZero<Float_32>(), 1.0f, 2.0f, 3.0f};
      alignas(16) const std::array normal{10.0f, 20.0f, 30.0f, 40.0f};
      const auto specialPack = _mm_load_ps(special.data());
      const auto normalPack = _mm_load_ps(normal.data());
      std::array<Float_32, VectorSizes::W> actual{};

      _mm_store_ps(actual.data(), blend<false, true, true, true>(specialPack, normalPack));
      Assert::AreEqual(0.0f, actual.at(VectorIndices::X), L"-0.0 value changed", LINE_INFO());
      Assert::IsTrue(std::signbit(actual.at(VectorIndices::X)), L"-0.0 sign bit lost in blend", LINE_INFO());
    }

    TEST_METHOD(blendPreservesNaN) {
      alignas(16) const std::array normal{1.0f, 2.0f, 3.0f, 4.0f};
      alignas(16) const std::array withNaN{10.0f, NotANumber<Float_32>(), 30.0f, 40.0f};
      const auto normalPack = _mm_load_ps(normal.data());
      const auto nanPack = _mm_load_ps(withNaN.data());
      std::array<Float_32, VectorSizes::W> actual{};

      _mm_store_ps(actual.data(), blend<false, true, false, false>(normalPack, nanPack));
      Assert::AreEqual(1.0f, actual.at(VectorIndices::X), L"normal value changed", LINE_INFO());
      Assert::IsTrue(std::isnan(actual.at(VectorIndices::Y)), L"NaN not preserved in blend", LINE_INFO());
      Assert::AreEqual(3.0f, actual.at(VectorIndices::Z), L"normal value changed", LINE_INFO());
    }

    TEST_METHOD(blendPreservesInfinity) {
      alignas(16) const std::array normal{1.0f, 2.0f, 3.0f, 4.0f};
      alignas(16) const std::array withInf{Infinity<Float_32>(), 20.0f, NegativeInfinity<Float_32>(), 40.0f};
      const auto normalPack = _mm_load_ps(normal.data());
      const auto infPack = _mm_load_ps(withInf.data());
      std::array<Float_32, VectorSizes::W> actual{};

      _mm_store_ps(actual.data(), blend<true, false, true, false>(normalPack, infPack));
      Assert::AreEqual(Infinity<Float_32>(), actual.at(VectorIndices::X), L"+inf not preserved", LINE_INFO());
      Assert::AreEqual(2.0f, actual.at(VectorIndices::Y), L"normal value changed", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_32>(), actual.at(VectorIndices::Z), L"-inf not preserved", LINE_INFO());
      Assert::AreEqual(4.0f, actual.at(VectorIndices::W), L"normal value changed", LINE_INFO());
    }

    TEST_METHOD(blendMasked128sPreservesSpecialValues) {
      alignas(16) const std::array normal{1.0f, 2.0f, 3.0f, 4.0f};
      alignas(16) const std::array special{
          NegativeZero<Float_32>(), NotANumber<Float_32>(), Infinity<Float_32>(), NegativeInfinity<Float_32>()
      };
      const auto normalPack = _mm_load_ps(normal.data());
      const auto specialPack = _mm_load_ps(special.data());
      std::array<Float_32, VectorSizes::W> actual{};

      _mm_store_ps(actual.data(), blendMasked(normalPack, specialPack, MaskAll<PackedFloat4_128>()));

      Assert::IsTrue(std::signbit(actual.at(VectorIndices::X)), L"-0.0 sign bit lost", LINE_INFO());
      Assert::IsTrue(std::isnan(actual.at(VectorIndices::Y)), L"NaN not preserved", LINE_INFO());
      Assert::AreEqual(Infinity<Float_32>(), actual.at(VectorIndices::Z), L"+inf not preserved", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_32>(), actual.at(VectorIndices::W), L"-inf not preserved", LINE_INFO());
    }
  };
}
