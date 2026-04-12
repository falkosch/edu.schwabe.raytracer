#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_4_ConstantsTest) {
  public:
    TEST_METHOD(hasMaskAll) {
      Assert::AreEqual(ASizeT{128}, bitCount(MaskAll<v_ui32_4>().components));
    }

    TEST_METHOD(hasMaskX) {
      const auto actual = MaskX<v_ui32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(
              swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(
              swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskXY) {
      const auto actual = MaskXY<v_ui32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(
              swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(
              swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskXYZ) {
      const auto actual = MaskXYZ<v_ui32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(
              swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(
              swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskXYZW) {
      Assert::AreEqual(ASizeT{128}, bitCount(MaskXYZW<v_ui32_4>().components));
    }

    TEST_METHOD(hasMaskY) {
      const auto actual = MaskY<v_ui32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(
              swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(
              swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskYZ) {
      const auto actual = MaskYZ<v_ui32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(
              swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::Y, VectorIndices::Z>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskYZW) {
      const auto actual = MaskYZW<v_ui32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(
              swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskZ) {
      const auto actual = MaskZ<v_ui32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(
              swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskZW) {
      const auto actual = MaskZW<v_ui32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(
              swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskW) {
      const auto actual = MaskW<v_ui32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(
              swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(v_ui32_4{actual})
                  .components
          ),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasZero) {
      Assert::AreEqual(ASizeT{0}, bitCount(Zero<v_ui32_4>().components));
    }

    TEST_METHOD(hasOne) {
      const auto actual = One<v_ui32_4>();
      Assert::AreEqual(UInt_32{1}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{1}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{1}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{1}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(hasTwo) {
      const auto actual = Two<v_ui32_4>();
      Assert::AreEqual(UInt_32{2}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{2}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{2}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{2}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(hasOneW) {
      const v_ui32_4 expected{0u, 0u, 0u, 1u};
      Assert::IsTrue(allTrue(expected == OneW<v_ui32_4>()));
    }
  };
}
