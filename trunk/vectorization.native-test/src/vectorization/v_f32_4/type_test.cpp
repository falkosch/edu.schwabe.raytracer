#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(v_f32_4_TypeTest)
        {
        public:

            static const std::array<v_f32_4::ValueType, v_f32_4::SIZE> sampleArray() {
                return StandardSample::ofArrayType<v_f32_4::ValueType, v_f32_4::SIZE>();
            }

            static const v_f32_4 sampleVector() {
                return StandardSample::ofVectorType<v_f32_4>();
            }

            TEST_METHOD(definesArchitecture) {
                Assert::AreEqual(VectorSizes::W, v_f32_4::SIZE, L"Size mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<Float_32, v_f32_4::ValueType>::value, L"Type mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<Int_32, v_f32_4::BoolType>::value, L"Type mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<v_f32_4, v_f32_4::VectorType>::value, L"Type mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<v_i32_4, v_f32_4::VectorBoolType>::value, L"Type mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<PackedTypes<Float_32, VectorSizes::W>::Type, v_f32_4::PackedType>::value, L"Type mismatch", LINE_INFO());
            }

            TEST_METHOD(constructsZero) {
                Iterate::toExclusiveEnd<VectorSizes::W>([](auto i, auto message) {
                    Assert::AreEqual(
                        Zero<v_f32_4::ValueType>(),
                        v_f32_4()[i],
                        message.c_str()
                    );
                });
            }

            TEST_METHOD(constructsCopy) {
                Iterate::toExclusiveEnd<VectorSizes::W>([](auto i, auto message) {
                    Assert::AreEqual(
                        sampleVector()[i],
                        v_f32_4(sampleVector())[i],
                        message.c_str()
                    );
                });
            }

            TEST_METHOD(constructsFromPackedType) {
                Iterate::toExclusiveEnd<VectorSizes::W>([](auto i, auto message) {
                    Assert::AreEqual(
                        sampleVector()[i],
                        v_f32_4(sampleVector().components)[i],
                        message.c_str()
                    );
                });
            }

            TEST_METHOD(constructsFromValueType) {
                Iterate::toExclusiveEnd<VectorSizes::W>([](auto i, auto message) {
                    staticFor<VectorIndices::X, VectorSizes::W>([&](auto j) {
                        Assert::AreEqual(
                            sampleVector()[i],
                            v_f32_4(sampleVector()[i])[j],
                            message.c_str()
                        );
                    });
                });
            }

            TEST_METHOD(constructsFromXY00) {
                auto expected = sampleVector();
                auto actual = v_f32_4(x(expected), y(expected));

                Assert::AreEqual(x(expected), x(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(Zero<v_f32_4::ValueType>(), z(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(Zero<v_f32_4::ValueType>(), w(actual), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(constructsFromXYZ0) {
                auto expected = sampleVector();
                auto actual = v_f32_4(x(expected), y(expected), z(expected));

                Assert::AreEqual(x(expected), x(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(Zero<v_f32_4::ValueType>(), w(actual), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(constructsFromXYZW) {
                auto expected = sampleVector();
                auto actual = v_f32_4(x(expected), y(expected), z(expected), w(expected));

                Assert::AreEqual(x(expected), x(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(constructsFromVectorTypePtr) {
                auto expected = sampleVector();
                auto actual = v_f32_4(&expected);

                Assert::AreEqual(x(expected), x(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(constructsFromValueTypePtr) {
                auto expected = sampleArray();
                auto actual = v_f32_4(expected.data());

                Assert::AreEqual(expected.at(VectorIndices::X), x(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(expected.at(VectorIndices::Y), y(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(expected.at(VectorIndices::Z), z(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(expected.at(VectorIndices::W), w(actual), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(constructsFromPackedTypePtr) {
                auto expected = sampleVector();
                auto actual = v_f32_4(&expected.components);

                Assert::AreEqual(x(expected), x(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(implcitlyConvertsFromComponentsPackedType) {
                auto expected = sampleVector();
                v_f32_4 actual = expected.components;

                Assert::AreEqual(x(expected), x(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(implcitlyConvertsFromAnyPackedType) {
                auto expected = sampleVector();
                v_f32_4::PackedType actual = expected;
                v_f32_4 actual2 = actual;

                Assert::AreEqual(x(expected), x(actual2), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual2), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual2), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual2), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(implcitlyConvertsFromAnyConstPackedType) {
                auto expected = sampleVector();
                const v_f32_4 actual = expected;
                const v_f32_4::PackedType actual2 = actual;
                v_f32_4 actual3 = actual2;

                Assert::AreEqual(x(expected), x(actual3), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual3), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual3), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual3), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(readsAtIntIndex) {
                auto expected = sampleVector();
                v_f32_4 actual = v_f32_4();
                actual[static_cast<int>(VectorIndices::X)] = x(expected);
                actual[static_cast<int>(VectorIndices::Y)] = y(expected);
                actual[static_cast<int>(VectorIndices::Z)] = z(expected);
                actual[static_cast<int>(VectorIndices::W)] = w(expected);

                Assert::AreEqual(x(expected), x(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"Ctor value mismatch", LINE_INFO());

                Assert::AreEqual(x(expected), actual[static_cast<int>(VectorIndices::X)], L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), actual[static_cast<int>(VectorIndices::Y)], L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), actual[static_cast<int>(VectorIndices::Z)], L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), actual[static_cast<int>(VectorIndices::W)], L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(readsAtConstIntIndex) {
                auto expected = sampleVector();
                const v_f32_4 actual = expected;

                Assert::AreEqual(x(expected), actual[static_cast<int>(VectorIndices::X)], L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), actual[static_cast<int>(VectorIndices::Y)], L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), actual[static_cast<int>(VectorIndices::Z)], L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), actual[static_cast<int>(VectorIndices::W)], L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(readsAtASizeIndex) {
                auto expected = sampleVector();
                v_f32_4 actual = v_f32_4();
                actual[VectorIndices::X] = x(expected);
                actual[VectorIndices::Y] = y(expected);
                actual[VectorIndices::Z] = z(expected);
                actual[VectorIndices::W] = w(expected);

                Assert::AreEqual(x(expected), x(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"Ctor value mismatch", LINE_INFO());

                Assert::AreEqual(x(expected), actual[VectorIndices::X], L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), actual[VectorIndices::Y], L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), actual[VectorIndices::Z], L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), actual[VectorIndices::W], L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(readsAtConstASizeIndex) {
                auto expected = sampleVector();
                const v_f32_4 actual = expected;

                Assert::AreEqual(x(expected), actual[VectorIndices::X], L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), actual[VectorIndices::Y], L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), actual[VectorIndices::Z], L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), actual[VectorIndices::W], L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(storesToVectorType) {
                auto expected = sampleVector();
                v_f32_4 actual;
                store(expected, &actual);

                Assert::AreEqual(x(expected), x(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(storesToPackedType) {
                auto expected = sampleVector();
                v_f32_4::PackedType actual;
                store(expected, &actual);
                v_f32_4 actual2 = actual;

                Assert::AreEqual(x(expected), x(actual2), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual2), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual2), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual2), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(storesToValueTypeArray) {
                auto expected = sampleVector();
                std::array<v_f32_4::ValueType, VectorSizes::W> actual;
                store(expected, actual.data());

                Assert::AreEqual(x(expected), actual.at(VectorIndices::X), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), actual.at(VectorIndices::Y), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), actual.at(VectorIndices::Z), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), actual.at(VectorIndices::W), L"Ctor value mismatch", LINE_INFO());
            }
        };
    }
}