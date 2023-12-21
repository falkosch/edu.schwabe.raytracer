#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(ValueNegativeTwoTest) {
  public:
    TEST_METHOD(returnsInt_8) {
      Assert::AreEqual(Int_8{-2}, NegativeTwo<Int_8>());
    }

    TEST_METHOD(returnsInt_16) {
      Assert::AreEqual(Int_16{-2}, NegativeTwo<Int_16>());
    }

    TEST_METHOD(returnsInt_32) {
      Assert::AreEqual(Int_32{-2}, NegativeTwo<Int_32>());
    }

    TEST_METHOD(returnsInt_64) {
      Assert::AreEqual(Int_64{-2}, NegativeTwo<Int_64>());
    }

    TEST_METHOD(returnsFloat_32) {
      Assert::AreEqual(-2.0f, NegativeTwo<Float_32>());
    }

    TEST_METHOD(returnsFloat_64) {
      Assert::AreEqual(-2.0, NegativeTwo<Float_64>());
    }

    TEST_METHOD(returnsPackedFloat2_128) {
      std::array<Float_64, VectorSizes::Y> expected{-2.0, -2.0};
      std::array<Float_64, VectorSizes::Y> actual{};
      _mm_store_pd(actual.data(), NegativeTwo<PackedFloat2_128>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat4_128) {
      std::array<Float_32, VectorSizes::W> expected{-2.0f, -2.0f, -2.0f, -2.0f};
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), NegativeTwo<PackedFloat4_128>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat4_256) {
      std::array<Float_64, VectorSizes::W> expected{-2.0, -2.0, -2.0, -2.0};
      std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), NegativeTwo<PackedFloat4_256>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat8_256) {
      std::array<Float_32, VectorSizes::X8> expected{-2.0f, -2.0f, -2.0f, -2.0f, -2.0f, -2.0f, -2.0f, -2.0f};
      std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), NegativeTwo<PackedFloat8_256>());
      Assert::AreEqual(expected, actual);
    }
  };
}
