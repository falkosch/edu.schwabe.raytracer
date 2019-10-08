#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(v_f32_4_ConversionsTest)
        {
        public:

            TEST_METHOD(testExample1) {
                auto expected = 0;
                auto actual = 0;
                Assert::AreEqual(expected, actual, L"", LINE_INFO());
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