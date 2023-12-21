#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(MaskXYZWTest) {
  public:
    TEST_METHOD(returnsPackedFloat4_128Mask) {
      auto actual = MaskXYZW<PackedFloat4_128>();
      Assert::AreEqual(
          ASizeT{128}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(actual))
      );
    }

    TEST_METHOD(returnsPackedFloat8_256Mask) {
      auto actual = MaskXYZW<PackedFloat8_256>();
      Assert::AreEqual(
          ASizeT{256}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(actual))
      );
    }

    TEST_METHOD(returnsPackedFloat4_256Mask) {
      auto actual = MaskXYZW<PackedFloat4_256>();
      Assert::AreEqual(
          ASizeT{256}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(actual))
      );
    }
  };
}
