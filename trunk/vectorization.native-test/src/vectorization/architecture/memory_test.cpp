#include <CppUnitTest.h>
#include <vectorization.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
    namespace test
    {

        TEST_CLASS(MemoryTest)
        {
        public:

            static void testAlignedAllocation(const std::size_t expectedAlignment, void*& actualPtr) {
                auto actualAddress = reinterpret_cast<std::size_t>(actualPtr);
                Assert::AreNotEqual(Zero<std::size_t>(), actualAddress, L"invalid allocation", LINE_INFO());
                Assert::AreEqual(Zero<std::size_t>(), actualAddress % expectedAlignment, L"allocation is not aligned", LINE_INFO());

                vectorization::free(actualPtr);
                Assert::AreEqual(Zero<std::size_t>(), reinterpret_cast<std::size_t>(actualPtr), L"allocation pointer is not reset", LINE_INFO());
            }

            static void testAlignedAllocation(const std::size_t expectedAlignment) {
                auto dummyAllocation = vectorization::alloc(1);
                auto actualPtr = vectorization::alloc(1, expectedAlignment);
                MemoryTest::testAlignedAllocation(expectedAlignment, actualPtr);
                vectorization::free(dummyAllocation);
            }

            TEST_METHOD(hasAlignmentEnumeration)
            {
                Assert::IsTrue(0 < Alignments::X86, L"Bad alignment enumeration", LINE_INFO());
                Assert::IsTrue(Alignments::X86 < Alignments::MM, L"Bad alignment enumeration", LINE_INFO());
                Assert::IsTrue(Alignments::MM < Alignments::XMM, L"Bad alignment enumeration", LINE_INFO());
                Assert::IsTrue(Alignments::XMM < Alignments::YMM, L"Bad alignment enumeration", LINE_INFO());
                Assert::IsTrue(Alignments::YMM < Alignments::ZMM, L"Bad alignment enumeration", LINE_INFO());

                Assert::IsTrue(0 < Alignments::ARCH, L"Bad arch alignment enumeration", LINE_INFO());
                Assert::IsTrue(Alignments::ZMM >= Alignments::ARCH, L"Bad arch alignment enumeration", LINE_INFO());

                Assert::IsTrue(0 < Alignments::Best, L"Bad best alignment enumeration", LINE_INFO());
                Assert::IsTrue(Alignments::ZMM >= Alignments::Best, L"Bad best alignment enumeration", LINE_INFO());
            }

            TEST_METHOD(alignedAllocWithoutAlignmentParameter)
            {
                auto dummyAllocation = vectorization::alloc(1);
                auto actualPtr = vectorization::alloc(1);
                MemoryTest::testAlignedAllocation(Alignments::Best, actualPtr);
                vectorization::free(dummyAllocation);
            }

            TEST_METHOD(alignedAllocWithSizeAndX86Alignment)
            {
                MemoryTest::testAlignedAllocation(Alignments::X86);
            }

            TEST_METHOD(alignedAllocWithSizeAndMMAlignment)
            {
                MemoryTest::testAlignedAllocation(Alignments::MM);
            }

            TEST_METHOD(alignedAllocWithSizeAndXMMAlignment)
            {
                MemoryTest::testAlignedAllocation(Alignments::XMM);
            }

            TEST_METHOD(alignedAllocWithSizeAndYMMAlignment)
            {
                MemoryTest::testAlignedAllocation(Alignments::YMM);
            }

            TEST_METHOD(alignedAllocWithSizeAndZMMAlignment)
            {
                MemoryTest::testAlignedAllocation(Alignments::ZMM);
            }

            TEST_METHOD(alignedAllocWithSizeAndARCHAlignment)
            {
                MemoryTest::testAlignedAllocation(Alignments::ARCH);
            }

            TEST_METHOD(alignedAllocWithSizeAndBestAlignment)
            {
                MemoryTest::testAlignedAllocation(Alignments::Best);
            }

        };

    }
}
