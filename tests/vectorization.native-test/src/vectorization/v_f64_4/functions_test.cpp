#include "vectorization_tests.h"

namespace vectorization::test
{
    TEST_CLASS (v_f64_4_FunctionsTest)
    {
        public
        :
        TEST_METHOD(computesMinAndMax)
        {
            const v_f64_4 a{1.0, 4.0, 2.0, 5.0};
            const v_f64_4 b{3.0, 2.0, 6.0, 1.0};

            const auto mn = min(a, b);
            Assert::AreEqual(1.0, x(mn), L"min X mismatch", LINE_INFO());
            Assert::AreEqual(2.0, y(mn), L"min Y mismatch", LINE_INFO());
            Assert::AreEqual(2.0, z(mn), L"min Z mismatch", LINE_INFO());
            Assert::AreEqual(1.0, w(mn), L"min W mismatch", LINE_INFO());

            const auto mx = max(a, b);
            Assert::AreEqual(3.0, x(mx), L"max X mismatch", LINE_INFO());
            Assert::AreEqual(4.0, y(mx), L"max Y mismatch", LINE_INFO());
            Assert::AreEqual(6.0, z(mx), L"max Z mismatch", LINE_INFO());
            Assert::AreEqual(5.0, w(mx), L"max W mismatch", LINE_INFO());

            // scalar reductions
            const v_f64_4 v{3.0, 1.0, 5.0, 2.0};
            Assert::AreEqual(1.0, min(v), L"min(v) mismatch", LINE_INFO());
            Assert::AreEqual(5.0, max(v), L"max(v) mismatch", LINE_INFO());
        }

        TEST_METHOD(computesHorizontalSumAndDot)
        {
            const v_f64_4 v{1.0, 2.0, 3.0, 4.0};
            Assert::AreEqual(10.0, horizontalSum(v), L"horizontalSum mismatch", LINE_INFO());

            const v_f64_4 a{1.0, 2.0, 3.0, 4.0};
            const v_f64_4 b{5.0, 6.0, 7.0, 8.0};
            // dot = 1*5 + 2*6 + 3*7 + 4*8 = 5+12+21+32 = 70
            Assert::AreEqual(70.0, dot(a, b), L"dot mismatch", LINE_INFO());
        }

        TEST_METHOD(computesLengthNormalizeAndDistance)
        {
            // length({3,0,4,0}) = sqrt(9+0+16+0) = 5
            const v_f64_4 v{3.0, 0.0, 4.0, 0.0};
            Assert::AreEqual(5.0, length(v), 1e-10, L"length mismatch", LINE_INFO());

            // normalize
            const auto n = normalize(v);
            Assert::AreEqual(0.6, x(n), 1e-10, L"normalize X mismatch", LINE_INFO());
            Assert::AreEqual(0.0, y(n), 1e-10, L"normalize Y mismatch", LINE_INFO());
            Assert::AreEqual(0.8, z(n), 1e-10, L"normalize Z mismatch", LINE_INFO());
            Assert::AreEqual(0.0, w(n), 1e-10, L"normalize W mismatch", LINE_INFO());

            // distance
            const v_f64_4 a{1.0, 2.0, 3.0, 0.0};
            const v_f64_4 b{4.0, 6.0, 3.0, 0.0};
            // distance = length({3,4,0,0}) = sqrt(9+16) = 5
            Assert::AreEqual(5.0, distance(a, b), 1e-10, L"distance mismatch", LINE_INFO());
        }

        TEST_METHOD(computesSqrtReciprocalAndAbs)
        {
            const v_f64_4 v{4.0, 9.0, 16.0, 25.0};
            const auto s = sqrt(v);
            Assert::AreEqual(2.0, x(s), 1e-10, L"sqrt X mismatch", LINE_INFO());
            Assert::AreEqual(3.0, y(s), 1e-10, L"sqrt Y mismatch", LINE_INFO());
            Assert::AreEqual(4.0, z(s), 1e-10, L"sqrt Z mismatch", LINE_INFO());
            Assert::AreEqual(5.0, w(s), 1e-10, L"sqrt W mismatch", LINE_INFO());

            const auto rec = reciprocal(v_f64_4{2.0, 4.0, 5.0, 10.0});
            Assert::AreEqual(0.5, x(rec), 1e-10, L"reciprocal X mismatch", LINE_INFO());
            Assert::AreEqual(0.25, y(rec), 1e-10, L"reciprocal Y mismatch", LINE_INFO());
            Assert::AreEqual(0.2, z(rec), 1e-10, L"reciprocal Z mismatch", LINE_INFO());
            Assert::AreEqual(0.1, w(rec), 1e-10, L"reciprocal W mismatch", LINE_INFO());

            const v_f64_4 neg{-3.0, 7.0, -5.0, -1.0};
            const auto a = abs(neg);
            Assert::AreEqual(3.0, x(a), L"abs X mismatch", LINE_INFO());
            Assert::AreEqual(7.0, y(a), L"abs Y mismatch", LINE_INFO());
            Assert::AreEqual(5.0, z(a), L"abs Z mismatch", LINE_INFO());
            Assert::AreEqual(1.0, w(a), L"abs W mismatch", LINE_INFO());
        }

        TEST_METHOD(computesSqr)
        {
            const v_f64_4 v{2.0, -3.0, 4.0, 0.5};
            const auto s = sqr(v);
            Assert::AreEqual(4.0, x(s), L"sqr X mismatch", LINE_INFO());
            Assert::AreEqual(9.0, y(s), L"sqr Y mismatch", LINE_INFO());
            Assert::AreEqual(16.0, z(s), L"sqr Z mismatch", LINE_INFO());
            Assert::AreEqual(0.25, w(s), L"sqr W mismatch", LINE_INFO());
        }

        TEST_METHOD(computesClampAndMix)
        {
            const v_f64_4 v{-1.0, 5.0, 2.0, 10.0};
            const v_f64_4 lower{0.0, 0.0, 0.0, 0.0};
            const v_f64_4 upper{3.0, 3.0, 3.0, 3.0};
            const auto clamped = clamp(v, lower, upper);
            Assert::AreEqual(0.0, x(clamped), L"clamp X mismatch", LINE_INFO());
            Assert::AreEqual(3.0, y(clamped), L"clamp Y mismatch", LINE_INFO());
            Assert::AreEqual(2.0, z(clamped), L"clamp Z mismatch", LINE_INFO());
            Assert::AreEqual(3.0, w(clamped), L"clamp W mismatch", LINE_INFO());

            // mix(a, b, 0.5) = midpoint
            const v_f64_4 a{0.0, 2.0, 4.0, 6.0};
            const v_f64_4 b{4.0, 6.0, 8.0, 10.0};
            const auto mid = mix(a, b, 0.5);
            Assert::AreEqual(2.0, x(mid), 1e-10, L"mix X mismatch", LINE_INFO());
            Assert::AreEqual(4.0, y(mid), 1e-10, L"mix Y mismatch", LINE_INFO());
            Assert::AreEqual(6.0, z(mid), 1e-10, L"mix Z mismatch", LINE_INFO());
            Assert::AreEqual(8.0, w(mid), 1e-10, L"mix W mismatch", LINE_INFO());
        }

        TEST_METHOD(computesIsNegativeAndPredicates)
        {
            const v_f64_4 v{-1.0, 2.0, -3.0, 4.0};
            const auto mask = isNegative(v);
            // X negative (bit 0) and Z negative (bit 2)
            Assert::IsTrue((mask & static_cast<v_f64_4::BoolType>(VectorBits::X)) != 0, L"isNegative X", LINE_INFO());
            Assert::IsTrue((mask & static_cast<v_f64_4::BoolType>(VectorBits::Y)) == 0, L"isNegative Y", LINE_INFO());
            Assert::IsTrue((mask & static_cast<v_f64_4::BoolType>(VectorBits::Z)) != 0, L"isNegative Z", LINE_INFO());
            Assert::IsTrue((mask & static_cast<v_f64_4::BoolType>(VectorBits::W)) == 0, L"isNegative W", LINE_INFO());

            const v_f64_4 allOnes{1.0, 1.0, 1.0, 1.0};
            const auto cmpResult = allOnes == allOnes;
            Assert::IsTrue(allTrue(cmpResult), L"allTrue should be true for equal vectors", LINE_INFO());
            Assert::IsFalse(anyFalse(cmpResult), L"anyFalse should be false for equal vectors", LINE_INFO());
        }

        TEST_METHOD(computesCeilFloorRoundFract)
        {
            const v_f64_4 v{1.3, 2.7, -0.3, 3.5};

            const auto c = ceil(v);
            Assert::AreEqual(2.0, x(c), L"ceil X mismatch", LINE_INFO());
            Assert::AreEqual(3.0, y(c), L"ceil Y mismatch", LINE_INFO());
            Assert::AreEqual(0.0, z(c), L"ceil Z mismatch", LINE_INFO());
            Assert::AreEqual(4.0, w(c), L"ceil W mismatch", LINE_INFO());

            const auto f = floor(v);
            Assert::AreEqual(1.0, x(f), L"floor X mismatch", LINE_INFO());
            Assert::AreEqual(2.0, y(f), L"floor Y mismatch", LINE_INFO());
            Assert::AreEqual(-1.0, z(f), L"floor Z mismatch", LINE_INFO());
            Assert::AreEqual(3.0, w(f), L"floor W mismatch", LINE_INFO());

            // fract = v - floor(v)
            const auto fr = fract(v_f64_4{1.75, 2.25, 3.5, 4.125});
            Assert::AreEqual(0.75, x(fr), 1e-10, L"fract X mismatch", LINE_INFO());
            Assert::AreEqual(0.25, y(fr), 1e-10, L"fract Y mismatch", LINE_INFO());
            Assert::AreEqual(0.5, z(fr), 1e-10, L"fract Z mismatch", LINE_INFO());
            Assert::AreEqual(0.125, w(fr), 1e-10, L"fract W mismatch", LINE_INFO());
        }

        TEST_METHOD(computesVectorReturnVariants)
        {
            const v_f64_4 given{-1.0, 2.0, 2.0, 4.0};
            Assert::IsTrue(allTrue(v_f64_4{-1.0} == minv(given)), L"minv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_4{4.0} == maxv(given)), L"maxv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_4{7.0} == horizontalSumv(given)), L"horizontalSumv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_4{5.0} == lengthv(given)), L"lengthv mismatch", LINE_INFO());
            Assert::AreEqual(0.2, rlength(given), 1e-12, L"rlength mismatch", LINE_INFO());
            Assert::AreEqual(0.2, w(rlengthv(given)), 1e-12, L"rlengthv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_4{5.0} == distancev(given, Zero<v_f64_4>())), L"distancev", LINE_INFO());
            const auto r = rsqrt(v_f64_4{4.0, 16.0, 64.0, 0.25});
            Assert::AreEqual(0.5, x(r), 1e-12, L"rsqrt X mismatch", LINE_INFO());
            Assert::AreEqual(0.125, z(r), 1e-12, L"rsqrt Z mismatch", LINE_INFO());
            Assert::AreEqual(2.0, w(r), 1e-12, L"rsqrt W mismatch", LINE_INFO());
            const auto rounded = round(v_f64_4{1.4, 1.6, -1.4, -1.6});
            Assert::IsTrue(allTrue(v_f64_4{1.0, 2.0, -1.0, -2.0} == rounded), L"round mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMultiplyAddFamily)
        {
            const v_f64_4 a{1.0, 2.0, 3.0, 4.0};
            const v_f64_4 b{5.0, 6.0, -7.0, 8.0};
            const v_f64_4 c{10.0, 20.0, 30.0, 40.0};
            // a * b = {5, 12, -21, 32}
            Assert::IsTrue(allTrue(v_f64_4{15.0, 32.0, 9.0, 72.0} == multiplyAdd(a, b, c)), L"multiplyAdd", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_4{-5.0, -8.0, -51.0, -8.0} == multiplySub(a, b, c)), L"multiplySub", LINE_INFO());
            Assert::IsTrue(
                allTrue(v_f64_4{-5.0, 32.0, -51.0, 72.0} == multiplyAddSub(a, b, c)), L"multiplyAddSub", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_f64_4{15.0, -8.0, 9.0, -8.0} == multiplySubAdd(a, b, c)), L"multiplySubAdd", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_f64_4{5.0, 8.0, 51.0, 8.0} == negativeMultiplyAdd(a, b, c)), L"negativeMultiplyAdd", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_f64_4{-15.0, -32.0, -9.0, -72.0} == negativeMultiplySub(a, b, c)), L"negativeMultiplySub",
                LINE_INFO()
            );
            Assert::IsTrue(allTrue(v_f64_4{-9.0, 22.0, -27.0, 44.0} == addSubtract(a, c)), L"addSubtract", LINE_INFO());
        }

