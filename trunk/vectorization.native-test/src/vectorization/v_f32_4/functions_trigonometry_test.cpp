#include "../../Compare.h"
#include "../../MessageFormat.h"
#include "../../StandardSample.h"

#include <CppUnitTest.h>
#include <vectorization.h>

#include <algorithm>
#include <array>
#include <cmath>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
    namespace test
    {

        TEST_CLASS(v_f32_4_FunctionsTrigonometryTest)
        {
        public:

            TEST_METHOD(computesLog)
            {
                auto test = StandardSample::importantFloatConstants();

                auto matchers = {
                    Compare::equalsOrdered, // Zero
                    Compare::equalsOrdered,
                    Compare::nearEqualsRelative,
                    Compare::isBothNotANumber,
                    Compare::equalsOrdered, // One
                    Compare::isBothNotANumber,
                    Compare::nearEqualsRelative,
                    Compare::isBothNotANumber,

                    Compare::nearEqualsRelative, // Two
                    Compare::isBothNotANumber,
                    Compare::nearEqualsRelative,
                    Compare::isBothNotANumber,
                    Compare::nearEqualsRelative, // ReciprocalPi
                    Compare::isBothNotANumber,
                    Compare::nearEqualsRelative,
                    Compare::isBothNotANumber,

                    Compare::nearEqualsRelative, // RadianToDegree
                    Compare::isBothNotANumber,
                    Compare::nearEqualsRelative,
                    Compare::isBothNotANumber,
                    Compare::nearEqualsRelative, // Sin45
                    Compare::isBothNotANumber,
                    Compare::equalsOrdered,
                    Compare::isBothNotANumber,

                    Compare::nearEqualsRelative, // Epsilon
                    Compare::isBothNotANumber,
                    Compare::nearEqualsRelative,
                    Compare::isBothNotANumber,
                    Compare::nearEqualsRelative, // max
                    Compare::isBothNotANumber,
                    Compare::nearEqualsRelative,
                    Compare::isBothNotANumber,

                    Compare::nearEqualsRelative, // denorm_min
                    Compare::isBothNotANumber,
                    Compare::nearEqualsRelative,
                    Compare::isBothNotANumber,
                    Compare::nearEqualsRelative, // round_error
                    Compare::isBothNotANumber,
                    Compare::isBothNotANumber
                };

                auto matcher = matchers.begin();
                for (auto i = Zero<ASizeT>(); i < test.size(); ++i, ++matcher)
                {
                    auto testValue = test.at(i);
                    auto testVector = v_f32_4(testValue);
                    auto actual = vectorization::log(testVector);
                    auto expected = v_f32_4(std::log(testValue));
                    auto match = (*matcher)(actual, expected);

                    auto wmessage = MessageFormat::forFunction(i, testValue, "value mismatch ", x(actual), " !~~ ", x(expected));
                    Assert::IsTrue(allTrue(match), wmessage.c_str(), LINE_INFO());
                }
            }

            TEST_METHOD(computesExp)
            {
                auto test = StandardSample::importantFloatConstants();

                auto matchers = {
                    Compare::nearEqualsRelative, // Zero
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative, // One
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,

                    Compare::nearEqualsRelative, // Two
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative, // ReciprocalPi
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,

                    Compare::nearEqualsRelative, // RadianToDegree
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative, // Sin45
                    Compare::nearEqualsRelative,
                    Compare::equalsOrdered,
                    Compare::equalsOrdered,

                    Compare::nearEqualsRelative, // Epsilon
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,
                    Compare::equalsOrdered, // max
                    Compare::equalsOrdered,
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,

                    Compare::nearEqualsRelative, // denorm_min
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative,
                    Compare::nearEqualsRelative, // round_error
                    Compare::nearEqualsRelative,
                    Compare::isBothNotANumber
                };

                auto matcher = matchers.begin();
                for (auto i = Zero<ASizeT>(); i < test.size(); ++i, ++matcher)
                {
                    auto testValue = test.at(i);
                    auto actual = vectorization::exp(v_f32_4(testValue));
                    auto expected = v_f32_4(std::exp(testValue));
                    auto match = (*matcher)(actual, expected);

                    auto wmessage = MessageFormat::forFunction(i, testValue, "value mismatch ", x(actual), " !~~ ", x(expected));
                    Assert::IsTrue(allTrue(match), wmessage.c_str(), LINE_INFO());
                }
            }


        };

    }
}
