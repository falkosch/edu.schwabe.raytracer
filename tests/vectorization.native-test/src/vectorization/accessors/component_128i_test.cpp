#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Component128iTest) {
  public:
    TEST_METHOD(readsComponent) {
      const std::array<Int_32, VectorSizes::W> expectedComponents{10, 20, 30, 40};
      const auto givenPack = _mm_set_epi32(
          expectedComponents.at(VectorIndices::W), expectedComponents.at(VectorIndices::Z),
          expectedComponents.at(VectorIndices::Y), expectedComponents.at(VectorIndices::X)
      );

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
      const std::array<Int_32, VectorSizes::W> expectedComponents{10, 20, 30, 40};
      const auto givenPack = _mm_set_epi32(
          expectedComponents.at(VectorIndices::W), expectedComponents.at(VectorIndices::Z),
          expectedComponents.at(VectorIndices::Y), expectedComponents.at(VectorIndices::X)
      );

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
      const std::array<Int_32, VectorSizes::W> expectedComponents{10, 20, 30, 40};
      const auto givenPack = _mm_set_epi32(
          expectedComponents.at(VectorIndices::W), expectedComponents.at(VectorIndices::Z),
          expectedComponents.at(VectorIndices::Y), expectedComponents.at(VectorIndices::X)
      );

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
