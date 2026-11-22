#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i64_4_FunctionsTest) {
  public:
    TEST_METHOD(computesAndnot) {
      const auto a = NegativeOne<v_i64_4>();
      const v_i64_4 b{10LL, 20LL, 30LL, 40LL};
      const auto actual = andnot(a, b);
      Assert::IsTrue(allTrue(Zero<v_i64_4>() == actual), L"andnot(allBits, b) should be zero", LINE_INFO());

      const auto actual2 = andnot(Zero<v_i64_4>(), NegativeOne<v_i64_4>());
      Assert::IsTrue(allTrue(NegativeOne<v_i64_4>() == actual2), L"andnot(zero, allBits) should be allBits", LINE_INFO());
    }

    TEST_METHOD(checksAnyTrue) {
      Assert::IsTrue(anyTrue(v_i64_4{1LL, 0LL, 0LL, 0LL}), L"anyTrue should be true with X nonzero", LINE_INFO());
      Assert::IsTrue(anyTrue(v_i64_4{0LL, 0LL, 0LL, 1LL}), L"anyTrue should be true with W nonzero", LINE_INFO());
      Assert::IsFalse(anyTrue(Zero<v_i64_4>()), L"anyTrue should be false for all zero", LINE_INFO());
    }

    TEST_METHOD(checksAllTrue) {
      Assert::IsTrue(allTrue(NegativeOne<v_i64_4>()), L"allTrue should be true for all-bits-set", LINE_INFO());
      Assert::IsFalse(allTrue(v_i64_4{-1LL, 0LL, -1LL, -1LL}), L"allTrue should be false with one zero", LINE_INFO());
    }

    TEST_METHOD(checksAnyFalse) {
      Assert::IsTrue(anyFalse(v_i64_4{-1LL, 0LL, -1LL, -1LL}), L"anyFalse should be true with one zero", LINE_INFO());
      Assert::IsFalse(anyFalse(NegativeOne<v_i64_4>()), L"anyFalse should be false for all-bits-set", LINE_INFO());
    }

    TEST_METHOD(checksAllFalse) {
      Assert::IsTrue(allFalse(Zero<v_i64_4>()), L"allFalse should be true for all zero", LINE_INFO());
      Assert::IsFalse(allFalse(v_i64_4{0LL, 1LL, 0LL, 0LL}), L"allFalse should be false with one nonzero", LINE_INFO());
    }

    TEST_METHOD(testsTruthBitwise) {
      // Components with only some bits set are neither fully true nor false.
      const v_i64_4 partial{1LL};
      Assert::IsTrue(anyTrue(partial), L"anyTrue of partial bits", LINE_INFO());
      Assert::IsTrue(anyFalse(partial), L"anyFalse of partial bits", LINE_INFO());
      Assert::IsFalse(allTrue(partial), L"allTrue of partial bits", LINE_INFO());
      Assert::IsFalse(allFalse(partial), L"allFalse of partial bits", LINE_INFO());
    }

    TEST_METHOD(checksTruth3IgnoringW) {
      Assert::IsTrue(anyTrue3(v_i64_4{0LL, 0LL, 1LL, 0LL}), L"anyTrue3 with Z set", LINE_INFO());
      Assert::IsFalse(anyTrue3(v_i64_4{0LL, 0LL, 0LL, -1LL}), L"anyTrue3 ignores W", LINE_INFO());
      Assert::IsTrue(anyFalse3(v_i64_4{-1LL, 0LL, -1LL, -1LL}), L"anyFalse3 with Y clear", LINE_INFO());
      Assert::IsFalse(anyFalse3(v_i64_4{-1LL, -1LL, -1LL, 0LL}), L"anyFalse3 ignores W", LINE_INFO());
      Assert::IsTrue(allTrue3(v_i64_4{-1LL, -1LL, -1LL, 0LL}), L"allTrue3 ignores W", LINE_INFO());
      Assert::IsFalse(allTrue3(v_i64_4{-1LL, 1LL, -1LL, -1LL}), L"allTrue3 with partial Y", LINE_INFO());
      Assert::IsTrue(allFalse3(v_i64_4{0LL, 0LL, 0LL, -1LL}), L"allFalse3 ignores W", LINE_INFO());
      Assert::IsFalse(allFalse3(v_i64_4{0LL, 1LL, 0LL, 0LL}), L"allFalse3 with Y set", LINE_INFO());
    }

    TEST_METHOD(setsAndClearsW) {
      const v_i64_4 given{1LL, 2LL, 3LL, 4LL};
      Assert::IsTrue(allTrue(v_i64_4{1LL, 2LL, 3LL, 0LL} == zeroW(given)), L"zeroW mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{1LL, 2LL, 3LL, 1LL} == oneW(given)), L"oneW mismatch", LINE_INFO());
    }

    TEST_METHOD(detectsNegativeValues) {
      Assert::AreEqual(Int_64{0b1001}, isNegative(v_i64_4{-1LL, 0LL, 1LL, -2LL}), L"isNegative mismatch", LINE_INFO());
    }

    TEST_METHOD(computesAbsoluteValueAndSqr) {
      const v_i64_4 given{-3000000000LL, 2LL, -3LL, 0LL};
      Assert::IsTrue(allTrue(v_i64_4{3000000000LL, 2LL, 3LL, 0LL} == abs(given)), L"abs mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{9000000000000000000LL, 4LL, 9LL, 0LL} == sqr(given)), L"sqr mismatch", LINE_INFO());
    }

    TEST_METHOD(computesComponentWiseMinAndMax) {
      const v_i64_4 a{1LL, -5LL, 3000000000LL, 8LL};
      const v_i64_4 b{4LL, 2LL, -3000000000LL, 1LL};
      Assert::IsTrue(allTrue(v_i64_4{1LL, -5LL, -3000000000LL, 1LL} == min(a, b)), L"min mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{4LL, 2LL, 3000000000LL, 8LL} == max(a, b)), L"max mismatch", LINE_INFO());
    }

    TEST_METHOD(computesHorizontalMinAndMax) {
      const v_i64_4 given{3LL, -1LL, 4LL, -9LL};
      Assert::AreEqual(Int_64{-9LL}, min(given), L"min mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{-9LL} == minv(given)), L"minv mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{-1LL}, min3(given), L"min3 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{-1LL} == min3v(given)), L"min3v mismatch", LINE_INFO());
      const v_i64_4 high{3LL, -1LL, 4LL, 99LL};
      Assert::AreEqual(Int_64{99LL}, max(high), L"max mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{99LL} == maxv(high)), L"maxv mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{4LL}, max3(high), L"max3 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{4LL} == max3v(high)), L"max3v mismatch", LINE_INFO());
    }

    TEST_METHOD(computesHorizontalSumAndDot) {
      const v_i64_4 a{1LL, -2LL, 3000000000LL, 40LL};
      Assert::AreEqual(Int_64{3000000039LL}, horizontalSum(a), L"horizontalSum mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{3000000039LL} == horizontalSumv(a)), L"horizontalSumv mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{2999999999LL}, horizontalSum3(a), L"horizontalSum3 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{2999999999LL} == horizontalSum3v(a)), L"horizontalSum3v mismatch", LINE_INFO());
      const v_i64_4 b{5LL, 6LL, 2LL, 10LL};
      // 5 - 12 + 6000000000 + 400
      Assert::AreEqual(Int_64{6000000393LL}, dot(a, b), L"dot mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{6000000393LL} == dotv(a, b)), L"dotv mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{5999999993LL}, dot3(a, b), L"dot3 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{5999999993LL} == dot3v(a, b)), L"dot3v mismatch", LINE_INFO());
    }

    TEST_METHOD(computesManhattanLengthAndDistance) {
      const v_i64_4 given{1LL, -2LL, 3LL, -40LL};
      Assert::AreEqual(Int_64{46LL}, length(given), L"length mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{46LL} == lengthv(given)), L"lengthv mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{6LL}, length3(given), L"length3 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{6LL} == length3v(given)), L"length3v mismatch", LINE_INFO());
      const v_i64_4 a{1LL, 2LL, 3LL, 4LL};
      const v_i64_4 b{4LL, -6LL, 9LL, 14LL};
      // |3| + |-8| + |6| + |10| = 27, without w: 17
      Assert::AreEqual(Int_64{27LL}, distance(a, b), L"distance mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{27LL} == distancev(a, b)), L"distancev mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{17LL}, distance3(a, b), L"distance3 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{17LL} == distance3v(a, b)), L"distance3v mismatch", LINE_INFO());
    }

    TEST_METHOD(computesMultiplyAddFamily) {
      const v_i64_4 a{1LL, 2LL, 3LL, 4LL};
      const v_i64_4 b{5LL, 6LL, -7LL, 8LL};
      const v_i64_4 c{10LL, 20LL, 30LL, 40LL};
      // a * b = {5, 12, -21, 32}
      Assert::IsTrue(allTrue(v_i64_4{15LL, 32LL, 9LL, 72LL} == multiplyAdd(a, b, c)), L"multiplyAdd", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_4{-5LL, -8LL, -51LL, -8LL} == multiplySub(a, b, c)), L"multiplySub", LINE_INFO());
      Assert::IsTrue(
          allTrue(v_i64_4{-5LL, 32LL, -51LL, 72LL} == multiplyAddSub(a, b, c)), L"multiplyAddSub", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_i64_4{15LL, -8LL, 9LL, -8LL} == multiplySubAdd(a, b, c)), L"multiplySubAdd", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_i64_4{5LL, 8LL, 51LL, 8LL} == negativeMultiplyAdd(a, b, c)), L"negativeMultiplyAdd", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_i64_4{-15LL, -32LL, -9LL, -72LL} == negativeMultiplySub(a, b, c)), L"negativeMultiplySub",
          LINE_INFO()
      );
    }

    TEST_METHOD(computesAddSubtract) {
      const v_i64_4 a{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 b{1LL, 2LL, 3LL, 4LL};
      Assert::IsTrue(allTrue(v_i64_4{9LL, 22LL, 27LL, 44LL} == addSubtract(a, b)), L"addSubtract mismatch", LINE_INFO());
    }

    TEST_METHOD(computesTruncatingModulo) {
      const v_i64_4 a{7LL, -7LL, 7LL, -10000000007LL};
      const v_i64_4 b{3LL, 3LL, -3LL, 10000000000LL};
      Assert::IsTrue(allTrue(v_i64_4{1LL, -1LL, 1LL, -7LL} == modulo(a, b)), L"modulo mismatch", LINE_INFO());
    }

    TEST_METHOD(clampsValues) {
      const v_i64_4 given{-5LL, 3LL, 15LL, 7LL};
      const v_i64_4 lower{0LL};
      const v_i64_4 upper{10LL};
      Assert::IsTrue(allTrue(v_i64_4{0LL, 3LL, 10LL, 7LL} == clamp(given, lower, upper)), L"clamp mismatch", LINE_INFO());
    }

    TEST_METHOD(findsArgMinAndArgMax) {
      Assert::AreEqual(ASizeT{1}, argmin(v_i64_4{3LL, -1LL, 4LL, 2LL}), L"argmin mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{3}, argmin(v_i64_4{3LL, 1LL, 4LL, -2LL}), L"argmin W mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{2}, argmax(v_i64_4{3LL, 1LL, 4LL, 2LL}), L"argmax mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{0}, argmax(v_i64_4{5LL, 1LL, 4LL, 2LL}), L"argmax X mismatch", LINE_INFO());
    }

    TEST_METHOD(findsArgMin3AndArgMax3IgnoringW) {
      Assert::AreEqual(ASizeT{2}, argmin3(v_i64_4{3LL, 2LL, 1LL, -9LL}), L"argmin3 mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{0}, argmin3(v_i64_4{1LL, 2LL, 3LL, -9LL}), L"argmin3 X mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{1}, argmax3(v_i64_4{1LL, 3LL, 2LL, 99LL}), L"argmax3 mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{0}, argmax3(v_i64_4{4LL, 3LL, 2LL, 99LL}), L"argmax3 X mismatch", LINE_INFO());
    }

    TEST_METHOD(picksHighestIndexOfEqualExtrema) {
      Assert::AreEqual(ASizeT{2}, argmin(v_i64_4{1LL, 5LL, 1LL, 7LL}), L"argmin tie", LINE_INFO());
      Assert::AreEqual(ASizeT{3}, argmax(v_i64_4{7LL, 5LL, 1LL, 7LL}), L"argmax tie", LINE_INFO());
      Assert::AreEqual(ASizeT{1}, argmin3(v_i64_4{1LL, 1LL, 5LL, 0LL}), L"argmin3 tie", LINE_INFO());
      Assert::AreEqual(ASizeT{2}, argmax3(v_i64_4{5LL, 1LL, 5LL, 9LL}), L"argmax3 tie", LINE_INFO());
    }
  };
}
