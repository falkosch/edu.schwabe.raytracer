#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(ReplaceComponent256dTest) {
  public:
    TEST_METHOD(replacesComponent) {
      constexpr auto expectedReplacement = 5.0;
      const auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
      const auto givenPack = _mm256_load_pd(given.data());
      std::array<Float_64, VectorSizes::W> expected{};
      std::array<Float_64, VectorSizes::W> actual{};

      {
        Mutate::copyAndReplaceAt<VectorIndices::X>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
        _mm256_store_pd(actual.data(), replaceComponent<VectorIndices::X>(givenPack, expectedReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::Y>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
        _mm256_store_pd(actual.data(), replaceComponent<VectorIndices::Y>(givenPack, expectedReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::Z>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
        _mm256_store_pd(actual.data(), replaceComponent<VectorIndices::Z>(givenPack, expectedReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::W>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
        _mm256_store_pd(actual.data(), replaceComponent<VectorIndices::W>(givenPack, expectedReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesXYZW) {
      constexpr auto expectedReplacement = 5.0;
      const auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
      const auto givenPack = _mm256_load_pd(given.data());
      std::array<Float_64, VectorSizes::W> expected{};
      std::array<Float_64, VectorSizes::W> actual{};

      {
        Mutate::copyAndReplaceAt<VectorIndices::X>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
        _mm256_store_pd(actual.data(), replaceX(givenPack, expectedReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::Y>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
        _mm256_store_pd(actual.data(), replaceY(givenPack, expectedReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::Z>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
        _mm256_store_pd(actual.data(), replaceZ(givenPack, expectedReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::W>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
        _mm256_store_pd(actual.data(), replaceW(givenPack, expectedReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesXi) {
      constexpr auto expectedReplacement = 5.0;
      const auto given = StandardSample::ofArrayType<Float_64, VectorSizes::X4>();
      const auto givenPack = _mm256_load_pd(given.data());
      std::array<Float_64, VectorSizes::X4> expected{};
      std::array<Float_64, VectorSizes::X4> actual{};

      {
        Mutate::copyAndReplaceAt<VectorIndices::X1>(
            given.cbegin(), given.cend(), expected.begin(), expectedReplacement
        );
        _mm256_store_pd(actual.data(), replaceX1(givenPack, expectedReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X2>(
            given.cbegin(), given.cend(), expected.begin(), expectedReplacement
        );
        _mm256_store_pd(actual.data(), replaceX2(givenPack, expectedReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X3>(
            given.cbegin(), given.cend(), expected.begin(), expectedReplacement
        );
        _mm256_store_pd(actual.data(), replaceX3(givenPack, expectedReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X4>(
            given.cbegin(), given.cend(), expected.begin(), expectedReplacement
        );
        _mm256_store_pd(actual.data(), replaceX4(givenPack, expectedReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesComponentPackedFloat4_256AtIndex) {
      constexpr auto givenReplacement = 5.0;
      const auto givenReplacementPack = _mm256_set1_pd(givenReplacement);
      const auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
      const auto givenPack = _mm256_load_pd(given.data());
      std::array<Float_64, VectorSizes::W> expected{};
      std::array<Float_64, VectorSizes::W> actual{};

      {
        Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm256_store_pd(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X1));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm256_store_pd(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X2));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X3>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm256_store_pd(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X3));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X4>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm256_store_pd(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X4));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        _mm256_store_pd(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X5));
        Assert::AreEqual(given, actual, L"wrong replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesComponentFloat_64AtIndex) {
      constexpr auto givenReplacement = 5.0;
      const auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
      const auto givenPack = _mm256_load_pd(given.data());
      std::array<Float_64, VectorSizes::W> expected{};
      std::array<Float_64, VectorSizes::W> actual{};

      {
        Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm256_store_pd(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X1));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm256_store_pd(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X2));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X3>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm256_store_pd(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X3));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X4>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm256_store_pd(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X4));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        _mm256_store_pd(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X5));
        Assert::AreEqual(given, actual, L"wrong replace component mismatch", LINE_INFO());
      }
    }
  };
}
