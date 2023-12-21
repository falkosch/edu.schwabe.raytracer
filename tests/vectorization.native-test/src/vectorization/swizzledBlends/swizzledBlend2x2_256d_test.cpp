#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(SwizzledBlends2x2_256dTest) {
  public:
    TEST_METHOD(genericallySwizzleBlendsIdentityAndItsReverse) {
      std::array<Float_64, VectorSizes::W> givenOnBitNotSet{1.0, 2.0, 3.0, 4.0};
      auto givenOnBitNotSetPack = _mm256_load_pd(givenOnBitNotSet.data());
      std::array<Float_64, VectorSizes::W> givenOnBitSet{5.0, 6.0, 7.0, 8.0};
      auto givenOnBitSetPack = _mm256_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::W> actual{};

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(), swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
                               givenOnBitNotSetPack, givenOnBitSetPack
                           )
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::W), givenOnBitNotSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::X)
        };
        _mm256_store_pd(
            actual.data(), swizzledBlend2x2<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(
                               givenOnBitNotSetPack, givenOnBitSetPack
                           )
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzleBlendsBroadcast) {
      std::array<Float_64, VectorSizes::W> givenOnBitNotSet{1.0, 2.0, 3.0, 4.0};
      auto givenOnBitNotSetPack = _mm256_load_pd(givenOnBitNotSet.data());
      std::array<Float_64, VectorSizes::W> givenOnBitSet{5.0, 6.0, 7.0, 8.0};
      auto givenOnBitSetPack = _mm256_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::W> actual{};

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X),
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::X)
        };
        _mm256_store_pd(
            actual.data(), swizzledBlend2x2<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(
                               givenOnBitNotSetPack, givenOnBitSetPack
                           )
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm256_store_pd(
            actual.data(), swizzledBlend2x2<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(
                               givenOnBitNotSetPack, givenOnBitSetPack
                           )
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::Z)
        };
        _mm256_store_pd(
            actual.data(), swizzledBlend2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(
                               givenOnBitNotSetPack, givenOnBitSetPack
                           )
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::W), givenOnBitNotSet.at(VectorIndices::W),
            givenOnBitSet.at(VectorIndices::W), givenOnBitSet.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(), swizzledBlend2x2<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(
                               givenOnBitNotSetPack, givenOnBitSetPack
                           )
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzleBlends) {
      std::array<Float_64, VectorSizes::W> givenOnBitNotSet{1.0, 2.0, 3.0, 4.0};
      auto givenOnBitNotSetPack = _mm256_load_pd(givenOnBitNotSet.data());
      std::array<Float_64, VectorSizes::W> givenOnBitSet{5.0, 6.0, 7.0, 8.0};
      auto givenOnBitSetPack = _mm256_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::W> actual{};

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X),
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::X)
        };
        _mm256_store_pd(actual.data(), xx_xx(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X),
            givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm256_store_pd(actual.data(), xx_yy(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm256_store_pd(actual.data(), xy_xy(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm256_store_pd(actual.data(), xy_zw(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Z)
        };
        _mm256_store_pd(actual.data(), xz_xz(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::X),
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm256_store_pd(actual.data(), yx_xy(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm256_store_pd(actual.data(), yy_yy(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::W),
            givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::W)
        };
        _mm256_store_pd(actual.data(), yw_yw(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm256_store_pd(actual.data(), zz_yy(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::Z)
        };
        _mm256_store_pd(actual.data(), zz_zz(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W),
            givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
        };
        _mm256_store_pd(actual.data(), zw_zw(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::W), givenOnBitNotSet.at(VectorIndices::W),
            givenOnBitSet.at(VectorIndices::W), givenOnBitSet.at(VectorIndices::W)
        };
        _mm256_store_pd(actual.data(), ww_ww(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }
  };
}
