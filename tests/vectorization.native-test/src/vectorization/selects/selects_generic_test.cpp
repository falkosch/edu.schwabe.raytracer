#include "vectorization_tests.h"

#include <cstdint>

namespace vectorization::test {
  TEST_CLASS(SelectsGenericTest) {
  public:
    TEST_METHOD(boolMaskProducesAllBitsForTrueAndZeroForFalseOnInt_8) {
      Assert::AreEqual(Int_8{-1}, boolMask<Int_8>(true), L"bool mask mismatch", LINE_INFO());
      Assert::AreEqual(Int_8{0}, boolMask<Int_8>(false), L"bool mask mismatch", LINE_INFO());
    }

    TEST_METHOD(boolMaskProducesAllBitsForTrueAndZeroForFalseOnUInt_8) {
      Assert::AreEqual(
          static_cast<UInt_8>(0xFF), boolMask<UInt_8>(true), L"bool mask mismatch", LINE_INFO()
      );
      Assert::AreEqual(UInt_8{0}, boolMask<UInt_8>(false), L"bool mask mismatch", LINE_INFO());
    }

    TEST_METHOD(boolMaskProducesAllBitsForTrueAndZeroForFalseOnInt_16) {
      Assert::AreEqual(Int_16{-1}, boolMask<Int_16>(true), L"bool mask mismatch", LINE_INFO());
      Assert::AreEqual(Int_16{0}, boolMask<Int_16>(false), L"bool mask mismatch", LINE_INFO());
    }

    TEST_METHOD(boolMaskProducesAllBitsForTrueAndZeroForFalseOnUInt_16) {
      Assert::AreEqual(
          static_cast<UInt_16>(0xFFFF), boolMask<UInt_16>(true), L"bool mask mismatch", LINE_INFO()
      );
      Assert::AreEqual(UInt_16{0}, boolMask<UInt_16>(false), L"bool mask mismatch", LINE_INFO());
    }

    TEST_METHOD(boolMaskProducesAllBitsForTrueAndZeroForFalseOnInt_32) {
      Assert::AreEqual(Int_32{-1}, boolMask<Int_32>(true), L"bool mask mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, boolMask<Int_32>(false), L"bool mask mismatch", LINE_INFO());
    }

    TEST_METHOD(boolMaskProducesAllBitsForTrueAndZeroForFalseOnUInt_32) {
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, boolMask<UInt_32>(true), L"bool mask mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0u}, boolMask<UInt_32>(false), L"bool mask mismatch", LINE_INFO());
    }

    TEST_METHOD(boolMaskProducesAllBitsForTrueAndZeroForFalseOnInt_64) {
      Assert::AreEqual(Int_64{-1}, boolMask<Int_64>(true), L"bool mask mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, boolMask<Int_64>(false), L"bool mask mismatch", LINE_INFO());
    }

    TEST_METHOD(boolMaskProducesAllBitsForTrueAndZeroForFalseOnUInt_64) {
      Assert::AreEqual(
          UInt_64{0xFFFFFFFFFFFFFFFFull}, boolMask<UInt_64>(true), L"bool mask mismatch", LINE_INFO()
      );
      Assert::AreEqual(UInt_64{0ull}, boolMask<UInt_64>(false), L"bool mask mismatch", LINE_INFO());
    }

    TEST_METHOD(boolMaskIsConstexpr) {
      constexpr auto onTrue = boolMask<Int_32>(true);
      constexpr auto onFalse = boolMask<Int_32>(false);
      static_assert(onTrue == Int_32{-1}, "boolMask<Int_32>(true) should be -1 at compile time");
      static_assert(onFalse == Int_32{0}, "boolMask<Int_32>(false) should be 0 at compile time");

      Assert::AreEqual(Int_32{-1}, onTrue, L"bool mask mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, onFalse, L"bool mask mismatch", LINE_INFO());
    }

    TEST_METHOD(genericSelectReturnsOnTrueForTrueAndOnFalseForFalse) {
      struct Custom {
        Int_32 value;
        constexpr bool operator==(const Custom &other) const noexcept {
          return value == other.value;
        }
      };
      constexpr Custom onTrue{42};
      constexpr Custom onFalse{-1};

      const auto actualTrue = select(true, onTrue, onFalse);
      Assert::IsTrue(actualTrue == onTrue, L"generic select should return onTrue", LINE_INFO());

      const auto actualFalse = select(false, onTrue, onFalse);
      Assert::IsTrue(actualFalse == onFalse, L"generic select should return onFalse", LINE_INFO());
    }

    TEST_METHOD(genericSelectIsConstexpr) {
      struct Custom {
        Int_32 value;
      };
      constexpr Custom onTrue{7};
      constexpr Custom onFalse{13};

      constexpr auto pickedTrue = select(true, onTrue, onFalse);
      constexpr auto pickedFalse = select(false, onTrue, onFalse);
      static_assert(pickedTrue.value == 7, "generic select should be constexpr");
      static_assert(pickedFalse.value == 13, "generic select should be constexpr");

      Assert::AreEqual(Int_32{7}, pickedTrue.value, L"select value mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{13}, pickedFalse.value, L"select value mismatch", LINE_INFO());
    }
  };
}
