#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(v_f32_4_TypeTest)
        {
        public:

            TEST_METHOD(definesArchitecture) {
                Assert::AreEqual(VectorSizes::W, v_f32_4::SIZE, L"Size mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<Float_32, v_f32_4::ValueType>::value, L"Type mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<Int_32, v_f32_4::BoolType>::value, L"Type mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<v_f32_4, v_f32_4::VectorType>::value, L"Type mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<v_i32_4, v_f32_4::VectorBoolType>::value, L"Type mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<PackedTypes<Float_32, VectorSizes::W>::Type, v_f32_4::PackedType>::value, L"Type mismatch", LINE_INFO());
            }

            TEST_METHOD(constructsZero) {
                v_f32_4 actual{ };
                Assert::IsTrue(allTrue(Zero<v_f32_4>() == v_f32_4()));
            }

            TEST_METHOD(constructsFromXYZW) {
                std::array<v_f32_4::ValueType, v_f32_4::SIZE> given{ 1.0f, 2.0f, 3.0f, 4.0f };

                v_f32_4 actualVector{
                    given.at(VectorIndices::X),
                    given.at(VectorIndices::Y),
                    given.at(VectorIndices::Z),
                    given.at(VectorIndices::W),
                };
                std::array<v_f32_4::ValueType, v_f32_4::SIZE> actual{ };
                _mm_store_ps(actual.data(), actualVector.components);

                Assert::AreEqual(given, actual);
            }

            TEST_METHOD(constructsCopy) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 actual{ given };
                Assert::IsTrue(allTrue(given == actual));
            }

            TEST_METHOD(constructsFromPackedType) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 actual{ given.components };
                Assert::IsTrue(allTrue(given == actual));
            }

            TEST_METHOD(constructsBroadcastFromValueType) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 actual{ given.components };
                Assert::IsTrue(allTrue(given == actual));
            }

            TEST_METHOD(constructsFromXY00) {
                v_f32_4 given{ 1.0f, 2.0f, 0.0f, 0.0f };
                v_f32_4 actual{ 1.0f, 2.0f };
                Assert::IsTrue(allTrue(given == actual));
            }

            TEST_METHOD(constructsFromXYZ0) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 0.0f };
                v_f32_4 actual{ 1.0f, 2.0f, 3.0f };
                Assert::IsTrue(allTrue(given == actual));
            }

            TEST_METHOD(constructsFromVectorTypePtr) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 actual{ &given };
                Assert::IsTrue(allTrue(given == actual));
            }

            TEST_METHOD(constructsFromValueTypePtr) {
                std::array<v_f32_4::ValueType, v_f32_4::SIZE> given{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 expected{
                    given.at(VectorIndices::X),
                    given.at(VectorIndices::Y),
                    given.at(VectorIndices::Z),
                    given.at(VectorIndices::W),
                };
                v_f32_4 actual{ given.data() };
                Assert::IsTrue(allTrue(expected == actual));
            }

            TEST_METHOD(constructsFromPackedTypePtr) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 actual{ given.components };
                Assert::IsTrue(allTrue(given == actual));
            }

            TEST_METHOD(implcitlyConvertsFromComponentsPackedType) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 actual = given.components;
                Assert::IsTrue(allTrue(given == actual));
            }

            TEST_METHOD(implcitlyConvertsFromAnyPackedType) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4::PackedType actual = given.components;
                v_f32_4 actual2 = actual;
                Assert::IsTrue(allTrue(given == actual2));
            }

            TEST_METHOD(implcitlyConvertsFromAnyConstPackedType) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 4.0f };
                const v_f32_4::PackedType actual = given.components;
                v_f32_4 actual2 = actual;
                Assert::IsTrue(allTrue(given == actual2));
            }

            TEST_METHOD(readsAtIndex) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 actual{
                    given[VectorIndices::X],
                    given[VectorIndices::Y],
                    given[VectorIndices::Z],
                    given[VectorIndices::W]
                };
                Assert::IsTrue(allTrue(given == actual));
            }

            TEST_METHOD(storesToVectorType) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 actual;
                store(given, &actual);
                Assert::IsTrue(allTrue(given == actual));
            }

            TEST_METHOD(storesToPackedType) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4::PackedType actualPack;
                store(given, &actualPack);
                v_f32_4 actual{ actualPack };
                Assert::IsTrue(allTrue(given == actual));
            }

            TEST_METHOD(storesToValueTypeArray) {
                v_f32_4 given{ 1.0f, 2.0f, 3.0f, 4.0f };
                std::array<v_f32_4::ValueType, v_f32_4::SIZE> actualArray;
                store(given, actualArray.data());
                v_f32_4 actual{ actualArray.data() };
                Assert::IsTrue(allTrue(given == actual));
            }
        };
    }
}