#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_8_SwizzlesTest) {
  public:
    TEST_METHOD(identitySwizzle) {
      const v_ui32_8 given{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};
      const auto actual = swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(given);
      Assert::IsTrue(allTrue(given == actual), L"identity swizzle mismatch", LINE_INFO());
    }

    TEST_METHOD(broadcastSwizzles) {
      const v_ui32_8 given{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};

      {
        const auto actual = xxxx(given);
        Assert::AreEqual(UInt_32{10}, x1(actual), L"xxxx X1", LINE_INFO());
        Assert::AreEqual(UInt_32{10}, x4(actual), L"xxxx X4", LINE_INFO());
        Assert::AreEqual(UInt_32{50}, x5(actual), L"xxxx X5", LINE_INFO());
        Assert::AreEqual(UInt_32{50}, x8(actual), L"xxxx X8", LINE_INFO());
      }

      Assert::AreEqual(UInt_32{20}, x1(yyyy(given)), L"yyyy X1", LINE_INFO());
      Assert::AreEqual(UInt_32{30}, x1(zzzz(given)), L"zzzz X1", LINE_INFO());
      Assert::AreEqual(UInt_32{40}, x1(wwww(given)), L"wwww X1", LINE_INFO());
    }

    TEST_METHOD(duplicateFormSwizzles) {
      const v_ui32_8 given{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};

      {
        const auto actual = xxyy(given);
        Assert::AreEqual(UInt_32{10}, x1(actual), L"xxyy X1", LINE_INFO());
        Assert::AreEqual(UInt_32{10}, x2(actual), L"xxyy X2", LINE_INFO());
        Assert::AreEqual(UInt_32{20}, x3(actual), L"xxyy X3", LINE_INFO());
        Assert::AreEqual(UInt_32{20}, x4(actual), L"xxyy X4", LINE_INFO());
      }

      {
        const auto actual = zzww(given);
        Assert::AreEqual(UInt_32{30}, x1(actual), L"zzww X1", LINE_INFO());
        Assert::AreEqual(UInt_32{30}, x2(actual), L"zzww X2", LINE_INFO());
        Assert::AreEqual(UInt_32{40}, x3(actual), L"zzww X3", LINE_INFO());
        Assert::AreEqual(UInt_32{40}, x4(actual), L"zzww X4", LINE_INFO());
      }
    }
  };
}
