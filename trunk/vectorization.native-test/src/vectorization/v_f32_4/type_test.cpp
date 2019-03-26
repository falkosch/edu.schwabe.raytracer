#include <CppUnitTest.h>
#include <vectorization.h>

#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
	namespace test
	{

		TEST_CLASS(v_f32_4_TypeTest)
		{
		public:

			static std::array<v_f32_4::ValueType, v_f32_4::SIZE> sample;
			static v_f32_4 sampleVector;

			TEST_CLASS_INITIALIZE(v_f32_4_TypeTestInitialize) {
				sample = {
					One<v_f32_4::ValueType>(),
					Two<v_f32_4::ValueType>(),
					OneHalf<v_f32_4::ValueType>(),
					Infinity<v_f32_4::ValueType>()
				};
				sampleVector = v_f32_4(
					sample.at(VectorIndices::X),
					sample.at(VectorIndices::Y),
					sample.at(VectorIndices::Z),
					sample.at(VectorIndices::W)
				);
			}

			TEST_METHOD(testArchitecture)
			{
				Assert::AreEqual(VectorSizes::W, v_f32_4::SIZE, L"Size mismatch", LINE_INFO());
				Assert::IsTrue(std::is_same<Float_32, v_f32_4::ValueType>::value, L"Type mismatch", LINE_INFO());
				Assert::IsTrue(std::is_same<Int_32, v_f32_4::BoolType>::value, L"Type mismatch", LINE_INFO());
				Assert::IsTrue(std::is_same<v_f32_4, v_f32_4::VectorType>::value, L"Type mismatch", LINE_INFO());
				Assert::IsTrue(std::is_same<v_i32_4, v_f32_4::VectorBoolType>::value, L"Type mismatch", LINE_INFO());
				Assert::IsTrue(std::is_same<PackedTypes<Float_32, VectorSizes::W>::Type, v_f32_4::PackedType>::value, L"Type mismatch", LINE_INFO());
			}

			TEST_METHOD(testCtorZero)
			{
				auto expected = Zero<v_f32_4::ValueType>();
				auto actual = v_f32_4();
				Assert::AreEqual(expected, x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(expected, y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(expected, z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(expected, w(actual), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testCtorCopy)
			{
				auto actual = sampleVector;
				Assert::AreEqual(x(sampleVector), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), w(actual), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testCtorPackedType)
			{
				auto actual = v_f32_4(sampleVector.components);
				Assert::AreEqual(x(sampleVector), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), w(actual), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testCtorValueType)
			{
				auto actual = v_f32_4(x(sampleVector));
				Assert::AreEqual(x(sampleVector), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(x(sampleVector), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(x(sampleVector), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(x(sampleVector), w(actual), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testCtorXY00)
			{
				auto actual = v_f32_4(x(sampleVector), y(sampleVector));
				Assert::AreEqual(x(sampleVector), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(Zero<v_f32_4::ValueType>(), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(Zero<v_f32_4::ValueType>(), w(actual), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testCtorXYZ0)
			{
				auto actual = v_f32_4(x(sampleVector), y(sampleVector), z(sampleVector));
				Assert::AreEqual(x(sampleVector), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(Zero<v_f32_4::ValueType>(), w(actual), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testCtorXYZW)
			{
				auto actual = v_f32_4(
					sample.at(VectorIndices::X),
					sample.at(VectorIndices::Y),
					sample.at(VectorIndices::Z),
					sample.at(VectorIndices::W)
				);
				Assert::AreEqual(sample.at(VectorIndices::X), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(sample.at(VectorIndices::Y), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(sample.at(VectorIndices::Z), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(sample.at(VectorIndices::W), w(actual), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testCtorVectorTypePtr)
			{
				auto actual = v_f32_4(&sampleVector);
				Assert::AreEqual(x(sampleVector), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), w(actual), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testCtorValueTypePtr)
			{
				auto actual = v_f32_4(sample.data());
				Assert::AreEqual(sample.at(VectorIndices::X), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(sample.at(VectorIndices::Y), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(sample.at(VectorIndices::Z), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(sample.at(VectorIndices::W), w(actual), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testCtorPackedTypePtr)
			{
				auto actual = v_f32_4(&sampleVector.components);
				Assert::AreEqual(x(sampleVector), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), w(actual), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testAssignmentOperatorPackedType)
			{
				v_f32_4 actual = sampleVector.components;
				Assert::AreEqual(x(sampleVector), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), w(actual), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testImplicitConversionOperatorPackedType)
			{
				v_f32_4::PackedType actual = sampleVector;
				v_f32_4 actual2 = actual;
				Assert::AreEqual(x(sampleVector), x(actual2), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual2), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), z(actual2), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), w(actual2), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testImplicitConversionOperatorPackedTypeConst)
			{
				const v_f32_4 actual = sampleVector;
				const v_f32_4::PackedType actual2 = sampleVector;
				v_f32_4 actual3 = actual2;
				Assert::AreEqual(x(sampleVector), x(actual3), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual3), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), z(actual3), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), w(actual3), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testIndexOperatorInt)
			{
				v_f32_4 actual = v_f32_4();
				actual[static_cast<int>(VectorIndices::X)] = x(sampleVector);
				actual[static_cast<int>(VectorIndices::Y)] = y(sampleVector);
				actual[static_cast<int>(VectorIndices::Z)] = z(sampleVector);
				actual[static_cast<int>(VectorIndices::W)] = w(sampleVector);

				Assert::AreEqual(x(sampleVector), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), w(actual), L"Ctor value mismatch", LINE_INFO());

				Assert::AreEqual(x(sampleVector), actual[static_cast<int>(VectorIndices::X)], L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), actual[static_cast<int>(VectorIndices::Y)], L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), actual[static_cast<int>(VectorIndices::Z)], L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), actual[static_cast<int>(VectorIndices::W)], L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testConstIndexOperatorInt)
			{
				const v_f32_4 actual = sampleVector;
				Assert::AreEqual(x(sampleVector), actual[static_cast<int>(VectorIndices::X)], L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), actual[static_cast<int>(VectorIndices::Y)], L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), actual[static_cast<int>(VectorIndices::Z)], L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), actual[static_cast<int>(VectorIndices::W)], L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testIndexOperatorASize)
			{
				v_f32_4 actual = v_f32_4();
				actual[VectorIndices::X] = x(sampleVector);
				actual[VectorIndices::Y] = y(sampleVector);
				actual[VectorIndices::Z] = z(sampleVector);
				actual[VectorIndices::W] = w(sampleVector);

				Assert::AreEqual(x(sampleVector), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), w(actual), L"Ctor value mismatch", LINE_INFO());

				Assert::AreEqual(x(sampleVector), actual[VectorIndices::X], L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), actual[VectorIndices::Y], L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), actual[VectorIndices::Z], L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), actual[VectorIndices::W], L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testConstIndexOperatorASize)
			{
				const v_f32_4 actual = sampleVector;
				Assert::AreEqual(x(sampleVector), actual[VectorIndices::X], L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), actual[VectorIndices::Y], L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), actual[VectorIndices::Z], L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), actual[VectorIndices::W], L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testStoreVectorType)
			{
				v_f32_4 actual;
				store(sampleVector, &actual);
				Assert::AreEqual(x(sampleVector), x(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), z(actual), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), w(actual), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testStorePackedType)
			{
				v_f32_4::PackedType actual;
				store(sampleVector, &actual);
				v_f32_4 actual2 = actual;
				Assert::AreEqual(x(sampleVector), x(actual2), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), y(actual2), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), z(actual2), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), w(actual2), L"Ctor value mismatch", LINE_INFO());
			}

			TEST_METHOD(testStoreValueType)
			{
				std::array<v_f32_4::ValueType, VectorSizes::W> actual;
				store(sampleVector, actual.data());
				Assert::AreEqual(x(sampleVector), actual.at(VectorIndices::X), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(y(sampleVector), actual.at(VectorIndices::Y), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(z(sampleVector), actual.at(VectorIndices::Z), L"Ctor value mismatch", LINE_INFO());
				Assert::AreEqual(w(sampleVector), actual.at(VectorIndices::W), L"Ctor value mismatch", LINE_INFO());
			}

		};

		std::array<v_f32_4::ValueType, 4> v_f32_4_TypeTest::sample;
		v_f32_4 v_f32_4_TypeTest::sampleVector;

	}
}
