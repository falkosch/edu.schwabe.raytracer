#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(MaskXYZTest) {
  public:
    TEST_METHOD(returnsPackedFloat4_128Mask) {
      const auto actual = MaskXYZ<PackedFloat4_128>();
      Assert::AreEqual(
          ASizeT{128}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(actual))
      );
    }

    TEST_METHOD(returnsPackedFloat8_256Mask) {
      const auto actual = MaskXYZ<PackedFloat8_256>();
      Assert::AreEqual(
          ASizeT{256}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(actual))
      );
    }

    TEST_METHOD(returnsPackedFloat4_256Mask) {
      const auto actual = MaskXYZ<PackedFloat4_256>();
      Assert::AreEqual(
          ASizeT{256}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(actual))
      );
    }
  };
}
