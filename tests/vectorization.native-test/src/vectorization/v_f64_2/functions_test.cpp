#include "vectorization_tests.h"

namespace vectorization::test
{
    TEST_CLASS (v_f64_2_FunctionsTest)
    {
        public
        :
        TEST_METHOD(computesMinAndMax)
        {
            const v_f64_2 a{1.0, 4.0};
            const v_f64_2 b{3.0, 2.0};

            const auto mn = min(a, b);
            Assert::AreEqual(1.0, x(mn), L"min X mismatch", LINE_INFO());
            Assert::AreEqual(2.0, y(mn), L"min Y mismatch", LINE_INFO());

            const auto mx = max(a, b);
            Assert::AreEqual(3.0, x(mx), L"max X mismatch", LINE_INFO());
            Assert::AreEqual(4.0, y(mx), L"max Y mismatch", LINE_INFO());

            // scalar reductions
            const v_f64_2 v{5.0, 2.0};
            Assert::AreEqual(2.0, min(v), L"min(v) mismatch", LINE_INFO());
            Assert::AreEqual(5.0, max(v), L"max(v) mismatch", LINE_INFO());
        }

        TEST_METHOD(computesHorizontalSumAndDot)
        {
            const v_f64_2 v{1.0, 2.0};
            Assert::AreEqual(3.0, horizontalSum(v), L"horizontalSum mismatch", LINE_INFO());

            const v_f64_2 a{2.0, 3.0};
            const v_f64_2 b{4.0, 5.0};
            // dot = 2*4 + 3*5 = 8+15 = 23
            Assert::AreEqual(23.0, dot(a, b), L"dot mismatch", LINE_INFO());
        }

        TEST_METHOD(computesLengthNormalizeAndDistance)
        {
            // length({3,4}) = sqrt(9+16) = 5
            const v_f64_2 v{3.0, 4.0};
            Assert::AreEqual(5.0, length(v), 1e-10, L"length mismatch", LINE_INFO());

            // normalize
            const auto n = normalize(v);
            Assert::AreEqual(0.6, x(n), 1e-10, L"normalize X mismatch", LINE_INFO());
            Assert::AreEqual(0.8, y(n), 1e-10, L"normalize Y mismatch", LINE_INFO());

            // distance
            const v_f64_2 a{1.0, 1.0};
            const v_f64_2 b{4.0, 5.0};
            // distance = length({3,4}) = 5
            Assert::AreEqual(5.0, distance(a, b), 1e-10, L"distance mismatch", LINE_INFO());
        }

        TEST_METHOD(computesSqrtReciprocalAndAbs)
        {
            const v_f64_2 v{4.0, 9.0};
            const auto s = sqrt(v);
            Assert::AreEqual(2.0, x(s), 1e-10, L"sqrt X mismatch", LINE_INFO());
            Assert::AreEqual(3.0, y(s), 1e-10, L"sqrt Y mismatch", LINE_INFO());

            const auto rec = reciprocal(v_f64_2{2.0, 4.0});
            Assert::AreEqual(0.5, x(rec), 1e-10, L"reciprocal X mismatch", LINE_INFO());
            Assert::AreEqual(0.25, y(rec), 1e-10, L"reciprocal Y mismatch", LINE_INFO());

            const v_f64_2 neg{-3.0, -7.0};
            const auto a = abs(neg);
            Assert::AreEqual(3.0, x(a), L"abs X mismatch", LINE_INFO());
            Assert::AreEqual(7.0, y(a), L"abs Y mismatch", LINE_INFO());
        }

        TEST_METHOD(computesSqr)
        {
            const v_f64_2 v{3.0, -5.0};
            const auto s = sqr(v);
            Assert::AreEqual(9.0, x(s), L"sqr X mismatch", LINE_INFO());
            Assert::AreEqual(25.0, y(s), L"sqr Y mismatch", LINE_INFO());

            const auto z = sqr(v_f64_2{0.0, 0.5});
            Assert::AreEqual(0.0, x(z), L"sqr zero", LINE_INFO());
            Assert::AreEqual(0.25, y(z), L"sqr 0.5", LINE_INFO());
        }

        TEST_METHOD(computesClampAndMix)
        {
            const v_f64_2 v{-1.0, 5.0};
            const v_f64_2 lower{0.0, 0.0};
            const v_f64_2 upper{3.0, 3.0};
            const auto clamped = clamp(v, lower, upper);
            Assert::AreEqual(0.0, x(clamped), L"clamp X mismatch", LINE_INFO());
            Assert::AreEqual(3.0, y(clamped), L"clamp Y mismatch", LINE_INFO());

            // mix(a, b, 0.5) = midpoint
            const v_f64_2 a{0.0, 2.0};
            const v_f64_2 b{4.0, 6.0};
            const auto mid = mix(a, b, 0.5);
            Assert::AreEqual(2.0, x(mid), 1e-10, L"mix X mismatch", LINE_INFO());
            Assert::AreEqual(4.0, y(mid), 1e-10, L"mix Y mismatch", LINE_INFO());
        }

        TEST_METHOD(computesIsNegativeAndPredicates)
        {
            const v_f64_2 v{-1.0, 2.0};
            const auto mask = isNegative(v);
            // X negative (bit 0) => mask should have bit 0 set
            Assert::IsTrue((mask & static_cast<v_f64_2::BoolType>(VectorBits::X)) != 0, L"isNegative X mismatch",
                           LINE_INFO());

            const v_f64_2 allOnes{1.0, 1.0};
            const auto cmpResult = allOnes == allOnes;
            Assert::IsTrue(allTrue(cmpResult), L"allTrue should be true for equal vectors", LINE_INFO());
            Assert::IsFalse(anyFalse(cmpResult), L"anyFalse should be false for equal vectors", LINE_INFO());

            const auto neq = allOnes != v_f64_2{1.0, 2.0};
            Assert::IsTrue(anyTrue(neq), L"anyTrue should detect Y mismatch", LINE_INFO());
        }

        TEST_METHOD(computesCeilFloorRoundFract)
        {
            const v_f64_2 v{1.3, -0.7};

            const auto c = ceil(v);
            Assert::AreEqual(2.0, x(c), L"ceil X mismatch", LINE_INFO());
            Assert::AreEqual(0.0, y(c), L"ceil Y mismatch", LINE_INFO());

            const auto f = floor(v);
            Assert::AreEqual(1.0, x(f), L"floor X mismatch", LINE_INFO());
            Assert::AreEqual(-1.0, y(f), L"floor Y mismatch", LINE_INFO());

            const auto r = round(v_f64_2{1.5, 2.7});
            Assert::AreEqual(2.0, x(r), 1e-10, L"round X mismatch", LINE_INFO());
            Assert::AreEqual(3.0, y(r), 1e-10, L"round Y mismatch", LINE_INFO());

            // fract = v - floor(v)
            const auto fr = fract(v_f64_2{1.75, 2.25});
            Assert::AreEqual(0.75, x(fr), 1e-10, L"fract X mismatch", LINE_INFO());
            Assert::AreEqual(0.25, y(fr), 1e-10, L"fract Y mismatch", LINE_INFO());
        }

        TEST_METHOD(computesVectorReturnVariants)
        {
            const v_f64_2 given{-3.0, 4.0};
            Assert::IsTrue(allTrue(v_f64_2{-3.0} == minv(given)), L"minv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_2{4.0} == maxv(given)), L"maxv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_2{1.0} == horizontalSumv(given)), L"horizontalSumv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_2{5.0} == lengthv(given)), L"lengthv mismatch", LINE_INFO());
            Assert::AreEqual(0.2, rlength(given), 1e-12, L"rlength mismatch", LINE_INFO());
            Assert::AreEqual(0.2, y(rlengthv(given)), 1e-12, L"rlengthv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_2{5.0} == distancev(given, Zero<v_f64_2>())), L"distancev", LINE_INFO());
            Assert::AreEqual(0.5, x(rsqrt(v_f64_2{4.0, 16.0})), 1e-12, L"rsqrt X mismatch", LINE_INFO());
            Assert::AreEqual(0.25, y(rsqrt(v_f64_2{4.0, 16.0})), 1e-12, L"rsqrt Y mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMultiplyAddFamily)
        {
            const v_f64_2 a{1.0, 2.0};
            const v_f64_2 b{5.0, -6.0};
            const v_f64_2 c{10.0, 20.0};
            // a * b = {5, -12}
            Assert::IsTrue(allTrue(v_f64_2{15.0, 8.0} == multiplyAdd(a, b, c)), L"multiplyAdd", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_2{-5.0, -32.0} == multiplySub(a, b, c)), L"multiplySub", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_2{-5.0, 8.0} == multiplyAddSub(a, b, c)), L"multiplyAddSub", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_2{15.0, -32.0} == multiplySubAdd(a, b, c)), L"multiplySubAdd", LINE_INFO());
            Assert::IsTrue(
                allTrue(v_f64_2{5.0, 32.0} == negativeMultiplyAdd(a, b, c)), L"negativeMultiplyAdd", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_f64_2{-15.0, -8.0} == negativeMultiplySub(a, b, c)), L"negativeMultiplySub", LINE_INFO()
            );
            Assert::IsTrue(allTrue(v_f64_2{-9.0, 22.0} == addSubtract(a, c)), L"addSubtract", LINE_INFO());
        }

