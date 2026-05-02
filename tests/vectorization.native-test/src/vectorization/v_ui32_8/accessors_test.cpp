#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_8_AccessorsTest) {
  public:
    TEST_METHOD(readsComponent) {
      const v_ui32_8 given{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};

      Assert::AreEqual(UInt_32{10}, component<VectorIndices::X1>(given), L"X1 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{20}, component<VectorIndices::X2>(given), L"X2 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{30}, component<VectorIndices::X3>(given), L"X3 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{40}, component<VectorIndices::X4>(given), L"X4 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{50}, component<VectorIndices::X5>(given), L"X5 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{60}, component<VectorIndices::X6>(given), L"X6 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{70}, component<VectorIndices::X7>(given), L"X7 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{80}, component<VectorIndices::X8>(given), L"X8 mismatch", LINE_INFO());
    }

    TEST_METHOD(readsX1ThroughX8) {
      const v_ui32_8 given{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};

      Assert::AreEqual(UInt_32{10}, x1(given), L"x1 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{40}, x4(given), L"x4 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{80}, x8(given), L"x8 mismatch", LINE_INFO());
    }

    TEST_METHOD(replacesComponent) {
      const UInt_32 r = 99u;
      const v_ui32_8 given{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};

      {
        const auto actual = replaceComponent<VectorIndices::X1>(given, r);
        Assert::AreEqual(r, x1(actual), L"replaceX1 mismatch", LINE_INFO());
        Assert::AreEqual(UInt_32{20}, x2(actual), L"replaceX1 side effect", LINE_INFO());
      }

      {
        const auto actual = replaceComponent<VectorIndices::X5>(given, r);
        Assert::AreEqual(UInt_32{40}, x4(actual), L"replaceX5 side effect", LINE_INFO());
        Assert::AreEqual(r, x5(actual), L"replaceX5 mismatch", LINE_INFO());
      }

      {
        const auto actual = replaceComponent<VectorIndices::X8>(given, r);
        Assert::AreEqual(UInt_32{70}, x7(actual), L"replaceX8 side effect", LINE_INFO());
        Assert::AreEqual(r, x8(actual), L"replaceX8 mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesX1ThroughX8) {
      const UInt_32 r = 99u;
      const v_ui32_8 given{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};

      Assert::AreEqual(r, x1(replaceX1(given, r)), L"replaceX1 mismatch", LINE_INFO());
      Assert::AreEqual(r, x4(replaceX4(given, r)), L"replaceX4 mismatch", LINE_INFO());
      Assert::AreEqual(r, x8(replaceX8(given, r)), L"replaceX8 mismatch", LINE_INFO());
    }
  };
}
