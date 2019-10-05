#include "Iterate.h"

#include <CppUnitTest.h>
#include <vectorization.h>

#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(v_f32_4_ConstantsTest)
        {
        public:

            TEST_METHOD(hasMaskAll) {
                Iterate::toExclusiveEnd<v_f32_4::SIZE>(
                    [](auto i, auto message) {
                    Assert::AreEqual(MaskAll<v_f32_4::ValueType>(), MaskAll<v_f32_4>()[i], message.c_str(), LINE_INFO());
                }
                );
            }

            TEST_METHOD(testExample2) {
                auto expected = 0;
                auto actual = 1;
                Assert::AreEqual(expected, actual, L"", LINE_INFO());
            }

            TEST_METHOD(testExample3) {
                std::array<vectorization::UInt_32, 5> specimen = { 0, 1, 2, 3, 4 };

                for (const auto & v : specimen) {
                    Assert::IsTrue(v > -1, L"", LINE_INFO());
                }
            }
        };
    }
}