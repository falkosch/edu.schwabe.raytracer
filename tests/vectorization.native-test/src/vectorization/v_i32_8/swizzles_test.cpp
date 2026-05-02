#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i32_8_SwizzlesTest) {
  public:
    TEST_METHOD(identitySwizzle) {
      const v_i32_8 given{10, 20, 30, 40, 50, 60, 70, 80};
      const auto actual = swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(given);
      Assert::IsTrue(allTrue(given == actual), L"identity swizzle mismatch", LINE_INFO());
    }

    TEST_METHOD(broadcastSwizzles) {
      const v_i32_8 given{10, 20, 30, 40, 50, 60, 70, 80};

      {
        const auto actual = xxxx(given);
        Assert::AreEqual(Int_32{10}, x1(actual), L"xxxx X1 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{10}, x2(actual), L"xxxx X2 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{10}, x3(actual), L"xxxx X3 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{10}, x4(actual), L"xxxx X4 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{50}, x5(actual), L"xxxx X5 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{50}, x6(actual), L"xxxx X6 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{50}, x7(actual), L"xxxx X7 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{50}, x8(actual), L"xxxx X8 mismatch", LINE_INFO());
      }

      {
        const auto actual = yyyy(given);
        Assert::AreEqual(Int_32{20}, x1(actual), L"yyyy X1 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{60}, x5(actual), L"yyyy X5 mismatch", LINE_INFO());
      }

      {
        const auto actual = zzzz(given);
        Assert::AreEqual(Int_32{30}, x1(actual), L"zzzz X1 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{70}, x5(actual), L"zzzz X5 mismatch", LINE_INFO());
      }

      {
        const auto actual = wwww(given);
        Assert::AreEqual(Int_32{40}, x1(actual), L"wwww X1 mismatch", LINE_INFO());
        Assert::AreEqual(Int_32{80}, x5(actual), L"wwww X5 mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(duplicateFormSwizzles) {
      const v_i32_8 given{10, 20, 30, 40, 50, 60, 70, 80};

      {
        const auto actual = xxyy(given);
        Assert::AreEqual(Int_32{10}, x1(actual), L"xxyy X1", LINE_INFO());
        Assert::AreEqual(Int_32{10}, x2(actual), L"xxyy X2", LINE_INFO());
        Assert::AreEqual(Int_32{20}, x3(actual), L"xxyy X3", LINE_INFO());
        Assert::AreEqual(Int_32{20}, x4(actual), L"xxyy X4", LINE_INFO());
        Assert::AreEqual(Int_32{50}, x5(actual), L"xxyy X5", LINE_INFO());
        Assert::AreEqual(Int_32{50}, x6(actual), L"xxyy X6", LINE_INFO());
        Assert::AreEqual(Int_32{60}, x7(actual), L"xxyy X7", LINE_INFO());
        Assert::AreEqual(Int_32{60}, x8(actual), L"xxyy X8", LINE_INFO());
      }

      {
        const auto actual = xyxy(given);
        Assert::AreEqual(Int_32{10}, x1(actual), L"xyxy X1", LINE_INFO());
        Assert::AreEqual(Int_32{20}, x2(actual), L"xyxy X2", LINE_INFO());
        Assert::AreEqual(Int_32{10}, x3(actual), L"xyxy X3", LINE_INFO());
        Assert::AreEqual(Int_32{20}, x4(actual), L"xyxy X4", LINE_INFO());
      }

      {
        const auto actual = zzww(given);
        Assert::AreEqual(Int_32{30}, x1(actual), L"zzww X1", LINE_INFO());
        Assert::AreEqual(Int_32{30}, x2(actual), L"zzww X2", LINE_INFO());
        Assert::AreEqual(Int_32{40}, x3(actual), L"zzww X3", LINE_INFO());
        Assert::AreEqual(Int_32{40}, x4(actual), L"zzww X4", LINE_INFO());
      }

      {
        const auto actual = zwzw(given);
        Assert::AreEqual(Int_32{30}, x1(actual), L"zwzw X1", LINE_INFO());
        Assert::AreEqual(Int_32{40}, x2(actual), L"zwzw X2", LINE_INFO());
        Assert::AreEqual(Int_32{30}, x3(actual), L"zwzw X3", LINE_INFO());
        Assert::AreEqual(Int_32{40}, x4(actual), L"zwzw X4", LINE_INFO());
      }

      {
        const auto actual = yyww(given);
        Assert::AreEqual(Int_32{20}, x1(actual), L"yyww X1", LINE_INFO());
        Assert::AreEqual(Int_32{20}, x2(actual), L"yyww X2", LINE_INFO());
        Assert::AreEqual(Int_32{40}, x3(actual), L"yyww X3", LINE_INFO());
        Assert::AreEqual(Int_32{40}, x4(actual), L"yyww X4", LINE_INFO());
      }
    }
  };
}
