#include "stdafx.h"
#include <CppUnitTest.h>
#include <vectorization.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorizationnativetest
{

	TEST_CLASS(CompilerDefinitions)
	{
	public:

		TEST_METHOD(HasIntrinsicsLevels)
		{
			ASSERT_MSG("Malformed intrinsics levels");

			Assert::AreEqual(0, VECTORIZATION_SSE, MESSAGE, LINE_INFO());
			Assert::IsTrue(VECTORIZATION_SSE < VECTORIZATION_SSE4, MESSAGE, LINE_INFO());
			Assert::IsTrue(VECTORIZATION_SSE4 < VECTORIZATION_AVX, MESSAGE, LINE_INFO());
			Assert::IsTrue(VECTORIZATION_AVX < VECTORIZATION_AVX2, MESSAGE, LINE_INFO());
			Assert::IsTrue(VECTORIZATION_AVX2 < VECTORIZATION_AVX512, MESSAGE, LINE_INFO());

			Assert::IsTrue(VECTORIZATION_SSE <= VECTORIZATION_INTRINSICS_LEVEL, MESSAGE, LINE_INFO());
			Assert::IsTrue(VECTORIZATION_AVX512 >= VECTORIZATION_INTRINSICS_LEVEL, MESSAGE, LINE_INFO());
		}

		TEST_METHOD(HasEitherDebugOrNDebugSet)
		{
#if defined(DEBUG) && defined(NDEBUG)
			Assert::Fail();
#endif
		}

		TEST_METHOD(EnforcesIntrinsicsCodePathWhenCompilerFlagIsOn)
		{
			ASSERT_MSG("Invalid code path");

			Assert::IsTrue(VECTORIZATION_SSE <= VECTORIZATION_INTRINSICS_LEVEL, MESSAGE, LINE_INFO());

#if defined(__AVX__)
			Assert::IsTrue(VECTORIZATION_AVX <= VECTORIZATION_INTRINSICS_LEVEL, MESSAGE, LINE_INFO());
#endif

#if defined(__AVX2__)
			Assert::IsTrue(VECTORIZATION_AVX2 <= VECTORIZATION_INTRINSICS_LEVEL, MESSAGE, LINE_INFO());
#endif

#if defined(__AVX512F__)
			Assert::IsTrue(VECTORIZATION_AVX512 <= VECTORIZATION_INTRINSICS_LEVEL, MESSAGE, LINE_INFO());
#endif
		}

		TEST_METHOD(HasAlignmentParameters)
		{
			ASSERT_MSG("Malformed alignment parameters");

			Assert::IsTrue(0 < X86_ALIGNMENT, MESSAGE, LINE_INFO());
			Assert::IsTrue(X86_ALIGNMENT < MM_ALIGNMENT, MESSAGE, LINE_INFO());
			Assert::IsTrue(MM_ALIGNMENT < XMM_ALIGNMENT, MESSAGE, LINE_INFO());
			Assert::IsTrue(XMM_ALIGNMENT < YMM_ALIGNMENT, MESSAGE, LINE_INFO());
			Assert::IsTrue(YMM_ALIGNMENT < ZMM_ALIGNMENT, MESSAGE, LINE_INFO());

			Assert::IsTrue(0 < ARCH_ALIGNMENT, MESSAGE, LINE_INFO());
			Assert::IsTrue(ZMM_ALIGNMENT >= ARCH_ALIGNMENT, MESSAGE, LINE_INFO());

			Assert::IsTrue(0 < BEST_ALIGNMENT, MESSAGE, LINE_INFO());
			Assert::IsTrue(ZMM_ALIGNMENT >= BEST_ALIGNMENT, MESSAGE, LINE_INFO());
		}
	};

}
