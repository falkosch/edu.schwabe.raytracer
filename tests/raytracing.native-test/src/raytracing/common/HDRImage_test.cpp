#include "raytracing_tests.h"

#include <cmath>

namespace raytracer::test {
  TEST_CLASS(HDRImageTest) {
  public:
    TEST_METHOD(defaultConstructsOneByOne) {
      const HDRImage image{};
      const auto resolution = image.getResolution();
      Assert::AreEqual(static_cast<Size2::ValueType>(1), x(resolution), L"default width",
                       LINE_INFO());
      Assert::AreEqual(static_cast<Size2::ValueType>(1), y(resolution), L"default height",
                       LINE_INFO());
    }

    TEST_METHOD(constructFromResolution) {
      const HDRImage image{Size2{4, 8}};
      const auto resolution = image.getResolution();
      Assert::AreEqual(static_cast<Size2::ValueType>(4), x(resolution), L"width 4",
                       LINE_INFO());
      Assert::AreEqual(static_cast<Size2::ValueType>(8), y(resolution), L"height 8",
                       LINE_INFO());
    }

    TEST_METHOD(operatorIndexReadWrite) {
      HDRImage image{Size2{2, 2}};
      image[0] = Float4{0.25f, 0.5f, 0.75f, 1.0f};
      Assert::AreEqual(0.25f, x(image[0]), L"r", LINE_INFO());
      Assert::AreEqual(0.5f, y(image[0]), L"g", LINE_INFO());
      Assert::AreEqual(0.75f, z(image[0]), L"b", LINE_INFO());
      Assert::AreEqual(1.0f, w(image[0]), L"a", LINE_INFO());
    }

    TEST_METHOD(fromBitmapBlackStaysBlack) {
      Bitmap bitmap{Size2{1, 1}};
      bitmap[0] = 0;
      bitmap[1] = 0;
      bitmap[2] = 0;

      const HDRImage image{bitmap};

      Assert::AreEqual(0.0f, x(image[0]), 0.001f, L"black R stays 0", LINE_INFO());
      Assert::AreEqual(0.0f, y(image[0]), 0.001f, L"black G stays 0", LINE_INFO());
      Assert::AreEqual(0.0f, z(image[0]), 0.001f, L"black B stays 0", LINE_INFO());
    }

    TEST_METHOD(fromBitmapWComponentIsZero) {
      Bitmap bitmap{Size2{1, 1}};
      bitmap[0] = 128;
      bitmap[1] = 128;
      bitmap[2] = 128;

      const HDRImage image{bitmap};

      Assert::AreEqual(0.0f, w(image[0]), 1e-6f, L"W is zero after conversion", LINE_INFO());
    }

    TEST_METHOD(fromBitmapBrighterInputProducesHigherValues) {
      Bitmap bitmap{Size2{2, 1}};
      bitmap[0] = 64;
      bitmap[1] = 64;
      bitmap[2] = 64;
      bitmap[3] = 200;
      bitmap[4] = 200;
      bitmap[5] = 200;

      const HDRImage image{bitmap};

      Assert::IsTrue(x(image[0]) < x(image[1]), L"brighter input → higher output", LINE_INFO());
      Assert::IsTrue(y(image[0]) < y(image[1]), L"brighter input → higher output G", LINE_INFO());
      Assert::IsTrue(z(image[0]) < z(image[1]), L"brighter input → higher output B", LINE_INFO());
    }

    TEST_METHOD(fromBitmapValuesAreNonNegative) {
      Bitmap bitmap{Size2{1, 1}};
      bitmap[0] = 128;
      bitmap[1] = 64;
      bitmap[2] = 200;

      const HDRImage image{bitmap};

      Assert::IsTrue(x(image[0]) >= 0.0f, L"R non-negative", LINE_INFO());
      Assert::IsTrue(y(image[0]) >= 0.0f, L"G non-negative", LINE_INFO());
      Assert::IsTrue(z(image[0]) >= 0.0f, L"B non-negative", LINE_INFO());
    }

    TEST_METHOD(hdrImageStoresValuesGreaterThanOne) {
      HDRImage image{Size2{1, 1}};
      image[0] = Float4{2.5f, 5.0f, 100.0f, 1.0f};
      Assert::AreEqual(2.5f, x(image[0]), L"x > 1 preserved", LINE_INFO());
      Assert::AreEqual(5.0f, y(image[0]), L"y > 1 preserved", LINE_INFO());
      Assert::AreEqual(100.0f, z(image[0]), L"z >> 1 preserved", LINE_INFO());
    }

    TEST_METHOD(hdrImageStoresNegativeValues) {
      HDRImage image{Size2{1, 1}};
      image[0] = Float4{-0.5f, -2.0f, -100.0f, 1.0f};
      Assert::AreEqual(-0.5f, x(image[0]), L"negative x preserved", LINE_INFO());
      Assert::AreEqual(-2.0f, y(image[0]), L"negative y preserved", LINE_INFO());
      Assert::AreEqual(-100.0f, z(image[0]), L"large negative z preserved", LINE_INFO());
    }

    TEST_METHOD(hdrToBitmapClampsValuesGreaterThanOne) {
      HDRImage image{Size2{1, 1}};
      image[0] = Float4{2.0f, 5.0f, 100.0f, 1.0f};
      const Bitmap clamped{image};
      Assert::AreEqual(static_cast<int>(255), static_cast<int>(clamped[0]),
                       L"channel 0 clamps to 255", LINE_INFO());
      Assert::AreEqual(static_cast<int>(255), static_cast<int>(clamped[1]),
                       L"channel 1 clamps to 255", LINE_INFO());
      Assert::AreEqual(static_cast<int>(255), static_cast<int>(clamped[2]),
                       L"channel 2 clamps to 255", LINE_INFO());
    }

    TEST_METHOD(hdrToBitmapClampsNegativeValuesToZero) {
      HDRImage image{Size2{1, 1}};
      image[0] = Float4{-1.0f, -50.0f, -0.001f, 1.0f};
      const Bitmap clamped{image};
      Assert::AreEqual(static_cast<int>(0), static_cast<int>(clamped[0]),
                       L"channel 0 clamps to 0", LINE_INFO());
      Assert::AreEqual(static_cast<int>(0), static_cast<int>(clamped[1]),
                       L"channel 1 clamps to 0", LINE_INFO());
      Assert::AreEqual(static_cast<int>(0), static_cast<int>(clamped[2]),
                       L"channel 2 clamps to 0", LINE_INFO());
    }

    TEST_METHOD(hdrImageStoresNaN) {
      HDRImage image{Size2{1, 1}};
      const auto nan = std::numeric_limits<Float>::quiet_NaN();
      image[0] = Float4{nan, nan, nan, 1.0f};
      Assert::IsTrue(std::isnan(x(image[0])), L"NaN in x preserved", LINE_INFO());
      Assert::IsTrue(std::isnan(y(image[0])), L"NaN in y preserved", LINE_INFO());
      Assert::IsTrue(std::isnan(z(image[0])), L"NaN in z preserved", LINE_INFO());
    }

    TEST_METHOD(hdrImageStoresInfinity) {
      HDRImage image{Size2{1, 1}};
      const auto inf = std::numeric_limits<Float>::infinity();
      image[0] = Float4{inf, -inf, inf, 1.0f};
      Assert::IsTrue(std::isinf(x(image[0])), L"+inf preserved", LINE_INFO());
      Assert::IsTrue(std::isinf(y(image[0])) && y(image[0]) < 0.0f, L"-inf preserved",
                     LINE_INFO());
      Assert::IsTrue(std::isinf(z(image[0])), L"+inf z preserved", LINE_INFO());
    }

    TEST_METHOD(normalizeEachChannelMapsToZeroOne) {
      HDRImage image{Size2{2, 1}};
      image[0] = Float4{2.0f, 4.0f, 6.0f, 1.0f};
      image[1] = Float4{4.0f, 8.0f, 12.0f, 1.0f};
      image.normalizeEachChannel();
      Assert::AreEqual(0.0f, x(image[0]), 0.001f, L"min x -> 0", LINE_INFO());
      Assert::AreEqual(1.0f, x(image[1]), 0.001f, L"max x -> 1", LINE_INFO());
      Assert::AreEqual(0.0f, y(image[0]), 0.001f, L"min y -> 0", LINE_INFO());
      Assert::AreEqual(1.0f, y(image[1]), 0.001f, L"max y -> 1", LINE_INFO());
    }

    TEST_METHOD(sampleBilinearAtTexelCenter) {
      HDRImage image{Size2{2, 2}};
      image[0] = Float4{1.0f, 0.0f, 0.0f, 1.0f};
      image[1] = Float4{0.0f, 1.0f, 0.0f, 1.0f};
      image[2] = Float4{0.0f, 0.0f, 1.0f, 1.0f};
      image[3] = Float4{1.0f, 1.0f, 1.0f, 1.0f};

      const auto sample = image.sampleBilinear(Float4{0.0f, 0.0f, 0.0f, 0.0f});
      Assert::AreEqual(1.0f, x(sample), 0.001f, L"sample x", LINE_INFO());
      Assert::AreEqual(0.0f, y(sample), 0.001f, L"sample y", LINE_INFO());
    }

    TEST_METHOD(getResolutionStableAfterBitmapConstruction) {
      Bitmap source{Size2{8, 4}};
      const HDRImage image{source};
      const auto res = image.getResolution();
      Assert::AreEqual(static_cast<Size2::ValueType>(8), x(res), L"width preserved",
                       LINE_INFO());
      Assert::AreEqual(static_cast<Size2::ValueType>(4), y(res), L"height preserved",
                       LINE_INFO());
    }

    TEST_METHOD(largeImageFromBitmapDoesNotCrash) {
      Bitmap source{Size2{32, 32}};
      for (ASizeT i = 0; i < source.getStride() * 32; ++i) {
        source[i] = static_cast<Bitmap::VectorType::ValueType>(i & 0xff);
      }
      const HDRImage image{source};
      const auto res = image.getResolution();
      Assert::AreEqual(static_cast<Size2::ValueType>(32), x(res), L"width 32", LINE_INFO());
      Assert::AreEqual(static_cast<Size2::ValueType>(32), y(res), L"height 32", LINE_INFO());
    }
  };
}
