#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(Swizzle128sTest)
        {
        public:

            TEST_METHOD(genericallySwizzlesIdentityAndItsReverse) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::Z), given.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::W), given.at(VectorIndices::Z), given.at(VectorIndices::Y), given.at(VectorIndices::X)
                    };
                    _mm_store_ps(actual.data(), swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzlesBroadcast) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X)
                    };
                    _mm_store_ps(actual.data(), swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::Y)
                    };
                    _mm_store_ps(actual.data(), swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z)
                    };
                    _mm_store_ps(actual.data(), swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::W), given.at(VectorIndices::W), given.at(VectorIndices::W), given.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzlesDuplicateForms) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::Y)
                    };
                    _mm_store_ps(actual.data(), swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Z), given.at(VectorIndices::Z)
                    };
                    _mm_store_ps(actual.data(), swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::X), given.at(VectorIndices::Y)
                    };
                    _mm_store_ps(actual.data(), swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::W), given.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::Z), given.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzlesOfXFirst) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X)
                    };
                    _mm_store_ps(actual.data(), xxxx(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Z)
                    };
                    _mm_store_ps(actual.data(), xxxz(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::Y)
                    };
                    _mm_store_ps(actual.data(), xxyy(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Z), given.at(VectorIndices::Z)
                    };
                    _mm_store_ps(actual.data(), xxzz(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::X), given.at(VectorIndices::Y)
                    };
                    _mm_store_ps(actual.data(), xyxy(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z)
                    };
                    _mm_store_ps(actual.data(), xzzz(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzlesOfYFirst) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::Y)
                    };
                    _mm_store_ps(actual.data(), yyyy(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Y)
                    };
                    _mm_store_ps(actual.data(), yxxy(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::X), given.at(VectorIndices::W), given.at(VectorIndices::Z)
                    };
                    _mm_store_ps(actual.data(), yxwz(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::W), given.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), yyww(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Z), given.at(VectorIndices::X), given.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), yzxw(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::X)
                    };
                    _mm_store_ps(actual.data(), yzwx(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzlesOfZFirst) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z)
                    };
                    _mm_store_ps(actual.data(), zzzz(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), zxyw(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Y), given.at(VectorIndices::Y)
                    };
                    _mm_store_ps(actual.data(), zzyy(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), zzww(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::X), given.at(VectorIndices::Y)
                    };
                    _mm_store_ps(actual.data(), zwxy(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::Z), given.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), zwzw(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzlesOfWFirst) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::W), given.at(VectorIndices::W), given.at(VectorIndices::W), given.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), wwww(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::W), given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::Z)
                    };
                    _mm_store_ps(actual.data(), wxyz(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        given.at(VectorIndices::W), given.at(VectorIndices::Z), given.at(VectorIndices::Y), given.at(VectorIndices::X)
                    };
                    _mm_store_ps(actual.data(), wzyx(_mm_load_ps(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }
        };
    }
}