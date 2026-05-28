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
    };
}
