#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i64_2_ConstantsTest) {
  public:
    TEST_METHOD(hasMaskAll) {
      Assert::AreEqual(ASizeT{128}, bitCount(MaskAll<v_i64_2>().components));
    }

    TEST_METHOD(hasMaskX) {
      const auto actual = MaskX<v_i64_2>();
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::X, VectorIndices::X>(actual).components), L"wrong mask constant value",
          LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::Y, VectorIndices::Y>(actual).components),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskXY) {
      Assert::AreEqual(ASizeT{128}, bitCount(MaskXY<v_i64_2>().components));
    }

    TEST_METHOD(hasMaskY) {
      const auto actual = MaskY<v_i64_2>();
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::Y, VectorIndices::Y>(actual).components), L"wrong mask constant value",
          LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::X>(actual).components),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasZero) {
      Assert::AreEqual(ASizeT{0}, bitCount(Zero<v_i64_2>().components));
    }

    TEST_METHOD(hasOne) {
      const auto actual = One<v_i64_2>();
      Assert::AreEqual(Int_64{1LL}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{1LL}, y(actual), L"Y mismatch", LINE_INFO());
    }

    TEST_METHOD(hasNegativeOne) {
      const auto actual = NegativeOne<v_i64_2>();
      Assert::AreEqual(Int_64{-1LL}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{-1LL}, y(actual), L"Y mismatch", LINE_INFO());
    }
  };
}
