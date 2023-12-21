#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Blend128sTest) {
  public:
    TEST_METHOD(genericallyBlendsIdentities) {
      std::array<Float_32, VectorSizes::W> givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      std::array<Float_32, VectorSizes::W> givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, false, false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, true, true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsIndividuals) {
      std::array<Float_32, VectorSizes::W> givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      std::array<Float_32, VectorSizes::W> givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, false, false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, true, false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, false, true, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, false, false, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsPairs) {
      std::array<Float_32, VectorSizes::W> givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      std::array<Float_32, VectorSizes::W> givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, true, false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, false, true, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, true, false, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, false, true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsTriplets) {
      std::array<Float_32, VectorSizes::W> givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      std::array<Float_32, VectorSizes::W> givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, true, true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, false, true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, true, false, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z),
            givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, true, true, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForBlends) {
      std::array<Float_32, VectorSizes::W> givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      std::array<Float_32, VectorSizes::W> givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        std::array<Float_32, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), x_yzw(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }
  };
}
