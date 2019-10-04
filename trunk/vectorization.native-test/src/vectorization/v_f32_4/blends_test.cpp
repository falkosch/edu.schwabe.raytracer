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

            static const std::array<v_f32_4::ValueType, v_f32_4::SIZE> sampleArray() {
                return StandardSample::ofArrayType<v_f32_4::ValueType, v_f32_4::SIZE>();
            }

            static const v_f32_4 sampleVector() {
                return StandardSample::ofVectorType<v_f32_4>();
            }

            TEST_METHOD(blendsXYZWGenerically)
            {
                auto givenVector = One<v_f32_4>();
                auto expectedBlends = Two<v_f32_4>();

                {
                    auto testBlended = blend<false, false, false, false>(givenVector, expectedBlends);
                    Assert::IsTrue(allTrue(expectedBlends != testBlended));
                }

                {
                    auto testBlended = blend<true, false, false, false>(givenVector, expectedBlends);
                    Assert::AreEqual(x(expectedBlends), x(testBlended));
                    Assert::AreNotEqual(y(expectedBlends), y(testBlended));
                    Assert::AreNotEqual(z(expectedBlends), z(testBlended));
                    Assert::AreNotEqual(w(expectedBlends), w(testBlended));
                }

                {
                    auto testBlended = blend<false, true, false, false>(givenVector, expectedBlends);
                    Assert::AreNotEqual(x(expectedBlends), x(testBlended));
                    Assert::AreEqual(y(expectedBlends), y(testBlended));
                    Assert::AreNotEqual(z(expectedBlends), z(testBlended));
                    Assert::AreNotEqual(w(expectedBlends), w(testBlended));
                }

                {
                    auto testBlended = blend<false, false, true, false>(givenVector, expectedBlends);
                    Assert::AreNotEqual(x(expectedBlends), x(testBlended));
                    Assert::AreNotEqual(y(expectedBlends), y(testBlended));
                    Assert::AreEqual(z(expectedBlends), z(testBlended));
                    Assert::AreNotEqual(w(expectedBlends), w(testBlended));
                }

                {
                    auto testBlended = blend<false, false, false, true>(givenVector, expectedBlends);
                    Assert::AreNotEqual(x(expectedBlends), x(testBlended));
                    Assert::AreNotEqual(y(expectedBlends), y(testBlended));
                    Assert::AreNotEqual(z(expectedBlends), z(testBlended));
                    Assert::AreEqual(w(expectedBlends), w(testBlended));
                }
            }

        };

    }
}
