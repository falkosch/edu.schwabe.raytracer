#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(MaskWTest) {
  public:
    TEST_METHOD(returnsPackedFloat4_128Mask) {
      auto actual = MaskW<PackedFloat4_128>();
      Assert::AreEqual(
          ASizeT{128}, bitCount(swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(actual))
      );
    }

    TEST_METHOD(returnsPackedFloat8_256Mask) {
      auto actual = MaskW<PackedFloat8_256>();
      Assert::AreEqual(
          ASizeT{256}, bitCount(swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(actual))
      );
    }

    TEST_METHOD(returnsPackedFloat4_256Mask) {
      auto actual = MaskW<PackedFloat4_256>();
      Assert::AreEqual(
          ASizeT{256}, bitCount(swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(actual))
      );
    }
  };
}
