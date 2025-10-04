#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(MemoryTest) {
  public:
    static void testAlignedAllocation(const std::size_t expectedAlignment, void *&actualPtr) {
      const auto actualAddress = reinterpret_cast<std::size_t>(actualPtr);
      Assert::AreNotEqual(std::size_t{0}, actualAddress, L"invalid allocation", LINE_INFO());
      Assert::AreEqual(std::size_t{0}, actualAddress % expectedAlignment, L"allocation is not aligned", LINE_INFO());

      vectorization::free(actualPtr);
      Assert::AreEqual(
          std::size_t{0}, reinterpret_cast<std::size_t>(actualPtr), L"allocation pointer is not reset", LINE_INFO()
      );
    }

    static void testAlignedAllocation(const std::size_t expectedAlignment) {
      auto dummyAllocation = vectorization::alloc(1);
      auto actualPtr = vectorization::alloc(1, expectedAlignment);
      testAlignedAllocation(expectedAlignment, actualPtr);
      vectorization::free(dummyAllocation);
    }

    TEST_METHOD(hasAlignmentEnumeration) {
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

    TEST_METHOD(alignedAllocWithoutAlignmentParameter) {
      auto dummyAllocation = vectorization::alloc(1);
      auto actualPtr = vectorization::alloc(1);
      testAlignedAllocation(Alignments::Best, actualPtr);
      vectorization::free(dummyAllocation);
    }

    TEST_METHOD(alignedAllocWithSizeAndX86Alignment) {
      testAlignedAllocation(Alignments::X86);
    }

    TEST_METHOD(alignedAllocWithSizeAndMMAlignment) {
      testAlignedAllocation(Alignments::MM);
    }

    TEST_METHOD(alignedAllocWithSizeAndXMMAlignment) {
      testAlignedAllocation(Alignments::XMM);
    }

    TEST_METHOD(alignedAllocWithSizeAndYMMAlignment) {
      testAlignedAllocation(Alignments::YMM);
    }

    TEST_METHOD(alignedAllocWithSizeAndZMMAlignment) {
      testAlignedAllocation(Alignments::ZMM);
    }

    TEST_METHOD(alignedAllocWithSizeAndARCHAlignment) {
      testAlignedAllocation(Alignments::ARCH);
    }

    TEST_METHOD(alignedAllocWithSizeAndBestAlignment) {
      testAlignedAllocation(Alignments::Best);
    }
  };
}
