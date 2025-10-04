#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4_AccessorsTest) {
  public:
    TEST_METHOD(readsComponent) {
      const auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        const v_f32_4 actual{
            component<VectorIndices::X>(given), component<VectorIndices::Y>(given), component<VectorIndices::Z>(given),
            component<VectorIndices::W>(given)
        };
        Assert::IsTrue(allTrue(given == actual), L"component value mismatch", LINE_INFO());
      }

      {
        const v_f32_4 actual{
            component<VectorIndices::W>(given), component<VectorIndices::X>(given), component<VectorIndices::Y>(given),
            component<VectorIndices::Z>(given)
        };
        Assert::IsTrue(allTrue(given != actual), L"wrong component match", LINE_INFO());
      }
    }

    TEST_METHOD(readsXYZW) {
      const auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        const v_f32_4 actual{x(given), y(given), z(given), w(given)};
        Assert::IsTrue(allTrue(given == actual), L"component value mismatch", LINE_INFO());
      }

      {
        const v_f32_4 actual{w(given), x(given), y(given), z(given)};
        Assert::IsTrue(allTrue(given != actual), L"wrong component match", LINE_INFO());
      }
    }

    TEST_METHOD(readsXi) {
      const auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        const v_f32_4 actual{x1(given), x2(given), x3(given), x4(given)};
        Assert::IsTrue(allTrue(given == actual), L"component value mismatch", LINE_INFO());
      }

      {
        const v_f32_4 actual{x4(given), x1(given), x2(given), x3(given)};
        Assert::IsTrue(allTrue(given != actual), L"wrong component match", LINE_INFO());
      }
    }

    TEST_METHOD(replacesComponent) {
      auto givenReplacement = 5.0f;
      auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        v_f32_4 expected{givenReplacement, y(given), z(given), w(given)};
        auto actual = replaceComponent<VectorIndices::X>(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), givenReplacement, z(given), w(given)};
        auto actual = replaceComponent<VectorIndices::Y>(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), y(given), givenReplacement, w(given)};
        auto actual = replaceComponent<VectorIndices::Z>(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), y(given), z(given), givenReplacement};
        auto actual = replaceComponent<VectorIndices::W>(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesXYZW) {
      auto givenReplacement = 5.0f;
      auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        v_f32_4 expected{givenReplacement, y(given), z(given), w(given)};
        auto actual = replaceX(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), givenReplacement, z(given), w(given)};
        auto actual = replaceY(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), y(given), givenReplacement, w(given)};
        auto actual = replaceZ(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), y(given), z(given), givenReplacement};
        auto actual = replaceW(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesXi) {
      auto givenReplacement = 5.0f;
      auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        v_f32_4 expected{givenReplacement, x2(given), x3(given), x4(given)};
        auto actual = replaceX1(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x1(given), givenReplacement, x3(given), x4(given)};
        auto actual = replaceX2(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x1(given), x2(given), givenReplacement, x4(given)};
        auto actual = replaceX3(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x1(given), x2(given), x3(given), givenReplacement};
        auto actual = replaceX4(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesComponentAtIndex) {
      auto givenReplacement = 5.0f;
      auto givenReplacementVector = v_f32_4(givenReplacement);
      auto given = StandardSample::ofVectorType<v_f32_4>();

      {
        v_f32_4 expected{givenReplacement, y(given), z(given), w(given)};
        auto actual = replaceComponent(given, givenReplacementVector, VectorIndices::X);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), givenReplacement, z(given), w(given)};
        auto actual = replaceComponent(given, givenReplacementVector, VectorIndices::Y);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), y(given), givenReplacement, w(given)};
        auto actual = replaceComponent(given, givenReplacement, VectorIndices::Z);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        v_f32_4 expected{x(given), y(given), z(given), givenReplacement};
        auto actual = replaceComponent(given, givenReplacement, VectorIndices::W);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        auto actual = replaceComponent(given, givenReplacement, VectorIndices::X5);
        Assert::IsTrue(allTrue(given == actual), L"wrong replace component mismatch", LINE_INFO());
      }
    }
  };
}
