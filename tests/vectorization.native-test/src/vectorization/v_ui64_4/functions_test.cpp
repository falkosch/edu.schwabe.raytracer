#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui64_4_FunctionsTest) {
  public:
    TEST_METHOD(computesAndnot) {
      const v_ui64_4 a{std::numeric_limits<UInt_64>::max()};
      const v_ui64_4 b{10ULL, 20ULL, 30ULL, 40ULL};
      const auto actual = andnot(a, b);
      Assert::IsTrue(allTrue(Zero<v_ui64_4>() == actual), L"andnot(allBits, b) should be zero", LINE_INFO());

      const v_ui64_4 allBits{std::numeric_limits<UInt_64>::max()};
      const auto actual2 = andnot(Zero<v_ui64_4>(), allBits);
      Assert::IsTrue(allTrue(allBits == actual2), L"andnot(zero, allBits) should be allBits", LINE_INFO());
    }

    TEST_METHOD(computesMinBasic) {
      const v_ui64_4 a{10ULL, 50ULL, 30ULL, 40ULL};
      const v_ui64_4 b{20ULL, 5ULL, 60ULL, 40ULL};
      const v_ui64_4 expected{10ULL, 5ULL, 30ULL, 40ULL};
      const auto actual = min(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"min value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesMinUnsignedSemantics) {
      const v_ui64_4 a{std::numeric_limits<UInt_64>::max(), 0ULL, 100ULL, 1ULL};
      const v_ui64_4 b{0ULL, std::numeric_limits<UInt_64>::max(), 50ULL, 2ULL};
      const v_ui64_4 expected{0ULL, 0ULL, 50ULL, 1ULL};
      const auto actual = min(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"min unsigned semantics mismatch", LINE_INFO());
    }

    TEST_METHOD(computesMaxBasic) {
      const v_ui64_4 a{10ULL, 50ULL, 30ULL, 40ULL};
      const v_ui64_4 b{20ULL, 5ULL, 60ULL, 40ULL};
      const v_ui64_4 expected{20ULL, 50ULL, 60ULL, 40ULL};
      const auto actual = max(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"max value mismatch", LINE_INFO());
    }

    TEST_METHOD(computesMaxUnsignedSemantics) {
      const v_ui64_4 a{std::numeric_limits<UInt_64>::max(), 0ULL, 100ULL, 1ULL};
      const v_ui64_4 b{0ULL, std::numeric_limits<UInt_64>::max(), 50ULL, 2ULL};
      const v_ui64_4 expected{std::numeric_limits<UInt_64>::max(), std::numeric_limits<UInt_64>::max(), 100ULL, 2ULL};
      const auto actual = max(a, b);
      Assert::IsTrue(allTrue(expected == actual), L"max unsigned semantics mismatch", LINE_INFO());
    }

    TEST_METHOD(checksAnyTrue) {
      Assert::IsTrue(anyTrue(v_ui64_4{1ULL, 0ULL, 0ULL, 0ULL}), L"anyTrue should be true with X nonzero", LINE_INFO());
      Assert::IsTrue(anyTrue(v_ui64_4{0ULL, 0ULL, 0ULL, 1ULL}), L"anyTrue should be true with W nonzero", LINE_INFO());
      Assert::IsFalse(anyTrue(Zero<v_ui64_4>()), L"anyTrue should be false for all zero", LINE_INFO());
    }

    TEST_METHOD(checksAllTrue) {
      const v_ui64_4 allBits{std::numeric_limits<UInt_64>::max()};
      Assert::IsTrue(allTrue(allBits), L"allTrue should be true for all-bits-set", LINE_INFO());
      const v_ui64_4 withOneZero{std::numeric_limits<UInt_64>::max(), 0ULL, std::numeric_limits<UInt_64>::max(), std::numeric_limits<UInt_64>::max()};
      Assert::IsFalse(allTrue(withOneZero), L"allTrue should be false with one zero", LINE_INFO());
    }

    TEST_METHOD(checksAnyFalse) {
      const v_ui64_4 withOneZero{std::numeric_limits<UInt_64>::max(), 0ULL, std::numeric_limits<UInt_64>::max(), std::numeric_limits<UInt_64>::max()};
      Assert::IsTrue(anyFalse(withOneZero), L"anyFalse should be true with one zero", LINE_INFO());
      const v_ui64_4 allBits{std::numeric_limits<UInt_64>::max()};
      Assert::IsFalse(anyFalse(allBits), L"anyFalse should be false for all-bits-set", LINE_INFO());
    }

    TEST_METHOD(checksAllFalse) {
      Assert::IsTrue(allFalse(Zero<v_ui64_4>()), L"allFalse should be true for all zero", LINE_INFO());
      Assert::IsFalse(allFalse(v_ui64_4{0ULL, 1ULL, 0ULL, 0ULL}), L"allFalse should be false with one nonzero", LINE_INFO());
    }

    TEST_METHOD(testsTruthBitwise) {
      // Components with only some bits set are neither fully true nor false.
      const v_ui64_4 partial{1ULL};
      Assert::IsTrue(anyTrue(partial), L"anyTrue of partial bits", LINE_INFO());
      Assert::IsTrue(anyFalse(partial), L"anyFalse of partial bits", LINE_INFO());
      Assert::IsFalse(allTrue(partial), L"allTrue of partial bits", LINE_INFO());
      Assert::IsFalse(allFalse(partial), L"allFalse of partial bits", LINE_INFO());
    }

    TEST_METHOD(checksTruth3IgnoringW) {
      const auto ones = std::numeric_limits<UInt_64>::max();
      Assert::IsTrue(anyTrue3(v_ui64_4{0ULL, 0ULL, 1ULL, 0ULL}), L"anyTrue3 with Z set", LINE_INFO());
      Assert::IsFalse(anyTrue3(v_ui64_4{0ULL, 0ULL, 0ULL, ones}), L"anyTrue3 ignores W", LINE_INFO());
      Assert::IsTrue(anyFalse3(v_ui64_4{ones, 0ULL, ones, ones}), L"anyFalse3 with Y clear", LINE_INFO());
      Assert::IsFalse(anyFalse3(v_ui64_4{ones, ones, ones, 0ULL}), L"anyFalse3 ignores W", LINE_INFO());
      Assert::IsTrue(allTrue3(v_ui64_4{ones, ones, ones, 0ULL}), L"allTrue3 ignores W", LINE_INFO());
      Assert::IsFalse(allTrue3(v_ui64_4{ones, 1ULL, ones, ones}), L"allTrue3 with partial Y", LINE_INFO());
      Assert::IsTrue(allFalse3(v_ui64_4{0ULL, 0ULL, 0ULL, ones}), L"allFalse3 ignores W", LINE_INFO());
      Assert::IsFalse(allFalse3(v_ui64_4{0ULL, 1ULL, 0ULL, 0ULL}), L"allFalse3 with Y set", LINE_INFO());
    }

    TEST_METHOD(setsAndClearsW) {
      const v_ui64_4 given{1ULL, 2ULL, 3ULL, 4ULL};
      Assert::IsTrue(allTrue(v_ui64_4{1ULL, 2ULL, 3ULL, 0ULL} == zeroW(given)), L"zeroW mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{1ULL, 2ULL, 3ULL, 1ULL} == oneW(given)), L"oneW mismatch", LINE_INFO());
    }

    TEST_METHOD(computesSqr) {
      const v_ui64_4 given{3000000000ULL, 2ULL, 3ULL, 0ULL};
      Assert::IsTrue(allTrue(v_ui64_4{9000000000000000000ULL, 4ULL, 9ULL, 0ULL} == sqr(given)), L"sqr mismatch", LINE_INFO());
    }

    TEST_METHOD(computesHorizontalMinAndMax) {
      // above INT64_MAX: compared unsigned
      const auto large = std::numeric_limits<UInt_64>::max();
      const v_ui64_4 given{3ULL, large, 4ULL, 1ULL};
      Assert::AreEqual(UInt_64{1}, min(given), L"min mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{1ULL} == minv(given)), L"minv mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{3}, min3(given), L"min3 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{3ULL} == min3v(given)), L"min3v mismatch", LINE_INFO());
      const v_ui64_4 high{3ULL, 1ULL, 4ULL, large};
      Assert::AreEqual(large, max(high), L"max mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{large} == maxv(high)), L"maxv mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{4}, max3(high), L"max3 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{4ULL} == max3v(high)), L"max3v mismatch", LINE_INFO());
    }

    TEST_METHOD(computesHorizontalSumAndDot) {
      const v_ui64_4 a{1ULL, 2ULL, 3000000000ULL, 40ULL};
      Assert::AreEqual(UInt_64{3000000043}, horizontalSum(a), L"horizontalSum mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{3000000043ULL} == horizontalSumv(a)), L"horizontalSumv mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{3000000003}, horizontalSum3(a), L"horizontalSum3 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{3000000003ULL} == horizontalSum3v(a)), L"horizontalSum3v mismatch", LINE_INFO());
      const v_ui64_4 b{5ULL, 6ULL, 2ULL, 10ULL};
      // 5 + 12 + 6000000000 + 400
      Assert::AreEqual(UInt_64{6000000417}, dot(a, b), L"dot mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{6000000417ULL} == dotv(a, b)), L"dotv mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{6000000017}, dot3(a, b), L"dot3 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{6000000017ULL} == dot3v(a, b)), L"dot3v mismatch", LINE_INFO());
    }

    TEST_METHOD(computesManhattanLengthAndDistance) {
      const v_ui64_4 given{1ULL, 2ULL, 3ULL, 40ULL};
      Assert::AreEqual(UInt_64{46}, length(given), L"length mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{46ULL} == lengthv(given)), L"lengthv mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{6}, length3(given), L"length3 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{6ULL} == length3v(given)), L"length3v mismatch", LINE_INFO());
      // components of b above those of a must not wrap around
      const v_ui64_4 a{1ULL, 20ULL, 3ULL, 40ULL};
      const v_ui64_4 b{10ULL, 2ULL, 30ULL, 4ULL};
      // 9 + 18 + 27 + 36 = 90, without w: 54
      Assert::AreEqual(UInt_64{90}, distance(a, b), L"distance mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{90}, distance(b, a), L"distance is symmetric", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{90ULL} == distancev(a, b)), L"distancev mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{54}, distance3(a, b), L"distance3 mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{54ULL} == distance3v(a, b)), L"distance3v mismatch", LINE_INFO());
    }

    TEST_METHOD(computesMultiplyAddFamily) {
      const v_ui64_4 a{1ULL, 2ULL, 3ULL, 4ULL};
      const v_ui64_4 b{50ULL, 60ULL, 70ULL, 80ULL};
      const v_ui64_4 c{10ULL, 20ULL, 30ULL, 40ULL};
      // a * b = {50, 120, 210, 320}
      Assert::IsTrue(allTrue(v_ui64_4{60ULL, 140ULL, 240ULL, 360ULL} == multiplyAdd(a, b, c)), L"multiplyAdd", LINE_INFO());
      Assert::IsTrue(allTrue(v_ui64_4{40ULL, 100ULL, 180ULL, 280ULL} == multiplySub(a, b, c)), L"multiplySub", LINE_INFO());
      Assert::IsTrue(
          allTrue(v_ui64_4{40ULL, 140ULL, 180ULL, 360ULL} == multiplyAddSub(a, b, c)), L"multiplyAddSub", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_ui64_4{60ULL, 100ULL, 240ULL, 280ULL} == multiplySubAdd(a, b, c)), L"multiplySubAdd", LINE_INFO()
      );
      const v_ui64_4 large{1000ULL};
      Assert::IsTrue(
          allTrue(v_ui64_4{950ULL, 880ULL, 790ULL, 680ULL} == negativeMultiplyAdd(a, b, large)), L"negativeMultiplyAdd",
          LINE_INFO()
      );
      // -(a * b) - c wraps around
      Assert::IsTrue(
          allTrue(v_ui64_4{UInt_64(-60), UInt_64(-140), UInt_64(-240), UInt_64(-360)} == negativeMultiplySub(a, b, c)),
          L"negativeMultiplySub", LINE_INFO()
      );
    }

    TEST_METHOD(computesAddSubtract) {
      const v_ui64_4 a{10ULL, 20ULL, 30ULL, 40ULL};
      const v_ui64_4 b{1ULL, 2ULL, 3ULL, 4ULL};
      Assert::IsTrue(allTrue(v_ui64_4{9ULL, 22ULL, 27ULL, 44ULL} == addSubtract(a, b)), L"addSubtract mismatch", LINE_INFO());
    }

    TEST_METHOD(computesModulo) {
      const v_ui64_4 a{7ULL, 8ULL, 9ULL, std::numeric_limits<UInt_64>::max()};
      const v_ui64_4 b{3ULL, 3ULL, 4ULL, 0x8000000000000000ULL};
      const v_ui64_4 expected{1ULL, 2ULL, 1ULL, 0x7FFFFFFFFFFFFFFFULL};
      Assert::IsTrue(allTrue(expected == modulo(a, b)), L"modulo mismatch", LINE_INFO());
    }

    TEST_METHOD(clampsValues) {
      const v_ui64_4 given{1ULL, 3ULL, 15ULL, 7ULL};
      const v_ui64_4 lower{2ULL};
      const v_ui64_4 upper{10ULL};
      Assert::IsTrue(allTrue(v_ui64_4{2ULL, 3ULL, 10ULL, 7ULL} == clamp(given, lower, upper)), L"clamp mismatch", LINE_INFO());
    }

    TEST_METHOD(findsArgMinAndArgMax) {
      const auto large = std::numeric_limits<UInt_64>::max();
      Assert::AreEqual(ASizeT{1}, argmin(v_ui64_4{3ULL, 0ULL, 4ULL, 2ULL}), L"argmin mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{3}, argmin(v_ui64_4{3ULL, 1ULL, 4ULL, 0ULL}), L"argmin W mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{2}, argmax(v_ui64_4{3ULL, 1ULL, large, 2ULL}), L"argmax mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{0}, argmax(v_ui64_4{5ULL, 1ULL, 4ULL, 2ULL}), L"argmax X mismatch", LINE_INFO());
    }

    TEST_METHOD(findsArgMin3AndArgMax3IgnoringW) {
      Assert::AreEqual(ASizeT{2}, argmin3(v_ui64_4{3ULL, 2ULL, 1ULL, 0ULL}), L"argmin3 mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{0}, argmin3(v_ui64_4{1ULL, 2ULL, 3ULL, 0ULL}), L"argmin3 X mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{1}, argmax3(v_ui64_4{1ULL, 3ULL, 2ULL, 99ULL}), L"argmax3 mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{0}, argmax3(v_ui64_4{4ULL, 3ULL, 2ULL, 99ULL}), L"argmax3 X mismatch", LINE_INFO());
    }

    TEST_METHOD(picksHighestIndexOfEqualExtrema) {
      Assert::AreEqual(ASizeT{2}, argmin(v_ui64_4{1ULL, 5ULL, 1ULL, 7ULL}), L"argmin tie", LINE_INFO());
      Assert::AreEqual(ASizeT{3}, argmax(v_ui64_4{7ULL, 5ULL, 1ULL, 7ULL}), L"argmax tie", LINE_INFO());
      Assert::AreEqual(ASizeT{1}, argmin3(v_ui64_4{1ULL, 1ULL, 5ULL, 0ULL}), L"argmin3 tie", LINE_INFO());
      Assert::AreEqual(ASizeT{2}, argmax3(v_ui64_4{5ULL, 1ULL, 5ULL, 9ULL}), L"argmax3 tie", LINE_INFO());
    }
  };
}
