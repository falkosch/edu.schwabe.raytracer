#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Select128dTest) {
  public:
    TEST_METHOD(selectsOneOfTwoPackedFloat2_128WithBoolConditionMask) {
      constexpr std::array givenTruthy{1.0, 2.0};
      const auto givenTruthyPack = _mm_load_pd(givenTruthy.data());
      constexpr std::array givenFalsy{3.0, 4.0};
      const auto givenFalsyPack = _mm_load_pd(givenFalsy.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        _mm_store_pd(actual.data(), select(true, givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        _mm_store_pd(actual.data(), select(false, givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsComponentsOfTwoPackedFloat2_128WithPackedInts_128ConditionMask) {
      constexpr std::array givenTruthy{1.0, 2.0};
      const auto givenTruthyPack = _mm_load_pd(givenTruthy.data());
      constexpr std::array givenFalsy{3.0, 4.0};
      const auto givenFalsyPack = _mm_load_pd(givenFalsy.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        _mm_store_pd(actual.data(), select(_mm_set1_epi64x(42), givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        _mm_store_pd(actual.data(), select(_mm_set1_epi64x(0), givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsComponentsOfTwoPackedFloat2_128WithPackedFloat2_128ConditionMask) {
      constexpr std::array givenTruthy{1.0, 2.0};
      const auto givenTruthyPack = _mm_load_pd(givenTruthy.data());
      constexpr std::array givenFalsy{3.0, 4.0};
      const auto givenFalsyPack = _mm_load_pd(givenFalsy.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        _mm_store_pd(actual.data(), select(_mm_set1_pd(42.0), givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        _mm_store_pd(actual.data(), select(_mm_set1_pd(0.0), givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }
  };
}
