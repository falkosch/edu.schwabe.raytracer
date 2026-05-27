#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(MovemaskTest) {
  public:
    TEST_METHOD(movemaskPackedFloat4_128AllZero) {
      const auto v = _mm_set_ps1(0.0f);
      Assert::AreEqual(Int_32{0}, movemask(v), L"all-zero has no sign bits", LINE_INFO());
    }

    TEST_METHOD(movemaskPackedFloat4_128AllNegative) {
      const auto v = _mm_set_ps(-1.0f, -2.0f, -3.0f, -4.0f);
      Assert::AreEqual(Int_32{0xF}, movemask(v), L"all negative has all sign bits", LINE_INFO());
    }

    TEST_METHOD(movemaskPackedFloat4_128Mixed) {
      const auto v = _mm_set_ps(-1.0f, 2.0f, -3.0f, 4.0f);
      Assert::AreEqual(Int_32{0xA}, movemask(v), L"lanes 1,3 are negative", LINE_INFO());
    }

    TEST_METHOD(movemaskPackedFloat2_128AllZero) {
      const auto v = _mm_set_pd1(0.0);
      Assert::AreEqual(Int_32{0}, movemask(v), L"all-zero has no sign bits", LINE_INFO());
    }

    TEST_METHOD(movemaskPackedFloat2_128AllNegative) {
      const auto v = _mm_set_pd(-1.0, -2.0);
      Assert::AreEqual(Int_32{0x3}, movemask(v), L"both negative has both sign bits", LINE_INFO());
    }

    TEST_METHOD(movemaskPackedFloat2_128Mixed) {
      const auto v = _mm_set_pd(-1.0, 2.0);
      Assert::AreEqual(Int_32{0x2}, movemask(v), L"lane 1 is negative", LINE_INFO());
    }

    TEST_METHOD(movemaskPackedFloat8_256AllZero) {
      const auto v = _mm256_set1_ps(0.0f);
      Assert::AreEqual(Int_32{0}, movemask(v), L"all-zero has no sign bits", LINE_INFO());
    }

    TEST_METHOD(movemaskPackedFloat8_256AllNegative) {
      const auto v = _mm256_set1_ps(-1.0f);
      Assert::AreEqual(Int_32{0xFF}, movemask(v), L"all negative has all sign bits", LINE_INFO());
    }

    TEST_METHOD(movemaskPackedFloat8_256Mixed) {
      const auto v = _mm256_set_ps(-1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f, -7.0f, 8.0f);
      Assert::AreEqual(Int_32{0xAA}, movemask(v), L"alternating sign pattern", LINE_INFO());
    }

    TEST_METHOD(movemaskPackedFloat4_256AllZero) {
      const auto v = _mm256_set1_pd(0.0);
      Assert::AreEqual(Int_32{0}, movemask(v), L"all-zero has no sign bits", LINE_INFO());
    }

    TEST_METHOD(movemaskPackedFloat4_256AllNegative) {
      const auto v = _mm256_set1_pd(-1.0);
      Assert::AreEqual(Int_32{0xF}, movemask(v), L"all negative has all sign bits", LINE_INFO());
    }

    TEST_METHOD(movemaskPackedFloat4_256Mixed) {
      const auto v = _mm256_set_pd(-1.0, 2.0, -3.0, 4.0);
      Assert::AreEqual(Int_32{0xA}, movemask(v), L"lanes 1,3 are negative", LINE_INFO());
    }

    TEST_METHOD(movemaskDetectsNegativeZero) {
      const auto v = _mm_set_ps(0.0f, -0.0f, 0.0f, -0.0f);
      Assert::AreEqual(Int_32{0x5}, movemask(v), L"negative zero has sign bit set", LINE_INFO());
    }
  };
}
