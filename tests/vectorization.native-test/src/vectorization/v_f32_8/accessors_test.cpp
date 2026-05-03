#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_8_AccessorsTest) {
  public:
    TEST_METHOD(componentExtractsAllLanes) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      Assert::AreEqual(1.0f, component<VectorIndices::X1>(v), L"X1", LINE_INFO());
      Assert::AreEqual(2.0f, component<VectorIndices::X2>(v), L"X2", LINE_INFO());
      Assert::AreEqual(3.0f, component<VectorIndices::X3>(v), L"X3", LINE_INFO());
      Assert::AreEqual(4.0f, component<VectorIndices::X4>(v), L"X4", LINE_INFO());
      Assert::AreEqual(5.0f, component<VectorIndices::X5>(v), L"X5", LINE_INFO());
      Assert::AreEqual(6.0f, component<VectorIndices::X6>(v), L"X6", LINE_INFO());
      Assert::AreEqual(7.0f, component<VectorIndices::X7>(v), L"X7", LINE_INFO());
      Assert::AreEqual(8.0f, component<VectorIndices::X8>(v), L"X8", LINE_INFO());
    }

    TEST_METHOD(namedAccessorsMatchComponent) {
      const v_f32_8 v(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      Assert::AreEqual(10.0f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(20.0f, x2(v), L"x2", LINE_INFO());
      Assert::AreEqual(30.0f, x3(v), L"x3", LINE_INFO());
      Assert::AreEqual(40.0f, x4(v), L"x4", LINE_INFO());
      Assert::AreEqual(50.0f, x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(60.0f, x6(v), L"x6", LINE_INFO());
      Assert::AreEqual(70.0f, x7(v), L"x7", LINE_INFO());
      Assert::AreEqual(80.0f, x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(replaceComponentReplacesCorrectLane) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r1 = replaceComponent<VectorIndices::X1>(v, 99.0f);
      Assert::AreEqual(99.0f, x1(r1), L"replaced X1", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r1), L"X2 unchanged", LINE_INFO());

      const auto r5 = replaceComponent<VectorIndices::X5>(v, 99.0f);
      Assert::AreEqual(99.0f, x5(r5), L"replaced X5", LINE_INFO());
      Assert::AreEqual(4.0f, x4(r5), L"X4 unchanged", LINE_INFO());

      const auto r8 = replaceComponent<VectorIndices::X8>(v, 99.0f);
      Assert::AreEqual(99.0f, x8(r8), L"replaced X8", LINE_INFO());
      Assert::AreEqual(7.0f, x7(r8), L"X7 unchanged", LINE_INFO());
    }

    TEST_METHOD(namedReplaceMatchesTemplate) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      Assert::AreEqual(99.0f, x1(replaceX1(v, 99.0f)), L"replaceX1", LINE_INFO());
      Assert::AreEqual(99.0f, x4(replaceX4(v, 99.0f)), L"replaceX4", LINE_INFO());
      Assert::AreEqual(99.0f, x5(replaceX5(v, 99.0f)), L"replaceX5", LINE_INFO());
      Assert::AreEqual(99.0f, x8(replaceX8(v, 99.0f)), L"replaceX8", LINE_INFO());
    }

    TEST_METHOD(replaceX2ReplacesSecondLane) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = replaceX2(v, 99.0f);
      Assert::AreEqual(1.0f, x1(r), L"x1 unchanged", LINE_INFO());
      Assert::AreEqual(99.0f, x2(r), L"x2 replaced", LINE_INFO());
      Assert::AreEqual(3.0f, x3(r), L"x3 unchanged", LINE_INFO());
      Assert::AreEqual(4.0f, x4(r), L"x4 unchanged", LINE_INFO());
      Assert::AreEqual(5.0f, x5(r), L"x5 unchanged", LINE_INFO());
      Assert::AreEqual(6.0f, x6(r), L"x6 unchanged", LINE_INFO());
      Assert::AreEqual(7.0f, x7(r), L"x7 unchanged", LINE_INFO());
      Assert::AreEqual(8.0f, x8(r), L"x8 unchanged", LINE_INFO());
    }

    TEST_METHOD(replaceX3ReplacesThirdLane) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = replaceX3(v, 99.0f);
      Assert::AreEqual(1.0f, x1(r), L"x1 unchanged", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2 unchanged", LINE_INFO());
      Assert::AreEqual(99.0f, x3(r), L"x3 replaced", LINE_INFO());
      Assert::AreEqual(4.0f, x4(r), L"x4 unchanged", LINE_INFO());
      Assert::AreEqual(5.0f, x5(r), L"x5 unchanged", LINE_INFO());
      Assert::AreEqual(6.0f, x6(r), L"x6 unchanged", LINE_INFO());
      Assert::AreEqual(7.0f, x7(r), L"x7 unchanged", LINE_INFO());
      Assert::AreEqual(8.0f, x8(r), L"x8 unchanged", LINE_INFO());
    }

    TEST_METHOD(replaceX6ReplacesSixthLane) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = replaceX6(v, 99.0f);
      Assert::AreEqual(1.0f, x1(r), L"x1 unchanged", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2 unchanged", LINE_INFO());
      Assert::AreEqual(3.0f, x3(r), L"x3 unchanged", LINE_INFO());
      Assert::AreEqual(4.0f, x4(r), L"x4 unchanged", LINE_INFO());
      Assert::AreEqual(5.0f, x5(r), L"x5 unchanged", LINE_INFO());
      Assert::AreEqual(99.0f, x6(r), L"x6 replaced", LINE_INFO());
      Assert::AreEqual(7.0f, x7(r), L"x7 unchanged", LINE_INFO());
      Assert::AreEqual(8.0f, x8(r), L"x8 unchanged", LINE_INFO());
    }

    TEST_METHOD(replaceX7ReplacesSeventhLane) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = replaceX7(v, 99.0f);
      Assert::AreEqual(1.0f, x1(r), L"x1 unchanged", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2 unchanged", LINE_INFO());
      Assert::AreEqual(3.0f, x3(r), L"x3 unchanged", LINE_INFO());
      Assert::AreEqual(4.0f, x4(r), L"x4 unchanged", LINE_INFO());
      Assert::AreEqual(5.0f, x5(r), L"x5 unchanged", LINE_INFO());
      Assert::AreEqual(6.0f, x6(r), L"x6 unchanged", LINE_INFO());
      Assert::AreEqual(99.0f, x7(r), L"x7 replaced", LINE_INFO());
      Assert::AreEqual(8.0f, x8(r), L"x8 unchanged", LINE_INFO());
    }
  };
}
