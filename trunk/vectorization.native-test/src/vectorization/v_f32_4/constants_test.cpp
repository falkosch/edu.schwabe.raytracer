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

            TEST_METHOD(hasMaskNone) {
                Iterate::toExclusiveEnd<v_f32_4::SIZE>([](auto i, auto message) {
                    Assert::AreEqual(MaskNone<v_f32_4::ValueType>(), MaskNone<v_f32_4>()[i], message.c_str(), LINE_INFO());
                });
            }

            TEST_METHOD(hasMaskAll) {
                Iterate::toExclusiveEnd<v_f32_4::SIZE>([](auto i, auto message) {
                    Assert::AreEqual(MaskAll<v_f32_4::ValueType>(), MaskAll<v_f32_4>()[i], message.c_str(), LINE_INFO());
                });
            }

            TEST_METHOD(hasMaskX) {
                Assert::AreEqual(MaskAll<v_f32_4::ValueType>(), x(MaskX<v_f32_4>()), L"", LINE_INFO());
            }
        };
    }
}