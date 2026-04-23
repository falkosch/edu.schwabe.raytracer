#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_4_SwizzledBlendsTest) {
  public:
    TEST_METHOD(genericallySwizzleBlendsIdentity) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{5u, 6u, 7u, 8u};

      {
        const auto actual =
            swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, false, false, false, false>(
                a, b
            );
        Assert::IsTrue(allTrue(a == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        const auto actual =
            swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, true, true, true, true>(
                a, b
            );
        Assert::IsTrue(allTrue(b == actual), L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzleBlendsUnpackLow) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{5u, 6u, 7u, 8u};

      {
        // _mm_unpacklo_epi32(a, b): a.x, b.x, a.y, b.y
        const v_ui32_4 expected{1u, 5u, 2u, 6u};
        const auto actual = swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false, true, false, true>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // _mm_unpacklo_epi32(b, a): b.x, a.x, b.y, a.y
        const v_ui32_4 expected{5u, 1u, 6u, 2u};
        const auto actual = swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true, false, true, false>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzleBlendsUnpackHigh) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{5u, 6u, 7u, 8u};

      {
        // _mm_unpackhi_epi32(a, b): a.z, b.z, a.w, b.w
        const v_ui32_4 expected{3u, 7u, 4u, 8u};
        const auto actual = swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false, true, false, true>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // _mm_unpackhi_epi32(b, a): b.z, a.z, b.w, a.w
        const v_ui32_4 expected{7u, 3u, 8u, 4u};
        const auto actual = swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true, false, true, false>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzleBlendsUnpack64) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{5u, 6u, 7u, 8u};

      {
        // _mm_unpacklo_epi64(a, b): a.x, a.y, b.x, b.y
        const v_ui32_4 expected{1u, 2u, 5u, 6u};
        const auto actual = swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false, false, true, true>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // _mm_unpacklo_epi64(b, a): b.x, b.y, a.x, a.y
        const v_ui32_4 expected{5u, 6u, 1u, 2u};
        const auto actual = swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true, true, false, false>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // _mm_unpackhi_epi64(a, b): a.z, a.w, b.z, b.w
        const v_ui32_4 expected{3u, 4u, 7u, 8u};
        const auto actual = swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false, false, true, true>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // _mm_unpackhi_epi64(b, a): b.z, b.w, a.z, a.w
        const v_ui32_4 expected{7u, 8u, 3u, 4u};
        const auto actual = swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true, true, false, false>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzledBlend2x2Identity) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{5u, 6u, 7u, 8u};

      // XY from a, ZW from b (identity swizzle)
      const v_ui32_4 expected{1u, 2u, 7u, 8u};
      const auto actual =
          swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlend2x2XY) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{5u, 6u, 7u, 8u};

      // a.x, a.y, b.x, b.y
      const v_ui32_4 expected{1u, 2u, 5u, 6u};
      const auto actual =
          swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlend2x2ZW) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{5u, 6u, 7u, 8u};

      // a.z, a.w, b.z, b.w
      const v_ui32_4 expected{3u, 4u, 7u, 8u};
      const auto actual =
          swizzledBlend2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlendXY_XY) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{5u, 6u, 7u, 8u};

      // xy_xy: a.x, a.y, b.x, b.y
      const v_ui32_4 expected{1u, 2u, 5u, 6u};
      const auto actual = xy_xy(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlendZW_ZW) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{5u, 6u, 7u, 8u};

      // zw_zw: a.z, a.w, b.z, b.w
      const v_ui32_4 expected{3u, 4u, 7u, 8u};
      const auto actual = zw_zw(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlendMaskedPassThrough) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{5u, 6u, 7u, 8u};

      {
        const auto actual =
            swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
                a, b, Zero<v_ui32_4>()
            );
        Assert::IsTrue(allTrue(a == actual), L"zero mask: all from a", LINE_INFO());
      }

      {
        const auto actual =
            swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
                a, b, MaskAll<v_ui32_4>()
            );
        Assert::IsTrue(allTrue(b == actual), L"all mask: all from b", LINE_INFO());
      }
    }

    TEST_METHOD(swizzledBlendMaskedPartial) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{5u, 6u, 7u, 8u};

      const v_ui32_4 expected{5u, 6u, 3u, 4u};
      const auto actual =
          swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
              a, b, MaskXY<v_ui32_4>()
          );
      Assert::IsTrue(allTrue(expected == actual), L"partial mask mismatch", LINE_INFO());
    }
  };
}
