#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(v_f32_4_FunctionsTrigonometryTest)
        {
        public:

            TEST_METHOD(computesLog) {
                auto test = StandardSample::importantFloatConstants<v_f32_4::ValueType>();

                auto matchers = {
                    Compare::equalsOrdered<v_f32_4>, // Zero
                    Compare::equalsOrdered<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::equalsOrdered<v_f32_4>, // One
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::isBothNotANumber<v_f32_4>,

                    Compare::nearEqualsRelative<v_f32_4>, // Two
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>, // ReciprocalPi
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::isBothNotANumber<v_f32_4>,

                    Compare::nearEqualsRelative<v_f32_4>, // RadianToDegree
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>, // Sin45
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::equalsOrdered<v_f32_4>,
                    Compare::isBothNotANumber<v_f32_4>,

                    Compare::nearEqualsRelative<v_f32_4>, // Epsilon
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>, // max
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::isBothNotANumber<v_f32_4>,

                    Compare::nearEqualsRelative<v_f32_4>, // denorm_min
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>, // round_error
                    Compare::isBothNotANumber<v_f32_4>,
                    Compare::isBothNotANumber<v_f32_4>
                };

                auto matcher = matchers.begin();
                for (auto i = Zero<ASizeT>(); i < test.size(); ++i, ++matcher) {
                    auto testValue = test.at(i);
                    auto testVector = v_f32_4(testValue);
                    auto actual = vectorization::log(testVector);
                    auto expected = v_f32_4(std::log(testValue));
                    auto match = (*matcher)(actual, expected);

                    auto wmessage = MessageFormat::forFunction(i, testValue, "value mismatch ", x(actual), " !~~ ", x(expected));
                    Assert::IsTrue(allTrue(match), wmessage.c_str(), LINE_INFO());
                }
            }

            TEST_METHOD(computesExp) {
                auto test = StandardSample::importantFloatConstants<v_f32_4::ValueType>();

                auto matchers = {
                    Compare::nearEqualsRelative<v_f32_4>, // Zero
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>, // One
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,

                    Compare::nearEqualsRelative<v_f32_4>, // Two
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>, // ReciprocalPi
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,

                    Compare::nearEqualsRelative<v_f32_4>, // RadianToDegree
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>, // Sin45
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::equalsOrdered<v_f32_4>,
                    Compare::equalsOrdered<v_f32_4>,

                    Compare::nearEqualsRelative<v_f32_4>, // Epsilon
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::equalsOrdered<v_f32_4>, // max
                    Compare::equalsOrdered<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,

                    Compare::nearEqualsRelative<v_f32_4>, // denorm_min
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::nearEqualsRelative<v_f32_4>, // round_error
                    Compare::nearEqualsRelative<v_f32_4>,
                    Compare::isBothNotANumber<v_f32_4>
                };

                auto matcher = matchers.begin();
                for (auto i = Zero<ASizeT>(); i < test.size(); ++i, ++matcher) {
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