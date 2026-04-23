#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4x4_FunctionsTest) {
  public:
    TEST_METHOD(transposeConvertsSoaLayoutToAosRows) {
      const v_f32_4x4 given{
          v_f32_4{1.0f, 5.0f, 9.0f, 13.0f},   // soaX
          v_f32_4{2.0f, 6.0f, 10.0f, 14.0f},  // soaY
          v_f32_4{3.0f, 7.0f, 11.0f, 15.0f},  // soaZ
          v_f32_4{4.0f, 8.0f, 12.0f, 16.0f}   // soaW
      };

      const auto actual = transpose(given);

      Assert::IsTrue(
          allTrue(actual.soaX == v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}), L"soaX mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(actual.soaY == v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}), L"soaY mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(actual.soaZ == v_f32_4{9.0f, 10.0f, 11.0f, 12.0f}), L"soaZ mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(actual.soaW == v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}), L"soaW mismatch", LINE_INFO()
      );
    }

    TEST_METHOD(transposeOfTransposeReturnsOriginal) {
      const v_f32_4x4 given{
          v_f32_4{1.0f, 5.0f, 9.0f, 13.0f}, v_f32_4{2.0f, 6.0f, 10.0f, 14.0f}, v_f32_4{3.0f, 7.0f, 11.0f, 15.0f},
          v_f32_4{4.0f, 8.0f, 12.0f, 16.0f}
      };

      const auto actual = transpose(transpose(given));

      Assert::IsTrue(allTrue(actual.soaX == given.soaX), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == given.soaY), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == given.soaZ), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == given.soaW), L"soaW mismatch", LINE_INFO());
    }
  };
}
