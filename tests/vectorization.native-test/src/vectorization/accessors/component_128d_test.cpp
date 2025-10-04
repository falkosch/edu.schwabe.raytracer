#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Component128dTest) {
  public:
    TEST_METHOD(readsComponent) {
      const auto expectedComponents = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
      const auto givenPack = _mm_load_pd(expectedComponents.data());

      {
        const std::array actual{component<VectorIndices::X>(givenPack), component<VectorIndices::Y>(givenPack)};
        Assert::AreEqual(expectedComponents, actual, L"component value mismatch", LINE_INFO());
      }

      {
        const std::array actual{component<VectorIndices::Y>(givenPack), component<VectorIndices::X>(givenPack)};
        Assert::AreNotEqual(expectedComponents, actual, L"wrong component match", LINE_INFO());
      }
    }

    TEST_METHOD(readsXY) {
      const auto expectedComponents = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
      const auto givenPack = _mm_load_pd(expectedComponents.data());

      {
        const std::array actual{x(givenPack), y(givenPack)};
        Assert::AreEqual(expectedComponents, actual, L"component value mismatch", LINE_INFO());
      }

      {
        const std::array actual{y(givenPack), x(givenPack)};
        Assert::AreNotEqual(expectedComponents, actual, L"wrong component match", LINE_INFO());
      }
    }

    TEST_METHOD(readsXi) {
      const auto expectedComponents = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
      const auto givenPack = _mm_load_pd(expectedComponents.data());

      {
        const std::array actual{x1(givenPack), x2(givenPack)};
        Assert::AreEqual(expectedComponents, actual, L"component value mismatch", LINE_INFO());
      }

      {
        const std::array actual{x2(givenPack), x1(givenPack)};
        Assert::AreNotEqual(expectedComponents, actual, L"wrong component match", LINE_INFO());
      }
    }
  };
}
