#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui64_2_ConstantsTest) {
  public:
    TEST_METHOD(hasMaskAll) {
      Assert::AreEqual(ASizeT{128}, bitCount(MaskAll<v_ui64_2>().components));
    }

    TEST_METHOD(hasMaskX) {
      const auto actual = MaskX<v_ui64_2>();
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
      Assert::AreEqual(ASizeT{128}, bitCount(MaskXY<v_ui64_2>().components));
    }

    TEST_METHOD(hasMaskY) {
      const auto actual = MaskY<v_ui64_2>();
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
      Assert::AreEqual(ASizeT{0}, bitCount(Zero<v_ui64_2>().components));
    }

    TEST_METHOD(hasOne) {
      const auto actual = One<v_ui64_2>();
      Assert::AreEqual(UInt_64{1}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{1}, y(actual), L"Y mismatch", LINE_INFO());
    }

    TEST_METHOD(hasTwo) {
      const auto actual = Two<v_ui64_2>();
      Assert::AreEqual(UInt_64{2}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{2}, y(actual), L"Y mismatch", LINE_INFO());
    }
  };
}
