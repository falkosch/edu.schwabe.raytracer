#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(Swizzle256dTest)
        {
        public:

            TEST_METHOD(genericallySwizzlesIdentityAndItsReverse) {
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::Z), given.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::W), given.at(VectorIndices::Z), given.at(VectorIndices::Y), given.at(VectorIndices::X)
                    };
                    _mm256_store_pd(actual.data(), swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzlesBroadcast) {
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X)
                    };
                    _mm256_store_pd(actual.data(), swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::Y)
                    };
                    _mm256_store_pd(actual.data(), swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z)
                    };
                    _mm256_store_pd(actual.data(), swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::W), given.at(VectorIndices::W), given.at(VectorIndices::W), given.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzlesDuplicateForms) {
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::Y)
                    };
                    _mm256_store_pd(actual.data(), swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Z), given.at(VectorIndices::Z)
                    };
                    _mm256_store_pd(actual.data(), swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::X), given.at(VectorIndices::Y)
                    };
                    _mm256_store_pd(actual.data(), swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::W), given.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::Z), given.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzlesOfXFirst) {
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X)
                    };
                    _mm256_store_pd(actual.data(), xxxx(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Z)
                    };
                    _mm256_store_pd(actual.data(), xxxz(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::Y)
                    };
                    _mm256_store_pd(actual.data(), xxyy(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Z), given.at(VectorIndices::Z)
                    };
                    _mm256_store_pd(actual.data(), xxzz(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::X), given.at(VectorIndices::Y)
                    };
                    _mm256_store_pd(actual.data(), xyxy(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z)
                    };
                    _mm256_store_pd(actual.data(), xzzz(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzlesOfYFirst) {
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::Y)
                    };
                    _mm256_store_pd(actual.data(), yyyy(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Y)
                    };
                    _mm256_store_pd(actual.data(), yxxy(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::X), given.at(VectorIndices::W), given.at(VectorIndices::Z)
                    };
                    _mm256_store_pd(actual.data(), yxwz(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::W), given.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), yyww(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Z), given.at(VectorIndices::X), given.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), yzxw(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::X)
                    };
                    _mm256_store_pd(actual.data(), yzwx(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzlesOfZFirst) {
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z)
                    };
                    _mm256_store_pd(actual.data(), zzzz(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), zxyw(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Y), given.at(VectorIndices::Y)
                    };
                    _mm256_store_pd(actual.data(), zzyy(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), zzww(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::X), given.at(VectorIndices::Y)
                    };
                    _mm256_store_pd(actual.data(), zwxy(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::Z), given.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), zwzw(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzlesOfWFirst) {
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::W), given.at(VectorIndices::W), given.at(VectorIndices::W), given.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), wwww(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::W), given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::Z)
                    };
                    _mm256_store_pd(actual.data(), wxyz(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        given.at(VectorIndices::W), given.at(VectorIndices::Z), given.at(VectorIndices::Y), given.at(VectorIndices::X)
                    };
                    _mm256_store_pd(actual.data(), wzyx(_mm256_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }
        };
    }
}