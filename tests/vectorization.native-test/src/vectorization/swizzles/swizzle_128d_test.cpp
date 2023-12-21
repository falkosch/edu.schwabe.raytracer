#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Swizzle128dTest) {
  public:
    TEST_METHOD(genericallySwizzlesIdentityAndItsReverse) {
      auto given = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
      auto givenPack = _mm_load_pd(given.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        std::array<Float_64, VectorSizes::Y> expected{
            given.at(VectorIndices::X),
            given.at(VectorIndices::Y),
        };
        _mm_store_pd(actual.data(), swizzle<VectorIndices::X, VectorIndices::Y>(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::Y> expected{
            given.at(VectorIndices::Y),
            given.at(VectorIndices::X),
        };
        _mm_store_pd(actual.data(), swizzle<VectorIndices::Y, VectorIndices::X>(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzlesBroadcast) {
      auto given = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
      auto givenPack = _mm_load_pd(given.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        std::array<Float_64, VectorSizes::Y> expected{
            given.at(VectorIndices::X),
            given.at(VectorIndices::X),
        };
        _mm_store_pd(actual.data(), swizzle<VectorIndices::X, VectorIndices::X>(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::Y> expected{
            given.at(VectorIndices::Y),
            given.at(VectorIndices::Y),
        };
        _mm_store_pd(actual.data(), swizzle<VectorIndices::Y, VectorIndices::Y>(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzles) {
      auto given = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
      auto givenPack = _mm_load_pd(given.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        std::array<Float_64, VectorSizes::Y> expected{
            given.at(VectorIndices::X),
            given.at(VectorIndices::X),
        };
        _mm_store_pd(actual.data(), xx(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::Y> expected{
            given.at(VectorIndices::Y),
            given.at(VectorIndices::X),
        };
        _mm_store_pd(actual.data(), yx(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::Y> expected{
            given.at(VectorIndices::Y),
            given.at(VectorIndices::Y),
        };
        _mm_store_pd(actual.data(), yy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }
  };
}
