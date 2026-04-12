#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i32_4_ConstantsTest) {
  public:
    TEST_METHOD(hasMaskAll) {
      Assert::AreEqual(ASizeT{128}, bitCount(MaskAll<v_i32_4>().components));
    }

    TEST_METHOD(hasMaskX) {
      const auto actual = MaskX<v_i32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskXY) {
      const auto actual = MaskXY<v_i32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskXYZ) {
      const auto actual = MaskXYZ<v_i32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskXYZW) {
      Assert::AreEqual(ASizeT{128}, bitCount(MaskXYZW<v_i32_4>().components));
    }

    TEST_METHOD(hasMaskY) {
      const auto actual = MaskY<v_i32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskYZ) {
      const auto actual = MaskYZ<v_i32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::Y, VectorIndices::Z>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::W, VectorIndices::W>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskYZW) {
      const auto actual = MaskYZW<v_i32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskZ) {
      const auto actual = MaskZ<v_i32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskZW) {
      const auto actual = MaskZW<v_i32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskW) {
      const auto actual = MaskW<v_i32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0},
          bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(v_i32_4{actual})
                       .components),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasZero) {
      Assert::AreEqual(ASizeT{0}, bitCount(Zero<v_i32_4>().components));
    }

    TEST_METHOD(hasOne) {
      const auto actual = One<v_i32_4>();
      Assert::AreEqual(Int_32{1}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{1}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{1}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{1}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(hasTwo) {
      const auto actual = Two<v_i32_4>();
      Assert::AreEqual(Int_32{2}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{2}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{2}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{2}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(hasNegativeOne) {
      const auto actual = NegativeOne<v_i32_4>();
      Assert::AreEqual(Int_32{-1}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(hasNegativeTwo) {
      const auto actual = NegativeTwo<v_i32_4>();
      Assert::AreEqual(Int_32{-2}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-2}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-2}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-2}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(hasOneW) {
      const v_i32_4 expected{0, 0, 0, 1};
      Assert::IsTrue(allTrue(expected == OneW<v_i32_4>()));
    }
  };
}
