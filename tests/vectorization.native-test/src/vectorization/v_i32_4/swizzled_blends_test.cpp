#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i32_4_SwizzledBlendsTest) {
  public:
    TEST_METHOD(genericallySwizzleBlendsIdentity) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{5, 6, 7, 8};

      {
        const auto actual =
            swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, false, false, false,
                          false>(a, b);
        Assert::IsTrue(allTrue(a == actual), L"all from a mismatch", LINE_INFO());
      }

      {
        const auto actual =
            swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, true, true, true,
                          true>(a, b);
        Assert::IsTrue(allTrue(b == actual), L"all from b mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzleBlendsUnpackLow) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{5, 6, 7, 8};

      {
        const v_i32_4 expected{1, 5, 2, 6};
        const auto actual = swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false,
                                          true, false, true>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"unpacklo a,b mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{5, 1, 6, 2};
        const auto actual = swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true,
                                          false, true, false>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"unpacklo b,a mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzleBlendsUnpackHigh) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{5, 6, 7, 8};

      {
        const v_i32_4 expected{3, 7, 4, 8};
        const auto actual = swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false,
                                          true, false, true>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"unpackhi a,b mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{7, 3, 8, 4};
        const auto actual = swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true,
                                          false, true, false>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"unpackhi b,a mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzleBlendsUnpack64) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{5, 6, 7, 8};

      {
        const v_i32_4 expected{1, 2, 5, 6};
        const auto actual = swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false,
                                          false, true, true>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"unpacklo64 a,b mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{5, 6, 1, 2};
        const auto actual = swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true,
                                          true, false, false>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"unpacklo64 b,a mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{3, 4, 7, 8};
        const auto actual = swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false,
                                          false, true, true>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"unpackhi64 a,b mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{7, 8, 3, 4};
        const auto actual = swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true,
                                          true, false, false>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"unpackhi64 b,a mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzledBlend2x2Identity) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{5, 6, 7, 8};
      const v_i32_4 expected{1, 2, 7, 8};
      const auto actual =
          swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"2x2 identity mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlend2x2XY) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{5, 6, 7, 8};
      const v_i32_4 expected{1, 2, 5, 6};
      const auto actual =
          swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"2x2 XY mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlend2x2ZW) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{5, 6, 7, 8};
      const v_i32_4 expected{3, 4, 7, 8};
      const auto actual =
          swizzledBlend2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"2x2 ZW mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlendXY_XY) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{5, 6, 7, 8};
      const v_i32_4 expected{1, 2, 5, 6};
      const auto actual = xy_xy(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"xy_xy mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlendZW_ZW) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{5, 6, 7, 8};
      const v_i32_4 expected{3, 4, 7, 8};
      const auto actual = zw_zw(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"zw_zw mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlendMaskedPassThrough) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{5, 6, 7, 8};

      {
        const auto actual =
            swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
                a, b, Zero<v_i32_4>()
            );
        Assert::IsTrue(allTrue(a == actual), L"zero mask: all from a", LINE_INFO());
      }

      {
        const auto actual =
            swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
                a, b, MaskAll<v_i32_4>()
            );
        Assert::IsTrue(allTrue(b == actual), L"all mask: all from b", LINE_INFO());
      }
    }

    TEST_METHOD(swizzledBlendMaskedPartial) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{5, 6, 7, 8};
      const v_i32_4 expected{5, 6, 3, 4};
      const auto actual =
          swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
              a, b, MaskXY<v_i32_4>()
          );
      Assert::IsTrue(allTrue(expected == actual), L"partial mask mismatch", LINE_INFO());
    }
  };
}
