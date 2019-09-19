#include "../../Iterate.h"
#include "../../StandardSample.h"

#include <CppUnitTest.h>
#include <vectorization.h>

#include <array>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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

            TEST_METHOD(definesArchitecture)
            {
                Assert::AreEqual(VectorSizes::W, v_f32_4::SIZE, L"Size mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<Float_32, v_f32_4::ValueType>::value, L"Type mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<Int_32, v_f32_4::BoolType>::value, L"Type mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<v_f32_4, v_f32_4::VectorType>::value, L"Type mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<v_i32_4, v_f32_4::VectorBoolType>::value, L"Type mismatch", LINE_INFO());
                Assert::IsTrue(std::is_same<PackedTypes<Float_32, VectorSizes::W>::Type, v_f32_4::PackedType>::value, L"Type mismatch", LINE_INFO());
            }

            TEST_METHOD(constructsZero)
            {
                Iterate::toExclusiveEnd<VectorSizes::W>([](auto i, auto message) {
                    Assert::AreEqual(
                        Zero<v_f32_4::ValueType>(),
                        v_f32_4()[i],
                        message.c_str()
                    );
                });
            }

            TEST_METHOD(constructsCopy)
            {
                Iterate::toExclusiveEnd<VectorSizes::W>([](auto i, auto message) {
                    Assert::AreEqual(
                        sampleVector()[i],
                        v_f32_4(sampleVector())[i],
                        message.c_str()
                    );
                });
            }

            TEST_METHOD(constructsFromPackedType)
            {
                Iterate::toExclusiveEnd<VectorSizes::W>([](auto i, auto message) {
                    Assert::AreEqual(
                        sampleVector()[i],
                        v_f32_4(sampleVector().components)[i],
                        message.c_str()
                    );
                });
            }

            TEST_METHOD(constructsFromValueType)
            {
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

            TEST_METHOD(constructsFromXY00)
            {
                auto expected = sampleVector();
                auto actual = v_f32_4(x(expected), y(expected));

                Assert::AreEqual(x(expected), x(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(Zero<v_f32_4::ValueType>(), z(actual), L"Ctor value mismatch", LINE_INFO());
                Assert::AreEqual(Zero<v_f32_4::ValueType>(), w(actual), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(constructsFromXYZ0)
            {
                auto expected = sampleVector();
                auto actual = v_f32_4(x(expected), y(expected), z(expected));

                Iterate::toExclusiveEnd<VectorSizes::Z>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual[i], message.c_str());
                });

                Assert::AreEqual(Zero<v_f32_4::ValueType>(), w(actual), L"Ctor value mismatch", LINE_INFO());
            }

            TEST_METHOD(constructsFromXYZW)
            {
                auto expected = sampleVector();
                auto actual = v_f32_4(x(expected), y(expected), z(expected), w(expected));

                Iterate::toExclusiveEnd<VectorSizes::W>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual[i], message.c_str());
                });
            }

            TEST_METHOD(constructsFromVectorTypePtr)
            {
                Iterate::toExclusiveEnd<VectorSizes::W>([](auto i, auto message) {
                    Assert::AreEqual(
                        sampleVector()[i],
                        v_f32_4(&sampleVector())[i],
                        message.c_str()
                    );
                });
            }

            TEST_METHOD(constructsFromValueTypePtr)
            {
                Iterate::toExclusiveEnd<VectorSizes::W>([](auto i, auto message) {
                    Assert::AreEqual(
                        sampleArray()[i],
                        v_f32_4(sampleArray().data())[i],
                        message.c_str()
                    );
                });
            }

            TEST_METHOD(constructsFromPackedTypePtr)
            {
                auto expected = sampleVector();
                auto actual = v_f32_4(&(expected.components));

                Iterate::toExclusiveEnd<VectorSizes::W>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual[i], message.c_str());
                });
            }

            TEST_METHOD(implcitlyConvertsFromPackedType)
            {
                auto expected = sampleVector();
                v_f32_4 actual = expected.components;

                Iterate::toExclusiveEnd<VectorSizes::W>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual[i], message.c_str());
                });
            }

            TEST_METHOD(implcitlyConvertsToPackedType)
            {
                auto expected = sampleVector();
                v_f32_4::PackedType packedType = expected;
                v_f32_4 actual = packedType;

                Iterate::toExclusiveEnd<VectorSizes::W>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual[i], message.c_str());
                });
            }

            TEST_METHOD(implcitlyConvertsToConstPackedType)
            {
                const auto expected = sampleVector();
                const v_f32_4::PackedType constPackedType = expected;
                v_f32_4 actual = constPackedType;

                Iterate::toExclusiveEnd<VectorSizes::W>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual[i], message.c_str());
                });
            }

            TEST_METHOD(writesAndReadsAtIntIndex)
            {
                auto expected = sampleVector();
                auto actual = v_f32_4();
                actual[static_cast<int>(VectorIndices::X)] = x(expected);
                actual[static_cast<int>(VectorIndices::Y)] = y(expected);
                actual[static_cast<int>(VectorIndices::Z)] = z(expected);
                actual[static_cast<int>(VectorIndices::W)] = w(expected);

                Iterate::toExclusiveEnd<VectorSizes::W>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual[static_cast<int>(i)], message.c_str(), LINE_INFO());
                });
            }

            TEST_METHOD(readsConstAtIntIndex)
            {
                auto expected = sampleVector();
                const v_f32_4 actual = expected;

                Iterate::toExclusiveEnd<VectorSizes::W>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual[static_cast<int>(i)], message.c_str());
                });
            }

            TEST_METHOD(readsAtASizeIndex)
            {
                auto expected = sampleVector();
                auto actual = v_f32_4();
                actual[VectorIndices::X] = x(expected);
                actual[VectorIndices::Y] = y(expected);
                actual[VectorIndices::Z] = z(expected);
                actual[VectorIndices::W] = w(expected);

                Iterate::toExclusiveEnd<VectorSizes::W>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual[i], message.c_str());
                });
            }

            TEST_METHOD(readsConstAtASizeIndex)
            {
                auto expected = sampleVector();
                const auto actual = expected;

                Iterate::toExclusiveEnd<VectorSizes::W>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual[i], message.c_str());
                });
            }

            TEST_METHOD(storesToVectorType)
            {
                auto expected = sampleVector();
                v_f32_4 actual;
                store(expected, &actual);

                Iterate::toExclusiveEnd<VectorSizes::W>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual[i], message.c_str());
                });
            }

            TEST_METHOD(storesToPackedType)
            {
                auto expected = sampleVector();
                v_f32_4::PackedType packedType;
                store(expected, &packedType);
                v_f32_4 actual = packedType;

                Iterate::toExclusiveEnd<VectorSizes::W>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual[i], message.c_str());
                });
            }

            TEST_METHOD(storesToValueTypeArray)
            {
                auto expected = sampleVector();
                std::array<v_f32_4::ValueType, VectorSizes::W> actual;
                store(expected, actual.data());

                Iterate::toExclusiveEnd<VectorSizes::W>([&](auto i, auto message) {
                    Assert::AreEqual(expected[i], actual.at(i), message.c_str());
                });
            }

        };

    }
}
