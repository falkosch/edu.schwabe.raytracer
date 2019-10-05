#include <CppUnitTest.h>
#include <vectorization.h>

#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(CompilerIntrinsicsTest)
        {
        public:

            TEST_METHOD(generatesMasksForFloat4_32) {
                Assert::AreEqual(
                    0 + 0 + 0 + 0,
                    BLEND_MASK_FLOAT4_32(0, 0, 0, 0),
                    L" 0 != MASK(0,0,0,0)",
                    LINE_INFO()
                );

                Assert::AreEqual(
                    1 + 0 + 4 + 0,
                    BLEND_MASK_FLOAT4_32(1, 0, 1, 0),
                    L" 5 != MASK(1,0,1,0)",
                    LINE_INFO()
                );

                Assert::AreEqual(
                    0 + 2 + 0 + 8,
                    BLEND_MASK_FLOAT4_32(0, 1, 0, 1),
                    L"10 != MASK(0,1,0,1)",
                    LINE_INFO()
                );

                Assert::AreEqual(
                    1 + 2 + 4 + 8,
                    BLEND_MASK_FLOAT4_32(1, 1, 1, 1),
                    L"15 != MASK(1,1,1,1)",
                    LINE_INFO()
                );
            }

            TEST_METHOD(generatesMasksForInt4_32) {
                Assert::AreEqual(
                    0 + 0 + 0 + 0 + 0 + 0 + 0 + 0,
                    BLEND_MASK_INT4_32(0, 0, 0, 0),
                    L"  0 != MASK(0,0,0,0)",
                    LINE_INFO()
                );

                Assert::AreEqual(
                    1 + 2 + 0 + 0 + 16 + 32 + 0 + 0,
                    BLEND_MASK_INT4_32(1, 0, 1, 0),
                    L" 51 != MASK(1,0,1,0)",
                    LINE_INFO()
                );

                Assert::AreEqual(
                    0 + 0 + 4 + 8 + 0 + 0 + 64 + 128,
                    BLEND_MASK_INT4_32(0, 1, 0, 1),
                    L"204 != MASK(0,1,0,1)",
                    LINE_INFO()
                );

                Assert::AreEqual(
                    1 + 2 + 4 + 8 + 16 + 32 + 64 + 128,
                    BLEND_MASK_INT4_32(1, 1, 1, 1),
                    L"255 != MASK(1,1,1,1)",
                    LINE_INFO()
                );
            }

            TEST_METHOD(convertsUnsignedIntegersToFloats) {
                auto givenValues = std::array<vectorization::UInt_32, 5>({ 0, 1, 2, 3, 4 });

                for (auto givenValue : givenValues) {
                    auto expected = vectorization::convert<vectorization::Float_32>(givenValue);
                    auto expectedVector = vectorization::v_f32_4(expected);
                    auto givenVector = vectorization::v_ui32_4(givenValue);
                    auto actualVector = vectorization::_mm_cvtepu32_ps(givenVector);

                    Assert::IsTrue(
                        vectorization::allTrue(expectedVector == actualVector),
                        L"Conversion error in _mm_cvtepu32_ps",
                        LINE_INFO()
                    );
                }
            }
        };
    }
}