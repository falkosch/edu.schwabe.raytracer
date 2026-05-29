#include "vectorization_tests.h"

namespace vectorization::test
{
    TEST_CLASS (MultiplyAddTest)
    {
        public
        :
        TEST_METHOD(multiplyAddsFloat32)
        {
            Assert::AreEqual(14.0f, multiplyAdd(2.0f, 3.0f, 8.0f), L"2*3+8", LINE_INFO());
            Assert::AreEqual(0.5f, multiplyAdd(0.0f, 5.0f, 0.5f), L"0*5+0.5", LINE_INFO());
            Assert::AreEqual(-1.0f, multiplyAdd(2.0f, 3.0f, -7.0f), L"2*3+(-7)", LINE_INFO());
        }

        TEST_METHOD(multiplyAddsFloat64)
        {
            Assert::AreEqual(14.0, multiplyAdd(2.0, 3.0, 8.0), L"2*3+8", LINE_INFO());
            Assert::AreEqual(0.5, multiplyAdd(0.0, 5.0, 0.5), L"0*5+0.5", LINE_INFO());
            Assert::AreEqual(-1.0, multiplyAdd(2.0, 3.0, -7.0), L"2*3+(-7)", LINE_INFO());
        }

        TEST_METHOD(multiplyAddsPackedFloat4)
        {
            const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
            const auto b = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
            const auto c = _mm_set_ps(10.0f, 10.0f, 10.0f, 10.0f);
            std::array<Float_32, VectorSizes::W> actual{};
            _mm_store_ps(actual.data(), multiplyAdd(a, b, c));
            // a*b+c: {1*4+10, 2*3+10, 3*2+10, 4*1+10} = {14, 16, 16, 14}
            Assert::AreEqual(14.0f, actual[0], L"x lane", LINE_INFO());
            Assert::AreEqual(16.0f, actual[1], L"y lane", LINE_INFO());
            Assert::AreEqual(16.0f, actual[2], L"z lane", LINE_INFO());
            Assert::AreEqual(14.0f, actual[3], L"w lane", LINE_INFO());
        }

        TEST_METHOD(multiplyAddsPackedFloat2)
        {
            const auto a = _mm_set_pd(3.0, 2.0);
            const auto b = _mm_set_pd(4.0, 5.0);
            const auto c = _mm_set_pd(1.0, 1.0);
            std::array<Float_64, VectorSizes::X2> actual{};
            _mm_store_pd(actual.data(), multiplyAdd(a, b, c));
            Assert::AreEqual(11.0, actual[0], L"x lane", LINE_INFO());
            Assert::AreEqual(13.0, actual[1], L"y lane", LINE_INFO());
        }

        TEST_METHOD(multiplyAddsPackedFloat8)
        {
            const auto a = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
            const auto b = _mm256_set1_ps(2.0f);
            const auto c = _mm256_set1_ps(0.5f);
            alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
            _mm256_store_ps(actual.data(), multiplyAdd(a, b, c));
            for (ASizeT i = 0; i < actual.size(); ++i)
            {
                Assert::AreEqual(static_cast<Float_32>((i + 1) * 2) + 0.5f, actual[i], L"lane mismatch", LINE_INFO());
            }
        }

        TEST_METHOD(multiplyAddsPackedFloat4_256)
        {
            const auto a = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
            const auto b = _mm256_set1_pd(3.0);
            const auto c = _mm256_set1_pd(0.25);
            alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
            _mm256_store_pd(actual.data(), multiplyAdd(a, b, c));
            Assert::AreEqual(3.25, actual[0], L"x lane", LINE_INFO());
            Assert::AreEqual(6.25, actual[1], L"y lane", LINE_INFO());
            Assert::AreEqual(9.25, actual[2], L"z lane", LINE_INFO());
            Assert::AreEqual(12.25, actual[3], L"w lane", LINE_INFO());
        }

        TEST_METHOD(multiplyAddXOperatesOnXLaneOnly)
        {
            const auto a = _mm_set_ps(40.0f, 30.0f, 20.0f, 2.0f);
            const auto b = _mm_set_ps(4.0f, 3.0f, 2.0f, 3.0f);
            const auto c = _mm_set_ps(1.0f, 1.0f, 1.0f, 0.5f);
            std::array<Float_32, VectorSizes::W> actual{};
            _mm_store_ps(actual.data(), multiplyAddX(a, b, c));
            Assert::AreEqual(6.5f, actual[0], L"x = a.x*b.x+c.x = 2*3+0.5", LINE_INFO());
            Assert::AreEqual(20.0f, actual[1], L"y pass through from a", LINE_INFO());
            Assert::AreEqual(30.0f, actual[2], L"z pass through from a", LINE_INFO());
            Assert::AreEqual(40.0f, actual[3], L"w pass through from a", LINE_INFO());
        }

        TEST_METHOD(multiplyAddXFloat64OperatesOnXLaneOnly)
        {
            const auto a = _mm_set_pd(20.0, 3.0);
            const auto b = _mm_set_pd(2.0, 4.0);
            const auto c = _mm_set_pd(1.0, 0.5);
            std::array<Float_64, VectorSizes::X2> actual{};
            _mm_store_pd(actual.data(), multiplyAddX(a, b, c));
            Assert::AreEqual(12.5, actual[0], L"x = 3*4+0.5", LINE_INFO());
            Assert::AreEqual(20.0, actual[1], L"y pass through from a", LINE_INFO());
        }

        TEST_METHOD(multiplyAddWithZeroC)
        {
            const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
            const auto b = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
            const auto c = _mm_setzero_ps();
            std::array<Float_32, VectorSizes::W> actual{};
            _mm_store_ps(actual.data(), multiplyAdd(a, b, c));
            Assert::AreEqual(4.0f, actual[0], L"1*4+0", LINE_INFO());
            Assert::AreEqual(6.0f, actual[1], L"2*3+0", LINE_INFO());
            Assert::AreEqual(6.0f, actual[2], L"3*2+0", LINE_INFO());
            Assert::AreEqual(4.0f, actual[3], L"4*1+0", LINE_INFO());
        }

        TEST_METHOD(multiplyAddWithInfinity)
        {
            const auto a = _mm_set_ps1(Infinity<Float_32>());
            const auto b = _mm_set_ps1(Zero<Float_32>());
            const auto c = _mm_set_ps1(1.0f);
            std::array<Float_32, VectorSizes::W> actual{};
            _mm_store_ps(actual.data(), multiplyAdd(a, b, c));
            for (ASizeT i = 0; i < actual.size(); ++i)
            {
                Assert::IsTrue(std::isnan(actual[i]), L"inf*0+1 should be NaN", LINE_INFO());
            }
        }
    };
}
