#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(BlendMaskedScalarTest) {
  public:
    TEST_METHOD(variadicBlendsTwoBoolWithBoolMask) {
      auto givenOnBitNotSet = false;
      auto givenOnBitSet = true;

      {
        auto expected = givenOnBitNotSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, false);
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        auto expected = givenOnBitSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, true);
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoInt_8WithInt_8Mask) {
      auto givenOnBitNotSet = Int_8{15};
      auto givenOnBitSet = Int_8{122};

      {
        auto expected = givenOnBitNotSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<Int_8>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        auto expected = givenOnBitSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<Int_8>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoUInt_8WithUInt_8Mask) {
      auto givenOnBitNotSet = UInt_8{15};
      auto givenOnBitSet = UInt_8{240};

      {
        auto expected = givenOnBitNotSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<UInt_8>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        auto expected = givenOnBitSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<UInt_8>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoInt_16WithInt_16Mask) {
      auto givenOnBitNotSet = Int_16{15};
      auto givenOnBitSet = Int_16{122};

      {
        auto expected = givenOnBitNotSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<Int_16>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        auto expected = givenOnBitSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<Int_16>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoUInt_16WithUInt_16Mask) {
      auto givenOnBitNotSet = UInt_16{15};
      auto givenOnBitSet = UInt_16{240};

      {
        auto expected = givenOnBitNotSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<UInt_16>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        auto expected = givenOnBitSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<UInt_16>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoInt_32WithInt_32Mask) {
      auto givenOnBitNotSet = Int_32{15};
      auto givenOnBitSet = Int_32{122};

      {
        auto expected = givenOnBitNotSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<Int_32>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        auto expected = givenOnBitSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<Int_32>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoUInt_32WithUInt_32Mask) {
      auto givenOnBitNotSet = UInt_32{15};
      auto givenOnBitSet = UInt_32{240};

      {
        auto expected = givenOnBitNotSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<UInt_32>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        auto expected = givenOnBitSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<UInt_32>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoInt_64WithInt_64Mask) {
      auto givenOnBitNotSet = Int_64{15};
      auto givenOnBitSet = Int_64{122};

      {
        auto expected = givenOnBitNotSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<Int_64>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        auto expected = givenOnBitSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<Int_64>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoUInt_64WithUInt_64Mask) {
      auto givenOnBitNotSet = UInt_64{15};
      auto givenOnBitSet = UInt_64{240};

      {
        auto expected = givenOnBitNotSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<UInt_64>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        auto expected = givenOnBitSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<UInt_64>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoFloat_32WithBool_32Mask) {
      auto givenOnBitNotSet = Float_32{1.0f};
      auto givenOnBitSet = Float_32{5.0f};

      {
        auto expected = givenOnBitNotSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<Bool_32>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        auto expected = givenOnBitSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<Bool_32>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoFloat_64WithBool_64Mask) {
      auto givenOnBitNotSet = Float_64{1.0f};
      auto givenOnBitSet = Float_64{5.0f};

      {
        auto expected = givenOnBitNotSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<Bool_64>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        auto expected = givenOnBitSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<Bool_64>());
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }
  };
}
