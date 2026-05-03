#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_8_FunctionsTest) {
  public:
    TEST_METHOD(minElementWise) {
      const v_f32_8 a(1.0f, 4.0f, 3.0f, 8.0f, 5.0f, 2.0f, 7.0f, 6.0f);
      const v_f32_8 b(2.0f, 3.0f, 4.0f, 7.0f, 6.0f, 1.0f, 8.0f, 5.0f);
      const auto r = min(a, b);
      Assert::AreEqual(1.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(3.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(1.0f, x6(r), L"x6", LINE_INFO());
    }

    TEST_METHOD(maxElementWise) {
      const v_f32_8 a(1.0f, 4.0f, 3.0f, 8.0f, 5.0f, 2.0f, 7.0f, 6.0f);
      const v_f32_8 b(2.0f, 3.0f, 4.0f, 7.0f, 6.0f, 1.0f, 8.0f, 5.0f);
      const auto r = max(a, b);
      Assert::AreEqual(2.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(4.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(2.0f, x6(r), L"x6", LINE_INFO());
    }

    TEST_METHOD(minvReducesAcrossAllLanes) {
      const v_f32_8 v(5.0f, 3.0f, 7.0f, 1.0f, 8.0f, 2.0f, 6.0f, 4.0f);
      Assert::AreEqual(1.0f, min(v), L"min of 8", LINE_INFO());
    }

    TEST_METHOD(minvBroadcastsMinimum) {
      const v_f32_8 v(5.0f, 3.0f, 7.0f, 1.0f, 8.0f, 2.0f, 6.0f, 4.0f);
      const auto r = minv(v);
      Assert::AreEqual(1.0f, x1(r), L"x1 broadcast", LINE_INFO());
      Assert::AreEqual(1.0f, x2(r), L"x2 broadcast", LINE_INFO());
      Assert::AreEqual(1.0f, x5(r), L"x5 broadcast", LINE_INFO());
      Assert::AreEqual(1.0f, x8(r), L"x8 broadcast", LINE_INFO());
    }

    TEST_METHOD(maxvReducesAcrossAllLanes) {
      const v_f32_8 v(5.0f, 3.0f, 7.0f, 1.0f, 8.0f, 2.0f, 6.0f, 4.0f);
      Assert::AreEqual(8.0f, max(v), L"max of 8", LINE_INFO());
    }

    TEST_METHOD(maxvBroadcastsMaximum) {
      const v_f32_8 v(5.0f, 3.0f, 7.0f, 1.0f, 8.0f, 2.0f, 6.0f, 4.0f);
      const auto r = maxv(v);
      Assert::AreEqual(8.0f, x1(r), L"x1 broadcast", LINE_INFO());
      Assert::AreEqual(8.0f, x2(r), L"x2 broadcast", LINE_INFO());
      Assert::AreEqual(8.0f, x5(r), L"x5 broadcast", LINE_INFO());
      Assert::AreEqual(8.0f, x8(r), L"x8 broadcast", LINE_INFO());
    }

    TEST_METHOD(horizontalSumAcrossAllLanes) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      Assert::AreEqual(36.0f, horizontalSum(v), L"sum 1..8", LINE_INFO());
    }

    TEST_METHOD(horizontalSumvBroadcastsSum) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = horizontalSumv(v);
      Assert::AreEqual(36.0f, x1(r), L"x1 broadcast", LINE_INFO());
      Assert::AreEqual(36.0f, x4(r), L"x4 broadcast", LINE_INFO());
      Assert::AreEqual(36.0f, x5(r), L"x5 broadcast", LINE_INFO());
      Assert::AreEqual(36.0f, x8(r), L"x8 broadcast", LINE_INFO());
    }

    TEST_METHOD(dotProductAcross8Lanes) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(1.0f);
      Assert::AreEqual(36.0f, dot(a, b), L"dot with ones = sum", LINE_INFO());
    }

    TEST_METHOD(dotProductSquared) {
      const v_f32_8 a(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
      Assert::AreEqual(1.0f, dot(a, a), L"unit dot", LINE_INFO());
    }

    TEST_METHOD(dotvBroadcastsDotProduct) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(1.0f);
      const auto r = dotv(a, b);
      Assert::AreEqual(36.0f, x1(r), L"x1 broadcast", LINE_INFO());
      Assert::AreEqual(36.0f, x4(r), L"x4 broadcast", LINE_INFO());
      Assert::AreEqual(36.0f, x5(r), L"x5 broadcast", LINE_INFO());
      Assert::AreEqual(36.0f, x8(r), L"x8 broadcast", LINE_INFO());
    }

    TEST_METHOD(lengthOfUnitVector) {
      const v_f32_8 v(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
      Assert::AreEqual(1.0f, length(v), 1e-6f, L"unit length", LINE_INFO());
    }

    TEST_METHOD(lengthvBroadcastsLength) {
      const v_f32_8 v(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
      const auto r = lengthv(v);
      Assert::AreEqual(1.0f, x1(r), 1e-6f, L"x1 broadcast", LINE_INFO());
      Assert::AreEqual(1.0f, x4(r), 1e-6f, L"x4 broadcast", LINE_INFO());
      Assert::AreEqual(1.0f, x5(r), 1e-6f, L"x5 broadcast", LINE_INFO());
      Assert::AreEqual(1.0f, x8(r), 1e-6f, L"x8 broadcast", LINE_INFO());
    }

    TEST_METHOD(rlengthvBroadcastsReciprocalLength) {
      const v_f32_8 v(2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
      const auto r = rlengthv(v);
      Assert::AreEqual(0.5f, x1(r), 1e-4f, L"x1 broadcast", LINE_INFO());
      Assert::AreEqual(0.5f, x4(r), 1e-4f, L"x4 broadcast", LINE_INFO());
      Assert::AreEqual(0.5f, x5(r), 1e-4f, L"x5 broadcast", LINE_INFO());
      Assert::AreEqual(0.5f, x8(r), 1e-4f, L"x8 broadcast", LINE_INFO());
    }

    TEST_METHOD(normalizeProducesUnitLength) {
      const v_f32_8 v(3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
      const auto n = normalize(v);
      Assert::AreEqual(1.0f, length(n), 1e-4f, L"normalized length", LINE_INFO());
    }

    TEST_METHOD(distanceBetweenSameIsZero) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      Assert::AreEqual(0.0f, distance(v, v), 1e-6f, L"self distance", LINE_INFO());
    }

    TEST_METHOD(mixAtZeroReturnsA) {
      const v_f32_8 a(1.0f);
      const v_f32_8 b(2.0f);
      const auto r = mix(a, b, 0.0f);
      Assert::AreEqual(1.0f, x1(r), 1e-6f, L"mix 0", LINE_INFO());
    }

    TEST_METHOD(mixAtOneReturnsB) {
      const v_f32_8 a(1.0f);
      const v_f32_8 b(2.0f);
      const auto r = mix(a, b, 1.0f);
      Assert::AreEqual(2.0f, x1(r), 1e-6f, L"mix 1", LINE_INFO());
    }

    TEST_METHOD(absRemovesSign) {
      const v_f32_8 v(-1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f, -7.0f, 8.0f);
      const auto r = abs(v);
      Assert::AreEqual(1.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(3.0f, x3(r), L"x3", LINE_INFO());
      Assert::AreEqual(5.0f, x5(r), L"x5", LINE_INFO());
      Assert::AreEqual(7.0f, x7(r), L"x7", LINE_INFO());
    }

    TEST_METHOD(sqrtComputes) {
      const v_f32_8 v(4.0f, 9.0f, 16.0f, 25.0f, 36.0f, 49.0f, 64.0f, 81.0f);
      const auto r = sqrt(v);
      Assert::AreEqual(2.0f, x1(r), 1e-6f, L"x1", LINE_INFO());
      Assert::AreEqual(9.0f, x8(r), 1e-6f, L"x8", LINE_INFO());
    }

    TEST_METHOD(reciprocalComputesInverse) {
      const v_f32_8 v(2.0f, 4.0f, 5.0f, 10.0f, 8.0f, 20.0f, 25.0f, 50.0f);
      const auto r = reciprocal(v);
      Assert::AreEqual(0.5f, x1(r), 1e-3f, L"1/2", LINE_INFO());
      Assert::AreEqual(0.25f, x2(r), 1e-3f, L"1/4", LINE_INFO());
      Assert::AreEqual(0.2f, x3(r), 1e-3f, L"1/5", LINE_INFO());
      Assert::AreEqual(0.1f, x4(r), 1e-3f, L"1/10", LINE_INFO());
      Assert::AreEqual(0.125f, x5(r), 1e-3f, L"1/8", LINE_INFO());
      Assert::AreEqual(0.02f, x8(r), 1e-3f, L"1/50", LINE_INFO());
    }

    TEST_METHOD(rsqrtComputesInverseSqrt) {
      const v_f32_8 v(4.0f, 9.0f, 16.0f, 25.0f, 1.0f, 100.0f, 64.0f, 49.0f);
      const auto r = rsqrt(v);
      Assert::AreEqual(0.5f, x1(r), 1e-3f, L"1/sqrt(4)", LINE_INFO());
      Assert::AreEqual(1.0f / 3.0f, x2(r), 1e-3f, L"1/sqrt(9)", LINE_INFO());
      Assert::AreEqual(0.25f, x3(r), 1e-3f, L"1/sqrt(16)", LINE_INFO());
      Assert::AreEqual(0.2f, x4(r), 1e-3f, L"1/sqrt(25)", LINE_INFO());
      Assert::AreEqual(1.0f, x5(r), 1e-3f, L"1/sqrt(1)", LINE_INFO());
      Assert::AreEqual(0.1f, x6(r), 1e-3f, L"1/sqrt(100)", LINE_INFO());
    }

    TEST_METHOD(roundRoundsToNearest) {
      const v_f32_8 v(1.4f, 1.5f, 2.5f, -1.5f, 3.7f, -3.7f, 0.1f, -0.1f);
      const auto r = round(v);
      Assert::AreEqual(1.0f, x1(r), L"1.4 -> 1", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"1.5 -> 2", LINE_INFO());
      Assert::AreEqual(2.0f, x3(r), L"2.5 -> 2", LINE_INFO());
      Assert::AreEqual(-2.0f, x4(r), L"-1.5 -> -2", LINE_INFO());
      Assert::AreEqual(4.0f, x5(r), L"3.7 -> 4", LINE_INFO());
      Assert::AreEqual(-4.0f, x6(r), L"-3.7 -> -4", LINE_INFO());
      Assert::AreEqual(0.0f, x7(r), L"0.1 -> 0", LINE_INFO());
      Assert::AreEqual(0.0f, x8(r), L"-0.1 -> 0", LINE_INFO());
    }

    TEST_METHOD(fractExtractsFractionalPart) {
      const v_f32_8 v(1.5f, 2.7f, -1.3f, 4.0f, 0.25f, 3.9f, -2.8f, 5.5f);
      const auto r = fract(v);
      Assert::AreEqual(0.5f, x1(r), 1e-5f, L"fract(1.5)", LINE_INFO());
      Assert::AreEqual(0.7f, x2(r), 1e-5f, L"fract(2.7)", LINE_INFO());
      Assert::AreEqual(0.7f, x3(r), 1e-5f, L"fract(-1.3)", LINE_INFO());
      Assert::AreEqual(0.0f, x4(r), 1e-5f, L"fract(4.0)", LINE_INFO());
      Assert::AreEqual(0.25f, x5(r), 1e-5f, L"fract(0.25)", LINE_INFO());
      Assert::AreEqual(0.5f, x8(r), 1e-5f, L"fract(5.5)", LINE_INFO());
    }

    TEST_METHOD(moduloComputesFloatRemainder) {
      const v_f32_8 a(5.0f, 7.0f, 10.0f, 3.5f, 9.0f, 11.0f, 6.5f, 15.0f);
      const v_f32_8 b(3.0f, 4.0f, 3.0f, 2.0f, 4.0f, 5.0f, 3.0f, 7.0f);
      const auto r = modulo(a, b);
      Assert::AreEqual(2.0f, x1(r), 1e-5f, L"5 mod 3", LINE_INFO());
      Assert::AreEqual(3.0f, x2(r), 1e-5f, L"7 mod 4", LINE_INFO());
      Assert::AreEqual(1.0f, x3(r), 1e-5f, L"10 mod 3", LINE_INFO());
      Assert::AreEqual(1.5f, x4(r), 1e-5f, L"3.5 mod 2", LINE_INFO());
      Assert::AreEqual(1.0f, x5(r), 1e-5f, L"9 mod 4", LINE_INFO());
      Assert::AreEqual(1.0f, x6(r), 1e-5f, L"11 mod 5", LINE_INFO());
      Assert::AreEqual(0.5f, x7(r), 1e-5f, L"6.5 mod 3", LINE_INFO());
      Assert::AreEqual(1.0f, x8(r), 1e-5f, L"15 mod 7", LINE_INFO());
    }

    TEST_METHOD(ceilRoundsUp) {
      const v_f32_8 v(1.1f, 2.9f, -1.1f, -2.9f, 3.5f, 4.0f, -0.1f, 0.1f);
      const auto r = ceil(v);
      Assert::AreEqual(2.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(3.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(-1.0f, x3(r), L"x3", LINE_INFO());
      Assert::AreEqual(4.0f, x5(r), L"x5", LINE_INFO());
    }

    TEST_METHOD(floorRoundsDown) {
      const v_f32_8 v(1.1f, 2.9f, -1.1f, -2.9f, 3.5f, 4.0f, -0.1f, 0.1f);
      const auto r = floor(v);
      Assert::AreEqual(1.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(-2.0f, x3(r), L"x3", LINE_INFO());
      Assert::AreEqual(3.0f, x5(r), L"x5", LINE_INFO());
    }

    TEST_METHOD(clampClampsValues) {
      const v_f32_8 v(-1.0f, 0.5f, 2.0f, 0.0f, -1.0f, 0.5f, 2.0f, 0.0f);
      const auto r = clamp(v, Zero<v_f32_8>(), One<v_f32_8>());
      Assert::AreEqual(0.0f, x1(r), L"clamped low", LINE_INFO());
      Assert::AreEqual(0.5f, x2(r), L"unchanged", LINE_INFO());
      Assert::AreEqual(1.0f, x3(r), L"clamped high", LINE_INFO());
    }

    TEST_METHOD(isNegativeReturnsBitmask) {
      const v_f32_8 v(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
      const auto mask = isNegative(v);
      Assert::AreEqual(static_cast<Int_32>(0b01010101), mask, L"alternating", LINE_INFO());
    }

    TEST_METHOD(isNaNDetectsNaN) {
      const v_f32_8 v(1.0f, std::numeric_limits<float>::quiet_NaN(), 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = isNaN(v);
      Assert::AreEqual(0, x1(r), L"x1 not NaN", LINE_INFO());
      Assert::AreNotEqual(0, x2(r), L"x2 is NaN", LINE_INFO());
    }

    TEST_METHOD(multiplyAddComputes) {
      const v_f32_8 a(2.0f);
      const v_f32_8 b(3.0f);
      const v_f32_8 c(1.0f);
      const auto r = multiplyAdd(a, b, c);
      Assert::AreEqual(7.0f, x1(r), 1e-6f, L"2*3+1", LINE_INFO());
    }

    TEST_METHOD(multiplySubComputesATimesBMinusC) {
      const v_f32_8 a(2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
      const v_f32_8 b(3.0f);
      const v_f32_8 c(1.0f);
      const auto r = multiplySub(a, b, c);
      // a*b - c
      Assert::AreEqual(5.0f, x1(r), 1e-6f, L"2*3-1=5", LINE_INFO());
      Assert::AreEqual(8.0f, x2(r), 1e-6f, L"3*3-1=8", LINE_INFO());
      Assert::AreEqual(11.0f, x3(r), 1e-6f, L"4*3-1=11", LINE_INFO());
      Assert::AreEqual(26.0f, x8(r), 1e-6f, L"9*3-1=26", LINE_INFO());
    }

    TEST_METHOD(multiplyAddSubAlternatesSubAdd) {
      const v_f32_8 a(2.0f);
      const v_f32_8 b(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 c(0.5f);
      const auto r = multiplyAddSub(a, b, c);
      // even-indexed(0-based) = sub, odd-indexed(0-based) = add
      // lane1(idx0): 2*1 - 0.5 = 1.5
      // lane2(idx1): 2*2 + 0.5 = 4.5
      // lane3(idx2): 2*3 - 0.5 = 5.5
      // lane4(idx3): 2*4 + 0.5 = 8.5
      Assert::AreEqual(1.5f, x1(r), 1e-6f, L"x1: 2*1-0.5", LINE_INFO());
      Assert::AreEqual(4.5f, x2(r), 1e-6f, L"x2: 2*2+0.5", LINE_INFO());
      Assert::AreEqual(5.5f, x3(r), 1e-6f, L"x3: 2*3-0.5", LINE_INFO());
      Assert::AreEqual(8.5f, x4(r), 1e-6f, L"x4: 2*4+0.5", LINE_INFO());
      Assert::AreEqual(9.5f, x5(r), 1e-6f, L"x5: 2*5-0.5", LINE_INFO());
      Assert::AreEqual(12.5f, x6(r), 1e-6f, L"x6: 2*6+0.5", LINE_INFO());
      Assert::AreEqual(13.5f, x7(r), 1e-6f, L"x7: 2*7-0.5", LINE_INFO());
      Assert::AreEqual(16.5f, x8(r), 1e-6f, L"x8: 2*8+0.5", LINE_INFO());
    }

    TEST_METHOD(multiplySubAddAlternatesAddSub) {
      const v_f32_8 a(2.0f);
      const v_f32_8 b(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 c(0.5f);
      const auto r = multiplySubAdd(a, b, c);
      // even-indexed(0-based) = add, odd-indexed(0-based) = sub
      // lane1(idx0): 2*1 + 0.5 = 2.5
      // lane2(idx1): 2*2 - 0.5 = 3.5
      // lane3(idx2): 2*3 + 0.5 = 6.5
      // lane4(idx3): 2*4 - 0.5 = 7.5
      Assert::AreEqual(2.5f, x1(r), 1e-6f, L"x1: 2*1+0.5", LINE_INFO());
      Assert::AreEqual(3.5f, x2(r), 1e-6f, L"x2: 2*2-0.5", LINE_INFO());
      Assert::AreEqual(6.5f, x3(r), 1e-6f, L"x3: 2*3+0.5", LINE_INFO());
      Assert::AreEqual(7.5f, x4(r), 1e-6f, L"x4: 2*4-0.5", LINE_INFO());
      Assert::AreEqual(10.5f, x5(r), 1e-6f, L"x5: 2*5+0.5", LINE_INFO());
      Assert::AreEqual(11.5f, x6(r), 1e-6f, L"x6: 2*6-0.5", LINE_INFO());
      Assert::AreEqual(14.5f, x7(r), 1e-6f, L"x7: 2*7+0.5", LINE_INFO());
      Assert::AreEqual(15.5f, x8(r), 1e-6f, L"x8: 2*8-0.5", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplyAddComputesCMinusATimesB) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(2.0f);
      const v_f32_8 c(10.0f);
      const auto r = negativeMultiplyAdd(a, b, c);
      // -(a*b) + c = c - a*b
      Assert::AreEqual(8.0f, x1(r), 1e-6f, L"10-1*2=8", LINE_INFO());
      Assert::AreEqual(6.0f, x2(r), 1e-6f, L"10-2*2=6", LINE_INFO());
      Assert::AreEqual(4.0f, x3(r), 1e-6f, L"10-3*2=4", LINE_INFO());
      Assert::AreEqual(2.0f, x4(r), 1e-6f, L"10-4*2=2", LINE_INFO());
      Assert::AreEqual(0.0f, x5(r), 1e-6f, L"10-5*2=0", LINE_INFO());
      Assert::AreEqual(-2.0f, x6(r), 1e-6f, L"10-6*2=-2", LINE_INFO());
      Assert::AreEqual(-4.0f, x7(r), 1e-6f, L"10-7*2=-4", LINE_INFO());
      Assert::AreEqual(-6.0f, x8(r), 1e-6f, L"10-8*2=-6", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplySubComputesNegATimesBMinusC) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(2.0f);
      const v_f32_8 c(10.0f);
      const auto r = negativeMultiplySub(a, b, c);
      // -(a*b) - c
      Assert::AreEqual(-12.0f, x1(r), 1e-6f, L"-(1*2)-10=-12", LINE_INFO());
      Assert::AreEqual(-14.0f, x2(r), 1e-6f, L"-(2*2)-10=-14", LINE_INFO());
      Assert::AreEqual(-16.0f, x3(r), 1e-6f, L"-(3*2)-10=-16", LINE_INFO());
      Assert::AreEqual(-18.0f, x4(r), 1e-6f, L"-(4*2)-10=-18", LINE_INFO());
      Assert::AreEqual(-20.0f, x5(r), 1e-6f, L"-(5*2)-10=-20", LINE_INFO());
      Assert::AreEqual(-22.0f, x6(r), 1e-6f, L"-(6*2)-10=-22", LINE_INFO());
      Assert::AreEqual(-24.0f, x7(r), 1e-6f, L"-(7*2)-10=-24", LINE_INFO());
      Assert::AreEqual(-26.0f, x8(r), 1e-6f, L"-(8*2)-10=-26", LINE_INFO());
    }

    TEST_METHOD(addSubtractAlternatesSubAdd) {
      const v_f32_8 a(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      const v_f32_8 b(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = addSubtract(a, b);
      // even-indexed(0-based) = sub, odd-indexed(0-based) = add
      // lane1(idx0): 10-1=9
      // lane2(idx1): 20+2=22
      // lane3(idx2): 30-3=27
      // lane4(idx3): 40+4=44
      Assert::AreEqual(9.0f, x1(r), L"x1: 10-1", LINE_INFO());
      Assert::AreEqual(22.0f, x2(r), L"x2: 20+2", LINE_INFO());
      Assert::AreEqual(27.0f, x3(r), L"x3: 30-3", LINE_INFO());
      Assert::AreEqual(44.0f, x4(r), L"x4: 40+4", LINE_INFO());
      Assert::AreEqual(45.0f, x5(r), L"x5: 50-5", LINE_INFO());
      Assert::AreEqual(66.0f, x6(r), L"x6: 60+6", LINE_INFO());
      Assert::AreEqual(63.0f, x7(r), L"x7: 70-7", LINE_INFO());
      Assert::AreEqual(88.0f, x8(r), L"x8: 80+8", LINE_INFO());
    }

    TEST_METHOD(anyTrueDetectsAnyNonZero) {
      const v_f32_8 v(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
      Assert::IsTrue(anyTrue(v != Zero<v_f32_8>()), L"one non-zero", LINE_INFO());
    }

    TEST_METHOD(anyFalseDetectsAnyZero) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 9.0f);
      const auto mask = a == b;
      // lanes 1-7 are true, lane 8 is false
      Assert::IsTrue(anyFalse(mask), L"one lane differs", LINE_INFO());
    }

    TEST_METHOD(anyFalseReturnsFalseWhenAllTrue) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto mask = a == a;
      Assert::IsFalse(anyFalse(mask), L"all same, no false", LINE_INFO());
    }

    TEST_METHOD(allTrueRequiresAll) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      Assert::IsTrue(allTrue(a == a), L"self equality", LINE_INFO());
    }

    TEST_METHOD(allFalseWhenZero) {
      Assert::IsTrue(allFalse(Zero<v_f32_8>() != Zero<v_f32_8>()), L"zero != zero is all false", LINE_INFO());
    }

    TEST_METHOD(reciprocalApproximation) {
      const v_f32_8 v(2.0f, 4.0f, 5.0f, 8.0f, 10.0f, 16.0f, 20.0f, 32.0f);
      const auto r = reciprocal(v);
      Assert::AreEqual(0.5f, x1(r), 1e-3f, L"1/2", LINE_INFO());
      Assert::AreEqual(0.25f, x2(r), 1e-3f, L"1/4", LINE_INFO());
      Assert::AreEqual(0.03125f, x8(r), 1e-4f, L"1/32", LINE_INFO());
    }

    TEST_METHOD(rsqrtApproximation) {
      const v_f32_8 v(1.0f, 4.0f, 9.0f, 16.0f, 25.0f, 36.0f, 49.0f, 64.0f);
      const auto r = rsqrt(v);
      Assert::AreEqual(1.0f, x1(r), 1e-3f, L"1/sqrt(1)", LINE_INFO());
      Assert::AreEqual(0.5f, x2(r), 1e-3f, L"1/sqrt(4)", LINE_INFO());
      Assert::AreEqual(0.125f, x8(r), 1e-3f, L"1/sqrt(64)", LINE_INFO());
    }

    TEST_METHOD(isNegativeReturnsBitmaskForAllLanes) {
      const v_f32_8 v(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
      Assert::AreEqual(static_cast<Int_32>(0b01010101), isNegative(v), L"alternating", LINE_INFO());
      const v_f32_8 allNeg(-1.0f);
      Assert::AreEqual(static_cast<Int_32>(0xFF), isNegative(allNeg), L"all negative", LINE_INFO());
      Assert::AreEqual(static_cast<Int_32>(0), isNegative(One<v_f32_8>()), L"none negative", LINE_INFO());
    }

    TEST_METHOD(mixWithScalarFactor) {
      const v_f32_8 a(0.0f);
      const v_f32_8 b(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      const auto r = mix(a, b, 0.5f);
      Assert::AreEqual(5.0f, x1(r), 1e-5f, L"x1", LINE_INFO());
      Assert::AreEqual(40.0f, x8(r), 1e-5f, L"x8", LINE_INFO());
    }

    TEST_METHOD(multiplyAddPerLaneVariation) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
      const v_f32_8 c(10.0f);
      const auto r = multiplyAdd(a, b, c);
      Assert::AreEqual(12.0f, x1(r), 1e-5f, L"1*2+10", LINE_INFO());
      Assert::AreEqual(16.0f, x2(r), 1e-5f, L"2*3+10", LINE_INFO());
      Assert::AreEqual(82.0f, x8(r), 1e-5f, L"8*9+10", LINE_INFO());
    }
  };
}
