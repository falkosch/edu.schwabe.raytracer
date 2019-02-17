#include "stdafx.h"
#include <CppUnitTest.h>
#include <vectorization.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
	namespace test
	{

		TEST_CLASS(MetaTypesTest)
		{
		public:

			TEST_METHOD(testHasVectorSizes)
			{
				Assert::AreEqual(std::size_t(0), VectorSizes::TooSmall, L"Bad vector size", LINE_INFO());
				Assert::IsTrue(VectorSizes::TooSmall < VectorSizes::X, L"Bad vector size", LINE_INFO());
				Assert::IsTrue(VectorSizes::X < VectorSizes::Y, L"Bad vector size", LINE_INFO());
				Assert::IsTrue(VectorSizes::Y < VectorSizes::Z, L"Bad vector size", LINE_INFO());
				Assert::IsTrue(VectorSizes::Z < VectorSizes::W, L"Bad vector size", LINE_INFO());
				Assert::IsTrue(VectorSizes::W < VectorSizes::AboveW, L"Bad vector size", LINE_INFO());
			}

			TEST_METHOD(testHasVectorIndices)
			{
				Assert::AreEqual(std::size_t(0), VectorIndices::X, L"Bad vector index", LINE_INFO());
				Assert::IsTrue(VectorIndices::X < VectorIndices::Y, L"Bad vector index", LINE_INFO());
				Assert::IsTrue(VectorIndices::Y < VectorIndices::Z, L"Bad vector index", LINE_INFO());
				Assert::IsTrue(VectorIndices::Z < VectorIndices::W, L"Bad vector index", LINE_INFO());
				Assert::IsTrue(VectorIndices::W < VectorIndices::AboveW, L"Bad vector index", LINE_INFO());
			}

			TEST_METHOD(testHasVectorBits)
			{
				Assert::AreEqual(std::size_t(0), VectorBits::None, L"Bad vector bit mask", LINE_INFO());
				Assert::IsTrue(VectorBits::None < VectorBits::X, L"Bad vector bit mask", LINE_INFO());
				Assert::IsTrue(VectorBits::X < VectorBits::Y, L"Bad vector bit mask", LINE_INFO());
				Assert::IsTrue(VectorBits::Y < VectorBits::Z, L"Bad vector bit mask", LINE_INFO());
				Assert::IsTrue(VectorBits::Z < VectorBits::W, L"Bad vector bit mask", LINE_INFO());
				Assert::IsTrue(VectorBits::W < VectorBits::AboveW, L"Bad vector bit mask", LINE_INFO());
			}

		};

	}
}
