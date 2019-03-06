#include <CppUnitTest.h>
#include <vectorization.h>

#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
	namespace test
	{

		TEST_CLASS(v_f32_4_OperatorsTest)
		{
		public:

			static const std::array<v_f32_4::ValueType, v_f32_4::SIZE> sample1() {
				return {
					Zero<v_f32_4::ValueType>(),
					One<v_f32_4::ValueType>(),
					Infinity<v_f32_4::ValueType>(),
					NotANumber<v_f32_4::ValueType>()
				};
			}

			static const v_f32_4 sampleVector1() {
				auto sampleComponents = sample1();
				return vectorization::v_f32_4(
					sampleComponents.at(VectorIndices::X),
					sampleComponents.at(VectorIndices::Y),
					sampleComponents.at(VectorIndices::Z),
					sampleComponents.at(VectorIndices::W)
				);
			}

			static const std::array<v_f32_4::ValueType, v_f32_4::SIZE> sample2() {
				return {
					NegativeZero<v_f32_4::ValueType>(),
					NegativeOne<v_f32_4::ValueType>(),
					NegativeInfinity<v_f32_4::ValueType>(),
					-NotANumber<v_f32_4::ValueType>()
				};
			}

			static const v_f32_4 sampleVector2() {
				auto sampleComponents = sample2();
				return vectorization::v_f32_4(
					sampleComponents.at(VectorIndices::X),
					sampleComponents.at(VectorIndices::Y),
					sampleComponents.at(VectorIndices::Z),
					sampleComponents.at(VectorIndices::W)
				);
			}

			TEST_METHOD(testEqualsOperator)
			{
				auto actual = sampleVector1() == sampleVector2();
				auto expected = v_f32_4::VectorBoolType(MaskAll<v_f32_4::BoolType>(), 0, 0, 0);

				Assert::AreEqual(x(expected), x(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(y(expected), y(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(z(expected), z(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(w(expected), w(actual), L"Equals value mismatch", LINE_INFO());

				auto sampleVector3 = v_f32_4(sampleVector1());
				actual = sampleVector1() == sampleVector3;
				expected = v_f32_4::VectorBoolType(MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), 0);

				Assert::AreEqual(x(expected), x(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(y(expected), y(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(z(expected), z(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(w(expected), w(actual), L"Equals value mismatch", LINE_INFO());
			}

			TEST_METHOD(testNotEqualsOperator)
			{
				auto actual = sampleVector1() != sampleVector2();
				auto expected = v_f32_4::VectorBoolType(0, MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>());

				Assert::AreEqual(x(expected), x(actual), L"NotEquals value mismatch", LINE_INFO());
				Assert::AreEqual(y(expected), y(actual), L"NotEquals value mismatch", LINE_INFO());
				Assert::AreEqual(z(expected), z(actual), L"NotEquals value mismatch", LINE_INFO());
				Assert::AreEqual(w(expected), w(actual), L"NotEquals value mismatch", LINE_INFO());

				auto sampleVector3 = v_f32_4(sampleVector1());
				actual = sampleVector1() != sampleVector3;

				expected = v_f32_4::VectorBoolType(0, 0, 0, MaskAll<v_f32_4::BoolType>());
				Assert::AreEqual(x(expected), x(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(y(expected), y(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(z(expected), z(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(w(expected), w(actual), L"Equals value mismatch", LINE_INFO());
			}

		};

	}
}
