#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(MaskYTest) {
  public:
    TEST_METHOD(returnsPackedFloat2_128Mask) {
      auto actual = MaskY<PackedFloat2_128>();
      Assert::AreEqual(ASizeT{128}, bitCount(swizzle<VectorIndices::Y, VectorIndices::Y>(actual)));
      Assert::AreEqual(ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::X>(actual)));
    }

    TEST_METHOD(returnsPackedFloat4_128Mask) {
      auto actual = MaskY<PackedFloat4_128>();
      Assert::AreEqual(
          ASizeT{128}, bitCount(swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual))
      );
    }

    TEST_METHOD(returnsPackedFloat8_256Mask) {
      auto actual = MaskY<PackedFloat8_256>();
      Assert::AreEqual(
          ASizeT{256}, bitCount(swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual))
      );
    }

    TEST_METHOD(returnsPackedFloat4_256Mask) {
      auto actual = MaskY<PackedFloat4_256>();
      Assert::AreEqual(
          ASizeT{256}, bitCount(swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(actual))
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual))
      );
    }
  };
}
