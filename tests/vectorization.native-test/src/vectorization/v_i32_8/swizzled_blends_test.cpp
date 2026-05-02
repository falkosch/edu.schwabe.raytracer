#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i32_8_SwizzledBlendsTest) {
  public:
    TEST_METHOD(swizzledBlendIdentity) {
      const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
      const v_i32_8 b{11, 12, 13, 14, 15, 16, 17, 18};

      const auto actual = swizzledBlend<
          VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W,
          VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W,
          false, false, false, false, false, false, false, false>(a, b);
      Assert::IsTrue(allTrue(a == actual), L"swizzledBlend identity mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlendAllFromB) {
      const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
      const v_i32_8 b{11, 12, 13, 14, 15, 16, 17, 18};

      const auto actual = swizzledBlend<
          VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W,
          VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W,
          true, true, true, true, true, true, true, true>(a, b);
      Assert::IsTrue(allTrue(b == actual), L"swizzledBlend all-from-b mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlendMaskedIdentity) {
      const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
      const v_i32_8 b{11, 12, 13, 14, 15, 16, 17, 18};

      const auto actual = swizzledBlendMasked<
          VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(a, b, Zero<v_i32_8>());
      Assert::IsTrue(allTrue(a == actual), L"swizzledBlendMasked identity mismatch", LINE_INFO());
    }
  };
}
