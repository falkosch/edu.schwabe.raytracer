#include "vectorization/vectorization_tests.h"

namespace vectorization
{
    namespace test
    {

        TEST_CASE("compiler specific definitions")
        {
            SECTION("has intrinsics levels")
            {
                REQUIRE(VECTORIZATION_SSE == 0);
                REQUIRE(VECTORIZATION_SSE < VECTORIZATION_SSE4);
                REQUIRE(VECTORIZATION_SSE4 < VECTORIZATION_AVX);
                REQUIRE(VECTORIZATION_AVX < VECTORIZATION_AVX2);
                REQUIRE(VECTORIZATION_AVX2 < VECTORIZATION_AVX512);
            }

            SECTION("has either DEBUG or NDEBUG set but not both")
            {
#if defined(DEBUG) && !defined(NDEBUG)
                REQUIRE(DEBUG != 0);
#endif
#if !defined(DEBUG) && defined(NDEBUG)
                REQUIRE(NDEBUG != 0);
#endif
            }

            SECTION("selects intrinsics code path")
            {
                REQUIRE(VECTORIZATION_SSE <= VECTORIZATION_INTRINSICS_LEVEL);
                REQUIRE(VECTORIZATION_AVX512 >= VECTORIZATION_INTRINSICS_LEVEL);

#if defined(__AVX__)
                REQUIRE(VECTORIZATION_AVX <= VECTORIZATION_INTRINSICS_LEVEL);
#endif

#if defined(__AVX2__)
                REQUIRE(VECTORIZATION_AVX2 <= VECTORIZATION_INTRINSICS_LEVEL);
#endif

#if defined(__AVX512F__)
                REQUIRE(VECTORIZATION_AVX512 <= VECTORIZATION_INTRINSICS_LEVEL);
#endif
            }

            SECTION("has alignment parameters")
            {
                REQUIRE(0 < X86_ALIGNMENT);
                REQUIRE(X86_ALIGNMENT < MM_ALIGNMENT);
                REQUIRE(MM_ALIGNMENT < XMM_ALIGNMENT);
                REQUIRE(XMM_ALIGNMENT < YMM_ALIGNMENT);
                REQUIRE(YMM_ALIGNMENT < ZMM_ALIGNMENT);

                REQUIRE(0 < ARCH_ALIGNMENT);
                REQUIRE(ZMM_ALIGNMENT >= ARCH_ALIGNMENT);

                REQUIRE(0 < BEST_ALIGNMENT);
                REQUIRE(ZMM_ALIGNMENT >= BEST_ALIGNMENT);
            }

        }
    }
}
