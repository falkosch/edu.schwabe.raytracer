#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui64_4_AccessorsTest) {
  public:
    TEST_METHOD(readsComponent) {
      const v_ui64_4 given{10ULL, 20ULL, 30ULL, 40ULL};

      {
        const v_ui64_4 actual{
            component<VectorIndices::X>(given), component<VectorIndices::Y>(given), component<VectorIndices::Z>(given),
            component<VectorIndices::W>(given)
        };
        Assert::IsTrue(allTrue(given == actual), L"component value mismatch", LINE_INFO());
      }

      {
        const v_ui64_4 actual{
            component<VectorIndices::W>(given), component<VectorIndices::X>(given), component<VectorIndices::Y>(given),
            component<VectorIndices::Z>(given)
        };
        Assert::IsTrue(allTrue(given != actual), L"wrong component match", LINE_INFO());
      }
    }

    TEST_METHOD(readsXYZW) {
      const v_ui64_4 given{10ULL, 20ULL, 30ULL, 40ULL};

      {
        const v_ui64_4 actual{x(given), y(given), z(given), w(given)};
        Assert::IsTrue(allTrue(given == actual), L"component value mismatch", LINE_INFO());
      }

      {
        const v_ui64_4 actual{w(given), x(given), y(given), z(given)};
        Assert::IsTrue(allTrue(given != actual), L"wrong component match", LINE_INFO());
      }
    }

    TEST_METHOD(replacesComponent) {
      const UInt_64 givenReplacement = 99ULL;
      const v_ui64_4 given{10ULL, 20ULL, 30ULL, 40ULL};

      {
        const v_ui64_4 expected{givenReplacement, y(given), z(given), w(given)};
        const auto actual = replaceComponent<VectorIndices::X>(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        const v_ui64_4 expected{x(given), givenReplacement, z(given), w(given)};
        const auto actual = replaceComponent<VectorIndices::Y>(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        const v_ui64_4 expected{x(given), y(given), givenReplacement, w(given)};
        const auto actual = replaceComponent<VectorIndices::Z>(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        const v_ui64_4 expected{x(given), y(given), z(given), givenReplacement};
        const auto actual = replaceComponent<VectorIndices::W>(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesXYZW) {
      const UInt_64 givenReplacement = 99ULL;
      const v_ui64_4 given{10ULL, 20ULL, 30ULL, 40ULL};

      {
        const v_ui64_4 expected{givenReplacement, y(given), z(given), w(given)};
        const auto actual = replaceX(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        const v_ui64_4 expected{x(given), givenReplacement, z(given), w(given)};
        const auto actual = replaceY(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        const v_ui64_4 expected{x(given), y(given), givenReplacement, w(given)};
        const auto actual = replaceZ(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }

      {
        const v_ui64_4 expected{x(given), y(given), z(given), givenReplacement};
        const auto actual = replaceW(given, givenReplacement);
        Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
        Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
      }
    }
  };
}
