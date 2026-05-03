#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_4_SwizzlesTest) {
  public:
    TEST_METHOD(swizzlesIdentity) {
      const v_f64_4 given{1.0, 2.0, 3.0, 4.0};
      const auto result = swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(given);
      Assert::AreEqual(1.0, x(result), L"identity X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(result), L"identity Y mismatch", LINE_INFO());
      Assert::AreEqual(3.0, z(result), L"identity Z mismatch", LINE_INFO());
      Assert::AreEqual(4.0, w(result), L"identity W mismatch", LINE_INFO());
    }

    TEST_METHOD(xxxxBroadcastsX) {
      const v_f64_4 given{1.0, 2.0, 3.0, 4.0};
      const auto result = xxxx(given);
      Assert::AreEqual(1.0, x(result), L"xxxx X mismatch", LINE_INFO());
      Assert::AreEqual(1.0, y(result), L"xxxx Y mismatch", LINE_INFO());
      Assert::AreEqual(1.0, z(result), L"xxxx Z mismatch", LINE_INFO());
      Assert::AreEqual(1.0, w(result), L"xxxx W mismatch", LINE_INFO());
    }

    TEST_METHOD(yyyyBroadcastsY) {
      const v_f64_4 given{1.0, 2.0, 3.0, 4.0};
      const auto result = yyyy(given);
      Assert::AreEqual(2.0, x(result), L"yyyy X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(result), L"yyyy Y mismatch", LINE_INFO());
      Assert::AreEqual(2.0, z(result), L"yyyy Z mismatch", LINE_INFO());
      Assert::AreEqual(2.0, w(result), L"yyyy W mismatch", LINE_INFO());
    }

    TEST_METHOD(zzzzBroadcastsZ) {
      const v_f64_4 given{1.0, 2.0, 3.0, 4.0};
      const auto result = zzzz(given);
      Assert::AreEqual(3.0, x(result), L"zzzz X mismatch", LINE_INFO());
      Assert::AreEqual(3.0, y(result), L"zzzz Y mismatch", LINE_INFO());
      Assert::AreEqual(3.0, z(result), L"zzzz Z mismatch", LINE_INFO());
      Assert::AreEqual(3.0, w(result), L"zzzz W mismatch", LINE_INFO());
    }

    TEST_METHOD(wwwwBroadcastsW) {
      const v_f64_4 given{1.0, 2.0, 3.0, 4.0};
      const auto result = wwww(given);
      Assert::AreEqual(4.0, x(result), L"wwww X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, y(result), L"wwww Y mismatch", LINE_INFO());
      Assert::AreEqual(4.0, z(result), L"wwww Z mismatch", LINE_INFO());
      Assert::AreEqual(4.0, w(result), L"wwww W mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzlesDuplicateAndShufflePatterns) {
      const v_f64_4 given{1.0, 2.0, 3.0, 4.0};

      {
        const auto result = xxyy(given);
        Assert::AreEqual(1.0, x(result), L"xxyy X mismatch", LINE_INFO());
        Assert::AreEqual(1.0, y(result), L"xxyy Y mismatch", LINE_INFO());
        Assert::AreEqual(2.0, z(result), L"xxyy Z mismatch", LINE_INFO());
        Assert::AreEqual(2.0, w(result), L"xxyy W mismatch", LINE_INFO());
      }

      {
        const auto result = xyxy(given);
        Assert::AreEqual(1.0, x(result), L"xyxy X mismatch", LINE_INFO());
        Assert::AreEqual(2.0, y(result), L"xyxy Y mismatch", LINE_INFO());
        Assert::AreEqual(1.0, z(result), L"xyxy Z mismatch", LINE_INFO());
        Assert::AreEqual(2.0, w(result), L"xyxy W mismatch", LINE_INFO());
      }

      {
        const auto result = yyww(given);
        Assert::AreEqual(2.0, x(result), L"yyww X mismatch", LINE_INFO());
        Assert::AreEqual(2.0, y(result), L"yyww Y mismatch", LINE_INFO());
        Assert::AreEqual(4.0, z(result), L"yyww Z mismatch", LINE_INFO());
        Assert::AreEqual(4.0, w(result), L"yyww W mismatch", LINE_INFO());
      }

      {
        const auto result = zzww(given);
        Assert::AreEqual(3.0, x(result), L"zzww X mismatch", LINE_INFO());
        Assert::AreEqual(3.0, y(result), L"zzww Y mismatch", LINE_INFO());
        Assert::AreEqual(4.0, z(result), L"zzww Z mismatch", LINE_INFO());
        Assert::AreEqual(4.0, w(result), L"zzww W mismatch", LINE_INFO());
      }

      {
        const auto result = zwzw(given);
        Assert::AreEqual(3.0, x(result), L"zwzw X mismatch", LINE_INFO());
        Assert::AreEqual(4.0, y(result), L"zwzw Y mismatch", LINE_INFO());
        Assert::AreEqual(3.0, z(result), L"zwzw Z mismatch", LINE_INFO());
        Assert::AreEqual(4.0, w(result), L"zwzw W mismatch", LINE_INFO());
      }

      {
        const auto result = wzyx(given);
        Assert::AreEqual(4.0, x(result), L"wzyx X mismatch", LINE_INFO());
        Assert::AreEqual(3.0, y(result), L"wzyx Y mismatch", LINE_INFO());
        Assert::AreEqual(2.0, z(result), L"wzyx Z mismatch", LINE_INFO());
        Assert::AreEqual(1.0, w(result), L"wzyx W mismatch", LINE_INFO());
      }
    }
  };
}
