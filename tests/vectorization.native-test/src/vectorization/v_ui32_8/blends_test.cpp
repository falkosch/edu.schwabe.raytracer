#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_8_BlendsTest) {
  public:
    TEST_METHOD(genericallyBlendsIdentities) {
      const v_ui32_8 a{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 b{11u, 12u, 13u, 14u, 15u, 16u, 17u, 18u};

      {
        const auto actual = blend<false, false, false, false, false, false, false, false>(a, b);
        Assert::IsTrue(allTrue(a == actual), L"all-false blend mismatch", LINE_INFO());
      }

      {
        const auto actual = blend<true, true, true, true, true, true, true, true>(a, b);
        Assert::IsTrue(allTrue(b == actual), L"all-true blend mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsIndividuals) {
      const v_ui32_8 a{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 b{11u, 12u, 13u, 14u, 15u, 16u, 17u, 18u};

      {
        const auto actual = blend<true, false, false, false, false, false, false, false>(a, b);
        Assert::AreEqual(UInt_32{11}, x1(actual), L"X1 blend mismatch", LINE_INFO());
        Assert::AreEqual(UInt_32{2}, x2(actual), L"X2 blend mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(blendsMasked) {
      const v_ui32_8 a{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 b{11u, 12u, 13u, 14u, 15u, 16u, 17u, 18u};

      {
        const auto actual = blendMasked(a, b, Zero<v_ui32_8>());
        Assert::IsTrue(allTrue(a == actual), L"zero mask blend mismatch", LINE_INFO());
      }
    }
  };
}
