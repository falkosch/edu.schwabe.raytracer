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

			static std::array<v_f32_4::ValueType, v_f32_4::SIZE> sample1;
			static std::array<v_f32_4::ValueType, v_f32_4::SIZE> sample2;

			static v_f32_4 sampleVector1;
			static v_f32_4 sampleVector2;

			TEST_CLASS_INITIALIZE(v_f32_4_OperatorsTestInitialize) {
				sample1 = {
					Zero<v_f32_4::ValueType>(),
					One<v_f32_4::ValueType>(),
					Infinity<v_f32_4::ValueType>(),
					NotANumber<v_f32_4::ValueType>(),
				};

				sample2 = {
					NegativeZero<v_f32_4::ValueType>(),
					NegativeOne<v_f32_4::ValueType>(),
					NegativeInfinity<v_f32_4::ValueType>(),
					-NotANumber<v_f32_4::ValueType>(),
				};

				sampleVector1 = v_f32_4(
					sample1.at(VectorIndices::X),
					sample1.at(VectorIndices::Y),
					sample1.at(VectorIndices::Z),
					sample1.at(VectorIndices::W)
				);

				sampleVector2 = v_f32_4(
					sample2.at(VectorIndices::X),
					sample2.at(VectorIndices::Y),
					sample2.at(VectorIndices::Z),
					sample2.at(VectorIndices::W)
				);
			}

			TEST_METHOD(testEqualsOperator)
			{
				auto actual = sampleVector1 == sampleVector2;
				auto expected = v_f32_4::VectorBoolType(MaskAll<v_f32_4::BoolType>(), 0, 0, 0);
				Assert::AreEqual(x(expected), x(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(y(expected), y(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(z(expected), z(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(w(expected), w(actual), L"Equals value mismatch", LINE_INFO());

				auto sampleVector3 = v_f32_4(sampleVector1);
				actual = sampleVector1 == sampleVector3;
				expected = v_f32_4::VectorBoolType(MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), 0);
				Assert::AreEqual(x(expected), x(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(y(expected), y(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(z(expected), z(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(w(expected), w(actual), L"Equals value mismatch", LINE_INFO());
			}

			TEST_METHOD(testNotEqualsOperator)
			{
				auto actual = sampleVector1 != sampleVector2;
				auto expected = v_f32_4::VectorBoolType(0, MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>());
				Assert::AreEqual(x(expected), x(actual), L"NotEquals value mismatch", LINE_INFO());
				Assert::AreEqual(y(expected), y(actual), L"NotEquals value mismatch", LINE_INFO());
				Assert::AreEqual(z(expected), z(actual), L"NotEquals value mismatch", LINE_INFO());
				Assert::AreEqual(w(expected), w(actual), L"NotEquals value mismatch", LINE_INFO());

				auto sampleVector3 = v_f32_4(sampleVector1);
				actual = sampleVector1 != sampleVector3;
				expected = v_f32_4::VectorBoolType(0, 0, 0, MaskAll<v_f32_4::BoolType>());
				Assert::AreEqual(x(expected), x(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(y(expected), y(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(z(expected), z(actual), L"Equals value mismatch", LINE_INFO());
				Assert::AreEqual(w(expected), w(actual), L"Equals value mismatch", LINE_INFO());
			}

		};

		std::array<v_f32_4::ValueType, 4> v_f32_4_OperatorsTest::sample1;
		std::array<v_f32_4::ValueType, 4> v_f32_4_OperatorsTest::sample2;
		v_f32_4 v_f32_4_OperatorsTest::sampleVector1;
		v_f32_4 v_f32_4_OperatorsTest::sampleVector2;

	}
}
