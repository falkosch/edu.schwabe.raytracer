#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(MaskYZWTest) {
  public:
    TEST_METHOD(returnsPackedFloat4_128Mask) {
      const auto actual = MaskYZW<PackedFloat4_128>();
      Assert::AreEqual(
          ASizeT{128}, bitCount(swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(actual))
      );
    }

    TEST_METHOD(returnsPackedFloat8_256Mask) {
      const auto actual = MaskYZW<PackedFloat8_256>();
      Assert::AreEqual(
          ASizeT{256}, bitCount(swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(actual))
      );
    }

    TEST_METHOD(returnsPackedFloat4_256Mask) {
      const auto actual = MaskYZW<PackedFloat4_256>();
      Assert::AreEqual(
          ASizeT{256}, bitCount(swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(actual))
      );
    }
  };
}
