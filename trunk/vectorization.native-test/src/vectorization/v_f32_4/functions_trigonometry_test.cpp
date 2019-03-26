#include <CppUnitTest.h>
#include <vectorization.h>

#include <algorithm>
#include <array>
#include <limits>
#include <cmath>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
	namespace test
	{

		TEST_CLASS(v_f32_4_FunctionsTrigonometryTest)
		{
		public:

			typedef const v_f32_4::VectorBoolType(*MatcherFunctionPtr)(const v_f32_4 &, const v_f32_4 &);

			static const std::wstring format(const ASizeT index, const v_f32_4::ValueType value, const std::string & prefix, const v_f32_4::ValueType actual, const std::string & operatorText, const v_f32_4::ValueType expected)
			{
				auto nmessage = (std::ostringstream() << "[" << index << "]: " << value << " " << prefix << ": "
					<< actual << " " << operatorText << " " << expected).str();
				return std::wstring(nmessage.cbegin(), nmessage.cend());
			}

			static const v_f32_4::VectorBoolType nearEqualsRelative(const v_f32_4 & a, const v_f32_4 & b)
			{
				const auto maxOfAbsoluteOfBoth = max(abs(a), abs(b));
				return (abs(a - b) / maxOfAbsoluteOfBoth) < Epsilon<v_f32_4>();
			}

			static const v_f32_4::VectorBoolType equalsOrdered(const v_f32_4 & a, const v_f32_4 & b)
			{
				return a == b;
			}

			static const v_f32_4::VectorBoolType isBothNotANumber(const v_f32_4 & a, const v_f32_4 & b)
			{
				return vectorization::isNaN(a) & vectorization::isNaN(b);
			}

			static const std::array<v_f32_4::ValueType, 39> samples()
			{
				return {
					Zero<v_f32_4::ValueType>(),
					NegativeZero<v_f32_4::ValueType>(),
					Half<v_f32_4::ValueType>(),
					-Half<v_f32_4::ValueType>(),
					One<v_f32_4::ValueType>(),
					NegativeOne<v_f32_4::ValueType>(),
					OneHalf<v_f32_4::ValueType>(),
					-OneHalf<v_f32_4::ValueType>(),

					Two<v_f32_4::ValueType>(),
					NegativeTwo<v_f32_4::ValueType>(),
					Pi<v_f32_4::ValueType>(),
					-Pi<v_f32_4::ValueType>(),
					ReciprocalPi<v_f32_4::ValueType>(),
					-ReciprocalPi<v_f32_4::ValueType>(),
					DegreeToRadian<v_f32_4::ValueType>(),
					-DegreeToRadian<v_f32_4::ValueType>(),

					RadianToDegree<v_f32_4::ValueType>(),
					-RadianToDegree<v_f32_4::ValueType>(),
					RadianToUniform<v_f32_4::ValueType>(),
					-RadianToUniform<v_f32_4::ValueType>(),
					Sin45<v_f32_4::ValueType>(),
					-Sin45<v_f32_4::ValueType>(),
					Infinity<v_f32_4::ValueType>(),
					NegativeInfinity<v_f32_4::ValueType>(),

					Epsilon<v_f32_4::ValueType>(),
					-Epsilon<v_f32_4::ValueType>(),
					SelfOcclusionEpsilon<v_f32_4::ValueType>(),
					-SelfOcclusionEpsilon<v_f32_4::ValueType>(),
					std::numeric_limits<v_f32_4::ValueType>::max(),
					std::numeric_limits<v_f32_4::ValueType>::lowest(),
					std::numeric_limits<v_f32_4::ValueType>::min(),
					-std::numeric_limits<v_f32_4::ValueType>::min(),

					std::numeric_limits<v_f32_4::ValueType>::denorm_min(),
					-std::numeric_limits<v_f32_4::ValueType>::denorm_min(),
					std::numeric_limits<v_f32_4::ValueType>::epsilon(),
					-std::numeric_limits<v_f32_4::ValueType>::epsilon(),
					std::numeric_limits<v_f32_4::ValueType>::round_error(),
					-std::numeric_limits<v_f32_4::ValueType>::round_error(),
					NotANumber<v_f32_4::ValueType>()
				};
			}

			TEST_METHOD(testLog)
			{
				auto test = samples();

				auto matchers = {
					equalsOrdered, // Zero
					equalsOrdered,
					nearEqualsRelative,
					isBothNotANumber,
					equalsOrdered, // One
					isBothNotANumber,
					nearEqualsRelative,
					isBothNotANumber,

					nearEqualsRelative, // Two
					isBothNotANumber,
					nearEqualsRelative,
					isBothNotANumber,
					nearEqualsRelative, // ReciprocalPi
					isBothNotANumber,
					nearEqualsRelative,
					isBothNotANumber,

					nearEqualsRelative, // RadianToDegree
					isBothNotANumber,
					nearEqualsRelative,
					isBothNotANumber,
					nearEqualsRelative, // Sin45
					isBothNotANumber,
					equalsOrdered,
					isBothNotANumber,

					nearEqualsRelative, // Epsilon
					isBothNotANumber,
					nearEqualsRelative,
					isBothNotANumber,
					nearEqualsRelative, // max
					isBothNotANumber,
					nearEqualsRelative,
					isBothNotANumber,

					nearEqualsRelative, // denorm_min
					isBothNotANumber,
					nearEqualsRelative,
					isBothNotANumber,
					nearEqualsRelative, // round_error
					isBothNotANumber,
					isBothNotANumber
				};

				auto matcher = matchers.begin();
				for (auto i = Zero<ASizeT>(); i < test.size(); ++i, ++matcher)
				{
					auto testValue = test.at(i);
					auto testVector = v_f32_4(testValue);
					auto actual = vectorization::log(testVector);
					auto actual3 = vectorization::log3(testVector);
					auto expected = v_f32_4(std::log(testValue));
					auto match = (*matcher)(actual, expected);

					auto wmessage = format(i, testValue, "value mismatch ", x(actual), " !~~ ", x(expected));
					Assert::IsTrue(allTrue(match), wmessage.data(), LINE_INFO());
				}
			}

			TEST_METHOD(testExp)
			{
				auto test = samples();

				auto matchers = {
					nearEqualsRelative, // Zero
					nearEqualsRelative,
					nearEqualsRelative,
					nearEqualsRelative,
					nearEqualsRelative, // One
					nearEqualsRelative,
					nearEqualsRelative,
					nearEqualsRelative,

					nearEqualsRelative, // Two
					nearEqualsRelative,
					nearEqualsRelative,
					nearEqualsRelative,
					nearEqualsRelative, // ReciprocalPi
					nearEqualsRelative,
					nearEqualsRelative,
					nearEqualsRelative,

					nearEqualsRelative, // RadianToDegree
					nearEqualsRelative,
					nearEqualsRelative,
					nearEqualsRelative,
					nearEqualsRelative, // Sin45
					nearEqualsRelative,
					equalsOrdered,
					equalsOrdered,

					nearEqualsRelative, // Epsilon
					nearEqualsRelative,
					nearEqualsRelative,
					nearEqualsRelative,
					equalsOrdered, // max
					equalsOrdered,
					nearEqualsRelative,
					nearEqualsRelative,

					nearEqualsRelative, // denorm_min
					nearEqualsRelative,
					nearEqualsRelative,
					nearEqualsRelative,
					nearEqualsRelative, // round_error
					nearEqualsRelative,
					isBothNotANumber
				};

				auto matcher = matchers.begin();
				for (auto i = Zero<ASizeT>(); i < test.size(); ++i, ++matcher)
				{
					auto testValue = test.at(i);
					auto actual = vectorization::exp(v_f32_4(testValue));
					auto expected = v_f32_4(std::exp(testValue));
					auto match = (*matcher)(actual, expected);

					auto wmessage = format(i, testValue, "value mismatch ", x(actual), " !~~ ", x(expected));
					Assert::IsTrue(allTrue(match), wmessage.data(), LINE_INFO());
				}
			}


		};

	}
}
