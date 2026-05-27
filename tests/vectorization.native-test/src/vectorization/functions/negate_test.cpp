#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(NegateTest) {
  public:
    TEST_METHOD(negatesFloat32) {
      Assert::AreEqual(-3.0f, negate(3.0f), L"negate positive", LINE_INFO());
      Assert::AreEqual(5.0f, negate(-5.0f), L"negate negative", LINE_INFO());
      Assert::AreEqual(0.0f, negate(0.0f), L"negate zero", LINE_INFO());
    }

    TEST_METHOD(negatesFloat64) {
      Assert::AreEqual(-3.0, negate(3.0), L"negate positive", LINE_INFO());
      Assert::AreEqual(5.0, negate(-5.0), L"negate negative", LINE_INFO());
      Assert::AreEqual(0.0, negate(0.0), L"negate zero", LINE_INFO());
    }

    TEST_METHOD(negatesPackedFloat4_128) {
      const auto v = _mm_set_ps(4.0f, -3.0f, 2.0f, -1.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), negate(v));
      Assert::AreEqual(1.0f, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(-2.0f, actual[1], L"y lane", LINE_INFO());
      Assert::AreEqual(3.0f, actual[2], L"z lane", LINE_INFO());
      Assert::AreEqual(-4.0f, actual[3], L"w lane", LINE_INFO());
    }

    TEST_METHOD(negatesPackedFloat2_128) {
      const auto v = _mm_set_pd(-3.0, 2.0);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), negate(v));
      Assert::AreEqual(-2.0, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(3.0, actual[1], L"y lane", LINE_INFO());
    }

    TEST_METHOD(negatesPackedFloat8_256) {
      const auto v = _mm256_set_ps(8.0f, -7.0f, 6.0f, -5.0f, 4.0f, -3.0f, 2.0f, -1.0f);
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), negate(v));
      Assert::AreEqual(1.0f, actual[0], L"lane 0", LINE_INFO());
      Assert::AreEqual(-2.0f, actual[1], L"lane 1", LINE_INFO());
      Assert::AreEqual(3.0f, actual[2], L"lane 2", LINE_INFO());
      Assert::AreEqual(-4.0f, actual[3], L"lane 3", LINE_INFO());
      Assert::AreEqual(5.0f, actual[4], L"lane 4", LINE_INFO());
      Assert::AreEqual(-6.0f, actual[5], L"lane 5", LINE_INFO());
      Assert::AreEqual(7.0f, actual[6], L"lane 6", LINE_INFO());
      Assert::AreEqual(-8.0f, actual[7], L"lane 7", LINE_INFO());
    }

    TEST_METHOD(negatesPackedFloat4_256) {
      const auto v = _mm256_set_pd(-4.0, 3.0, -2.0, 1.0);
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), negate(v));
      Assert::AreEqual(-1.0, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(2.0, actual[1], L"y lane", LINE_INFO());
      Assert::AreEqual(-3.0, actual[2], L"z lane", LINE_INFO());
      Assert::AreEqual(4.0, actual[3], L"w lane", LINE_INFO());
    }

    TEST_METHOD(negateIsInvolution) {
      const auto v = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), negate(negate(v)));
      Assert::AreEqual(1.0f, actual[0], L"double negate restores x", LINE_INFO());
      Assert::AreEqual(2.0f, actual[1], L"double negate restores y", LINE_INFO());
      Assert::AreEqual(3.0f, actual[2], L"double negate restores z", LINE_INFO());
      Assert::AreEqual(4.0f, actual[3], L"double negate restores w", LINE_INFO());
    }

    TEST_METHOD(negateOfZeroProducesNegativeZero) {
      const auto v = _mm_set_ps1(0.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), negate(v));
      Assert::IsTrue(std::signbit(actual[0]), L"negate(0) should have negative sign", LINE_INFO());
    }
  };
}
