#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_4_BlendsTest) {
  public:
    TEST_METHOD(blendsAllFalseSelectsFirst) {
      const v_f64_4 a{1.0, 2.0, 3.0, 4.0};
      const v_f64_4 b{5.0, 6.0, 7.0, 8.0};
      const auto result = blend<false, false, false, false>(a, b);
      Assert::AreEqual(1.0, x(result), L"blend<ffff> X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(result), L"blend<ffff> Y mismatch", LINE_INFO());
      Assert::AreEqual(3.0, z(result), L"blend<ffff> Z mismatch", LINE_INFO());
      Assert::AreEqual(4.0, w(result), L"blend<ffff> W mismatch", LINE_INFO());
    }

    TEST_METHOD(blendsAllTrueSelectsSecond) {
      const v_f64_4 a{1.0, 2.0, 3.0, 4.0};
      const v_f64_4 b{5.0, 6.0, 7.0, 8.0};
      const auto result = blend<true, true, true, true>(a, b);
      Assert::AreEqual(5.0, x(result), L"blend<tttt> X mismatch", LINE_INFO());
      Assert::AreEqual(6.0, y(result), L"blend<tttt> Y mismatch", LINE_INFO());
      Assert::AreEqual(7.0, z(result), L"blend<tttt> Z mismatch", LINE_INFO());
      Assert::AreEqual(8.0, w(result), L"blend<tttt> W mismatch", LINE_INFO());
    }

    TEST_METHOD(blendsSelective) {
      const v_f64_4 a{1.0, 2.0, 3.0, 4.0};
      const v_f64_4 b{5.0, 6.0, 7.0, 8.0};

      {
        const auto result = blend<true, false, false, false>(a, b);
        Assert::AreEqual(5.0, x(result), L"blend<tfff> X mismatch", LINE_INFO());
        Assert::AreEqual(2.0, y(result), L"blend<tfff> Y mismatch", LINE_INFO());
        Assert::AreEqual(3.0, z(result), L"blend<tfff> Z mismatch", LINE_INFO());
        Assert::AreEqual(4.0, w(result), L"blend<tfff> W mismatch", LINE_INFO());
      }

      {
        const auto result = blend<false, true, true, false>(a, b);
        Assert::AreEqual(1.0, x(result), L"blend<fttf> X mismatch", LINE_INFO());
        Assert::AreEqual(6.0, y(result), L"blend<fttf> Y mismatch", LINE_INFO());
        Assert::AreEqual(7.0, z(result), L"blend<fttf> Z mismatch", LINE_INFO());
        Assert::AreEqual(4.0, w(result), L"blend<fttf> W mismatch", LINE_INFO());
      }

      {
        const auto result = blend<false, false, false, true>(a, b);
        Assert::AreEqual(1.0, x(result), L"blend<ffft> X mismatch", LINE_INFO());
        Assert::AreEqual(2.0, y(result), L"blend<ffft> Y mismatch", LINE_INFO());
        Assert::AreEqual(3.0, z(result), L"blend<ffft> Z mismatch", LINE_INFO());
        Assert::AreEqual(8.0, w(result), L"blend<ffft> W mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(blendsMaskedWithVectorMask) {
      const v_f64_4 a{1.0, 2.0, 3.0, 4.0};
      const v_f64_4 b{5.0, 6.0, 7.0, 8.0};

      // MaskNone: select all from a
      const auto r0 = blendMasked(a, b, MaskNone<v_f64_4>());
      Assert::AreEqual(1.0, x(r0), L"blendMasked(none) X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(r0), L"blendMasked(none) Y mismatch", LINE_INFO());
      Assert::AreEqual(3.0, z(r0), L"blendMasked(none) Z mismatch", LINE_INFO());
      Assert::AreEqual(4.0, w(r0), L"blendMasked(none) W mismatch", LINE_INFO());

      // MaskAll: select all from b
      const auto r1 = blendMasked(a, b, MaskAll<v_f64_4>());
      Assert::AreEqual(5.0, x(r1), L"blendMasked(all) X mismatch", LINE_INFO());
      Assert::AreEqual(6.0, y(r1), L"blendMasked(all) Y mismatch", LINE_INFO());
      Assert::AreEqual(7.0, z(r1), L"blendMasked(all) Z mismatch", LINE_INFO());
      Assert::AreEqual(8.0, w(r1), L"blendMasked(all) W mismatch", LINE_INFO());
    }

    TEST_METHOD(blendsMaskedWithBoolMask) {
      const v_f64_4 a{1.0, 2.0, 3.0, 4.0};
      const v_f64_4 b{5.0, 6.0, 7.0, 8.0};

      // Use comparison result as mask: only Y and W match
      const v_f64_4 cmpA{0.0, 1.0, 0.0, 1.0};
      const v_f64_4 cmpB{1.0, 1.0, 1.0, 1.0};
      const auto mask = cmpA == cmpB; // X:false, Y:true, Z:false, W:true
      const auto result = blendMasked(a, b, mask);
      Assert::AreEqual(1.0, x(result), L"blendMasked(bool) X mismatch", LINE_INFO());
      Assert::AreEqual(6.0, y(result), L"blendMasked(bool) Y mismatch", LINE_INFO());
      Assert::AreEqual(3.0, z(result), L"blendMasked(bool) Z mismatch", LINE_INFO());
      Assert::AreEqual(8.0, w(result), L"blendMasked(bool) W mismatch", LINE_INFO());
    }
  };
}
