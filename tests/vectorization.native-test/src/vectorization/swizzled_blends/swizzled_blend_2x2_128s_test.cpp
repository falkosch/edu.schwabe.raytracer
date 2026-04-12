#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(SwizzledBlends2x2_128sTest) {
  public:
    TEST_METHOD(genericallySwizzleBlendsIdentityAndItsReverse) {
      constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(
            actual.data(), swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
                               givenOnBitNotSetPack, givenOnBitSetPack
                           )
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::W), givenOnBitNotSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::X)
        };
        _mm_store_ps(
            actual.data(), swizzledBlend2x2<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(
                               givenOnBitNotSetPack, givenOnBitSetPack
                           )
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzleBlendsBroadcast) {
      constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X),
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::X)
        };
        _mm_store_ps(
            actual.data(), swizzledBlend2x2<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(
                               givenOnBitNotSetPack, givenOnBitSetPack
                           )
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm_store_ps(
            actual.data(), swizzledBlend2x2<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(
                               givenOnBitNotSetPack, givenOnBitSetPack
                           )
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::Z)
        };
        _mm_store_ps(
            actual.data(), swizzledBlend2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(
                               givenOnBitNotSetPack, givenOnBitSetPack
                           )
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::W), givenOnBitNotSet.at(VectorIndices::W),
            givenOnBitSet.at(VectorIndices::W), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(
            actual.data(), swizzledBlend2x2<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(
                               givenOnBitNotSetPack, givenOnBitSetPack
                           )
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzleBlends) {
      constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X),
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::X)
        };
        _mm_store_ps(actual.data(), xx_xx(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X),
            givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm_store_ps(actual.data(), xx_yy(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm_store_ps(actual.data(), xy_xy(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), xy_zw(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Z)
        };
        _mm_store_ps(actual.data(), xz_xz(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::X),
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm_store_ps(actual.data(), yx_xy(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm_store_ps(actual.data(), yy_yy(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::W),
            givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), yw_yw(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm_store_ps(actual.data(), zz_yy(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::Z)
        };
        _mm_store_ps(actual.data(), zz_zz(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), zw_zw(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::W), givenOnBitNotSet.at(VectorIndices::W),
            givenOnBitSet.at(VectorIndices::W), givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), ww_ww(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }
  };
}