        TEST_METHOD(computesFlooredModulo)
        {
            // the result has the sign of b, unlike the integer modulo
            const auto result = modulo(v_f64_2{-7.0, 7.0}, v_f64_2{3.0, -3.0});
            Assert::AreEqual(2.0, x(result), 1e-12, L"modulo X mismatch", LINE_INFO());
            Assert::AreEqual(-2.0, y(result), 1e-12, L"modulo Y mismatch", LINE_INFO());
        }

        TEST_METHOD(detectsNaN)
        {
            const v_f64_2 a{NotANumber<Float_64>(), 1.0};
            const v_f64_2 b{2.0, NotANumber<Float_64>()};
            Assert::AreEqual(std::numeric_limits<UInt_64>::max(), x(isNaN(a)), L"isNaN X", LINE_INFO());
            Assert::AreEqual(UInt_64{0}, y(isNaN(a)), L"isNaN Y", LINE_INFO());
            Assert::IsTrue(allTrue(isNaN(a, b)), L"isNaN of either", LINE_INFO());
            Assert::IsTrue(allFalse(isNaN(Zero<v_f64_2>(), One<v_f64_2>())), L"isNaN of neither", LINE_INFO());
        }

        TEST_METHOD(testsTruthBySignBit)
        {
            Assert::IsTrue(anyTrue(v_f64_2{1.0, -1.0}), L"anyTrue of mixed", LINE_INFO());
            Assert::IsTrue(anyFalse(v_f64_2{1.0, -1.0}), L"anyFalse of mixed", LINE_INFO());
            Assert::IsTrue(allFalse(v_f64_2{1.0, 2.0}), L"allFalse of positives", LINE_INFO());
            Assert::IsFalse(allFalse(v_f64_2{1.0, -2.0}), L"allFalse of mixed", LINE_INFO());
            Assert::IsTrue(allTrue(v_f64_2{-1.0, -2.0}), L"allTrue of negatives", LINE_INFO());
        }

