#include "vectorization_tests.h"

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

            TEST_METHOD(readsComponent) {
                auto expectedComponents = sampleArray();
                auto givenVector = sampleVector();

                Assert::AreEqual(expectedComponents.at(VectorIndices::X), component<VectorIndices::X>(givenVector));
                Assert::AreEqual(expectedComponents.at(VectorIndices::Y), component<VectorIndices::Y>(givenVector));
                Assert::AreEqual(expectedComponents.at(VectorIndices::Z), component<VectorIndices::Z>(givenVector));
                Assert::AreEqual(expectedComponents.at(VectorIndices::W), component<VectorIndices::W>(givenVector));
            }

            TEST_METHOD(readsXYZW) {
                auto expectedComponents = sampleArray();
                auto givenVector = sampleVector();

                Assert::AreEqual(expectedComponents.at(VectorIndices::X), x(givenVector));
                Assert::AreEqual(expectedComponents.at(VectorIndices::Y), y(givenVector));
                Assert::AreEqual(expectedComponents.at(VectorIndices::Z), z(givenVector));
                Assert::AreEqual(expectedComponents.at(VectorIndices::W), w(givenVector));
            }

            TEST_METHOD(replacesComponent) {
                auto givenVector = sampleVector();
                auto expectedReplacement = 5.0f;

                {
                    auto actualX = replaceComponent<VectorIndices::X>(givenVector, expectedReplacement);
                    Assert::AreEqual(expectedReplacement, x(actualX));
                    Assert::AreNotEqual(expectedReplacement, y(actualX));
                    Assert::AreNotEqual(expectedReplacement, z(actualX));
                    Assert::AreNotEqual(expectedReplacement, w(actualX));
                }

                {
                    auto actualY = replaceComponent<VectorIndices::Y>(givenVector, expectedReplacement);
                    Assert::AreNotEqual(expectedReplacement, x(actualY));
                    Assert::AreEqual(expectedReplacement, y(actualY));
                    Assert::AreNotEqual(expectedReplacement, z(actualY));
                    Assert::AreNotEqual(expectedReplacement, w(actualY));
                }

                {
                    auto actualZ = replaceComponent<VectorIndices::Z>(givenVector, expectedReplacement);
                    Assert::AreNotEqual(expectedReplacement, x(actualZ));
                    Assert::AreNotEqual(expectedReplacement, y(actualZ));
                    Assert::AreEqual(expectedReplacement, z(actualZ));
                    Assert::AreNotEqual(expectedReplacement, w(actualZ));
                }

                {
                    auto actualW = replaceComponent<VectorIndices::W>(givenVector, expectedReplacement);
                    Assert::AreNotEqual(expectedReplacement, x(actualW));
                    Assert::AreNotEqual(expectedReplacement, y(actualW));
                    Assert::AreNotEqual(expectedReplacement, z(actualW));
                    Assert::AreEqual(expectedReplacement, w(actualW));
                }
            }

            TEST_METHOD(replacesXYZW) {
                auto givenVector = sampleVector();
                auto expectedReplacement = 5.0f;

                {
                    auto actualX = replaceX(givenVector, expectedReplacement);
                    Assert::AreEqual(expectedReplacement, x(actualX));
                    Assert::AreNotEqual(expectedReplacement, y(actualX));
                    Assert::AreNotEqual(expectedReplacement, z(actualX));
                    Assert::AreNotEqual(expectedReplacement, w(actualX));
                }

                {
                    auto actualY = replaceY(givenVector, expectedReplacement);
                    Assert::AreNotEqual(expectedReplacement, x(actualY));
                    Assert::AreEqual(expectedReplacement, y(actualY));
                    Assert::AreNotEqual(expectedReplacement, z(actualY));
                    Assert::AreNotEqual(expectedReplacement, w(actualY));
                }

                {
                    auto actualZ = replaceZ(givenVector, expectedReplacement);
                    Assert::AreNotEqual(expectedReplacement, x(actualZ));
                    Assert::AreNotEqual(expectedReplacement, y(actualZ));
                    Assert::AreEqual(expectedReplacement, z(actualZ));
                    Assert::AreNotEqual(expectedReplacement, w(actualZ));
                }

                {
                    auto actualW = replaceW(givenVector, expectedReplacement);
                    Assert::AreNotEqual(expectedReplacement, x(actualW));
                    Assert::AreNotEqual(expectedReplacement, y(actualW));
                    Assert::AreNotEqual(expectedReplacement, z(actualW));
                    Assert::AreEqual(expectedReplacement, w(actualW));
                }
            }
        };
    }
}