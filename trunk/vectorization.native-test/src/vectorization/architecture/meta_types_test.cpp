#include <CppUnitTest.h>
#include <vectorization.h>

#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
    namespace test
    {

        TEST_CLASS(MetaTypesTest)
        {
        public:

            TEST_METHOD(hasVectorSizes)
            {
                Assert::AreEqual(ASizeT(0), VectorSizes::TooSmall, L"Bad vector size", LINE_INFO());
                Assert::IsTrue(VectorSizes::TooSmall < VectorSizes::X, L"Bad vector size", LINE_INFO());
                Assert::IsTrue(VectorSizes::X < VectorSizes::Y, L"Bad vector size", LINE_INFO());
                Assert::IsTrue(VectorSizes::Y < VectorSizes::Z, L"Bad vector size", LINE_INFO());
                Assert::IsTrue(VectorSizes::Z < VectorSizes::W, L"Bad vector size", LINE_INFO());
                Assert::IsTrue(VectorSizes::W < VectorSizes::AboveW, L"Bad vector size", LINE_INFO());
            }

            TEST_METHOD(hasVectorIndices)
            {
                Assert::AreEqual(ASizeT(0), VectorIndices::X, L"Bad vector index", LINE_INFO());
                Assert::IsTrue(VectorIndices::X < VectorIndices::Y, L"Bad vector index", LINE_INFO());
                Assert::IsTrue(VectorIndices::Y < VectorIndices::Z, L"Bad vector index", LINE_INFO());
                Assert::IsTrue(VectorIndices::Z < VectorIndices::W, L"Bad vector index", LINE_INFO());
                Assert::IsTrue(VectorIndices::W < VectorIndices::AboveW, L"Bad vector index", LINE_INFO());
            }

            TEST_METHOD(hasVectorBits)
            {
                Assert::AreEqual(ASizeT(0), VectorBits::None, L"Bad vector bit mask", LINE_INFO());
                Assert::IsTrue(VectorBits::None < VectorBits::X, L"Bad vector bit mask", LINE_INFO());
                Assert::IsTrue(VectorBits::X < VectorBits::Y, L"Bad vector bit mask", LINE_INFO());
                Assert::IsTrue(VectorBits::Y < VectorBits::Z, L"Bad vector bit mask", LINE_INFO());
                Assert::IsTrue(VectorBits::Z < VectorBits::W, L"Bad vector bit mask", LINE_INFO());
                Assert::IsTrue(VectorBits::W < VectorBits::AboveW, L"Bad vector bit mask", LINE_INFO());
            }

            TEST_METHOD(definesPackedTypesForInt_8)
            {
                typedef Int_8 TypeUnderTest;

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 8>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(16), PackedTypes<TypeUnderTest, 16>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 16>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(32), PackedTypes<TypeUnderTest, 32>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 32>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(64), PackedTypes<TypeUnderTest, 64>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 64>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 128>::Type>::value, L"Bad packed type definition", LINE_INFO());
            }

            TEST_METHOD(definesPackedTypesForInt_16)
            {
                typedef Int_16 TypeUnderTest;

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 4>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(8), PackedTypes<TypeUnderTest, 8>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 8>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(16), PackedTypes<TypeUnderTest, 16>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 16>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(32), PackedTypes<TypeUnderTest, 32>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 32>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 64>::Type>::value, L"Bad packed type definition", LINE_INFO());
            }

            TEST_METHOD(definesPackedTypesForInt_32)
            {
                typedef Int_32 TypeUnderTest;

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 2>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(4), PackedTypes<TypeUnderTest, 4>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 4>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(8), PackedTypes<TypeUnderTest, 8>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 8>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(16), PackedTypes<TypeUnderTest, 16>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 16>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 32>::Type>::value, L"Bad packed type definition", LINE_INFO());
            }

            TEST_METHOD(definesPackedTypesForInt_64)
            {
                typedef Int_64 TypeUnderTest;

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 1>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(2), PackedTypes<TypeUnderTest, 2>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 2>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(4), PackedTypes<TypeUnderTest, 4>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 4>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(8), PackedTypes<TypeUnderTest, 8>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 8>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 16>::Type>::value, L"Bad packed type definition", LINE_INFO());
            }

            TEST_METHOD(definesPackedTypesForUInt_8)
            {
                typedef UInt_8 TypeUnderTest;

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 8>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(16), PackedTypes<TypeUnderTest, 16>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 16>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(32), PackedTypes<TypeUnderTest, 32>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 32>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(64), PackedTypes<TypeUnderTest, 64>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 64>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 128>::Type>::value, L"Bad packed type definition", LINE_INFO());
            }

            TEST_METHOD(definesPackedTypesForUInt_16)
            {
                typedef UInt_16 TypeUnderTest;

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 4>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(8), PackedTypes<TypeUnderTest, 8>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 8>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(16), PackedTypes<TypeUnderTest, 16>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 16>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(32), PackedTypes<TypeUnderTest, 32>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 32>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 64>::Type>::value, L"Bad packed type definition", LINE_INFO());
            }

            TEST_METHOD(definesPackedTypesForUInt_32)
            {
                typedef UInt_32 TypeUnderTest;

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 2>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(4), PackedTypes<TypeUnderTest, 4>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 4>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(8), PackedTypes<TypeUnderTest, 8>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 8>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(16), PackedTypes<TypeUnderTest, 16>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 16>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 32>::Type>::value, L"Bad packed type definition", LINE_INFO());
            }

            TEST_METHOD(definesPackedTypesForUInt_64)
            {
                typedef UInt_64 TypeUnderTest;

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 1>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(2), PackedTypes<TypeUnderTest, 2>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 2>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(4), PackedTypes<TypeUnderTest, 4>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 4>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(8), PackedTypes<TypeUnderTest, 8>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 8>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 16>::Type>::value, L"Bad packed type definition", LINE_INFO());
            }

            TEST_METHOD(definesPackedTypesForFloat_32)
            {
                typedef Float_32 TypeUnderTest;

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 2>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(4), PackedTypes<TypeUnderTest, 4>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 4>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(8), PackedTypes<TypeUnderTest, 8>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 8>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(16), PackedTypes<TypeUnderTest, 16>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 16>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 32>::Type>::value, L"Bad packed type definition", LINE_INFO());
            }

            TEST_METHOD(definesPackedTypesForFloat_64)
            {
                typedef Float_64 TypeUnderTest;

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 1>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(2), PackedTypes<TypeUnderTest, 2>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 2>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(4), PackedTypes<TypeUnderTest, 4>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 4>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::AreEqual(ASizeT(8), PackedTypes<TypeUnderTest, 8>::Elements, L"Bad packed type definition", LINE_INFO());
                Assert::IsFalse(std::is_void<PackedTypes<TypeUnderTest, 8>::Type>::value, L"Bad packed type definition", LINE_INFO());

                Assert::IsTrue(std::is_void<PackedTypes<TypeUnderTest, 16>::Type>::value, L"Bad packed type definition", LINE_INFO());
            }

        };

    }
}
