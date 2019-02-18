#include "stdafx.h"
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

			TEST_METHOD(testComponent)
			{
				std::array<vectorization::Float_32, VectorSizes::W> specimen = { 0.5f, 1.0f, 2.0f, 3.0f };
				auto testValue = v_f32_4(&specimen.at(VectorIndices::X));

				Assert::AreEqual(specimen.at(VectorIndices::X), component<VectorIndices::X>(testValue));
				Assert::AreEqual(specimen.at(VectorIndices::Y), component<VectorIndices::Y>(testValue));
				Assert::AreEqual(specimen.at(VectorIndices::Z), component<VectorIndices::Z>(testValue));
				Assert::AreEqual(specimen.at(VectorIndices::W), component<VectorIndices::W>(testValue));
			}

			TEST_METHOD(testXYZW)
			{
				std::array<vectorization::Float_32, VectorSizes::W> specimen = { 0.5f, 1.0f, 2.0f, 3.0f };
				auto testValue = v_f32_4(&specimen.at(VectorIndices::X));

				Assert::AreEqual(specimen.at(VectorIndices::X), x(testValue));
				Assert::AreEqual(specimen.at(VectorIndices::Y), y(testValue));
				Assert::AreEqual(specimen.at(VectorIndices::Z), z(testValue));
				Assert::AreEqual(specimen.at(VectorIndices::W), w(testValue));
			}

			TEST_METHOD(testReplaceComponent)
			{
				std::array<vectorization::Float_32, 4> specimen = { 0.5f, 1.0f, 2.0f, 3.0f };

				auto testValue = v_f32_4(&specimen.at(VectorIndices::X));
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
				std::array<vectorization::Float_32, 4> specimen = { 0.5f, 1.0f, 2.0f, 3.0f };

				auto testValue = v_f32_4(&specimen.at(VectorIndices::X));
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
				std::array<vectorization::Float_32, 4> specimen = { 0.5f, 1.0f, 2.0f, 3.0f };

				auto testReplacement = 5.0f;
				v_f32_4 testValue;

				testValue = v_f32_4(&specimen.at(VectorIndices::X));
				setComponent<VectorIndices::X>(testValue, testReplacement);
				Assert::AreEqual(testReplacement, x(testValue));
				Assert::AreNotEqual(testReplacement, y(testValue));
				Assert::AreNotEqual(testReplacement, z(testValue));
				Assert::AreNotEqual(testReplacement, w(testValue));

				testValue = v_f32_4(&specimen.at(VectorIndices::X));
				setComponent<VectorIndices::Y>(testValue, testReplacement);
				Assert::AreNotEqual(testReplacement, x(testValue));
				Assert::AreEqual(testReplacement, y(testValue));
				Assert::AreNotEqual(testReplacement, z(testValue));
				Assert::AreNotEqual(testReplacement, w(testValue));

				testValue = v_f32_4(&specimen.at(VectorIndices::X));
				setComponent<VectorIndices::Z>(testValue, testReplacement);
				Assert::AreNotEqual(testReplacement, x(testValue));
				Assert::AreNotEqual(testReplacement, y(testValue));
				Assert::AreEqual(testReplacement, z(testValue));
				Assert::AreNotEqual(testReplacement, w(testValue));

				testValue = v_f32_4(&specimen.at(VectorIndices::X));
				setComponent<VectorIndices::W>(testValue, testReplacement);
				Assert::AreNotEqual(testReplacement, x(testValue));
				Assert::AreNotEqual(testReplacement, y(testValue));
				Assert::AreNotEqual(testReplacement, z(testValue));
				Assert::AreEqual(testReplacement, w(testValue));
			}

		};

	}
}
