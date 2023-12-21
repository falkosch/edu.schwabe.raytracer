#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Select256sTest) {
  public:
    TEST_METHOD(selectsOneOfTwoPackedFloat8_256WithBoolConditionMask) {
      std::array<Float_32, VectorSizes::X8> givenTruthy{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
      auto givenTruthyPack = _mm256_load_ps(givenTruthy.data());
      std::array<Float_32, VectorSizes::X8> givenFalsy{9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
      auto givenFalsyPack = _mm256_load_ps(givenFalsy.data());
      std::array<Float_32, VectorSizes::X8> actual{};

      {
        _mm256_store_ps(actual.data(), select(true, givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        _mm256_store_ps(actual.data(), select(false, givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsComponentsOfTwoPackedFloat8_256WithPackedInts_256ConditionMask) {
      std::array<Float_32, VectorSizes::X8> givenTruthy{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
      auto givenTruthyPack = _mm256_load_ps(givenTruthy.data());
      std::array<Float_32, VectorSizes::X8> givenFalsy{9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
      auto givenFalsyPack = _mm256_load_ps(givenFalsy.data());
      std::array<Float_32, VectorSizes::X8> actual{};

      {
        _mm256_store_ps(actual.data(), select(_mm256_set1_epi32(42), givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        _mm256_store_ps(actual.data(), select(_mm256_set1_epi32(0), givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsComponentsOfTwoPackedFloat8_256WithPackedFloat8_256ConditionMask) {
      std::array<Float_32, VectorSizes::X8> givenTruthy{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
      auto givenTruthyPack = _mm256_load_ps(givenTruthy.data());
      std::array<Float_32, VectorSizes::X8> givenFalsy{9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
      auto givenFalsyPack = _mm256_load_ps(givenFalsy.data());
      std::array<Float_32, VectorSizes::X8> actual{};

      {
        _mm256_store_ps(actual.data(), select(_mm256_set1_ps(42.0f), givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        _mm256_store_ps(actual.data(), select(_mm256_set1_ps(0.0f), givenTruthyPack, givenFalsyPack));
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }
  };
}
