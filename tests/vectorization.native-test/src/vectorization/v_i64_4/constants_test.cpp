#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i64_4_ConstantsTest) {
  public:
    TEST_METHOD(hasZero) {
      const auto actual = Zero<v_i64_4>();
      Assert::AreEqual(Int_64{0}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(hasOne) {
      const auto actual = One<v_i64_4>();
      Assert::AreEqual(Int_64{1}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{1}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{1}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{1}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(hasNegativeOne) {
      const auto actual = NegativeOne<v_i64_4>();
      Assert::AreEqual(Int_64{-1}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{-1}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{-1}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{-1}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(zeroIsAllZero) {
      const auto z = Zero<v_i64_4>();
      Assert::AreEqual(Int_64{0}, x(z), L"X", LINE_INFO());
      Assert::AreEqual(Int_64{0}, w(z), L"W", LINE_INFO());
    }
  };
}
