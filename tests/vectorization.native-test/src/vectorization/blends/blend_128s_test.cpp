#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Blend128sTest) {
  public:
    TEST_METHOD(genericallyBlendsIdentities) {
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      alignas(XMM_ALIGNMENT) std::array<Float_32, VectorSizes::W> actual{};

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, false, false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, true, true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsIndividuals) {
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      alignas(XMM_ALIGNMENT) std::array<Float_32, VectorSizes::W> actual{};

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, false, false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, true, false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, false, true, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, false, false, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsPairs) {
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      alignas(XMM_ALIGNMENT) std::array<Float_32, VectorSizes::W> actual{};

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, true, false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, false, true, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, true, false, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, false, true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsTriplets) {
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      alignas(XMM_ALIGNMENT) std::array<Float_32, VectorSizes::W> actual{};

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<false, true, true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, false, true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, true, false, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z),
            givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blend<true, true, true, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForBlends) {
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      alignas(XMM_ALIGNMENT) std::array<Float_32, VectorSizes::W> actual{};

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), x_yzw(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }
  };
}
