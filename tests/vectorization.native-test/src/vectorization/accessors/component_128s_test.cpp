#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Component128sTest) {
  public:
    TEST_METHOD(readsComponent) {
      const auto expectedComponents = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
      const auto givenPack = _mm_load_ps(expectedComponents.data());

      {
        const std::array actual{
            component<VectorIndices::X>(givenPack), component<VectorIndices::Y>(givenPack),
            component<VectorIndices::Z>(givenPack), component<VectorIndices::W>(givenPack)
        };
        Assert::AreEqual(expectedComponents, actual, L"component value mismatch", LINE_INFO());
      }

      {
        const std::array actual{
            component<VectorIndices::W>(givenPack), component<VectorIndices::X>(givenPack),
            component<VectorIndices::Y>(givenPack), component<VectorIndices::Z>(givenPack)
        };
        Assert::AreNotEqual(expectedComponents, actual, L"wrong component match", LINE_INFO());
      }
    }

    TEST_METHOD(readsXYZW) {
      const auto expectedComponents = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
      const auto givenPack = _mm_load_ps(expectedComponents.data());

      {
        const std::array actual{x(givenPack), y(givenPack), z(givenPack), w(givenPack)};
        Assert::AreEqual(expectedComponents, actual, L"component value mismatch", LINE_INFO());
      }

      {
        const std::array actual{w(givenPack), x(givenPack), y(givenPack), z(givenPack)};
        Assert::AreNotEqual(expectedComponents, actual, L"wrong component match", LINE_INFO());
      }
    }

    TEST_METHOD(readsXi) {
      const auto expectedComponents = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
      const auto givenPack = _mm_load_ps(expectedComponents.data());

      {
        const std::array actual{x1(givenPack), x2(givenPack), x3(givenPack), x4(givenPack)};
        Assert::AreEqual(expectedComponents, actual, L"component value mismatch", LINE_INFO());
      }

      {
        const std::array actual{x4(givenPack), x1(givenPack), x2(givenPack), x3(givenPack)};
        Assert::AreNotEqual(expectedComponents, actual, L"wrong component match", LINE_INFO());
      }
    }
  };
}
