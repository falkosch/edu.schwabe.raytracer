#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4_FunctionsTest) {
  public:
    TEST_METHOD(computesMinComponentWiseAndScalarAndMin3) {
      const v_f32_4 a{1.0f, 4.0f, 2.0f, 5.0f};
      const v_f32_4 b{3.0f, 2.0f, 6.0f, 1.0f};

      // component-wise min(a, b)
      const auto cw = min(a, b);
      Assert::AreEqual(1.0f, x(cw), L"min(a,b) X mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, y(cw), L"min(a,b) Y mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, z(cw), L"min(a,b) Z mismatch", LINE_INFO());
      Assert::AreEqual(1.0f, w(cw), L"min(a,b) W mismatch", LINE_INFO());

      // scalar min over all 4 components
      const v_f32_4 v{3.0f, 1.0f, 5.0f, 2.0f};
      Assert::AreEqual(1.0f, min(v), L"min(v) mismatch", LINE_INFO());

      // scalar min over first 3 components (ignores W)
      const v_f32_4 v2{4.0f, 2.0f, 3.0f, 0.0f};
      Assert::AreEqual(2.0f, min3(v2), L"min3(v) mismatch", LINE_INFO());
    }

    TEST_METHOD(computesMaxComponentWiseAndScalarAndMax3) {
      const v_f32_4 a{1.0f, 4.0f, 2.0f, 5.0f};
      const v_f32_4 b{3.0f, 2.0f, 6.0f, 1.0f};

      // component-wise max(a, b)
      const auto cw = max(a, b);
      Assert::AreEqual(3.0f, x(cw), L"max(a,b) X mismatch", LINE_INFO());
      Assert::AreEqual(4.0f, y(cw), L"max(a,b) Y mismatch", LINE_INFO());
      Assert::AreEqual(6.0f, z(cw), L"max(a,b) Z mismatch", LINE_INFO());
      Assert::AreEqual(5.0f, w(cw), L"max(a,b) W mismatch", LINE_INFO());

      // scalar max over all 4 components
      const v_f32_4 v{3.0f, 1.0f, 5.0f, 2.0f};
      Assert::AreEqual(5.0f, max(v), L"max(v) mismatch", LINE_INFO());

      // scalar max over first 3 components (ignores W)
      const v_f32_4 v2{4.0f, 2.0f, 3.0f, 10.0f};
      Assert::AreEqual(4.0f, max3(v2), L"max3(v) mismatch", LINE_INFO());
    }

    TEST_METHOD(computesHorizontalSumAndHorizontalSum3) {
      const v_f32_4 v{1.0f, 2.0f, 3.0f, 4.0f};
      Assert::AreEqual(10.0f, horizontalSum(v), L"horizontalSum mismatch", LINE_INFO());

      // horizontalSum3 sums only X, Y, Z (ignores W)
      Assert::AreEqual(6.0f, horizontalSum3(v), L"horizontalSum3 mismatch", LINE_INFO());
    }

    TEST_METHOD(computesDotAndDot3) {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{5.0f, 6.0f, 7.0f, 8.0f};

      // dot(a,b) = 1*5 + 2*6 + 3*7 + 4*8 = 5+12+21+32 = 70
      Assert::AreEqual(70.0f, dot(a, b), L"dot mismatch", LINE_INFO());

      // dot3(a,b) = 1*5 + 2*6 + 3*7 = 5+12+21 = 38
      Assert::AreEqual(38.0f, dot3(a, b), L"dot3 mismatch", LINE_INFO());
    }

    TEST_METHOD(computesLengthAndLength3) {
      // length({3,0,4,0}) = sqrt(9+0+16+0) = sqrt(25) = 5
      const v_f32_4 v{3.0f, 0.0f, 4.0f, 0.0f};
      Assert::AreEqual(5.0f, length(v), L"length mismatch", LINE_INFO());

      // length3({3,4,0,99}) = sqrt(9+16+0) = sqrt(25) = 5, ignoring W
      const v_f32_4 v2{3.0f, 4.0f, 0.0f, 99.0f};
      Assert::AreEqual(5.0f, length3(v2), L"length3 mismatch", LINE_INFO());
    }

    TEST_METHOD(computesNormalizeAndNormalize3) {
      // normalize({3,0,4,0}) with length=5 gives {0.6, 0, 0.8, 0}
      const v_f32_4 v{3.0f, 0.0f, 4.0f, 0.0f};
      const auto n = normalize(v);
      const auto epsilon = Epsilon<Float_32>();
      Assert::AreEqual(0.6f, x(n), epsilon, L"normalize X mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, y(n), epsilon, L"normalize Y mismatch", LINE_INFO());
      Assert::AreEqual(0.8f, z(n), epsilon, L"normalize Z mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, w(n), epsilon, L"normalize W mismatch", LINE_INFO());

      // normalize3({3,4,0,99}) with 3-length=5 gives {0.6, 0.8, 0, 0} (W zeroed)
      const v_f32_4 v2{3.0f, 4.0f, 0.0f, 99.0f};
      const auto n3 = normalize3(v2);
      Assert::AreEqual(0.6f, x(n3), epsilon, L"normalize3 X mismatch", LINE_INFO());
      Assert::AreEqual(0.8f, y(n3), epsilon, L"normalize3 Y mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, z(n3), epsilon, L"normalize3 Z mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, w(n3), epsilon, L"normalize3 W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesDistanceAndDistance3) {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 0.0f};
      const v_f32_4 b{4.0f, 6.0f, 3.0f, 0.0f};

      // distance = length(b-a) = length({3,4,0,0}) = sqrt(9+16) = 5
      Assert::AreEqual(5.0f, distance(a, b), L"distance mismatch", LINE_INFO());

      // distance3 uses only XYZ components
      const v_f32_4 a2{1.0f, 2.0f, 3.0f, 100.0f};
      const v_f32_4 b2{4.0f, 6.0f, 3.0f, 200.0f};
      Assert::AreEqual(5.0f, distance3(a2, b2), L"distance3 mismatch", LINE_INFO());
    }

    TEST_METHOD(computesReflect) {
      // reflect(incident, normal) = incident - 2*dot(normal, incident)*normal
      // incident going down at 45 deg: {1, -1, 0, 0}, normal pointing up: {0, 1, 0, 0}
      // dot(normal, incident) = -1
      // reflect = {1,-1,0,0} - 2*(-1)*{0,1,0,0} = {1,-1,0,0} + {0,2,0,0} = {1,1,0,0}
      const v_f32_4 incident{1.0f, -1.0f, 0.0f, 0.0f};
      const v_f32_4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const auto r = reflect(incident, normal);
      const auto epsilon = Epsilon<Float_32>();
      Assert::AreEqual(1.0f, x(r), epsilon, L"reflect X mismatch", LINE_INFO());
      Assert::AreEqual(1.0f, y(r), epsilon, L"reflect Y mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, z(r), epsilon, L"reflect Z mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, w(r), epsilon, L"reflect W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesCross3) {
      // cross3({1,0,0,0}, {0,1,0,0}) = {0,0,1,0}
      const v_f32_4 a{1.0f, 0.0f, 0.0f, 0.0f};
      const v_f32_4 b{0.0f, 1.0f, 0.0f, 0.0f};
      const auto c = cross3(a, b);
      Assert::AreEqual(0.0f, x(c), L"cross3 X mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, y(c), L"cross3 Y mismatch", LINE_INFO());
      Assert::AreEqual(1.0f, z(c), L"cross3 Z mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, w(c), L"cross3 W mismatch", LINE_INFO());

      // cross3({0,1,0,0}, {0,0,1,0}) = {1,0,0,0}
      const v_f32_4 d{0.0f, 1.0f, 0.0f, 0.0f};
      const v_f32_4 e{0.0f, 0.0f, 1.0f, 0.0f};
      const auto f = cross3(d, e);
      Assert::AreEqual(1.0f, x(f), L"cross3 X mismatch (2)", LINE_INFO());
      Assert::AreEqual(0.0f, y(f), L"cross3 Y mismatch (2)", LINE_INFO());
      Assert::AreEqual(0.0f, z(f), L"cross3 Z mismatch (2)", LINE_INFO());
    }

    TEST_METHOD(computesMix) {
      // mix(a, b, factor) = a + (b - a) * factor
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{5.0f, 6.0f, 7.0f, 8.0f};

      // factor = 0.0 should return a
      const auto r0 = mix(a, b, 0.0f);
      Assert::AreEqual(1.0f, x(r0), L"mix(0) X mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, y(r0), L"mix(0) Y mismatch", LINE_INFO());

      // factor = 1.0 should return b
      const auto r1 = mix(a, b, 1.0f);
      Assert::AreEqual(5.0f, x(r1), L"mix(1) X mismatch", LINE_INFO());
      Assert::AreEqual(8.0f, w(r1), L"mix(1) W mismatch", LINE_INFO());

      // factor = 0.5 should return midpoint
      const auto rh = mix(a, b, 0.5f);
      Assert::AreEqual(3.0f, x(rh), L"mix(0.5) X mismatch", LINE_INFO());
      Assert::AreEqual(4.0f, y(rh), L"mix(0.5) Y mismatch", LINE_INFO());
      Assert::AreEqual(5.0f, z(rh), L"mix(0.5) Z mismatch", LINE_INFO());
      Assert::AreEqual(6.0f, w(rh), L"mix(0.5) W mismatch", LINE_INFO());

      // vector factor overload
      const v_f32_4 factor{0.0f, 0.25f, 0.5f, 1.0f};
      const auto rv = mix(a, b, factor);
      Assert::AreEqual(1.0f, x(rv), L"mix(vec) X mismatch", LINE_INFO());
      Assert::AreEqual(3.0f, y(rv), L"mix(vec) Y mismatch", LINE_INFO());
      Assert::AreEqual(5.0f, z(rv), L"mix(vec) Z mismatch", LINE_INFO());
      Assert::AreEqual(8.0f, w(rv), L"mix(vec) W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesZeroWAndOneW) {
      const v_f32_4 v{1.0f, 2.0f, 3.0f, 4.0f};

      // zeroW sets W to 0, preserves XYZ
      const auto zw = zeroW(v);
      Assert::AreEqual(1.0f, x(zw), L"zeroW X mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, y(zw), L"zeroW Y mismatch", LINE_INFO());
      Assert::AreEqual(3.0f, z(zw), L"zeroW Z mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, w(zw), L"zeroW W mismatch", LINE_INFO());

      // oneW sets W to 1, preserves XYZ
      const auto ow = oneW(v);
      Assert::AreEqual(1.0f, x(ow), L"oneW X mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, y(ow), L"oneW Y mismatch", LINE_INFO());
      Assert::AreEqual(3.0f, z(ow), L"oneW Z mismatch", LINE_INFO());
      Assert::AreEqual(1.0f, w(ow), L"oneW W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesIsNegative) {
      // isNegative returns a movemask bitmask: bit 0 = X, bit 1 = Y, bit 2 = Z, bit 3 = W
      const v_f32_4 v{-1.0f, 2.0f, -3.0f, 4.0f};
      const auto mask = isNegative(v);
      // X negative (bit 0) and Z negative (bit 2) => 0b0101 = 5
      Assert::AreEqual(static_cast<v_f32_4::BoolType>(VectorBits::X | VectorBits::Z), mask, L"isNegative mismatch", LINE_INFO());

      // all positive
      const auto maskPos = isNegative(v_f32_4{1.0f, 2.0f, 3.0f, 4.0f});
      Assert::AreEqual(static_cast<v_f32_4::BoolType>(VectorBits::None), maskPos, L"isNegative all-positive mismatch", LINE_INFO());

      // all negative
      const auto maskNeg = isNegative(v_f32_4{-1.0f, -2.0f, -3.0f, -4.0f});
      Assert::AreEqual(
          static_cast<v_f32_4::BoolType>(VectorBits::X | VectorBits::Y | VectorBits::Z | VectorBits::W), maskNeg,
          L"isNegative all-negative mismatch", LINE_INFO()
      );
    }

    TEST_METHOD(computesIsNaN) {
      // No NaN values
      const v_f32_4 normal{1.0f, 2.0f, 3.0f, 4.0f};
      const auto noNaN = isNaN(normal);
      Assert::IsFalse(allTrue(noNaN), L"isNaN should not flag normal values", LINE_INFO());

      // All NaN values
      const v_f32_4 allNaN{NotANumber<Float_32>(), NotANumber<Float_32>(), NotANumber<Float_32>(), NotANumber<Float_32>()};
      const auto allNaNResult = isNaN(allNaN);
      Assert::IsTrue(allTrue(allNaNResult), L"isNaN should flag all NaN values", LINE_INFO());

      // Mixed NaN: Y is NaN
      const v_f32_4 mixed{1.0f, NotANumber<Float_32>(), 3.0f, 4.0f};
      const auto mixedResult = isNaN(mixed);
      // At least one NaN should be detected; not allTrue since only Y is NaN
      Assert::IsFalse(allTrue(mixedResult), L"isNaN should not be allTrue for single NaN", LINE_INFO());
    }

    TEST_METHOD(computesSqrtReciprocalAndRsqrt) {
      // sqrt of perfect squares
      const v_f32_4 v{4.0f, 9.0f, 16.0f, 25.0f};
      const auto s = sqrt(v);
      Assert::AreEqual(2.0f, x(s), L"sqrt X mismatch", LINE_INFO());
      Assert::AreEqual(3.0f, y(s), L"sqrt Y mismatch", LINE_INFO());
      Assert::AreEqual(4.0f, z(s), L"sqrt Z mismatch", LINE_INFO());
      Assert::AreEqual(5.0f, w(s), L"sqrt W mismatch", LINE_INFO());

      // reciprocal(v) = 1/v
      const v_f32_4 r{2.0f, 4.0f, 5.0f, 10.0f};
      const auto rec = reciprocal(r);
      const auto epsilon = Epsilon<Float_32>();
      Assert::AreEqual(0.5f, x(rec), epsilon, L"reciprocal X mismatch", LINE_INFO());
      Assert::AreEqual(0.25f, y(rec), epsilon, L"reciprocal Y mismatch", LINE_INFO());
      Assert::AreEqual(0.2f, z(rec), epsilon, L"reciprocal Z mismatch", LINE_INFO());
      Assert::AreEqual(0.1f, w(rec), epsilon, L"reciprocal W mismatch", LINE_INFO());

      // rsqrt(v) = 1/sqrt(v)
      const auto rs = rsqrt(v);
      Assert::AreEqual(0.5f, x(rs), epsilon, L"rsqrt X mismatch", LINE_INFO());
      Assert::AreEqual(1.0f / 3.0f, y(rs), epsilon, L"rsqrt Y mismatch", LINE_INFO());
      Assert::AreEqual(0.25f, z(rs), epsilon, L"rsqrt Z mismatch", LINE_INFO());
      Assert::AreEqual(0.2f, w(rs), epsilon, L"rsqrt W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesCeilFloorRoundAndFract) {
      const v_f32_4 v{1.3f, 2.7f, -0.3f, 3.5f};

      // ceil
      const auto c = ceil(v);
      Assert::AreEqual(2.0f, x(c), L"ceil X mismatch", LINE_INFO());
      Assert::AreEqual(3.0f, y(c), L"ceil Y mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, z(c), L"ceil Z mismatch", LINE_INFO());
      Assert::AreEqual(4.0f, w(c), L"ceil W mismatch", LINE_INFO());

      // floor
      const auto f = floor(v);
      Assert::AreEqual(1.0f, x(f), L"floor X mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, y(f), L"floor Y mismatch", LINE_INFO());
      Assert::AreEqual(-1.0f, z(f), L"floor Z mismatch", LINE_INFO());
      Assert::AreEqual(3.0f, w(f), L"floor W mismatch", LINE_INFO());

      // round (banker's rounding: 3.5 -> 4.0 or 3.5 -> 4.0 with SSE _MM_FROUND_TO_NEAREST_INT)
      const v_f32_4 rv{1.3f, 2.7f, -0.3f, 4.5f};
      const auto r = round(rv);
      Assert::AreEqual(1.0f, x(r), L"round X mismatch", LINE_INFO());
      Assert::AreEqual(3.0f, y(r), L"round Y mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, z(r), L"round Z mismatch", LINE_INFO());
      Assert::AreEqual(4.0f, w(r), L"round W mismatch", LINE_INFO());

      // fract(v) = v - floor(v)
      const v_f32_4 fv{1.5f, 2.7f, -0.3f, 3.0f};
      const auto fr = fract(fv);
      const auto epsilon = Epsilon<Float_32>();
      Assert::AreEqual(0.5f, x(fr), epsilon, L"fract X mismatch", LINE_INFO());
      Assert::AreEqual(0.7f, y(fr), epsilon, L"fract Y mismatch", LINE_INFO());
      Assert::AreEqual(0.7f, z(fr), epsilon, L"fract Z mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, w(fr), epsilon, L"fract W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesAbs) {
      const v_f32_4 v{-1.0f, 2.0f, -3.5f, 0.0f};
      const auto a = abs(v);
      Assert::AreEqual(1.0f, x(a), L"abs X mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, y(a), L"abs Y mismatch", LINE_INFO());
      Assert::AreEqual(3.5f, z(a), L"abs Z mismatch", LINE_INFO());
      Assert::AreEqual(0.0f, w(a), L"abs W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesClamp) {
      const v_f32_4 v{-1.0f, 0.5f, 1.5f, 3.0f};
      const v_f32_4 lower{0.0f, 0.0f, 0.0f, 0.0f};
      const v_f32_4 upper{1.0f, 1.0f, 1.0f, 1.0f};
      const auto c = clamp(v, lower, upper);
      Assert::AreEqual(0.0f, x(c), L"clamp X mismatch", LINE_INFO());
      Assert::AreEqual(0.5f, y(c), L"clamp Y mismatch", LINE_INFO());
      Assert::AreEqual(1.0f, z(c), L"clamp Z mismatch", LINE_INFO());
      Assert::AreEqual(1.0f, w(c), L"clamp W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesSubadd) {
      // subadd: X=a.x-b.x, Y=a.y+b.y, Z=a.z-b.z, W=a.w+b.w
      const v_f32_4 a{10.0f, 20.0f, 30.0f, 40.0f};
      const v_f32_4 b{1.0f, 2.0f, 3.0f, 4.0f};
      const auto r = subadd(a, b);
      Assert::AreEqual(9.0f, x(r), L"subadd X mismatch", LINE_INFO());
      Assert::AreEqual(22.0f, y(r), L"subadd Y mismatch", LINE_INFO());
      Assert::AreEqual(27.0f, z(r), L"subadd Z mismatch", LINE_INFO());
      Assert::AreEqual(44.0f, w(r), L"subadd W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesArgmin3AndArgmax3) {
      // argmin3 returns index (0, 1, or 2) of the minimum among X, Y, Z
      const v_f32_4 v1{3.0f, 1.0f, 2.0f, 0.0f}; // min is Y at index 1
      Assert::AreEqual(ASizeT{1}, argmin3(v1), L"argmin3 mismatch", LINE_INFO());

      const v_f32_4 v2{1.0f, 3.0f, 2.0f, 0.0f}; // min is X at index 0
      Assert::AreEqual(ASizeT{0}, argmin3(v2), L"argmin3 mismatch (X)", LINE_INFO());

      const v_f32_4 v3{3.0f, 2.0f, 1.0f, 0.0f}; // min is Z at index 2
      Assert::AreEqual(ASizeT{2}, argmin3(v3), L"argmin3 mismatch (Z)", LINE_INFO());

      // argmax3 returns index (0, 1, or 2) of the maximum among X, Y, Z
      const v_f32_4 v4{1.0f, 3.0f, 2.0f, 99.0f}; // max is Y at index 1
      Assert::AreEqual(ASizeT{1}, argmax3(v4), L"argmax3 mismatch", LINE_INFO());

      const v_f32_4 v5{5.0f, 3.0f, 2.0f, 99.0f}; // max is X at index 0
      Assert::AreEqual(ASizeT{0}, argmax3(v5), L"argmax3 mismatch (X)", LINE_INFO());

      const v_f32_4 v6{1.0f, 2.0f, 5.0f, 99.0f}; // max is Z at index 2
      Assert::AreEqual(ASizeT{2}, argmax3(v6), L"argmax3 mismatch (Z)", LINE_INFO());
    }

    TEST_METHOD(computesVectorReturnVariants) {
      const v_f32_4 a{1.0f, 4.0f, 2.0f, 3.0f};
      const v_f32_4 b{3.0f, 2.0f, 6.0f, 1.0f};

      // minv broadcasts scalar min to all lanes
      const auto minResult = minv(a);
      Assert::AreEqual(min(a), x(minResult), L"minv", LINE_INFO());

      // maxv broadcasts scalar max
      const auto maxResult = maxv(a);
      Assert::AreEqual(max(a), x(maxResult), L"maxv", LINE_INFO());

      // min3v, max3v
      Assert::AreEqual(min3(a), x(min3v(a)), L"min3v", LINE_INFO());
      Assert::AreEqual(max3(a), x(max3v(a)), L"max3v", LINE_INFO());

      // horizontalSumv, horizontalSum3v
      Assert::AreEqual(horizontalSum(a), x(horizontalSumv(a)), L"horizontalSumv", LINE_INFO());
      Assert::AreEqual(horizontalSum3(a), x(horizontalSum3v(a)), L"horizontalSum3v", LINE_INFO());

      // dotv, dot3v
      Assert::AreEqual(dot(a, b), x(dotv(a, b)), L"dotv", LINE_INFO());
      Assert::AreEqual(dot3(a, b), x(dot3v(a, b)), L"dot3v", LINE_INFO());

      // lengthv, length3v
      Assert::AreEqual(length(a), x(lengthv(a)), L"lengthv", LINE_INFO());
      Assert::AreEqual(length3(a), x(length3v(a)), L"length3v", LINE_INFO());

      // rlengthv, rlength3v
      Assert::AreEqual(rlength(a), x(rlengthv(a)), L"rlengthv", LINE_INFO());
      Assert::AreEqual(rlength3(a), x(rlength3v(a)), L"rlength3v", LINE_INFO());

      // distancev, distance3v
      Assert::AreEqual(distance(a, b), x(distancev(a, b)), L"distancev", LINE_INFO());
      Assert::AreEqual(distance3(a, b), x(distance3v(a, b)), L"distance3v", LINE_INFO());
    }

    TEST_METHOD(computesReflectWithPrecomputedNdotI) {
      const v_f32_4 incident{1.0f, -1.0f, 0.0f, 0.0f};
      const v_f32_4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const auto ndotI = dotv(normal, incident);
      const auto result = reflect(incident, normal, ndotI);
      const auto expected = reflect(incident, normal);
      Assert::AreEqual(x(expected), x(result), L"X", LINE_INFO());
      Assert::AreEqual(y(expected), y(result), L"Y", LINE_INFO());
      Assert::AreEqual(z(expected), z(result), L"Z", LINE_INFO());
    }

    TEST_METHOD(computesRefractEta) {
      // Normal incidence with eta=1 should pass through unchanged (direction only)
      const v_f32_4 incident{0.0f, -1.0f, 0.0f, 0.0f};
      const v_f32_4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const v_f32_4 eta{1.0f};
      const auto result = refractEta(incident, normal, eta);
      // With eta=1, refracted = incident
      Assert::AreEqual(0.0f, x(result), 0.001f, L"X", LINE_INFO());
      Assert::AreEqual(-1.0f, y(result), 0.001f, L"Y", LINE_INFO());
      Assert::AreEqual(0.0f, z(result), 0.001f, L"Z", LINE_INFO());
    }

    TEST_METHOD(computesRefract) {
      // refract wraps refractEta with sign-corrected normal based on NdotI
      const v_f32_4 incident{0.0f, -1.0f, 0.0f, 0.0f};
      const v_f32_4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const v_f32_4 eta{1.0f};
      const auto result = refract(incident, normal, eta);
      Assert::AreEqual(0.0f, x(result), 0.001f, L"X", LINE_INFO());
      Assert::AreEqual(-1.0f, y(result), 0.001f, L"Y", LINE_INFO());
    }

    TEST_METHOD(computesIsNaNTwoArg) {
      const v_f32_4 a{1.0f, NotANumber<Float_32>(), 3.0f, 4.0f};
      const v_f32_4 b{NotANumber<Float_32>(), 2.0f, 3.0f, NotANumber<Float_32>()};
      const auto result = isNaN(a, b);
      // Should be true where EITHER a or b is NaN: X=true(b), Y=true(a), Z=false, W=true(b)
      Assert::IsTrue(anyTrue(result), L"should detect NaN", LINE_INFO());
    }

    TEST_METHOD(computesModulo) {
      const v_f32_4 a{5.5f, 7.0f, 10.3f, 3.0f};
      const v_f32_4 b{2.0f, 3.0f, 4.0f, 1.5f};
      const auto result = modulo(a, b);
      Assert::AreEqual(std::fmod(5.5f, 2.0f), x(result), 0.001f, L"X", LINE_INFO());
      Assert::AreEqual(std::fmod(7.0f, 3.0f), y(result), 0.001f, L"Y", LINE_INFO());
      Assert::AreEqual(std::fmod(10.3f, 4.0f), z(result), 0.001f, L"Z", LINE_INFO());
      Assert::AreEqual(std::fmod(3.0f, 1.5f), w(result), 0.001f, L"W", LINE_INFO());
    }

    TEST_METHOD(computesMapOrthogonal) {
      // mapOrthogonal projects v onto s/t plane axes
      // result: X = dot(v, sPlane), Y = dot(v, tPlane), Z = 0, W = 1
      const v_f32_4 v{1.0f, 2.0f, 3.0f, 0.0f};
      const v_f32_4 sPlane{1.0f, 0.0f, 0.0f, 0.0f};
      const v_f32_4 tPlane{0.0f, 1.0f, 0.0f, 0.0f};
      const auto result = mapOrthogonal(v, sPlane, tPlane);
      const auto epsilon = Epsilon<Float_32>();
      Assert::AreEqual(1.0f, x(result), epsilon, L"X = dot(v, sPlane)", LINE_INFO());
      Assert::AreEqual(2.0f, y(result), epsilon, L"Y = dot(v, tPlane)", LINE_INFO());
      Assert::AreEqual(0.0f, z(result), epsilon, L"Z = 0", LINE_INFO());
      Assert::AreEqual(1.0f, w(result), epsilon, L"W = 1", LINE_INFO());
    }

    TEST_METHOD(computesMapSpherical) {
      // mapSpherical converts direction to (u, v) texture coordinates
      // X = atan2(z, x) * RadianToUniform + 0.5
      // Y = acos(-y) * ReciprocalPi
      // Z = 0, W = 1
      const auto epsilon = Epsilon<Float_32>();

      {
        // Direction pointing up: {0, 1, 0, 0}
        // Y = acos(-1) * ReciprocalPi = pi / pi = 1.0
        const v_f32_4 up{0.0f, 1.0f, 0.0f, 0.0f};
        const auto result = mapSpherical(up);
        Assert::AreEqual(1.0f, y(result), epsilon, L"up Y", LINE_INFO());
        Assert::AreEqual(0.0f, z(result), epsilon, L"up Z", LINE_INFO());
        Assert::AreEqual(1.0f, w(result), epsilon, L"up W", LINE_INFO());
      }

      {
        // Direction pointing down: {0, -1, 0, 0}
        // Y = acos(1) * ReciprocalPi = 0 / pi = 0.0
        const v_f32_4 down{0.0f, -1.0f, 0.0f, 0.0f};
        const auto result = mapSpherical(down);
        Assert::AreEqual(0.0f, y(result), epsilon, L"down Y", LINE_INFO());
        Assert::AreEqual(0.0f, z(result), epsilon, L"down Z", LINE_INFO());
        Assert::AreEqual(1.0f, w(result), epsilon, L"down W", LINE_INFO());
      }

      {
        // Equator direction: {0, 0, 1, 0} (pointing along +Z)
        // Y = acos(0) * ReciprocalPi = (pi/2) / pi = 0.5
        const v_f32_4 front{0.0f, 0.0f, 1.0f, 0.0f};
        const auto result = mapSpherical(front);
        Assert::AreEqual(0.5f, y(result), epsilon, L"front Y", LINE_INFO());
        Assert::AreEqual(0.0f, z(result), epsilon, L"front Z", LINE_INFO());
        Assert::AreEqual(1.0f, w(result), epsilon, L"front W", LINE_INFO());
      }
    }

    TEST_METHOD(computesAxisAlignedNormal3) {
      // axisAlignedNormal3 snaps normal to nearest axis with correct sign
      // For {0.1, -0.9, 0.2, 0.0}: dominant axis is Y (|0.9|), Y is negative => {0, -1, 0, 0}
      const v_f32_4 normal{0.1f, -0.9f, 0.2f, 0.0f};
      const auto result = axisAlignedNormal3(normal);
      Assert::AreEqual(0.0f, x(result), L"X", LINE_INFO());
      Assert::AreEqual(-1.0f, y(result), L"Y", LINE_INFO());
      Assert::AreEqual(0.0f, z(result), L"Z", LINE_INFO());
      Assert::AreEqual(0.0f, w(result), L"W", LINE_INFO());

      // For {0.0, 0.0, 0.8, 0.0}: dominant axis is Z, positive => {0, 0, 1, 0}
      const v_f32_4 normal2{0.0f, 0.0f, 0.8f, 0.0f};
      const auto result2 = axisAlignedNormal3(normal2);
      Assert::AreEqual(0.0f, x(result2), L"X (2)", LINE_INFO());
      Assert::AreEqual(0.0f, y(result2), L"Y (2)", LINE_INFO());
      Assert::AreEqual(1.0f, z(result2), L"Z (2)", LINE_INFO());
      Assert::AreEqual(0.0f, w(result2), L"W (2)", LINE_INFO());
    }
  };
}
