#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(SelectScalarTest) {
  public:
    TEST_METHOD(selectsOneOfTwoBoolWithBoolConditionMask) {
      constexpr auto givenTruthy = true;
      constexpr auto givenFalsy = false;

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoInt_8WithBoolConditionMask) {
      constexpr auto givenTruthy = Int_8{15};
      constexpr auto givenFalsy = Int_8{122};

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoInt_8WithInt_8ConditionMask) {
      constexpr auto givenTruthy = Int_8{15};
      constexpr auto givenFalsy = Int_8{122};

      {
        const auto actual = select(Int_8{42}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(Int_8{0}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoUInt_8WithBoolConditionMask) {
      constexpr auto givenTruthy = UInt_8{15};
      constexpr auto givenFalsy = UInt_8{240};

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoUInt_8WithUInt_8ConditionMask) {
      constexpr auto givenTruthy = UInt_8{15};
      constexpr auto givenFalsy = UInt_8{122};

      {
        const auto actual = select(UInt_8{42}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(UInt_8{0}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoInt_16WithBoolConditionMask) {
      constexpr auto givenTruthy = Int_16{15};
      constexpr auto givenFalsy = Int_16{122};

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoInt_16WithInt_16ConditionMask) {
      constexpr auto givenTruthy = Int_16{15};
      constexpr auto givenFalsy = Int_16{122};

      {
        const auto actual = select(Int_16{42}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(Int_16{0}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoUInt_16WithBoolConditionMask) {
      constexpr auto givenTruthy = UInt_16{15};
      constexpr auto givenFalsy = UInt_16{240};

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoUInt_16WithUInt_16ConditionMask) {
      constexpr auto givenTruthy = UInt_16{15};
      constexpr auto givenFalsy = UInt_16{122};

      {
        const auto actual = select(UInt_16{42}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(UInt_16{0}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoInt_32WithBoolConditionMask) {
      constexpr auto givenTruthy = Int_32{15};
      constexpr auto givenFalsy = Int_32{122};

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoInt_32WithInt_32ConditionMask) {
      constexpr auto givenTruthy = Int_32{15};
      constexpr auto givenFalsy = Int_32{122};

      {
        const auto actual = select(Int_32{42}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(Int_32{0}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoUInt_32WithBoolConditionMask) {
      constexpr auto givenTruthy = UInt_32{15};
      constexpr auto givenFalsy = UInt_32{240};

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoUInt_32WithUInt_32ConditionMask) {
      constexpr auto givenTruthy = UInt_32{15};
      constexpr auto givenFalsy = UInt_32{122};

      {
        const auto actual = select(UInt_32{42}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(UInt_32{0}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoInt_64WithBoolConditionMask) {
      constexpr auto givenTruthy = Int_64{15};
      constexpr auto givenFalsy = Int_64{122};

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoInt_64WithInt_64ConditionMask) {
      constexpr auto givenTruthy = Int_64{15};
      constexpr auto givenFalsy = Int_64{122};

      {
        const auto actual = select(Int_64{42}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(Int_64{0}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoUInt_64WithBoolConditionMask) {
      constexpr auto givenTruthy = UInt_64{15};
      constexpr auto givenFalsy = UInt_64{240};

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoUInt_64WithUInt_64ConditionMask) {
      constexpr auto givenTruthy = UInt_64{15};
      constexpr auto givenFalsy = UInt_64{122};

      {
        const auto actual = select(UInt_64{42}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(UInt_64{0}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoFloat_32WithBoolConditionMask) {
      constexpr auto givenTruthy = Float_32{1.0f};
      constexpr auto givenFalsy = Float_32{5.0f};

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoFloat_32WithBoolTypeOfFloat_32ConditionMask) {
      constexpr auto givenTruthy = Float_32{1.0f};
      constexpr auto givenFalsy = Float_32{5.0f};

      {
        const auto actual = select(BoolTypes<Float_32>::Type{42}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(BoolTypes<Float_32>::Type{0}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoFloat_64WithBoolConditionMask) {
      constexpr auto givenTruthy = Float_64{1.0};
      constexpr auto givenFalsy = Float_64{5.0};

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsOneOfTwoFloat_64WithBoolTypeOfFloat_64ConditionMask) {
      constexpr auto givenTruthy = Float_64{1.0f};
      constexpr auto givenFalsy = Float_64{5.0f};

      {
        const auto actual = select(BoolTypes<Float_64>::Type{42}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenTruthy, actual, L"select value mismatch", LINE_INFO());
      }

      {
        const auto actual = select(BoolTypes<Float_64>::Type{0}, givenTruthy, givenFalsy);
        Assert::AreEqual(givenFalsy, actual, L"select value mismatch", LINE_INFO());
      }
    }
  };
}