        TEST_METHOD(computesFlooredModulo)
        {
            // the result has the sign of b, unlike the integer modulo
            const auto result = modulo(v_f64_4{-7.0, 7.0, 7.5, 9.0}, v_f64_4{3.0, -3.0, 2.0, 3.0});
            Assert::AreEqual(2.0, x(result), 1e-12, L"modulo X mismatch", LINE_INFO());
            Assert::AreEqual(-2.0, y(result), 1e-12, L"modulo Y mismatch", LINE_INFO());
            Assert::AreEqual(1.5, z(result), 1e-12, L"modulo Z mismatch", LINE_INFO());
            Assert::AreEqual(0.0, w(result), 1e-12, L"modulo W mismatch", LINE_INFO());
        }

        TEST_METHOD(detectsNaN)
        {
            const v_f64_4 a{NotANumber<Float_64>(), 1.0, 2.0, 3.0};
            const v_f64_4 b{2.0, NotANumber<Float_64>(), 2.0, NotANumber<Float_64>()};
            Assert::IsTrue(allTrue(isNaN(a) == v_i64_4{-1LL, 0LL, 0LL, 0LL}), L"isNaN", LINE_INFO());
            Assert::IsTrue(allTrue(isNaN(a, b) == v_i64_4{-1LL, -1LL, 0LL, -1LL}), L"isNaN of either", LINE_INFO());
        }

