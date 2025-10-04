#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Select128sTest) {
  public:
    TEST_METHOD(selectsOneOfTwoPackedFloat4_128WithBoolConditionMask) {
      constexpr std::array givenTruthy{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenTruthyPack = _mm_load_ps(givenTruthy.data());
      constexpr std::array givenFalsy{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenFalsyPack = _mm_load_ps(givenFalsy.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        _mm_store_ps(actual.data(), select(true, givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        _mm_store_ps(actual.data(), select(false, givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsComponentsOfTwoPackedFloat4_128WithPackedInts_128ConditionMask) {
      constexpr std::array givenTruthy{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenTruthyPack = _mm_load_ps(givenTruthy.data());
      constexpr std::array givenFalsy{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenFalsyPack = _mm_load_ps(givenFalsy.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        _mm_store_ps(actual.data(), select(_mm_set1_epi32(42), givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        _mm_store_ps(actual.data(), select(_mm_set1_epi32(0), givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsComponentsOfTwoPackedFloat4_128WithPackedFloat4_128ConditionMask) {
      constexpr std::array givenTruthy{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenTruthyPack = _mm_load_ps(givenTruthy.data());
      constexpr std::array givenFalsy{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenFalsyPack = _mm_load_ps(givenFalsy.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        _mm_store_ps(actual.data(), select(_mm_set1_ps(42.0f), givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        _mm_store_ps(actual.data(), select(_mm_set1_ps(0.0f), givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }
  };
}
