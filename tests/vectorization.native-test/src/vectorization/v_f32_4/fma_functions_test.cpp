#include "vectorization_tests.h"

namespace vectorization::test
{
  TEST_CLASS(V_f32_4_FmaFunctionsTest)
  {
  public:
    TEST_METHOD(multiplyAddComputesATimesBPlusC)
    {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{4.0f, 3.0f, 2.0f, 1.0f};
      const v_f32_4 c{10.0f, 10.0f, 10.0f, 10.0f};
      const auto r = multiplyAdd(a, b, c);
      Assert::AreEqual(14.0f, x(r), L"1*4+10", LINE_INFO());
      Assert::AreEqual(16.0f, y(r), L"2*3+10", LINE_INFO());
      Assert::AreEqual(16.0f, z(r), L"3*2+10", LINE_INFO());
      Assert::AreEqual(14.0f, w(r), L"4*1+10", LINE_INFO());
    }

    TEST_METHOD(multiplySubComputesATimesBMinusC)
    {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{4.0f, 3.0f, 2.0f, 1.0f};
      const v_f32_4 c{10.0f, 10.0f, 10.0f, 10.0f};
      const auto r = multiplySub(a, b, c);
      Assert::AreEqual(-6.0f, x(r), L"1*4-10", LINE_INFO());
      Assert::AreEqual(-4.0f, y(r), L"2*3-10", LINE_INFO());
      Assert::AreEqual(-4.0f, z(r), L"3*2-10", LINE_INFO());
      Assert::AreEqual(-6.0f, w(r), L"4*1-10", LINE_INFO());
    }

    TEST_METHOD(multiplyAddSubAlternatesSubAdd)
    {
      const v_f32_4 a{2.0f, 2.0f, 2.0f, 2.0f};
      const v_f32_4 b{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 c{0.5f, 0.5f, 0.5f, 0.5f};
      const auto r = multiplyAddSub(a, b, c);
      // even=sub, odd=add: {2*1-0.5, 2*2+0.5, 2*3-0.5, 2*4+0.5}
      Assert::AreEqual(1.5f, x(r), L"x: 2-0.5", LINE_INFO());
      Assert::AreEqual(4.5f, y(r), L"y: 4+0.5", LINE_INFO());
      Assert::AreEqual(5.5f, z(r), L"z: 6-0.5", LINE_INFO());
      Assert::AreEqual(8.5f, w(r), L"w: 8+0.5", LINE_INFO());
    }

    TEST_METHOD(multiplySubAddAlternatesAddSub)
    {
      const v_f32_4 a{2.0f, 2.0f, 2.0f, 2.0f};
      const v_f32_4 b{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 c{0.5f, 0.5f, 0.5f, 0.5f};
      const auto r = multiplySubAdd(a, b, c);
      // even=add, odd=sub: {2*1+0.5, 2*2-0.5, 2*3+0.5, 2*4-0.5}
      Assert::AreEqual(2.5f, x(r), L"x: 2+0.5", LINE_INFO());
      Assert::AreEqual(3.5f, y(r), L"y: 4-0.5", LINE_INFO());
      Assert::AreEqual(6.5f, z(r), L"z: 6+0.5", LINE_INFO());
      Assert::AreEqual(7.5f, w(r), L"w: 8-0.5", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplyAddComputesCMinusATimesB)
    {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{4.0f, 3.0f, 2.0f, 1.0f};
      const v_f32_4 c{10.0f, 10.0f, 10.0f, 10.0f};
      const auto r = negativeMultiplyAdd(a, b, c);
      Assert::AreEqual(6.0f, x(r), L"10-1*4", LINE_INFO());
      Assert::AreEqual(4.0f, y(r), L"10-2*3", LINE_INFO());
      Assert::AreEqual(4.0f, z(r), L"10-3*2", LINE_INFO());
      Assert::AreEqual(6.0f, w(r), L"10-4*1", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplySubComputesNegATimesBMinusC)
    {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{4.0f, 3.0f, 2.0f, 1.0f};
      const v_f32_4 c{10.0f, 10.0f, 10.0f, 10.0f};
      const auto r = negativeMultiplySub(a, b, c);
      // -(a*b)-c: {-4-10, -6-10, -6-10, -4-10}
      Assert::AreEqual(-14.0f, x(r), L"-(1*4)-10", LINE_INFO());
      Assert::AreEqual(-16.0f, y(r), L"-(2*3)-10", LINE_INFO());
      Assert::AreEqual(-16.0f, z(r), L"-(3*2)-10", LINE_INFO());
      Assert::AreEqual(-14.0f, w(r), L"-(4*1)-10", LINE_INFO());
    }

    TEST_METHOD(multiplyAddWithSignedZero)
    {
      const v_f32_4 a{1.0f, -1.0f, 0.0f, -0.0f};
      const v_f32_4 b{0.0f, 0.0f, 1.0f, 1.0f};
      const v_f32_4 c = Zero<v_f32_4>();
      const auto r = multiplyAdd(a, b, c);
      Assert::AreEqual(0.0f, x(r), L"1*0+0=0", LINE_INFO());
      Assert::AreEqual(0.0f, y(r), L"-1*0+0=0 (may be -0)", LINE_INFO());
      Assert::AreEqual(0.0f, z(r), L"0*1+0=0", LINE_INFO());
      Assert::AreEqual(0.0f, w(r), L"-0*1+0=0 (may be -0)", LINE_INFO());
    }

    TEST_METHOD(multiplySubWithSignedZero)
    {
      const v_f32_4 a{2.0f, 2.0f, 2.0f, 2.0f};
      const v_f32_4 b{3.0f, 3.0f, 3.0f, 3.0f};
      const v_f32_4 c = NegativeZero<v_f32_4>();
      const auto r = multiplySub(a, b, c);
      // 2*3 - (-0) = 6.0 (IEEE: subtracting -0 gives +0 result)
      Assert::AreEqual(6.0f, x(r), L"2*3-(-0)", LINE_INFO());
    }
  };

  TEST_CLASS(FmaCallSiteRegressionTest)
  {
  public:
    TEST_METHOD(cross3ComputesCorrectResult)
    {
      const v_f32_4 a{1.0f, 0.0f, 0.0f, 0.0f};
      const v_f32_4 b{0.0f, 1.0f, 0.0f, 0.0f};
      const auto r = cross3(a, b);
      Assert::AreEqual(0.0f, x(r), L"cross3 x", LINE_INFO());
      Assert::AreEqual(0.0f, y(r), L"cross3 y", LINE_INFO());
      Assert::AreEqual(1.0f, z(r), L"cross3 z", LINE_INFO());
    }

    TEST_METHOD(cross3IsAntiCommutative)
    {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 0.0f};
      const v_f32_4 b{4.0f, 5.0f, 6.0f, 0.0f};
      const auto ab = cross3(a, b);
      const auto ba = cross3(b, a);
      Assert::AreEqual(x(ab), -x(ba), L"cross3 anti-commutative x", LINE_INFO());
      Assert::AreEqual(y(ab), -y(ba), L"cross3 anti-commutative y", LINE_INFO());
      Assert::AreEqual(z(ab), -z(ba), L"cross3 anti-commutative z", LINE_INFO());
    }

    TEST_METHOD(cross3OfParallelVectorsIsZero)
    {
      const v_f32_4 a{2.0f, 4.0f, 6.0f, 0.0f};
      const v_f32_4 b{1.0f, 2.0f, 3.0f, 0.0f};
      const auto r = cross3(a, b);
      Assert::AreEqual(0.0f, x(r), L"parallel cross3 x", LINE_INFO());
      Assert::AreEqual(0.0f, y(r), L"parallel cross3 y", LINE_INFO());
      Assert::AreEqual(0.0f, z(r), L"parallel cross3 z", LINE_INFO());
    }

    TEST_METHOD(reflectComputesCorrectResult)
    {
      const v_f32_4 incident{1.0f, -1.0f, 0.0f, 0.0f};
      const v_f32_4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const auto r = reflect(incident, normal);
      // reflect = incident - 2 * dot(normal, incident) * normal
      // dot(normal, incident) = -1, so reflect = {1,-1,0,0} - 2*(-1)*{0,1,0,0} = {1,1,0,0}
      Assert::AreEqual(1.0f, x(r), L"reflect x", LINE_INFO());
      Assert::AreEqual(1.0f, y(r), L"reflect y", LINE_INFO());
      Assert::AreEqual(0.0f, z(r), L"reflect z", LINE_INFO());
    }

    TEST_METHOD(reflectAt45Degrees)
    {
      const auto incident = normalize(v_f32_4{1.0f, -1.0f, 0.0f, 0.0f});
      const v_f32_4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const auto r = reflect(incident, normal);
      const auto expected = normalize(v_f32_4{1.0f, 1.0f, 0.0f, 0.0f});
      Assert::AreEqual(x(expected), x(r), 1e-6f, L"reflect 45 x", LINE_INFO());
      Assert::AreEqual(y(expected), y(r), 1e-6f, L"reflect 45 y", LINE_INFO());
      Assert::AreEqual(z(expected), z(r), 1e-6f, L"reflect 45 z", LINE_INFO());
    }

    TEST_METHOD(mixInterpolatesLinearly)
    {
      const v_f32_4 a{0.0f, 10.0f, 20.0f, 30.0f};
      const v_f32_4 b{100.0f, 110.0f, 120.0f, 130.0f};
      const auto r = mix(a, b, 0.25f);
      Assert::AreEqual(25.0f, x(r), L"mix 0.25 x", LINE_INFO());
      Assert::AreEqual(35.0f, y(r), L"mix 0.25 y", LINE_INFO());
      Assert::AreEqual(45.0f, z(r), L"mix 0.25 z", LINE_INFO());
      Assert::AreEqual(55.0f, w(r), L"mix 0.25 w", LINE_INFO());
    }

    TEST_METHOD(mixAtZeroReturnsA)
    {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{10.0f, 20.0f, 30.0f, 40.0f};
      const auto r = mix(a, b, 0.0f);
      Assert::AreEqual(1.0f, x(r), L"mix 0 x", LINE_INFO());
      Assert::AreEqual(2.0f, y(r), L"mix 0 y", LINE_INFO());
      Assert::AreEqual(3.0f, z(r), L"mix 0 z", LINE_INFO());
      Assert::AreEqual(4.0f, w(r), L"mix 0 w", LINE_INFO());
    }

    TEST_METHOD(mixAtOneReturnsB)
    {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{10.0f, 20.0f, 30.0f, 40.0f};
      const auto r = mix(a, b, 1.0f);
      Assert::AreEqual(10.0f, x(r), L"mix 1 x", LINE_INFO());
      Assert::AreEqual(20.0f, y(r), L"mix 1 y", LINE_INFO());
      Assert::AreEqual(30.0f, z(r), L"mix 1 z", LINE_INFO());
      Assert::AreEqual(40.0f, w(r), L"mix 1 w", LINE_INFO());
    }
  };
}
