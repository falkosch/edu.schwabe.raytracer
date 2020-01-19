#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(v_f32_4_SwizzledBlendsTest)
        {
        public:

            TEST_METHOD(testExample2) {
                auto expected = 0;
                auto actual = 1;
                Assert::AreEqual(expected, actual, L"", LINE_INFO());
            }
        };
    }
}