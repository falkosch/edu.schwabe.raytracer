#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui64_2_SwizzledBlendsTest) {
  public:
    TEST_METHOD(genericallySwizzleBlendsIdentity) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 b{UInt_64{30}, UInt_64{40}};

      {
        const auto actual = swizzledBlend<VectorIndices::X, VectorIndices::Y, false, false>(a, b);
        Assert::IsTrue(allTrue(a == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        const auto actual = swizzledBlend<VectorIndices::X, VectorIndices::Y, true, true>(a, b);
        Assert::IsTrue(allTrue(b == actual), L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzleBlendsXX) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 b{UInt_64{30}, UInt_64{40}};

      {
        // _mm_unpacklo_epi64(a, b): a.x, b.x
        const v_ui64_2 expected{UInt_64{10}, UInt_64{30}};
        const auto actual = swizzledBlend<VectorIndices::X, VectorIndices::X, false, true>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // _mm_unpacklo_epi64(b, a): b.x, a.x
        const v_ui64_2 expected{UInt_64{30}, UInt_64{10}};
        const auto actual = swizzledBlend<VectorIndices::X, VectorIndices::X, true, false>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzleBlendsYY) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 b{UInt_64{30}, UInt_64{40}};

      {
        // _mm_unpackhi_epi64(a, b): a.y, b.y
        const v_ui64_2 expected{UInt_64{20}, UInt_64{40}};
        const auto actual = swizzledBlend<VectorIndices::Y, VectorIndices::Y, false, true>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // _mm_unpackhi_epi64(b, a): b.y, a.y
        const v_ui64_2 expected{UInt_64{40}, UInt_64{20}};
        const auto actual = swizzledBlend<VectorIndices::Y, VectorIndices::Y, true, false>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzledBlend1x1AllCombinations) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 b{UInt_64{30}, UInt_64{40}};

      {
        // <X,X>: a.x, b.x
        const v_ui64_2 expected{UInt_64{10}, UInt_64{30}};
        const auto actual = swizzledBlend1x1<VectorIndices::X, VectorIndices::X>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // <X,Y>: a.x, b.y
        const v_ui64_2 expected{UInt_64{10}, UInt_64{40}};
        const auto actual = swizzledBlend1x1<VectorIndices::X, VectorIndices::Y>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // <Y,X>: a.y, b.x
        const v_ui64_2 expected{UInt_64{20}, UInt_64{30}};
        const auto actual = swizzledBlend1x1<VectorIndices::Y, VectorIndices::X>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // <Y,Y>: a.y, b.y
        const v_ui64_2 expected{UInt_64{20}, UInt_64{40}};
        const auto actual = swizzledBlend1x1<VectorIndices::Y, VectorIndices::Y>(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzledBlendNamedFunctions) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 b{UInt_64{30}, UInt_64{40}};

      {
        // x_x: a.x, b.x
        const v_ui64_2 expected{UInt_64{10}, UInt_64{30}};
        const auto actual = x_x(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // x_y: a.x, b.y
        const v_ui64_2 expected{UInt_64{10}, UInt_64{40}};
        const auto actual = x_y(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // y_x: a.y, b.x
        const v_ui64_2 expected{UInt_64{20}, UInt_64{30}};
        const auto actual = y_x(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        // y_y: a.y, b.y
        const v_ui64_2 expected{UInt_64{20}, UInt_64{40}};
        const auto actual = y_y(a, b);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(swizzledBlendMaskedPassThrough) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 b{UInt_64{30}, UInt_64{40}};

      {
        const auto actual =
            swizzledBlendMasked<VectorIndices::X, VectorIndices::Y>(a, b, Zero<v_ui64_2>());
        Assert::IsTrue(allTrue(a == actual), L"zero mask: all from a", LINE_INFO());
      }

      {
        const auto actual =
            swizzledBlendMasked<VectorIndices::X, VectorIndices::Y>(a, b, MaskAll<v_ui64_2>());
        Assert::IsTrue(allTrue(b == actual), L"all mask: all from b", LINE_INFO());
      }
    }

    TEST_METHOD(swizzledBlendMaskedPartial) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 b{UInt_64{30}, UInt_64{40}};

      const v_ui64_2 expected{UInt_64{30}, UInt_64{20}};
      const auto actual =
          swizzledBlendMasked<VectorIndices::X, VectorIndices::Y>(a, b, MaskX<v_ui64_2>());
      Assert::IsTrue(allTrue(expected == actual), L"partial mask mismatch", LINE_INFO());
    }
  };
}
