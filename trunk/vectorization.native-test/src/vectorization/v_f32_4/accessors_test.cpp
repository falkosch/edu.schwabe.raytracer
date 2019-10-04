#include "StandardSample.h"

#include <CppUnitTest.h>
#include <vectorization.h>

#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
    namespace test
    {

        TEST_CLASS(v_f32_4_AccessorsTest)
        {
        public:

            static const std::array<v_f32_4::ValueType, v_f32_4::SIZE> sampleArray() {
                return StandardSample::ofArrayType<v_f32_4::ValueType, v_f32_4::SIZE>();
            }

            static const v_f32_4 sampleVector() {
                return StandardSample::ofVectorType<v_f32_4>();
            }

            TEST_METHOD(readsComponent)
            {
                auto expected = sampleArray();
                auto testValue = sampleVector();

                Assert::AreEqual(expected.at(VectorIndices::X), component<VectorIndices::X>(testValue));
                Assert::AreEqual(expected.at(VectorIndices::Y), component<VectorIndices::Y>(testValue));
                Assert::AreEqual(expected.at(VectorIndices::Z), component<VectorIndices::Z>(testValue));
                Assert::AreEqual(expected.at(VectorIndices::W), component<VectorIndices::W>(testValue));
            }

            TEST_METHOD(readsXYZW)
            {
                auto expected = sampleArray();
                auto testValue = sampleVector();

                Assert::AreEqual(expected.at(VectorIndices::X), x(testValue));
                Assert::AreEqual(expected.at(VectorIndices::Y), y(testValue));
                Assert::AreEqual(expected.at(VectorIndices::Z), z(testValue));
                Assert::AreEqual(expected.at(VectorIndices::W), w(testValue));
            }

            TEST_METHOD(replacesComponent)
            {
                auto testValue = sampleVector();
                auto testReplacement = 5.0f;

                {
                    auto testReplaced = replaceComponent<VectorIndices::X>(testValue, testReplacement);
                    Assert::AreEqual(testReplacement, x(testReplaced));
                    Assert::AreNotEqual(testReplacement, y(testReplaced));
                    Assert::AreNotEqual(testReplacement, z(testReplaced));
                    Assert::AreNotEqual(testReplacement, w(testReplaced));
                }

                {
                    auto testReplaced = replaceComponent<VectorIndices::Y>(testValue, testReplacement);
                    Assert::AreNotEqual(testReplacement, x(testReplaced));
                    Assert::AreEqual(testReplacement, y(testReplaced));
                    Assert::AreNotEqual(testReplacement, z(testReplaced));
                    Assert::AreNotEqual(testReplacement, w(testReplaced));
                }

                {
                    auto testReplaced = replaceComponent<VectorIndices::Z>(testValue, testReplacement);
                    Assert::AreNotEqual(testReplacement, x(testReplaced));
                    Assert::AreNotEqual(testReplacement, y(testReplaced));
                    Assert::AreEqual(testReplacement, z(testReplaced));
                    Assert::AreNotEqual(testReplacement, w(testReplaced));
                }

                {
                    auto testReplaced = replaceComponent<VectorIndices::W>(testValue, testReplacement);
                    Assert::AreNotEqual(testReplacement, x(testReplaced));
                    Assert::AreNotEqual(testReplacement, y(testReplaced));
                    Assert::AreNotEqual(testReplacement, z(testReplaced));
                    Assert::AreEqual(testReplacement, w(testReplaced));
                }
            }

            TEST_METHOD(replacesXYZW)
            {
                auto testValue = sampleVector();
                auto testReplacement = 5.0f;

                {
                    auto testReplaced = replaceX(testValue, testReplacement);
                    Assert::AreEqual(testReplacement, x(testReplaced));
                    Assert::AreNotEqual(testReplacement, y(testReplaced));
                    Assert::AreNotEqual(testReplacement, z(testReplaced));
                    Assert::AreNotEqual(testReplacement, w(testReplaced));
                }

                {
                    auto testReplaced = replaceY(testValue, testReplacement);
                    Assert::AreNotEqual(testReplacement, x(testReplaced));
                    Assert::AreEqual(testReplacement, y(testReplaced));
                    Assert::AreNotEqual(testReplacement, z(testReplaced));
                    Assert::AreNotEqual(testReplacement, w(testReplaced));
                }

                {
                    auto testReplaced = replaceZ(testValue, testReplacement);
                    Assert::AreNotEqual(testReplacement, x(testReplaced));
                    Assert::AreNotEqual(testReplacement, y(testReplaced));
                    Assert::AreEqual(testReplacement, z(testReplaced));
                    Assert::AreNotEqual(testReplacement, w(testReplaced));
                }

                {
                    auto testReplaced = replaceW(testValue, testReplacement);
                    Assert::AreNotEqual(testReplacement, x(testReplaced));
                    Assert::AreNotEqual(testReplacement, y(testReplaced));
                    Assert::AreNotEqual(testReplacement, z(testReplaced));
                    Assert::AreEqual(testReplacement, w(testReplaced));
                }
            }

            TEST_METHOD(setsComponent)
            {
                auto testReplacement = 5.0f;

                {
                    auto testValue = sampleVector();
                    setComponent<VectorIndices::X>(testValue, testReplacement);
                    Assert::AreEqual(testReplacement, x(testValue));
                    Assert::AreNotEqual(testReplacement, y(testValue));
                    Assert::AreNotEqual(testReplacement, z(testValue));
                    Assert::AreNotEqual(testReplacement, w(testValue));
                }

                {
                    auto testValue = sampleVector();
                    setComponent<VectorIndices::Y>(testValue, testReplacement);
                    Assert::AreNotEqual(testReplacement, x(testValue));
                    Assert::AreEqual(testReplacement, y(testValue));
                    Assert::AreNotEqual(testReplacement, z(testValue));
                    Assert::AreNotEqual(testReplacement, w(testValue));
                }

                {
                    auto testValue = sampleVector();
                    setComponent<VectorIndices::Z>(testValue, testReplacement);
                    Assert::AreNotEqual(testReplacement, x(testValue));
                    Assert::AreNotEqual(testReplacement, y(testValue));
                    Assert::AreEqual(testReplacement, z(testValue));
                    Assert::AreNotEqual(testReplacement, w(testValue));
                }

                {
                    auto testValue = sampleVector();
                    setComponent<VectorIndices::W>(testValue, testReplacement);
                    Assert::AreNotEqual(testReplacement, x(testValue));
                    Assert::AreNotEqual(testReplacement, y(testValue));
                    Assert::AreNotEqual(testReplacement, z(testValue));
                    Assert::AreEqual(testReplacement, w(testValue));
                }
            }

        };

    }
}
