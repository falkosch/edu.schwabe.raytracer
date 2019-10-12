#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(Swizzle256sTest)
        {
        public:

            TEST_METHOD(genericallySwizzlesIdentityAndItsReverseForPackedFloat_128x2) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                std::array<Float_32, VectorSizes::X8> actual{ };

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X2), given.at(VectorIndices::X3), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X6), given.at(VectorIndices::X7), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X4), given.at(VectorIndices::X3), given.at(VectorIndices::X2), given.at(VectorIndices::X1),
                        given.at(VectorIndices::X8), given.at(VectorIndices::X7), given.at(VectorIndices::X6), given.at(VectorIndices::X5)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzlesBroadcastForPackedFloat_128x2) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                std::array<Float_32, VectorSizes::X8> actual{ };

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X1),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X5)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X2), given.at(VectorIndices::X2), given.at(VectorIndices::X2), given.at(VectorIndices::X2),
                        given.at(VectorIndices::X6), given.at(VectorIndices::X6), given.at(VectorIndices::X6), given.at(VectorIndices::X6)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X3),
                        given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X7)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X4), given.at(VectorIndices::X4), given.at(VectorIndices::X4), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X8), given.at(VectorIndices::X8), given.at(VectorIndices::X8), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzlesDuplicateFormsForPackedFloat_128x2) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                std::array<Float_32, VectorSizes::X8> actual{ };

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X2), given.at(VectorIndices::X2),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X6), given.at(VectorIndices::X6)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X3), given.at(VectorIndices::X3),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X7), given.at(VectorIndices::X7)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X2), given.at(VectorIndices::X1), given.at(VectorIndices::X2),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X6), given.at(VectorIndices::X5), given.at(VectorIndices::X6)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X2), given.at(VectorIndices::X2), given.at(VectorIndices::X4), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X6), given.at(VectorIndices::X6), given.at(VectorIndices::X8), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X4), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X8), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X3), given.at(VectorIndices::X4), given.at(VectorIndices::X3), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X7), given.at(VectorIndices::X8), given.at(VectorIndices::X7), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzlesOfXFirstForPackedFloat_128x2) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                std::array<Float_32, VectorSizes::X8> actual{ };

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X1),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X5)
                    };
                    _mm256_store_ps(actual.data(), xxxx(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X3),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X7)
                    };
                    _mm256_store_ps(actual.data(), xxxz(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X2), given.at(VectorIndices::X2),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X6), given.at(VectorIndices::X6)
                    };
                    _mm256_store_ps(actual.data(), xxyy(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X3), given.at(VectorIndices::X3),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X7), given.at(VectorIndices::X7)
                    };
                    _mm256_store_ps(actual.data(), xxzz(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X2), given.at(VectorIndices::X1), given.at(VectorIndices::X2),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X6), given.at(VectorIndices::X5), given.at(VectorIndices::X6)
                    };
                    _mm256_store_ps(actual.data(), xyxy(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X3),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X7)
                    };
                    _mm256_store_ps(actual.data(), xzzz(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzlesOfYFirstForPackedFloat_128x2) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                std::array<Float_32, VectorSizes::X8> actual{ };

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X2), given.at(VectorIndices::X2), given.at(VectorIndices::X2), given.at(VectorIndices::X2),
                        given.at(VectorIndices::X6), given.at(VectorIndices::X6), given.at(VectorIndices::X6), given.at(VectorIndices::X6)
                    };
                    _mm256_store_ps(actual.data(), yyyy(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X2), given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X2),
                        given.at(VectorIndices::X6), given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X6)
                    };
                    _mm256_store_ps(actual.data(), yxxy(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X2), given.at(VectorIndices::X1), given.at(VectorIndices::X4), given.at(VectorIndices::X3),
                        given.at(VectorIndices::X6), given.at(VectorIndices::X5), given.at(VectorIndices::X8), given.at(VectorIndices::X7)
                    };
                    _mm256_store_ps(actual.data(), yxwz(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X2), given.at(VectorIndices::X2), given.at(VectorIndices::X4), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X6), given.at(VectorIndices::X6), given.at(VectorIndices::X8), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), yyww(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X2), given.at(VectorIndices::X3), given.at(VectorIndices::X1), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X6), given.at(VectorIndices::X7), given.at(VectorIndices::X5), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), yzxw(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X2), given.at(VectorIndices::X3), given.at(VectorIndices::X4), given.at(VectorIndices::X1),
                        given.at(VectorIndices::X6), given.at(VectorIndices::X7), given.at(VectorIndices::X8), given.at(VectorIndices::X5)
                    };
                    _mm256_store_ps(actual.data(), yzwx(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzlesOfZFirstForPackedFloat_128x2) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                std::array<Float_32, VectorSizes::X8> actual{ };

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X3),
                        given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X7)
                    };
                    _mm256_store_ps(actual.data(), zzzz(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X3), given.at(VectorIndices::X1), given.at(VectorIndices::X2), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X7), given.at(VectorIndices::X5), given.at(VectorIndices::X6), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), zxyw(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X2), given.at(VectorIndices::X2),
                        given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X6), given.at(VectorIndices::X6)
                    };
                    _mm256_store_ps(actual.data(), zzyy(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X4), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X8), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), zzww(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X3), given.at(VectorIndices::X4), given.at(VectorIndices::X1), given.at(VectorIndices::X2),
                        given.at(VectorIndices::X7), given.at(VectorIndices::X8), given.at(VectorIndices::X5), given.at(VectorIndices::X6)
                    };
                    _mm256_store_ps(actual.data(), zwxy(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X3), given.at(VectorIndices::X4), given.at(VectorIndices::X3), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X7), given.at(VectorIndices::X8), given.at(VectorIndices::X7), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), zwzw(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzlesOfWFirstForPackedFloat_128x2) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                std::array<Float_32, VectorSizes::X8> actual{ };

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X4), given.at(VectorIndices::X4), given.at(VectorIndices::X4), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X8), given.at(VectorIndices::X8), given.at(VectorIndices::X8), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), wwww(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X4), given.at(VectorIndices::X1), given.at(VectorIndices::X2), given.at(VectorIndices::X3),
                        given.at(VectorIndices::X8), given.at(VectorIndices::X5), given.at(VectorIndices::X6), given.at(VectorIndices::X7)
                    };
                    _mm256_store_ps(actual.data(), wxyz(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X4), given.at(VectorIndices::X3), given.at(VectorIndices::X2), given.at(VectorIndices::X1),
                        given.at(VectorIndices::X8), given.at(VectorIndices::X7), given.at(VectorIndices::X6), given.at(VectorIndices::X5)
                    };
                    _mm256_store_ps(actual.data(), wzyx(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzlesIdentityAndItsReverse) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                std::array<Float_32, VectorSizes::X8> actual{ };

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X2), given.at(VectorIndices::X3), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X6), given.at(VectorIndices::X7), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4,
                        VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X8), given.at(VectorIndices::X7), given.at(VectorIndices::X6), given.at(VectorIndices::X5),
                        given.at(VectorIndices::X4), given.at(VectorIndices::X3), given.at(VectorIndices::X2), given.at(VectorIndices::X1)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X8, VectorIndices::X7, VectorIndices::X6, VectorIndices::X5,
                        VectorIndices::X4, VectorIndices::X3, VectorIndices::X2, VectorIndices::X1
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzlesBroadcast) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                std::array<Float_32, VectorSizes::X8> actual{ };

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X1),
                        given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X1)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X1, VectorIndices::X1, VectorIndices::X1, VectorIndices::X1,
                        VectorIndices::X1, VectorIndices::X1, VectorIndices::X1, VectorIndices::X1
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X2), given.at(VectorIndices::X2), given.at(VectorIndices::X2), given.at(VectorIndices::X2),
                        given.at(VectorIndices::X2), given.at(VectorIndices::X2), given.at(VectorIndices::X2), given.at(VectorIndices::X2)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X2, VectorIndices::X2, VectorIndices::X2, VectorIndices::X2,
                        VectorIndices::X2, VectorIndices::X2, VectorIndices::X2, VectorIndices::X2
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X3),
                        given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X3)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X3, VectorIndices::X3, VectorIndices::X3, VectorIndices::X3,
                        VectorIndices::X3, VectorIndices::X3, VectorIndices::X3, VectorIndices::X3
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X4), given.at(VectorIndices::X4), given.at(VectorIndices::X4), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X4), given.at(VectorIndices::X4), given.at(VectorIndices::X4), given.at(VectorIndices::X4)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X4, VectorIndices::X4, VectorIndices::X4, VectorIndices::X4,
                        VectorIndices::X4, VectorIndices::X4, VectorIndices::X4, VectorIndices::X4
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X5),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X5)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X5, VectorIndices::X5, VectorIndices::X5, VectorIndices::X5,
                        VectorIndices::X5, VectorIndices::X5, VectorIndices::X5, VectorIndices::X5
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X6), given.at(VectorIndices::X6), given.at(VectorIndices::X6), given.at(VectorIndices::X6),
                        given.at(VectorIndices::X6), given.at(VectorIndices::X6), given.at(VectorIndices::X6), given.at(VectorIndices::X6)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X6, VectorIndices::X6, VectorIndices::X6, VectorIndices::X6,
                        VectorIndices::X6, VectorIndices::X6, VectorIndices::X6, VectorIndices::X6
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X7),
                        given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X7)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X7, VectorIndices::X7, VectorIndices::X7, VectorIndices::X7,
                        VectorIndices::X7, VectorIndices::X7, VectorIndices::X7, VectorIndices::X7
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X8), given.at(VectorIndices::X8), given.at(VectorIndices::X8), given.at(VectorIndices::X8),
                        given.at(VectorIndices::X8), given.at(VectorIndices::X8), given.at(VectorIndices::X8), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X8, VectorIndices::X8, VectorIndices::X8, VectorIndices::X8,
                        VectorIndices::X8, VectorIndices::X8, VectorIndices::X8, VectorIndices::X8
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzlesDuplicateForms) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                std::array<Float_32, VectorSizes::X8> actual{ };

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X2), given.at(VectorIndices::X2),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X6), given.at(VectorIndices::X6)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X1, VectorIndices::X1, VectorIndices::X2, VectorIndices::X2,
                        VectorIndices::X5, VectorIndices::X5, VectorIndices::X6, VectorIndices::X6
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X1), given.at(VectorIndices::X3), given.at(VectorIndices::X3),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X5), given.at(VectorIndices::X7), given.at(VectorIndices::X7)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X1, VectorIndices::X1, VectorIndices::X3, VectorIndices::X3,
                        VectorIndices::X5, VectorIndices::X5, VectorIndices::X7, VectorIndices::X7
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X1), given.at(VectorIndices::X2), given.at(VectorIndices::X3), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X1), given.at(VectorIndices::X2), given.at(VectorIndices::X3), given.at(VectorIndices::X4)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4,
                        VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X2), given.at(VectorIndices::X2), given.at(VectorIndices::X4), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X6), given.at(VectorIndices::X6), given.at(VectorIndices::X8), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X2, VectorIndices::X2, VectorIndices::X4, VectorIndices::X4,
                        VectorIndices::X6, VectorIndices::X6, VectorIndices::X8, VectorIndices::X8
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X3), given.at(VectorIndices::X3), given.at(VectorIndices::X4), given.at(VectorIndices::X4),
                        given.at(VectorIndices::X7), given.at(VectorIndices::X7), given.at(VectorIndices::X8), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X3, VectorIndices::X3, VectorIndices::X4, VectorIndices::X4,
                        VectorIndices::X7, VectorIndices::X7, VectorIndices::X8, VectorIndices::X8
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X5), given.at(VectorIndices::X6), given.at(VectorIndices::X7), given.at(VectorIndices::X8),
                        given.at(VectorIndices::X1), given.at(VectorIndices::X2), given.at(VectorIndices::X3), given.at(VectorIndices::X4)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8,
                        VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        given.at(VectorIndices::X5), given.at(VectorIndices::X6), given.at(VectorIndices::X7), given.at(VectorIndices::X8),
                        given.at(VectorIndices::X5), given.at(VectorIndices::X6), given.at(VectorIndices::X7), given.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), swizzle<
                        VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8,
                        VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8
                    >(_mm256_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }
        };
    }
}