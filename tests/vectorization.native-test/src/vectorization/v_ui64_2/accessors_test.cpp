#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui64_2_AccessorsTest) {
  public:
    TEST_METHOD(readsComponent) {
      const v_ui64_2 given{UInt_64{100}, UInt_64{200}};

      {
        const v_ui64_2 actual{component<VectorIndices::X>(given), component<VectorIndices::Y>(given)};
        Assert::IsTrue(allTrue(given == actual), L"component value mismatch", LINE_INFO());
      }

      {
        const v_ui64_2 actual{component<VectorIndices::Y>(given), component<VectorIndices::X>(given)};
        Assert::IsTrue(allTrue(given != actual), L"wrong component match", LINE_INFO());
      }
    }

    TEST_METHOD(readsXY) {
      const v_ui64_2 given{UInt_64{100}, UInt_64{200}};

      {
        const v_ui64_2 actual{x(given), y(given)};
        Assert::IsTrue(allTrue(given == actual), L"component value mismatch", LINE_INFO());
      }

      {
        const v_ui64_2 actual{y(given), x(given)};
        Assert::IsTrue(allTrue(given != actual), L"wrong component match", LINE_INFO());
      }
    }

    TEST_METHOD(replacesComponent) {
      const UInt_64 givenReplacement = 999ull;
      const v_ui64_2 given{UInt_64{100}, UInt_64{200}};

      {
        const v_ui64_2 expected{givenReplacement, y(given)};
        const auto actual = replaceComponent<VectorIndices::X>(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        const v_ui64_2 expected{x(given), givenReplacement};
        const auto actual = replaceComponent<VectorIndices::Y>(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesXY) {
      const UInt_64 givenReplacement = 999ull;
      const v_ui64_2 given{UInt_64{100}, UInt_64{200}};

      {
        const v_ui64_2 expected{givenReplacement, y(given)};
        const auto actual = replaceX(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        const v_ui64_2 expected{x(given), givenReplacement};
        const auto actual = replaceY(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }
    }
  };
}
