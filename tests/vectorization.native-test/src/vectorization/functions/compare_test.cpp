#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(CompareTest) {
  public:
    // compareLess

    TEST_METHOD(compareLessPackedFloat4_128) {
      const auto a = _mm_set_ps(4.0f, 2.0f, 3.0f, 1.0f);
      const auto b = _mm_set_ps(3.0f, 2.0f, 4.0f, 2.0f);
      const auto mask = _mm_movemask_ps(compareLess(a, b));
      Assert::AreEqual(0x3, mask, L"lanes 0,1 are less", LINE_INFO());
    }

    TEST_METHOD(compareLessPackedFloat2_128) {
      const auto a = _mm_set_pd(3.0, 1.0);
      const auto b = _mm_set_pd(2.0, 2.0);
      const auto mask = _mm_movemask_pd(compareLess(a, b));
      Assert::AreEqual(0x1, mask, L"lane 0 is less", LINE_INFO());
    }

    TEST_METHOD(compareLessPackedFloat8_256) {
      const auto a = _mm256_set_ps(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto b = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      const auto mask = _mm256_movemask_ps(compareLess(a, b));
      Assert::AreEqual(0xF0, mask, L"upper 4 lanes are less", LINE_INFO());
    }

    TEST_METHOD(compareLessPackedFloat4_256) {
      const auto a = _mm256_set_pd(1.0, 2.0, 3.0, 4.0);
      const auto b = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      const auto mask = _mm256_movemask_pd(compareLess(a, b));
      Assert::AreEqual(0xC, mask, L"upper 2 lanes are less", LINE_INFO());
    }

    // compareGreater

    TEST_METHOD(compareGreaterPackedFloat4_128) {
      const auto a = _mm_set_ps(4.0f, 2.0f, 3.0f, 1.0f);
      const auto b = _mm_set_ps(3.0f, 2.0f, 4.0f, 2.0f);
      const auto mask = _mm_movemask_ps(compareGreater(a, b));
      Assert::AreEqual(0x8, mask, L"lane 3 is greater", LINE_INFO());
    }

    TEST_METHOD(compareGreaterPackedFloat2_128) {
      const auto a = _mm_set_pd(3.0, 1.0);
      const auto b = _mm_set_pd(2.0, 2.0);
      const auto mask = _mm_movemask_pd(compareGreater(a, b));
      Assert::AreEqual(0x2, mask, L"lane 1 is greater", LINE_INFO());
    }

    TEST_METHOD(compareGreaterPackedFloat8_256) {
      const auto a = _mm256_set_ps(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto b = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      const auto mask = _mm256_movemask_ps(compareGreater(a, b));
      Assert::AreEqual(0x0F, mask, L"lower 4 lanes are greater", LINE_INFO());
    }

    TEST_METHOD(compareGreaterPackedFloat4_256) {
      const auto a = _mm256_set_pd(1.0, 2.0, 3.0, 4.0);
      const auto b = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      const auto mask = _mm256_movemask_pd(compareGreater(a, b));
      Assert::AreEqual(0x3, mask, L"lower 2 lanes are greater", LINE_INFO());
    }

    // compareLessEqual

    TEST_METHOD(compareLessEqualPackedFloat4_128) {
      const auto a = _mm_set_ps(4.0f, 2.0f, 3.0f, 1.0f);
      const auto b = _mm_set_ps(3.0f, 2.0f, 4.0f, 2.0f);
      const auto mask = _mm_movemask_ps(compareLessEqual(a, b));
      Assert::AreEqual(0x7, mask, L"lanes 0,1,2 are less-or-equal", LINE_INFO());
    }

    TEST_METHOD(compareLessEqualPackedFloat2_128) {
      const auto a = _mm_set_pd(2.0, 1.0);
      const auto b = _mm_set_pd(2.0, 2.0);
      const auto mask = _mm_movemask_pd(compareLessEqual(a, b));
      Assert::AreEqual(0x3, mask, L"both lanes are less-or-equal", LINE_INFO());
    }

    TEST_METHOD(compareLessEqualPackedFloat8_256) {
      const auto a = _mm256_set_ps(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto b = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      const auto mask = _mm256_movemask_ps(compareLessEqual(a, b));
      Assert::AreEqual(0xF0, mask, L"upper 4 lanes are less-or-equal", LINE_INFO());
    }

    TEST_METHOD(compareLessEqualPackedFloat4_256) {
      const auto a = _mm256_set_pd(1.0, 3.0, 3.0, 4.0);
      const auto b = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      const auto mask = _mm256_movemask_pd(compareLessEqual(a, b));
      Assert::AreEqual(0xC, mask, L"lanes 2,3 are less-or-equal", LINE_INFO());
    }

    // compareGreaterEqual

    TEST_METHOD(compareGreaterEqualPackedFloat4_128) {
      const auto a = _mm_set_ps(4.0f, 2.0f, 3.0f, 1.0f);
      const auto b = _mm_set_ps(3.0f, 2.0f, 4.0f, 2.0f);
      const auto mask = _mm_movemask_ps(compareGreaterEqual(a, b));
      Assert::AreEqual(0xC, mask, L"lanes 2,3 are greater-or-equal", LINE_INFO());
    }

    TEST_METHOD(compareGreaterEqualPackedFloat2_128) {
      const auto a = _mm_set_pd(2.0, 1.0);
      const auto b = _mm_set_pd(2.0, 2.0);
      const auto mask = _mm_movemask_pd(compareGreaterEqual(a, b));
      Assert::AreEqual(0x2, mask, L"lane 1 is greater-or-equal", LINE_INFO());
    }

    TEST_METHOD(compareGreaterEqualPackedFloat8_256) {
      const auto a = _mm256_set_ps(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto b = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      const auto mask = _mm256_movemask_ps(compareGreaterEqual(a, b));
      Assert::AreEqual(0x0F, mask, L"lower 4 lanes are greater-or-equal", LINE_INFO());
    }

    TEST_METHOD(compareGreaterEqualPackedFloat4_256) {
      const auto a = _mm256_set_pd(1.0, 3.0, 3.0, 4.0);
      const auto b = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      const auto mask = _mm256_movemask_pd(compareGreaterEqual(a, b));
      Assert::AreEqual(0x7, mask, L"lanes 0,1,2 are greater-or-equal", LINE_INFO());
    }

    // compareEqual

    TEST_METHOD(compareEqualPackedFloat4_128) {
      const auto a = _mm_set_ps(4.0f, 2.0f, 3.0f, 1.0f);
      const auto b = _mm_set_ps(3.0f, 2.0f, 3.0f, 2.0f);
      const auto mask = _mm_movemask_ps(compareEqual(a, b));
      Assert::AreEqual(0x6, mask, L"lanes 1,2 are equal", LINE_INFO());
    }

    TEST_METHOD(compareEqualPackedFloat2_128) {
      const auto a = _mm_set_pd(3.0, 2.0);
      const auto b = _mm_set_pd(3.0, 1.0);
      const auto mask = _mm_movemask_pd(compareEqual(a, b));
      Assert::AreEqual(0x2, mask, L"lane 1 is equal", LINE_INFO());
    }

    TEST_METHOD(compareEqualPackedFloat8_256) {
      const auto a = _mm256_set_ps(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto b = _mm256_set_ps(1.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f);
      const auto mask = _mm256_movemask_ps(compareEqual(a, b));
      Assert::AreEqual(0xF0, mask, L"upper 4 lanes are equal", LINE_INFO());
    }

    TEST_METHOD(compareEqualPackedFloat4_256) {
      const auto v = _mm256_set_pd(1.0, 2.0, 3.0, 4.0);
      const auto mask = _mm256_movemask_pd(compareEqual(v, v));
      Assert::AreEqual(0xF, mask, L"all lanes equal to self", LINE_INFO());
    }

    // compareNotEqual

    TEST_METHOD(compareNotEqualPackedFloat4_128) {
      const auto a = _mm_set_ps(4.0f, 2.0f, 3.0f, 1.0f);
      const auto b = _mm_set_ps(3.0f, 2.0f, 3.0f, 2.0f);
      const auto mask = _mm_movemask_ps(compareNotEqual(a, b));
      Assert::AreEqual(0x9, mask, L"lanes 0,3 are not-equal", LINE_INFO());
    }

    TEST_METHOD(compareNotEqualPackedFloat2_128) {
      const auto a = _mm_set_pd(3.0, 2.0);
      const auto b = _mm_set_pd(3.0, 1.0);
      const auto mask = _mm_movemask_pd(compareNotEqual(a, b));
      Assert::AreEqual(0x1, mask, L"lane 0 is not-equal", LINE_INFO());
    }

    TEST_METHOD(compareNotEqualPackedFloat8_256) {
      const auto v = _mm256_set_ps(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto mask = _mm256_movemask_ps(compareNotEqual(v, v));
      Assert::AreEqual(0x00, mask, L"no lanes differ from self", LINE_INFO());
    }

    TEST_METHOD(compareNotEqualPackedFloat4_256) {
      const auto a = _mm256_set_pd(1.0, 2.0, 3.0, 4.0);
      const auto b = _mm256_set_pd(1.0, 9.0, 3.0, 9.0);
      const auto mask = _mm256_movemask_pd(compareNotEqual(a, b));
      Assert::AreEqual(0x5, mask, L"lanes 0,2 differ", LINE_INFO());
    }

    // Edge case: equal values are not less

    TEST_METHOD(compareLessReturnsFalseForEqual) {
      const auto v = _mm_set_ps1(5.0f);
      const auto mask = _mm_movemask_ps(compareLess(v, v));
      Assert::AreEqual(0, mask, L"equal values are not less", LINE_INFO());
    }

    TEST_METHOD(compareGreaterReturnsFalseForEqual) {
      const auto v = _mm_set_ps1(5.0f);
      const auto mask = _mm_movemask_ps(compareGreater(v, v));
      Assert::AreEqual(0, mask, L"equal values are not greater", LINE_INFO());
    }
  };
}
