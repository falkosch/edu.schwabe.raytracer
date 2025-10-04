#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(MaskXYTest) {
  public:
    TEST_METHOD(returnsPackedFloat2_128Mask) {
      const auto actual = MaskXY<PackedFloat2_128>();
      Assert::AreEqual(ASizeT{128}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y>(actual)));
    }

    TEST_METHOD(returnsPackedFloat4_128Mask) {
      const auto actual = MaskXY<PackedFloat4_128>();
      Assert::AreEqual(
          ASizeT{128}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(actual))
      );
    }

    TEST_METHOD(returnsPackedFloat8_256Mask) {
      const auto actual = MaskXY<PackedFloat8_256>();
      Assert::AreEqual(
          ASizeT{256}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(actual))
      );
    }

    TEST_METHOD(returnsPackedFloat4_256Mask) {
      const auto actual = MaskXY<PackedFloat4_256>();
      Assert::AreEqual(
          ASizeT{256}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(actual))
      );
    }
  };
}
