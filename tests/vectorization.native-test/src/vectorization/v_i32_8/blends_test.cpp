#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i32_8_BlendsTest) {
  public:
    TEST_METHOD(genericallyBlendsIdentities) {
      const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
      const v_i32_8 b{11, 12, 13, 14, 15, 16, 17, 18};

      {
        const auto actual = blend<false, false, false, false, false, false, false, false>(a, b);
        Assert::IsTrue(allTrue(a == actual), L"all-false blend mismatch", LINE_INFO());
      }

      {
        const auto actual = blend<true, true, true, true, true, true, true, true>(a, b);
        Assert::IsTrue(allTrue(b == actual), L"all-true blend mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsIndividuals) {
      const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
      const v_i32_8 b{11, 12, 13, 14, 15, 16, 17, 18};

      {
        const auto actual = blend<true, false, false, false, false, false, false, false>(a, b);
        Assert::AreEqual(Int_32{11}, x1(actual), L"X1 blend mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{2}, x2(actual), L"X2 blend mismatch", LINE_INFO());
      }

      {
        const auto actual = blend<false, false, false, false, true, false, false, false>(a, b);
        Assert::AreEqual(Int_32{4}, x4(actual), L"X4 blend mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{15}, x5(actual), L"X5 blend mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{6}, x6(actual), L"X6 blend mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsHalves) {
      const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
      const v_i32_8 b{11, 12, 13, 14, 15, 16, 17, 18};

      {
        const auto actual = blend<true, true, true, true, false, false, false, false>(a, b);
        Assert::AreEqual(Int_32{11}, x1(actual), L"X1 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{14}, x4(actual), L"X4 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{5}, x5(actual), L"X5 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{8}, x8(actual), L"X8 mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(blendsMasked) {
      const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
      const v_i32_8 b{11, 12, 13, 14, 15, 16, 17, 18};

      {
        const auto actual = blendMasked(a, b, Zero<v_i32_8>());
        Assert::IsTrue(allTrue(a == actual), L"zero mask blend mismatch", LINE_INFO());
      }

      {
        const v_i32_8 mask{Int_32{-1}};
        const auto actual = blendMasked(a, b, mask);
        Assert::IsTrue(allTrue(b == actual), L"all mask blend mismatch", LINE_INFO());
      }
    }
  };
}