        TEST_METHOD(testsTruthBySignBit)
        {
            Assert::IsTrue(anyTrue(v_f64_4{1.0, 1.0, 1.0, -1.0}), L"anyTrue with W set", LINE_INFO());
            Assert::IsFalse(anyTrue(v_f64_4{1.0}), L"anyTrue of positives", LINE_INFO());
            Assert::IsTrue(anyFalse(v_f64_4{-1.0, 1.0, -1.0, -1.0}), L"anyFalse of mixed", LINE_INFO());
            Assert::IsTrue(allFalse(v_f64_4{1.0}), L"allFalse of positives", LINE_INFO());
            Assert::IsFalse(allFalse(v_f64_4{1.0, -2.0, 1.0, 1.0}), L"allFalse of mixed", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_4{-1.0}), L"allTrue of negatives", LINE_INFO());
        }

        TEST_METHOD(setsAndClearsW)
        {
            const v_f64_4 given{1.0, 2.0, 3.0, 4.0};
            Assert::IsTrue(allTrue(v_f64_4{1.0, 2.0, 3.0, 0.0} == zeroW(given)), L"zeroW mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_4{1.0, 2.0, 3.0, 1.0} == oneW(given)), L"oneW mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMin3AndMax3IgnoringW)
        {
            const v_f64_4 low{3.0, 1.0, 4.0, -9.0};
            Assert::AreEqual(1.0, min3(low), L"min3 mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_4{1.0} == min3v(low)), L"min3v mismatch", LINE_INFO());
            const v_f64_4 high{3.0, 1.0, 4.0, 99.0};
            Assert::AreEqual(4.0, max3(high), L"max3 mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_4{4.0} == max3v(high)), L"max3v mismatch", LINE_INFO());
        }

        TEST_METHOD(computesThreeComponentSumDotAndLengths)
        {
            const v_f64_4 a{1.0, 2.0, 2.0, 100.0};
            Assert::AreEqual(5.0, horizontalSum3(a), L"horizontalSum3 mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_4{5.0} == horizontalSum3v(a)), L"horizontalSum3v mismatch", LINE_INFO());
            const v_f64_4 b{3.0, -1.0, 0.5, 100.0};
            Assert::AreEqual(2.0, dot3(a, b), L"dot3 mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_4{2.0} == dot3v(a, b)), L"dot3v mismatch", LINE_INFO());
            Assert::AreEqual(3.0, length3(a), 1e-12, L"length3 mismatch", LINE_INFO());
            Assert::AreEqual(3.0, x(length3v(a)), 1e-12, L"length3v X mismatch", LINE_INFO());
            Assert::AreEqual(3.0, w(length3v(a)), 1e-12, L"length3v W mismatch", LINE_INFO());
            Assert::AreEqual(1.0 / 3.0, rlength3(a), 1e-12, L"rlength3 mismatch", LINE_INFO());
            Assert::AreEqual(1.0 / 3.0, w(rlength3v(a)), 1e-12, L"rlength3v W mismatch", LINE_INFO());
        }

        TEST_METHOD(normalizesThreeComponents)
        {
            const auto n = normalize3(v_f64_4{0.0, 3.0, 4.0, 7.0});
            Assert::AreEqual(0.0, x(n), 1e-12, L"normalize3 X", LINE_INFO());
            Assert::AreEqual(0.6, y(n), 1e-12, L"normalize3 Y", LINE_INFO());
            Assert::AreEqual(0.8, z(n), 1e-12, L"normalize3 Z", LINE_INFO());
            Assert::AreEqual(0.0, w(n), L"normalize3 clears W", LINE_INFO());
        }

        TEST_METHOD(computesDistance3)
        {
            const v_f64_4 a{1.0, 2.0, 3.0, 0.0};
            const v_f64_4 b{1.0, 5.0, 7.0, 100.0};
            Assert::AreEqual(5.0, distance3(a, b), 1e-12, L"distance3 mismatch", LINE_INFO());
            Assert::AreEqual(5.0, w(distance3v(a, b)), 1e-12, L"distance3v W mismatch", LINE_INFO());
        }

        TEST_METHOD(computesCross3)
        {
            const auto xCrossY = cross3(v_f64_4{1.0, 0.0, 0.0, 0.0}, v_f64_4{0.0, 1.0, 0.0, 0.0});
            Assert::IsTrue(allTrue(v_f64_4{0.0, 0.0, 1.0, 0.0} == xCrossY), L"x cross y", LINE_INFO());
            const auto c = cross3(v_f64_4{1.0, 2.0, 3.0, 0.0}, v_f64_4{4.0, 5.0, 6.0, 0.0});
            Assert::IsTrue(allTrue(v_f64_4{-3.0, 6.0, -3.0, 0.0} == c), L"general cross3", LINE_INFO());
        }

        TEST_METHOD(computesMapOrthogonal)
        {
            const v_f64_4 v{1.0, 2.0, 3.0, 0.0};
            const v_f64_4 sPlane{1.0, 0.0, 0.0, 0.0};
            const v_f64_4 tPlane{0.0, 1.0, 1.0, 0.0};
            const auto result = mapOrthogonal(v, sPlane, tPlane);
            Assert::IsTrue(allTrue(v_f64_4{1.0, 5.0, 0.0, 1.0} == result), L"mapOrthogonal mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMapSphericalLikeFloat4)
        {
            // the same texture coordinates as v_f32_4, which is Float4 in normal precision
            for (const auto& direction : {
                     v_f32_4{0.0f, 1.0f, 0.0f, 0.0f}, v_f32_4{0.0f, -1.0f, 0.0f, 0.0f}, v_f32_4{0.0f, 0.0f, 1.0f, 0.0f},
                     v_f32_4{0.6f, 0.0f, -0.8f, 0.0f}, v_f32_4{-0.48f, 0.6f, 0.64f, 0.0f}
                 })
            {
                const auto expected = mapSpherical(direction);
                const auto actual = mapSpherical(v_f64_4{x(direction), y(direction), z(direction), w(direction)});
                Assert::AreEqual(static_cast<Float_64>(x(expected)), x(actual), 1e-6, L"mapSpherical X", LINE_INFO());
                Assert::AreEqual(static_cast<Float_64>(y(expected)), y(actual), 1e-6, L"mapSpherical Y", LINE_INFO());
                Assert::AreEqual(0.0, z(actual), L"mapSpherical Z", LINE_INFO());
                Assert::AreEqual(1.0, w(actual), L"mapSpherical W", LINE_INFO());
            }
        }

        TEST_METHOD(findsArgMinAndArgMax)
        {
            Assert::AreEqual(ASizeT{1}, argmin(v_f64_4{3.0, -1.0, 4.0, 2.0}), L"argmin mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{3}, argmin(v_f64_4{3.0, 1.0, 4.0, -2.0}), L"argmin W mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{2}, argmax(v_f64_4{3.0, 1.0, 4.0, 2.0}), L"argmax mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{0}, argmax(v_f64_4{5.0, 1.0, 4.0, 2.0}), L"argmax X mismatch", LINE_INFO());
        }

        TEST_METHOD(findsArgMin3AndArgMax3IgnoringW)
        {
            Assert::AreEqual(ASizeT{2}, argmin3(v_f64_4{3.0, 2.0, 1.0, -9.0}), L"argmin3 mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{0}, argmin3(v_f64_4{1.0, 2.0, 3.0, -9.0}), L"argmin3 X mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{1}, argmax3(v_f64_4{1.0, 3.0, 2.0, 99.0}), L"argmax3 mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{0}, argmax3(v_f64_4{4.0, 3.0, 2.0, 99.0}), L"argmax3 X mismatch", LINE_INFO());
        }

        TEST_METHOD(picksHighestIndexOfEqualExtrema)
        {
            Assert::AreEqual(ASizeT{2}, argmin(v_f64_4{1.0, 5.0, 1.0, 7.0}), L"argmin tie", LINE_INFO());
            Assert::AreEqual(ASizeT{3}, argmax(v_f64_4{7.0, 5.0, 1.0, 7.0}), L"argmax tie", LINE_INFO());
            Assert::AreEqual(ASizeT{1}, argmin3(v_f64_4{1.0, 1.0, 5.0, 0.0}), L"argmin3 tie", LINE_INFO());
            Assert::AreEqual(ASizeT{2}, argmax3(v_f64_4{5.0, 1.0, 5.0, 9.0}), L"argmax3 tie", LINE_INFO());
        }

        TEST_METHOD(testsTruth3IgnoringW)
        {
            Assert::IsTrue(anyTrue3(v_f64_4{1.0, 1.0, -1.0, 1.0}), L"anyTrue3 with Z set", LINE_INFO());
            Assert::IsFalse(anyTrue3(v_f64_4{1.0, 1.0, 1.0, -1.0}), L"anyTrue3 ignores W", LINE_INFO());
            Assert::IsTrue(anyFalse3(v_f64_4{-1.0, 1.0, -1.0, -1.0}), L"anyFalse3 with Y clear", LINE_INFO());
            Assert::IsFalse(anyFalse3(v_f64_4{-1.0, -1.0, -1.0, 1.0}), L"anyFalse3 ignores W", LINE_INFO());
            Assert::IsTrue(allTrue3(v_f64_4{-1.0, -1.0, -1.0, 1.0}), L"allTrue3 ignores W", LINE_INFO());
            Assert::IsFalse(allTrue3(v_f64_4{-1.0, 1.0, -1.0, -1.0}), L"allTrue3 with Y clear", LINE_INFO());
            Assert::IsTrue(allFalse3(v_f64_4{1.0, 1.0, 1.0, -1.0}), L"allFalse3 ignores W", LINE_INFO());
            Assert::IsFalse(allFalse3(v_f64_4{1.0, -1.0, 1.0, 1.0}), L"allFalse3 with Y set", LINE_INFO());
        }

        TEST_METHOD(computesReflect)
        {
            const v_f64_4 incident{1.0, -1.0, 0.0, 0.0};
            const v_f64_4 normal{0.0, 1.0, 0.0, 0.0};
            Assert::IsTrue(allTrue(v_f64_4{1.0, 1.0, 0.0, 0.0} == reflect(incident, normal)), L"reflect", LINE_INFO());
            Assert::IsTrue(
                allTrue(v_f64_4{1.0, 1.0, 0.0, 0.0} == reflect(incident, normal, dotv(normal, incident))),
                L"reflect with NdotI", LINE_INFO()
            );
        }

        TEST_METHOD(computesRefractEta)
        {
            // 45 degrees into a medium with eta = 0.5: sin(phiT) = 0.5 * sin(45)
            const v_f64_4 incident{Sin45<Float_64>(), -Sin45<Float_64>(), 0.0, 0.0};
            const v_f64_4 normal{0.0, 1.0, 0.0, 0.0};
            const auto t = refractEta(incident, normal, v_f64_4{0.5});
            Assert::AreEqual(0.35355339059327, x(t), 1e-12, L"refractEta X", LINE_INFO());
            Assert::AreEqual(-0.93541434669349, y(t), 1e-12, L"refractEta Y", LINE_INFO());
            Assert::AreEqual(0.0, z(t), 1e-12, L"refractEta Z", LINE_INFO());
            // sin(phiT) = 2 * sin(45) > 1: total internal reflection
            const auto reflected = refractEta(incident, normal, v_f64_4{2.0});
            Assert::IsTrue(allTrue(Zero<v_f64_4>() == reflected), L"total internal reflection", LINE_INFO());
        }

        TEST_METHOD(computesRefractPickingEtaByOrientation)
        {
            // eta for entering in X, for leaving in Y; Z and W must not be used
            const v_f64_4 eta{0.5, 1.2, 9.0, 9.0};
            const v_f64_4 normal{0.0, 1.0, 0.0, 0.0};
            const auto in = refract(v_f64_4{Sin45<Float_64>(), -Sin45<Float_64>(), 0.0, 0.0}, normal, eta);
            Assert::AreEqual(0.35355339059327, x(in), 1e-12, L"entering X", LINE_INFO());
            Assert::AreEqual(-0.93541434669349, y(in), 1e-12, L"entering Y", LINE_INFO());
            const auto out = refract(v_f64_4{Sin45<Float_64>(), Sin45<Float_64>(), 0.0, 0.0}, normal, eta);
            Assert::AreEqual(0.84852813742386, x(out), 1e-12, L"leaving X", LINE_INFO());
            Assert::AreEqual(0.52915026221292, y(out), 1e-12, L"leaving Y", LINE_INFO());
        }

        TEST_METHOD(computesRefractWithCullingOrientation)
        {
            const v_f64_4 eta{0.5, 1.2, 9.0, 9.0};
            const v_f64_4 normal{0.0, 1.0, 0.0, 0.0};
            const v_f64_4 entering{Sin45<Float_64>(), -Sin45<Float_64>(), 0.0, 0.0};
            const v_f64_4 leaving{Sin45<Float_64>(), Sin45<Float_64>(), 0.0, 0.0};
            const v_i64_4 frontFaces{-1LL, 0LL, 0LL, 0LL};
            const v_i64_4 backFaces{0LL, -1LL, 0LL, 0LL};
            Assert::AreEqual(-0.93541434669349, y(refract(frontFaces, entering, normal, eta)), 1e-12, L"front", LINE_INFO());
            Assert::AreEqual(0.52915026221292, y(refract(backFaces, leaving, normal, eta)), 1e-12, L"back", LINE_INFO());
            Assert::AreEqual(
                -0.93541434669349, y(refract(Zero<v_i64_4>(), entering, normal, eta)), 1e-12, L"no culling", LINE_INFO()
            );
        }
    };
}
