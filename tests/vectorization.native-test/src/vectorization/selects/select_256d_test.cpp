#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(Select256dTest)
        {
        public:

            TEST_METHOD(selectsOneOfTwoPackedFloat4_256WithBoolConditionMask) {
                std::array<Float_64, VectorSizes::W> givenTruthy{ 1.0, 2.0, 3.0, 4.0 };
                auto givenTruthyPack = _mm256_load_pd(givenTruthy.data());
                std::array<Float_64, VectorSizes::W> givenFalsy{ 5.0, 6.0, 7.0, 8.0 };
                auto givenFalsyPack = _mm256_load_pd(givenFalsy.data());
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    _mm256_store_pd(actual.data(), select(true, givenTruthyPack, givenFalsyPack));
                    Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
                }

                {
                    _mm256_store_pd(actual.data(), select(false, givenTruthyPack, givenFalsyPack));
                    Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(selectsComponentsOfTwoPackedFloat4_256WithPackedInts_256ConditionMask) {
                std::array<Float_64, VectorSizes::W> givenTruthy{ 1.0, 2.0, 3.0, 4.0 };
                auto givenTruthyPack = _mm256_load_pd(givenTruthy.data());
                std::array<Float_64, VectorSizes::W> givenFalsy{ 5.0, 6.0, 7.0, 8.0 };
                auto givenFalsyPack = _mm256_load_pd(givenFalsy.data());
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    _mm256_store_pd(actual.data(), select(_mm256_set1_epi64x(42), givenTruthyPack, givenFalsyPack));
                    Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
                }

                {
                    _mm256_store_pd(actual.data(), select(_mm256_set1_epi64x(0), givenTruthyPack, givenFalsyPack));
                    Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(selectsComponentsOfTwoPackedFloat4_256WithPackedFloat4_256ConditionMask) {
                std::array<Float_64, VectorSizes::W> givenTruthy{ 1.0, 2.0, 3.0, 4.0 };
                auto givenTruthyPack = _mm256_load_pd(givenTruthy.data());
                std::array<Float_64, VectorSizes::W> givenFalsy{ 5.0, 6.0, 7.0, 8.0 };
                auto givenFalsyPack = _mm256_load_pd(givenFalsy.data());
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    _mm256_store_pd(actual.data(), select(_mm256_set1_pd(42.0), givenTruthyPack, givenFalsyPack));
                    Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
                }

                {
                    _mm256_store_pd(actual.data(), select(_mm256_set1_pd(0.0), givenTruthyPack, givenFalsyPack));
                    Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
                }
            }
        };
    }
}