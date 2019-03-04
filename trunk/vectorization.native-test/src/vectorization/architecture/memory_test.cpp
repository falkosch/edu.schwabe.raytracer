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

			TEST_METHOD(testHasAlignmentEnumeration)
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

			TEST_METHOD(testAlignedAllocWithSize)
			{
				auto memoryPtr = vectorization::alloc(1);
				auto memoryAddress = reinterpret_cast<std::size_t>(memoryPtr);
				auto zeroAddress = reinterpret_cast<std::size_t>(nullptr);

				vectorization::free(memoryPtr);

				Assert::AreEqual(zeroAddress, memoryAddress % Alignments::Best);
				Assert::AreNotEqual(zeroAddress, memoryAddress);

				Assert::AreEqual(zeroAddress, reinterpret_cast<std::size_t>(memoryPtr));
			}

			TEST_METHOD(testAlignedAllocWithSizeAndX86Alignment)
			{
				auto memoryPtr = vectorization::alloc(1, Alignments::X86);
				auto memoryAddress = reinterpret_cast<std::size_t>(memoryPtr);
				auto zeroAddress = reinterpret_cast<std::size_t>(nullptr);

				vectorization::free(memoryPtr);

				Assert::AreEqual(zeroAddress, memoryAddress % Alignments::X86);
				Assert::AreNotEqual(zeroAddress, memoryAddress);

				Assert::AreEqual(zeroAddress, reinterpret_cast<std::size_t>(memoryPtr));
			}

			TEST_METHOD(testAlignedAllocWithSizeAndMMAlignment)
			{
				auto memoryPtr = vectorization::alloc(1, Alignments::MM);
				auto memoryAddress = reinterpret_cast<std::size_t>(memoryPtr);
				auto zeroAddress = reinterpret_cast<std::size_t>(nullptr);

				vectorization::free(memoryPtr);

				Assert::AreEqual(zeroAddress, memoryAddress % Alignments::MM);
				Assert::AreNotEqual(zeroAddress, memoryAddress);

				Assert::AreEqual(zeroAddress, reinterpret_cast<std::size_t>(memoryPtr));
			}

			TEST_METHOD(testAlignedAllocWithSizeAndXMMAlignment)
			{
				auto memoryPtr = vectorization::alloc(1, Alignments::XMM);
				auto memoryAddress = reinterpret_cast<std::size_t>(memoryPtr);
				auto zeroAddress = reinterpret_cast<std::size_t>(nullptr);

				vectorization::free(memoryPtr);

				Assert::AreEqual(zeroAddress, memoryAddress % Alignments::XMM);
				Assert::AreNotEqual(zeroAddress, memoryAddress);

				Assert::AreEqual(zeroAddress, reinterpret_cast<std::size_t>(memoryPtr));
			}

			TEST_METHOD(testAlignedAllocWithSizeAndYMMAlignment)
			{
				auto memoryPtr = vectorization::alloc(1, Alignments::YMM);
				auto memoryAddress = reinterpret_cast<std::size_t>(memoryPtr);
				auto zeroAddress = reinterpret_cast<std::size_t>(nullptr);

				vectorization::free(memoryPtr);

				Assert::AreEqual(zeroAddress, memoryAddress % Alignments::YMM);
				Assert::AreNotEqual(zeroAddress, memoryAddress);

				Assert::AreEqual(zeroAddress, reinterpret_cast<std::size_t>(memoryPtr));
			}

			TEST_METHOD(testAlignedAllocWithSizeAndZMMAlignment)
			{
				auto memoryPtr = vectorization::alloc(1, Alignments::ZMM);
				auto memoryAddress = reinterpret_cast<std::size_t>(memoryPtr);
				auto zeroAddress = reinterpret_cast<std::size_t>(nullptr);

				vectorization::free(memoryPtr);

				Assert::AreEqual(zeroAddress, memoryAddress % Alignments::ZMM);
				Assert::AreNotEqual(zeroAddress, memoryAddress);

				Assert::AreEqual(zeroAddress, reinterpret_cast<std::size_t>(memoryPtr));
			}

			TEST_METHOD(testAlignedAllocWithSizeAndARCHAlignment)
			{
				auto memoryPtr = vectorization::alloc(1, Alignments::ARCH);
				auto memoryAddress = reinterpret_cast<std::size_t>(memoryPtr);
				auto zeroAddress = reinterpret_cast<std::size_t>(nullptr);

				vectorization::free(memoryPtr);

				Assert::AreEqual(zeroAddress, memoryAddress % Alignments::ARCH);
				Assert::AreNotEqual(zeroAddress, memoryAddress);

				Assert::AreEqual(zeroAddress, reinterpret_cast<std::size_t>(memoryPtr));
			}

			TEST_METHOD(testAlignedAllocWithSizeAndBestAlignment)
			{
				auto memoryPtr = vectorization::alloc(1, Alignments::Best);
				auto memoryAddress = reinterpret_cast<std::size_t>(memoryPtr);
				auto zeroAddress = reinterpret_cast<std::size_t>(nullptr);

				vectorization::free(memoryPtr);

				Assert::AreEqual(zeroAddress, memoryAddress % Alignments::Best);
				Assert::AreNotEqual(zeroAddress, memoryAddress);

				Assert::AreEqual(zeroAddress, reinterpret_cast<std::size_t>(memoryPtr));
			}

		};

	}
}
