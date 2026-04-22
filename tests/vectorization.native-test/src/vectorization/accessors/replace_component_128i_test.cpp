#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(ReplaceComponent128iTest) {
  public:
    TEST_METHOD(replacesComponent) {
      constexpr auto givenReplacement = Int_32{99};
      const std::array<Int_32, VectorSizes::W> given{10, 20, 30, 40};
      const auto givenPack =
          _mm_set_epi32(given.at(VectorIndices::W), given.at(VectorIndices::Z),
                        given.at(VectorIndices::Y), given.at(VectorIndices::X));
      std::array<Int_32, VectorSizes::W> expected{};
      alignas(16) std::array<Int_32, VectorSizes::W> actual{};

      {
        Mutate::copyAndReplaceAt<VectorIndices::X>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent<VectorIndices::X>(givenPack, givenReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::Y>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent<VectorIndices::Y>(givenPack, givenReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::Z>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent<VectorIndices::Z>(givenPack, givenReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::W>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent<VectorIndices::W>(givenPack, givenReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesXYZW) {
      constexpr auto givenReplacement = Int_32{99};
      const std::array<Int_32, VectorSizes::W> given{10, 20, 30, 40};
      const auto givenPack =
          _mm_set_epi32(given.at(VectorIndices::W), given.at(VectorIndices::Z),
                        given.at(VectorIndices::Y), given.at(VectorIndices::X));
      std::array<Int_32, VectorSizes::W> expected{};
      alignas(16) std::array<Int_32, VectorSizes::W> actual{};

      {
        Mutate::copyAndReplaceAt<VectorIndices::X>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()), replaceX(givenPack, givenReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::Y>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()), replaceY(givenPack, givenReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::Z>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()), replaceZ(givenPack, givenReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::W>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()), replaceW(givenPack, givenReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesXi) {
      constexpr auto givenReplacement = Int_32{99};
      const std::array<Int_32, VectorSizes::X4> given{10, 20, 30, 40};
      const auto givenPack =
          _mm_set_epi32(given.at(VectorIndices::X4), given.at(VectorIndices::X3),
                        given.at(VectorIndices::X2), given.at(VectorIndices::X1));
      std::array<Int_32, VectorSizes::X4> expected{};
      alignas(16) std::array<Int_32, VectorSizes::X4> actual{};

      {
        Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()), replaceX1(givenPack, givenReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()), replaceX2(givenPack, givenReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X3>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()), replaceX3(givenPack, givenReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X4>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()), replaceX4(givenPack, givenReplacement));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesComponentPackedInts_128AtIndex) {
      constexpr auto givenReplacement = Int_32{99};
      const auto givenReplacementPack = _mm_set1_epi32(givenReplacement);
      const std::array<Int_32, VectorSizes::W> given{10, 20, 30, 40};
      const auto givenPack =
          _mm_set_epi32(given.at(VectorIndices::W), given.at(VectorIndices::Z),
                        given.at(VectorIndices::Y), given.at(VectorIndices::X));
      std::array<Int_32, VectorSizes::W> expected{};
      alignas(16) std::array<Int_32, VectorSizes::W> actual{};

      {
        Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent(givenPack, givenReplacementPack, VectorIndices::X1));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent(givenPack, givenReplacementPack, VectorIndices::X2));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X3>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent(givenPack, givenReplacementPack, VectorIndices::X3));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X4>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent(givenPack, givenReplacementPack, VectorIndices::X4));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent(givenPack, givenReplacementPack, VectorIndices::X5));
        Assert::AreEqual(given, actual, L"wrong replace component mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(replacesComponentInt_32AtIndex) {
      constexpr auto givenReplacement = Int_32{99};
      const std::array<Int_32, VectorSizes::W> given{10, 20, 30, 40};
      const auto givenPack =
          _mm_set_epi32(given.at(VectorIndices::W), given.at(VectorIndices::Z),
                        given.at(VectorIndices::Y), given.at(VectorIndices::X));
      std::array<Int_32, VectorSizes::W> expected{};
      alignas(16) std::array<Int_32, VectorSizes::W> actual{};

      {
        Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent(givenPack, givenReplacement, VectorIndices::X1));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent(givenPack, givenReplacement, VectorIndices::X2));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X3>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent(givenPack, givenReplacement, VectorIndices::X3));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        Mutate::copyAndReplaceAt<VectorIndices::X4>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent(givenPack, givenReplacement, VectorIndices::X4));
        Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
        Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
      }

      {
        _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()),
                        replaceComponent(givenPack, givenReplacement, VectorIndices::X5));
        Assert::AreEqual(given, actual, L"wrong replace component mismatch", LINE_INFO());
      }
    }
  };
}
