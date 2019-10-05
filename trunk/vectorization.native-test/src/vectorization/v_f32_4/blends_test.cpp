#include "StandardSample.h"

#include <CppUnitTest.h>
#include <vectorization.h>

#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(v_f32_4_BlendsTest)
        {
        public:

            TEST_METHOD(genericallyBlendsIdentities) {
                auto given = One<v_f32_4>();
                auto expectedXYZW = Two<v_f32_4>();

                {
                    auto actual = blend<false, false, false, false>(given, expectedXYZW);
                    Assert::IsTrue(allTrue(expectedXYZW != actual), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualXYZW = blend<true, true, true, true>(given, expectedXYZW);
                    Assert::IsTrue(allTrue(expectedXYZW == actualXYZW), L"blend mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsIndividuals) {
                auto given = One<v_f32_4>();
                auto expectedXYZW = Two<v_f32_4>();

                {
                    auto actualX = blend<true, false, false, false>(given, expectedXYZW);
                    Assert::AreEqual(x(expectedXYZW), x(actualX), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(y(expectedXYZW), y(actualX), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(z(expectedXYZW), z(actualX), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(w(expectedXYZW), w(actualX), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualY = blend<false, true, false, false>(given, expectedXYZW);
                    Assert::AreNotEqual(x(expectedXYZW), x(actualY), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(y(expectedXYZW), y(actualY), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(z(expectedXYZW), z(actualY), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(w(expectedXYZW), w(actualY), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualZ = blend<false, false, true, false>(given, expectedXYZW);
                    Assert::AreNotEqual(x(expectedXYZW), x(actualZ), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(y(expectedXYZW), y(actualZ), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(z(expectedXYZW), z(actualZ), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(w(expectedXYZW), w(actualZ), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualW = blend<false, false, false, true>(given, expectedXYZW);
                    Assert::AreNotEqual(x(expectedXYZW), x(actualW), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(y(expectedXYZW), y(actualW), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(z(expectedXYZW), z(actualW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(w(expectedXYZW), w(actualW), L"blend mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsPairs) {
                auto given = One<v_f32_4>();
                auto expectedXYZW = Two<v_f32_4>();

                {
                    auto actualXY = blend<true, true, false, false>(given, expectedXYZW);
                    Assert::AreEqual(x(expectedXYZW), x(actualXY), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(y(expectedXYZW), y(actualXY), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(z(expectedXYZW), z(actualXY), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(w(expectedXYZW), w(actualXY), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualXZ = blend<true, false, true, false>(given, expectedXYZW);
                    Assert::AreEqual(x(expectedXYZW), x(actualXZ), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(y(expectedXYZW), y(actualXZ), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(z(expectedXYZW), z(actualXZ), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(w(expectedXYZW), w(actualXZ), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualYW = blend<false, true, false, true>(given, expectedXYZW);
                    Assert::AreNotEqual(x(expectedXYZW), x(actualYW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(y(expectedXYZW), y(actualYW), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(z(expectedXYZW), z(actualYW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(w(expectedXYZW), w(actualYW), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualZW = blend<false, false, true, true>(given, expectedXYZW);
                    Assert::AreNotEqual(x(expectedXYZW), x(actualZW), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(y(expectedXYZW), y(actualZW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(z(expectedXYZW), z(actualZW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(w(expectedXYZW), w(actualZW), L"blend mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsTriplets) {
                auto given = One<v_f32_4>();
                auto expectedXYZW = Two<v_f32_4>();

                {
                    auto actualYZW = blend<false, true, true, true>(given, expectedXYZW);
                    Assert::AreNotEqual(x(expectedXYZW), x(actualYZW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(y(expectedXYZW), y(actualYZW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(z(expectedXYZW), z(actualYZW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(w(expectedXYZW), w(actualYZW), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualXZW = blend<true, false, true, true>(given, expectedXYZW);
                    Assert::AreEqual(x(expectedXYZW), x(actualXZW), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(y(expectedXYZW), y(actualXZW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(z(expectedXYZW), z(actualXZW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(w(expectedXYZW), w(actualXZW), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualXYW = blend<true, true, false, true>(given, expectedXYZW);
                    Assert::AreEqual(x(expectedXYZW), x(actualXYW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(y(expectedXYZW), y(actualXYW), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(z(expectedXYZW), z(actualXYW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(w(expectedXYZW), w(actualXYW), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualXYZ = blend<true, true, true, false>(given, expectedXYZW);
                    Assert::AreEqual(x(expectedXYZW), x(actualXYZ), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(y(expectedXYZW), y(actualXYZ), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(z(expectedXYZW), z(actualXYZ), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(w(expectedXYZW), w(actualXYZ), L"blend mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormForBlends) {
                auto given = One<v_f32_4>();
                auto expectedXYZW = Two<v_f32_4>();

                {
                    auto actualYZW = x_yzw(given, expectedXYZW);
                    Assert::AreNotEqual(x(expectedXYZW), x(actualYZW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(y(expectedXYZW), y(actualYZW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(z(expectedXYZW), z(actualYZW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(w(expectedXYZW), w(actualYZW), L"blend mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(dynamicallyBlendsWithMaskOfSameType) {
                auto given = One<v_f32_4>();
                auto expectedXYZW = Two<v_f32_4>();

                {
                    auto actual = blendMasked(given, expectedXYZW, MaskNone<v_f32_4>());
                    Assert::IsTrue(allTrue(expectedXYZW != actual), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualXYZW = blendMasked(given, expectedXYZW, MaskAll<v_f32_4>());
                    Assert::IsTrue(allTrue(expectedXYZW == actualXYZW), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualX = blendMasked(given, expectedXYZW, MaskX<v_f32_4>());
                    Assert::AreEqual(x(expectedXYZW), x(actualX), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(y(expectedXYZW), y(actualX), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(z(expectedXYZW), z(actualX), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(w(expectedXYZW), w(actualX), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualY = blendMasked(given, expectedXYZW, MaskY<v_f32_4>());
                    Assert::AreNotEqual(x(expectedXYZW), x(actualY), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(y(expectedXYZW), y(actualY), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(z(expectedXYZW), z(actualY), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(w(expectedXYZW), w(actualY), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualZ = blendMasked(given, expectedXYZW, MaskZ<v_f32_4>());
                    Assert::AreNotEqual(x(expectedXYZW), x(actualZ), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(y(expectedXYZW), y(actualZ), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(z(expectedXYZW), z(actualZ), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(w(expectedXYZW), w(actualZ), L"blend mismatch", LINE_INFO());
                }

                {
                    auto actualW = blendMasked(given, expectedXYZW, MaskW<v_f32_4>());
                    Assert::AreNotEqual(x(expectedXYZW), x(actualW), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(y(expectedXYZW), y(actualW), L"blend mismatch", LINE_INFO());
                    Assert::AreNotEqual(z(expectedXYZW), z(actualW), L"blend mismatch", LINE_INFO());
                    Assert::AreEqual(w(expectedXYZW), w(actualW), L"blend mismatch", LINE_INFO());
                }
            }
        };
    }
}