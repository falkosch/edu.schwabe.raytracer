#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4x4_OperatorsTest) {
  public:
    TEST_METHOD(addsComponentWise) {
      const v_f32_4x4 a{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };
      const v_f32_4x4 b{
          v_f32_4{10.0f, 20.0f, 30.0f, 40.0f}, v_f32_4{50.0f, 60.0f, 70.0f, 80.0f},
          v_f32_4{90.0f, 100.0f, 110.0f, 120.0f}, v_f32_4{130.0f, 140.0f, 150.0f, 160.0f}
      };

      const auto actual = a + b;

      Assert::IsTrue(allTrue(actual.soaX == v_f32_4{11.0f, 22.0f, 33.0f, 44.0f}), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == v_f32_4{55.0f, 66.0f, 77.0f, 88.0f}), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == v_f32_4{99.0f, 110.0f, 121.0f, 132.0f}), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == v_f32_4{143.0f, 154.0f, 165.0f, 176.0f}), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(subtractsComponentWise) {
      const v_f32_4x4 a{
          v_f32_4{10.0f, 20.0f, 30.0f, 40.0f}, v_f32_4{50.0f, 60.0f, 70.0f, 80.0f},
          v_f32_4{90.0f, 100.0f, 110.0f, 120.0f}, v_f32_4{130.0f, 140.0f, 150.0f, 160.0f}
      };
      const v_f32_4x4 b{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };

      const auto actual = a - b;

      Assert::IsTrue(allTrue(actual.soaX == v_f32_4{9.0f, 18.0f, 27.0f, 36.0f}), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == v_f32_4{45.0f, 54.0f, 63.0f, 72.0f}), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == v_f32_4{81.0f, 90.0f, 99.0f, 108.0f}), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == v_f32_4{117.0f, 126.0f, 135.0f, 144.0f}), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(multipliesComponentWise) {
      const v_f32_4x4 a{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };
      const v_f32_4x4 b{
          v_f32_4{2.0f, 3.0f, 4.0f, 5.0f}, v_f32_4{2.0f, 3.0f, 4.0f, 5.0f}, v_f32_4{2.0f, 3.0f, 4.0f, 5.0f},
          v_f32_4{2.0f, 3.0f, 4.0f, 5.0f}
      };

      const auto actual = a * b;

      Assert::IsTrue(allTrue(actual.soaX == v_f32_4{2.0f, 6.0f, 12.0f, 20.0f}), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == v_f32_4{10.0f, 18.0f, 28.0f, 40.0f}), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == v_f32_4{18.0f, 30.0f, 44.0f, 60.0f}), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == v_f32_4{26.0f, 42.0f, 60.0f, 80.0f}), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(dividesComponentWise) {
      const v_f32_4x4 a{
          v_f32_4{10.0f, 20.0f, 30.0f, 40.0f}, v_f32_4{50.0f, 60.0f, 70.0f, 80.0f},
          v_f32_4{90.0f, 100.0f, 110.0f, 120.0f}, v_f32_4{130.0f, 140.0f, 150.0f, 160.0f}
      };
      const v_f32_4x4 b{
          v_f32_4{2.0f, 4.0f, 5.0f, 8.0f}, v_f32_4{10.0f, 12.0f, 14.0f, 16.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };

      const auto actual = a / b;

      Assert::IsTrue(allTrue(actual.soaX == v_f32_4{5.0f, 5.0f, 6.0f, 5.0f}), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == v_f32_4{5.0f, 5.0f, 5.0f, 5.0f}), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == v_f32_4{10.0f, 10.0f, 10.0f, 10.0f}), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == v_f32_4{10.0f, 10.0f, 10.0f, 10.0f}), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(moduloComponentWise) {
      const v_f32_4x4 a{
          v_f32_4{10.0f, 11.0f, 12.0f, 13.0f}, v_f32_4{7.0f, 8.0f, 9.0f, 10.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f},
          v_f32_4{3.0f, 4.0f, 5.0f, 6.0f}
      };
      const v_f32_4x4 b{
          v_f32_4{3.0f}, v_f32_4{3.0f}, v_f32_4{3.0f}, v_f32_4{3.0f}
      };

      const auto actual = a % b;

      Assert::IsTrue(allTrue(actual.soaX == v_f32_4{1.0f, 2.0f, 0.0f, 1.0f}), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == v_f32_4{1.0f, 2.0f, 0.0f, 1.0f}), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == v_f32_4{2.0f, 0.0f, 1.0f, 2.0f}), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == v_f32_4{0.0f, 1.0f, 2.0f, 0.0f}), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(bitwiseAndOfIdenticalValuesReturnsValue) {
      const v_f32_4x4 a{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };

      const auto actual = a & a;

      Assert::IsTrue(allTrue(actual.soaX == a.soaX), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == a.soaY), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == a.soaZ), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == a.soaW), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(bitwiseOrOfIdenticalValuesReturnsValue) {
      const v_f32_4x4 a{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };

      const auto actual = a | a;

      Assert::IsTrue(allTrue(actual.soaX == a.soaX), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == a.soaY), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == a.soaZ), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == a.soaW), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(bitwiseXorOfIdenticalValuesReturnsZero) {
      const v_f32_4x4 a{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };

      const auto actual = a ^ a;
      const v_f32_4 expected{0.0f};

      Assert::IsTrue(allTrue(actual.soaX == expected), L"soaX not zero", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == expected), L"soaY not zero", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == expected), L"soaZ not zero", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == expected), L"soaW not zero", LINE_INFO());
    }

    TEST_METHOD(compoundAddAssignsComponentWise) {
      v_f32_4x4 a{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };
      const v_f32_4x4 b{
          v_f32_4{1.0f}, v_f32_4{1.0f}, v_f32_4{1.0f}, v_f32_4{1.0f}
      };

      a += b;

      Assert::IsTrue(allTrue(a.soaX == v_f32_4{2.0f, 3.0f, 4.0f, 5.0f}), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaY == v_f32_4{6.0f, 7.0f, 8.0f, 9.0f}), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaZ == v_f32_4{10.0f, 11.0f, 12.0f, 13.0f}), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaW == v_f32_4{14.0f, 15.0f, 16.0f, 17.0f}), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(compoundSubtractAssignsComponentWise) {
      v_f32_4x4 a{
          v_f32_4{10.0f, 20.0f, 30.0f, 40.0f}, v_f32_4{50.0f, 60.0f, 70.0f, 80.0f},
          v_f32_4{90.0f, 100.0f, 110.0f, 120.0f}, v_f32_4{130.0f, 140.0f, 150.0f, 160.0f}
      };
      const v_f32_4x4 b{
          v_f32_4{1.0f}, v_f32_4{1.0f}, v_f32_4{1.0f}, v_f32_4{1.0f}
      };

      a -= b;

      Assert::IsTrue(allTrue(a.soaX == v_f32_4{9.0f, 19.0f, 29.0f, 39.0f}), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaY == v_f32_4{49.0f, 59.0f, 69.0f, 79.0f}), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaZ == v_f32_4{89.0f, 99.0f, 109.0f, 119.0f}), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaW == v_f32_4{129.0f, 139.0f, 149.0f, 159.0f}), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(compoundMultiplyAssignsComponentWise) {
      v_f32_4x4 a{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };
      const v_f32_4x4 b{
          v_f32_4{2.0f}, v_f32_4{2.0f}, v_f32_4{2.0f}, v_f32_4{2.0f}
      };

      a *= b;

      Assert::IsTrue(allTrue(a.soaX == v_f32_4{2.0f, 4.0f, 6.0f, 8.0f}), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaY == v_f32_4{10.0f, 12.0f, 14.0f, 16.0f}), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaZ == v_f32_4{18.0f, 20.0f, 22.0f, 24.0f}), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaW == v_f32_4{26.0f, 28.0f, 30.0f, 32.0f}), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(compoundDivideAssignsComponentWise) {
      v_f32_4x4 a{
          v_f32_4{2.0f, 4.0f, 6.0f, 8.0f}, v_f32_4{10.0f, 12.0f, 14.0f, 16.0f}, v_f32_4{18.0f, 20.0f, 22.0f, 24.0f},
          v_f32_4{26.0f, 28.0f, 30.0f, 32.0f}
      };
      const v_f32_4x4 b{
          v_f32_4{2.0f}, v_f32_4{2.0f}, v_f32_4{2.0f}, v_f32_4{2.0f}
      };

      a /= b;

      Assert::IsTrue(allTrue(a.soaX == v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaY == v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaZ == v_f32_4{9.0f, 10.0f, 11.0f, 12.0f}), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaW == v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(compoundModuloAssignsComponentWise) {
      v_f32_4x4 a{
          v_f32_4{10.0f, 11.0f, 12.0f, 13.0f}, v_f32_4{7.0f, 8.0f, 9.0f, 10.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f},
          v_f32_4{3.0f, 4.0f, 5.0f, 6.0f}
      };
      const v_f32_4x4 b{
          v_f32_4{3.0f}, v_f32_4{3.0f}, v_f32_4{3.0f}, v_f32_4{3.0f}
      };

      a %= b;

      Assert::IsTrue(allTrue(a.soaX == v_f32_4{1.0f, 2.0f, 0.0f, 1.0f}), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaY == v_f32_4{1.0f, 2.0f, 0.0f, 1.0f}), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaZ == v_f32_4{2.0f, 0.0f, 1.0f, 2.0f}), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaW == v_f32_4{0.0f, 1.0f, 2.0f, 0.0f}), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(compoundBitwiseAndAssignsComponentWise) {
      v_f32_4x4 a{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };
      const v_f32_4x4 b{a};

      a &= b;

      Assert::IsTrue(allTrue(a.soaX == b.soaX), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaY == b.soaY), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaZ == b.soaZ), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaW == b.soaW), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(compoundBitwiseOrAssignsComponentWise) {
      v_f32_4x4 a{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };
      const v_f32_4x4 b{a};

      a |= b;

      Assert::IsTrue(allTrue(a.soaX == b.soaX), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaY == b.soaY), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaZ == b.soaZ), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaW == b.soaW), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(compoundBitwiseXorOfIdenticalValueYieldsZero) {
      v_f32_4x4 a{
          v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f},
          v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}
      };
      const v_f32_4x4 b{a};

      a ^= b;

      const v_f32_4 expected{0.0f};
      Assert::IsTrue(allTrue(a.soaX == expected), L"soaX not zero", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaY == expected), L"soaY not zero", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaZ == expected), L"soaZ not zero", LINE_INFO());
      Assert::IsTrue(allTrue(a.soaW == expected), L"soaW not zero", LINE_INFO());
    }
  };
}
