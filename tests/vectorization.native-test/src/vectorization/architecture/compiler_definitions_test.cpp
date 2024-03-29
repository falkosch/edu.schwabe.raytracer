#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(CompilerDefinitionsTest) {
  public:
    TEST_METHOD(hasIntrinsicsLevels) {
      Assert::IsTrue(0 <= VECTORIZATION_SSE4, L"Bad intrinsics levels", LINE_INFO());
      Assert::IsTrue(VECTORIZATION_SSE4 < VECTORIZATION_AVX, L"Bad intrinsics levels", LINE_INFO());
      Assert::IsTrue(VECTORIZATION_AVX < VECTORIZATION_AVX2, L"Bad intrinsics levels", LINE_INFO());
      Assert::IsTrue(VECTORIZATION_AVX2 < VECTORIZATION_AVX512, L"Bad intrinsics levels", LINE_INFO());
    }

    TEST_METHOD(hasEitherDebugOrNDebugSet) {
#if defined(DEBUG) && defined(NDEBUG)
      Assert::Fail();
#endif
    }

    TEST_METHOD(enforcesIntrinsicsCodePathWhenCompilerFlagIsOn) {
      Assert::IsTrue(VECTORIZATION_SSE4 <= VECTORIZATION_INTRINSICS_LEVEL, L"Invalid code path", LINE_INFO());
      Assert::IsTrue(VECTORIZATION_AVX512 >= VECTORIZATION_INTRINSICS_LEVEL, L"Invalid code path", LINE_INFO());

#if defined(__AVX__)
      Assert::IsTrue(VECTORIZATION_AVX <= VECTORIZATION_INTRINSICS_LEVEL, L"Invalid code path", LINE_INFO());
#endif

#if defined(__AVX2__)
      Assert::IsTrue(VECTORIZATION_AVX2 <= VECTORIZATION_INTRINSICS_LEVEL, L"Invalid code path", LINE_INFO());
#endif

#if defined(__AVX512F__)
      Assert::IsTrue(VECTORIZATION_AVX512 <= VECTORIZATION_INTRINSICS_LEVEL, L"Invalid code path", LINE_INFO());
#endif
    }

    TEST_METHOD(hasAlignmentParameters) {
      Assert::IsTrue(0 < X86_ALIGNMENT, L"Bad alignment parameters", LINE_INFO());
      Assert::IsTrue(X86_ALIGNMENT < MM_ALIGNMENT, L"Bad alignment parameters", LINE_INFO());
      Assert::IsTrue(MM_ALIGNMENT < XMM_ALIGNMENT, L"Bad alignment parameters", LINE_INFO());
      Assert::IsTrue(XMM_ALIGNMENT < YMM_ALIGNMENT, L"Bad alignment parameters", LINE_INFO());
      Assert::IsTrue(YMM_ALIGNMENT < ZMM_ALIGNMENT, L"Bad alignment parameters", LINE_INFO());

      Assert::IsTrue(0 < ARCH_ALIGNMENT, L"Bad arch alignment parameters", LINE_INFO());
      Assert::IsTrue(ZMM_ALIGNMENT >= ARCH_ALIGNMENT, L"Bad arch alignment parameters", LINE_INFO());

      Assert::IsTrue(0 < BEST_ALIGNMENT, L"Bad best alignment parameters", LINE_INFO());
      Assert::IsTrue(ZMM_ALIGNMENT >= BEST_ALIGNMENT, L"Bad best alignment parameters", LINE_INFO());
    }
  };
}