        TEST_METHOD(findsArgMinAndArgMax)
        {
            const v_f64_2 given{-5.0, 10.0};
            Assert::AreEqual(ASizeT{0}, argmin(given), L"argmin mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{1}, argmax(given), L"argmax mismatch", LINE_INFO());
            const v_f64_2 swapped{10.0, -5.0};
            Assert::AreEqual(ASizeT{1}, argmin(swapped), L"argmin Y mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{0}, argmax(swapped), L"argmax X mismatch", LINE_INFO());
        }

        TEST_METHOD(picksHighestIndexOfEqualExtrema)
        {
            const v_f64_2 given{4.0};
            Assert::AreEqual(ASizeT{1}, argmin(given), L"argmin tie", LINE_INFO());
            Assert::AreEqual(ASizeT{1}, argmax(given), L"argmax tie", LINE_INFO());
        }

        TEST_METHOD(computesReflect)
        {
            const v_f64_2 incident{1.0, -1.0};
            const v_f64_2 normal{0.0, 1.0};
            const auto r = reflect(incident, normal);
            Assert::AreEqual(1.0, x(r), 1e-12, L"reflect X", LINE_INFO());
            Assert::AreEqual(1.0, y(r), 1e-12, L"reflect Y", LINE_INFO());
            const auto precomputed = reflect(incident, normal, dotv(normal, incident));
            Assert::AreEqual(1.0, y(precomputed), 1e-12, L"reflect with NdotI Y", LINE_INFO());
        }

        TEST_METHOD(computesRefractEta)
        {
            // 45 degrees into a medium with eta = 0.5: sin(phiT) = 0.5 * sin(45)
            const v_f64_2 incident{Sin45<Float_64>(), -Sin45<Float_64>()};
            const v_f64_2 normal{0.0, 1.0};
            const auto t = refractEta(incident, normal, v_f64_2{0.5});
            Assert::AreEqual(0.35355339059327, x(t), 1e-12, L"refractEta X", LINE_INFO());
            Assert::AreEqual(-0.93541434669349, y(t), 1e-12, L"refractEta Y", LINE_INFO());
            // sin(phiT) = 2 * sin(45) > 1: total internal reflection
            const auto reflected = refractEta(incident, normal, v_f64_2{2.0});
            Assert::IsTrue(allTrue(Zero<v_f64_2>() == reflected), L"total internal reflection", LINE_INFO());
        }

        TEST_METHOD(computesRefractPickingEtaByOrientation)
        {
            // eta for entering in X, for leaving in Y
            const v_f64_2 eta{0.5, 1.2};
            const v_f64_2 normal{0.0, 1.0};
            const auto in = refract(v_f64_2{Sin45<Float_64>(), -Sin45<Float_64>()}, normal, eta);
            Assert::AreEqual(0.35355339059327, x(in), 1e-12, L"entering X", LINE_INFO());
            Assert::AreEqual(-0.93541434669349, y(in), 1e-12, L"entering Y", LINE_INFO());
            const auto out = refract(v_f64_2{Sin45<Float_64>(), Sin45<Float_64>()}, normal, eta);
            Assert::AreEqual(0.84852813742386, x(out), 1e-12, L"leaving X", LINE_INFO());
            Assert::AreEqual(0.52915026221292, y(out), 1e-12, L"leaving Y", LINE_INFO());
        }

        TEST_METHOD(computesRefractWithCullingOrientation)
        {
            const v_f64_2 eta{0.5, 1.2};
            const v_f64_2 normal{0.0, 1.0};
            const v_f64_2 entering{Sin45<Float_64>(), -Sin45<Float_64>()};
            const v_f64_2 leaving{Sin45<Float_64>(), Sin45<Float_64>()};
            const auto ones = std::numeric_limits<UInt_64>::max();
            const v_ui64_2 frontFaces{ones, UInt_64{0}};
            const v_ui64_2 backFaces{UInt_64{0}, ones};
            Assert::AreEqual(-0.93541434669349, y(refract(frontFaces, entering, normal, eta)), 1e-12, L"front", LINE_INFO());
            Assert::AreEqual(0.52915026221292, y(refract(backFaces, leaving, normal, eta)), 1e-12, L"back", LINE_INFO());
            Assert::AreEqual(
                -0.93541434669349, y(refract(Zero<v_ui64_2>(), entering, normal, eta)), 1e-12, L"no culling", LINE_INFO()
            );
        }
    };
}
