#include <CppUnitTest.h>
#include <vectorization.h>

#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
	namespace test
	{

		TEST_CLASS(v_f32_4_AccessorsTest)
		{
		public:

			static const std::array<v_f32_4::ValueType, v_f32_4::SIZE> sample() {
				return {
					One<v_f32_4::ValueType>(),
					Two<v_f32_4::ValueType>(),
					OneHalf<v_f32_4::ValueType>(),
					Infinity<v_f32_4::ValueType>()
				};
			}

			static const v_f32_4 sampleVector() {
				auto sampleComponents = sample();
				return vectorization::v_f32_4(
					sampleComponents.at(VectorIndices::X),
					sampleComponents.at(VectorIndices::Y),
					sampleComponents.at(VectorIndices::Z),
					sampleComponents.at(VectorIndices::W)
				);
			}

			TEST_METHOD(testComponent)
			{
				auto expected = sample();
				auto testValue = sampleVector();

				Assert::AreEqual(expected.at(VectorIndices::X), component<VectorIndices::X>(testValue));
				Assert::AreEqual(expected.at(VectorIndices::Y), component<VectorIndices::Y>(testValue));
				Assert::AreEqual(expected.at(VectorIndices::Z), component<VectorIndices::Z>(testValue));
				Assert::AreEqual(expected.at(VectorIndices::W), component<VectorIndices::W>(testValue));
			}

			TEST_METHOD(testXYZW)
			{
				auto expected = sample();
				auto testValue = sampleVector();

				Assert::AreEqual(expected.at(VectorIndices::X), x(testValue));
				Assert::AreEqual(expected.at(VectorIndices::Y), y(testValue));
				Assert::AreEqual(expected.at(VectorIndices::Z), z(testValue));
				Assert::AreEqual(expected.at(VectorIndices::W), w(testValue));
			}

			TEST_METHOD(testReplaceComponent)
			{
				auto testValue = sampleVector();
				auto testReplacement = 5.0f;
				v_f32_4 testReplaced;

				testReplaced = replaceComponent<VectorIndices::X>(testValue, testReplacement);
				Assert::AreEqual(testReplacement, x(testReplaced));
				Assert::AreNotEqual(testReplacement, y(testReplaced));
				Assert::AreNotEqual(testReplacement, z(testReplaced));
				Assert::AreNotEqual(testReplacement, w(testReplaced));

				testReplaced = replaceComponent<VectorIndices::Y>(testValue, testReplacement);
				Assert::AreNotEqual(testReplacement, x(testReplaced));
				Assert::AreEqual(testReplacement, y(testReplaced));
				Assert::AreNotEqual(testReplacement, z(testReplaced));
				Assert::AreNotEqual(testReplacement, w(testReplaced));

				testReplaced = replaceComponent<VectorIndices::Z>(testValue, testReplacement);
				Assert::AreNotEqual(testReplacement, x(testReplaced));
				Assert::AreNotEqual(testReplacement, y(testReplaced));
				Assert::AreEqual(testReplacement, z(testReplaced));
				Assert::AreNotEqual(testReplacement, w(testReplaced));

				testReplaced = replaceComponent<VectorIndices::W>(testValue, testReplacement);
				Assert::AreNotEqual(testReplacement, x(testReplaced));
				Assert::AreNotEqual(testReplacement, y(testReplaced));
				Assert::AreNotEqual(testReplacement, z(testReplaced));
				Assert::AreEqual(testReplacement, w(testReplaced));
			}

			TEST_METHOD(testReplaceXYZW)
			{
				auto testValue = sampleVector();
				auto testReplacement = 5.0f;
				v_f32_4 testReplaced;

				testReplaced = replaceX(testValue, testReplacement);
				Assert::AreEqual(testReplacement, x(testReplaced));
				Assert::AreNotEqual(testReplacement, y(testReplaced));
				Assert::AreNotEqual(testReplacement, z(testReplaced));
				Assert::AreNotEqual(testReplacement, w(testReplaced));

				testReplaced = replaceY(testValue, testReplacement);
				Assert::AreNotEqual(testReplacement, x(testReplaced));
				Assert::AreEqual(testReplacement, y(testReplaced));
				Assert::AreNotEqual(testReplacement, z(testReplaced));
				Assert::AreNotEqual(testReplacement, w(testReplaced));

				testReplaced = replaceZ(testValue, testReplacement);
				Assert::AreNotEqual(testReplacement, x(testReplaced));
				Assert::AreNotEqual(testReplacement, y(testReplaced));
				Assert::AreEqual(testReplacement, z(testReplaced));
				Assert::AreNotEqual(testReplacement, w(testReplaced));

				testReplaced = replaceW(testValue, testReplacement);
				Assert::AreNotEqual(testReplacement, x(testReplaced));
				Assert::AreNotEqual(testReplacement, y(testReplaced));
				Assert::AreNotEqual(testReplacement, z(testReplaced));
				Assert::AreEqual(testReplacement, w(testReplaced));
			}

			TEST_METHOD(testSetComponent)
			{
				auto testReplacement = 5.0f;
				v_f32_4 testValue;

				testValue = sampleVector();
				setComponent<VectorIndices::X>(testValue, testReplacement);
				Assert::AreEqual(testReplacement, x(testValue));
				Assert::AreNotEqual(testReplacement, y(testValue));
				Assert::AreNotEqual(testReplacement, z(testValue));
				Assert::AreNotEqual(testReplacement, w(testValue));

				testValue = sampleVector();
				setComponent<VectorIndices::Y>(testValue, testReplacement);
				Assert::AreNotEqual(testReplacement, x(testValue));
				Assert::AreEqual(testReplacement, y(testValue));
				Assert::AreNotEqual(testReplacement, z(testValue));
				Assert::AreNotEqual(testReplacement, w(testValue));

				testValue = sampleVector();
				setComponent<VectorIndices::Z>(testValue, testReplacement);
				Assert::AreNotEqual(testReplacement, x(testValue));
				Assert::AreNotEqual(testReplacement, y(testValue));
				Assert::AreEqual(testReplacement, z(testValue));
				Assert::AreNotEqual(testReplacement, w(testValue));

				testValue = sampleVector();
				setComponent<VectorIndices::W>(testValue, testReplacement);
				Assert::AreNotEqual(testReplacement, x(testValue));
				Assert::AreNotEqual(testReplacement, y(testValue));
				Assert::AreNotEqual(testReplacement, z(testValue));
				Assert::AreEqual(testReplacement, w(testValue));
			}

		};

	}
}
