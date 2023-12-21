#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(ValueNegativeOneTest) {
  public:
    TEST_METHOD(returnsInt_8) {
      Assert::AreEqual(Int_8{-1}, NegativeOne<Int_8>());
    }

    TEST_METHOD(returnsInt_16) {
      Assert::AreEqual(Int_16{-1}, NegativeOne<Int_16>());
    }

    TEST_METHOD(returnsInt_32) {
      Assert::AreEqual(Int_32{-1}, NegativeOne<Int_32>());
    }

    TEST_METHOD(returnsInt_64) {
      Assert::AreEqual(Int_64{-1}, NegativeOne<Int_64>());
    }

    TEST_METHOD(returnsFloat_32) {
      Assert::AreEqual(-1.0f, NegativeOne<Float_32>());
    }

    TEST_METHOD(returnsFloat_64) {
      Assert::AreEqual(-1.0, NegativeOne<Float_64>());
    }

    TEST_METHOD(returnsPackedFloat2_128) {
      std::array<Float_64, VectorSizes::Y> expected{-1.0, -1.0};
      std::array<Float_64, VectorSizes::Y> actual{};
      _mm_store_pd(actual.data(), NegativeOne<PackedFloat2_128>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat4_128) {
      std::array<Float_32, VectorSizes::W> expected{-1.0f, -1.0f, -1.0f, -1.0f};
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), NegativeOne<PackedFloat4_128>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat4_256) {
      std::array<Float_64, VectorSizes::W> expected{-1.0, -1.0, -1.0, -1.0};
      std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), NegativeOne<PackedFloat4_256>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat8_256) {
      std::array<Float_32, VectorSizes::X8> expected{-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};
      std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), NegativeOne<PackedFloat8_256>());
      Assert::AreEqual(expected, actual);
    }
  };
}
