#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_8_SwizzlesTest) {
  public:
    TEST_METHOD(identitySwizzle) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(v);
      Assert::IsTrue(allTrue(v == r), L"identity", LINE_INFO());
    }

    TEST_METHOD(xxxxBroadcastsFirstOfEachHalf) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = xxxx(v);
      Assert::AreEqual(1.0f, x1(r), L"lo x1", LINE_INFO());
      Assert::AreEqual(1.0f, x2(r), L"lo x2", LINE_INFO());
      Assert::AreEqual(1.0f, x3(r), L"lo x3", LINE_INFO());
      Assert::AreEqual(1.0f, x4(r), L"lo x4", LINE_INFO());
      Assert::AreEqual(5.0f, x5(r), L"hi x5", LINE_INFO());
      Assert::AreEqual(5.0f, x6(r), L"hi x6", LINE_INFO());
      Assert::AreEqual(5.0f, x7(r), L"hi x7", LINE_INFO());
      Assert::AreEqual(5.0f, x8(r), L"hi x8", LINE_INFO());
    }

    TEST_METHOD(yyyyBroadcastsSecondOfEachHalf) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = yyyy(v);
      Assert::AreEqual(2.0f, x1(r), L"lo", LINE_INFO());
      Assert::AreEqual(6.0f, x5(r), L"hi", LINE_INFO());
    }

    TEST_METHOD(zzzzBroadcastsThirdOfEachHalf) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = zzzz(v);
      Assert::AreEqual(3.0f, x1(r), L"lo", LINE_INFO());
      Assert::AreEqual(7.0f, x5(r), L"hi", LINE_INFO());
    }

    TEST_METHOD(wwwwBroadcastsFourthOfEachHalf) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = wwww(v);
      Assert::AreEqual(4.0f, x1(r), L"lo", LINE_INFO());
      Assert::AreEqual(8.0f, x5(r), L"hi", LINE_INFO());
    }

    TEST_METHOD(xxyyInterleavesFirstTwo) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = xxyy(v);
      Assert::AreEqual(1.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(1.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(2.0f, x3(r), L"x3", LINE_INFO());
      Assert::AreEqual(2.0f, x4(r), L"x4", LINE_INFO());
      Assert::AreEqual(5.0f, x5(r), L"x5", LINE_INFO());
      Assert::AreEqual(5.0f, x6(r), L"x6", LINE_INFO());
      Assert::AreEqual(6.0f, x7(r), L"x7", LINE_INFO());
      Assert::AreEqual(6.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(xyxyRepeatsXY) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = xyxy(v);
      Assert::AreEqual(1.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(1.0f, x3(r), L"x3", LINE_INFO());
      Assert::AreEqual(2.0f, x4(r), L"x4", LINE_INFO());
      Assert::AreEqual(5.0f, x5(r), L"x5", LINE_INFO());
      Assert::AreEqual(6.0f, x6(r), L"x6", LINE_INFO());
      Assert::AreEqual(5.0f, x7(r), L"x7", LINE_INFO());
      Assert::AreEqual(6.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(yywwInterleavesSecondAndFourth) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = yyww(v);
      Assert::AreEqual(2.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(4.0f, x3(r), L"x3", LINE_INFO());
      Assert::AreEqual(4.0f, x4(r), L"x4", LINE_INFO());
      Assert::AreEqual(6.0f, x5(r), L"x5", LINE_INFO());
      Assert::AreEqual(6.0f, x6(r), L"x6", LINE_INFO());
      Assert::AreEqual(8.0f, x7(r), L"x7", LINE_INFO());
      Assert::AreEqual(8.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(zzwwInterleavesThirdAndFourth) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = zzww(v);
      Assert::AreEqual(3.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(3.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(4.0f, x3(r), L"x3", LINE_INFO());
      Assert::AreEqual(4.0f, x4(r), L"x4", LINE_INFO());
      Assert::AreEqual(7.0f, x5(r), L"x5", LINE_INFO());
      Assert::AreEqual(7.0f, x6(r), L"x6", LINE_INFO());
      Assert::AreEqual(8.0f, x7(r), L"x7", LINE_INFO());
      Assert::AreEqual(8.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(zwzwRepeatsZW) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = zwzw(v);
      Assert::AreEqual(3.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(4.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(3.0f, x3(r), L"x3", LINE_INFO());
      Assert::AreEqual(4.0f, x4(r), L"x4", LINE_INFO());
      Assert::AreEqual(7.0f, x5(r), L"x5", LINE_INFO());
      Assert::AreEqual(8.0f, x6(r), L"x6", LINE_INFO());
    }
  };
}
