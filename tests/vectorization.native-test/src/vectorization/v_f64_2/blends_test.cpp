#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_2_BlendsTest) {
  public:
    TEST_METHOD(blendsAllFalseSelectsFirst) {
      const v_f64_2 a{1.0, 2.0};
      const v_f64_2 b{3.0, 4.0};
      const auto result = blend<false, false>(a, b);
      Assert::AreEqual(1.0, x(result), L"blend<ff> X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(result), L"blend<ff> Y mismatch", LINE_INFO());
    }

    TEST_METHOD(blendsAllTrueSelectsSecond) {
      const v_f64_2 a{1.0, 2.0};
      const v_f64_2 b{3.0, 4.0};
      const auto result = blend<true, true>(a, b);
      Assert::AreEqual(3.0, x(result), L"blend<tt> X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, y(result), L"blend<tt> Y mismatch", LINE_INFO());
    }

    TEST_METHOD(blendsSelective) {
      const v_f64_2 a{1.0, 2.0};
      const v_f64_2 b{3.0, 4.0};

      {
        const auto result = blend<true, false>(a, b);
        Assert::AreEqual(3.0, x(result), L"blend<tf> X mismatch", LINE_INFO());
        Assert::AreEqual(2.0, y(result), L"blend<tf> Y mismatch", LINE_INFO());
      }

      {
        const auto result = blend<false, true>(a, b);
        Assert::AreEqual(1.0, x(result), L"blend<ft> X mismatch", LINE_INFO());
        Assert::AreEqual(4.0, y(result), L"blend<ft> Y mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(blendsMaskedWithVectorMask) {
      const v_f64_2 a{1.0, 2.0};
      const v_f64_2 b{3.0, 4.0};

      // MaskNone: select all from a
      const auto r0 = blendMasked(a, b, MaskNone<v_f64_2>());
      Assert::AreEqual(1.0, x(r0), L"blendMasked(none) X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(r0), L"blendMasked(none) Y mismatch", LINE_INFO());

      // MaskAll: select all from b
      const auto r1 = blendMasked(a, b, MaskAll<v_f64_2>());
      Assert::AreEqual(3.0, x(r1), L"blendMasked(all) X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, y(r1), L"blendMasked(all) Y mismatch", LINE_INFO());

      // MaskX: select X from b, Y from a
      const auto r2 = blendMasked(a, b, MaskX<v_f64_2>());
      Assert::AreEqual(3.0, x(r2), L"blendMasked(X) X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(r2), L"blendMasked(X) Y mismatch", LINE_INFO());

      // MaskY: select X from a, Y from b
      const auto r3 = blendMasked(a, b, MaskY<v_f64_2>());
      Assert::AreEqual(1.0, x(r3), L"blendMasked(Y) X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, y(r3), L"blendMasked(Y) Y mismatch", LINE_INFO());
    }

    TEST_METHOD(blendsMaskedWithBoolMask) {
      const v_f64_2 a{1.0, 2.0};
      const v_f64_2 b{3.0, 4.0};

      // Use comparison result as mask
      const v_f64_2 cmpA{1.0, 1.0};
      const v_f64_2 cmpB{0.0, 1.0};
      const auto mask = cmpA == cmpB; // X:false, Y:true
      const auto result = blendMasked(a, b, mask);
      Assert::AreEqual(1.0, x(result), L"blendMasked(bool) X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, y(result), L"blendMasked(bool) Y mismatch", LINE_INFO());
    }
  };
}
