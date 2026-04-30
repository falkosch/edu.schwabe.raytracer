#include "raytracing_tests.h"

#include <cmath>

namespace raytracer::test {
  // HDRImage exposes no I/O of its own. Its only "load" path is the Bitmap → HDRImage
  // constructor; its symmetric "save" is the Bitmap(const Image<Float4>&) constructor. The tests
  // below exercise these paired conversions and lock in HDR-range behavior (negative, > 1,
  // NaN/infinity inputs).

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

    TEST_METHOD(roundTripFromBitmapRecoversNormalizedValues) {
      Bitmap bitmap{Size2{2, 1}};
      // Bitmap stores BGR; the HDRImage constructor reads bytes in linear order so data[0..2]
      // become channel x,y,z respectively.
      bitmap[0] = 0;    // pixel 0 channel 0
      bitmap[1] = 128;  // pixel 0 channel 1
      bitmap[2] = 255;  // pixel 0 channel 2
      bitmap[3] = 255;  // pixel 1 channel 0
      bitmap[4] = 64;   // pixel 1 channel 1
      bitmap[5] = 0;    // pixel 1 channel 2

      const HDRImage image{bitmap};

      Assert::AreEqual(0.0f, x(image[0]), 0.001f, L"0 → 0.0", LINE_INFO());
      Assert::AreEqual(128.0f / 255.0f, y(image[0]), 0.005f, L"128 → ~0.502", LINE_INFO());
      Assert::AreEqual(1.0f, z(image[0]), 0.001f, L"255 → 1.0", LINE_INFO());

      Assert::AreEqual(1.0f, x(image[1]), 0.001f, L"255 → 1.0", LINE_INFO());
      Assert::AreEqual(64.0f / 255.0f, y(image[1]), 0.005f, L"64 → ~0.251", LINE_INFO());
      Assert::AreEqual(0.0f, z(image[1]), 0.001f, L"0 → 0.0", LINE_INFO());
    }

    TEST_METHOD(roundTripBitmapHDRBitmap) {
      Bitmap source{Size2{2, 2}};
      // Lay out four pixels' worth of BGR bytes (no padding for stride=8 means the row has 6
      // bytes of data + 2 padding bytes that init() leaves uninitialised — set them too).
      for (ASizeT i = 0; i < source.getStride() * 2; ++i) {
        source[i] = 0;
      }
      // Pixel (0,0): RGB = (10, 20, 30)
      source[0] = 10;
      source[1] = 20;
      source[2] = 30;
      // Pixel (1,0): RGB = (200, 100, 50)
      source[3] = 200;
      source[4] = 100;
      source[5] = 50;

      const HDRImage image{source};
      const Bitmap roundTripped{image};

      // The HDRImage→Bitmap constructor multiplies floats by 255 and clamps. Original byte values
      // should survive within ±1 (rounding).
      Assert::AreEqual(static_cast<int>(10), static_cast<int>(roundTripped[2]), L"R0 round-trip",
                       LINE_INFO());
      Assert::AreEqual(static_cast<int>(20), static_cast<int>(roundTripped[1]), L"G0 round-trip",
                       LINE_INFO());
      Assert::AreEqual(static_cast<int>(30), static_cast<int>(roundTripped[0]), L"B0 round-trip",
                       LINE_INFO());

      Assert::AreEqual(static_cast<int>(200), static_cast<int>(roundTripped[5]), L"R1 round-trip",
                       LINE_INFO());
      Assert::AreEqual(static_cast<int>(100), static_cast<int>(roundTripped[4]), L"G1 round-trip",
                       LINE_INFO());
      Assert::AreEqual(static_cast<int>(50), static_cast<int>(roundTripped[3]), L"B1 round-trip",
                       LINE_INFO());
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
      // Bitmap conversion clamps to [0, 255]. All HDR > 1 inputs collapse to 255.
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
      // NaN is preserved as-is in storage; lock in this behavior.
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
      // After normalization, the per-channel min becomes 0 and max becomes 1.
      Assert::AreEqual(0.0f, x(image[0]), 0.001f, L"min x → 0", LINE_INFO());
      Assert::AreEqual(1.0f, x(image[1]), 0.001f, L"max x → 1", LINE_INFO());
      Assert::AreEqual(0.0f, y(image[0]), 0.001f, L"min y → 0", LINE_INFO());
      Assert::AreEqual(1.0f, y(image[1]), 0.001f, L"max y → 1", LINE_INFO());
    }

    TEST_METHOD(sampleBilinearAtTexelCenter) {
      HDRImage image{Size2{2, 2}};
      image[0] = Float4{1.0f, 0.0f, 0.0f, 1.0f};
      image[1] = Float4{0.0f, 1.0f, 0.0f, 1.0f};
      image[2] = Float4{0.0f, 0.0f, 1.0f, 1.0f};
      image[3] = Float4{1.0f, 1.0f, 1.0f, 1.0f};

      // texCoords (0,0) hits texel 0 exactly (origin of the image).
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

    TEST_METHOD(largeImageRoundTrip) {
      // Lock in that conversion handles a non-trivial image without overflow.
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
