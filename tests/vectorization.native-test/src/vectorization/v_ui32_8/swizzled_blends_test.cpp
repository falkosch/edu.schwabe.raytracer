#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_8_SwizzledBlendsTest) {
  public:
    TEST_METHOD(swizzledBlendIdentity) {
      const v_ui32_8 a{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 b{11u, 12u, 13u, 14u, 15u, 16u, 17u, 18u};

      const auto actual = swizzledBlend<
          VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W,
          VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W,
          false, false, false, false, false, false, false, false>(a, b);
      Assert::IsTrue(allTrue(a == actual), L"swizzledBlend identity mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzledBlendMaskedIdentity) {
      const v_ui32_8 a{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 b{11u, 12u, 13u, 14u, 15u, 16u, 17u, 18u};

      const auto actual = swizzledBlendMasked<
          VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(a, b, Zero<v_ui32_8>());
      Assert::IsTrue(allTrue(a == actual), L"swizzledBlendMasked identity mismatch", LINE_INFO());
    }
  };
}
