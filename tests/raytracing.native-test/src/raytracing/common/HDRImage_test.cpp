#include "raytracing_tests.h"

#include <cmath>
#include <limits>

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

      // texel i's center lies at (i + 0.5) / 2
      const auto first = image.sampleBilinear(Float4{0.25f, 0.25f, 0.0f, 0.0f});
      const auto last = image.sampleBilinear(Float4{0.75f, 0.75f, 0.0f, 0.0f});
      Assert::AreEqual(1.0f, x(first), 0.001f, L"texel 0 x", LINE_INFO());
      Assert::AreEqual(0.0f, y(first), 0.001f, L"texel 0 y", LINE_INFO());
      Assert::AreEqual(1.0f, y(last), 0.001f, L"texel 3 y", LINE_INFO());
      Assert::AreEqual(1.0f, z(last), 0.001f, L"texel 3 z", LINE_INFO());
    }

    // 2 columns, 4 rows: x is the row (row 0 at v = 0), y the column. Row centers at v = 0.125, 0.375, 0.625 and
    // 0.875, column centers at u = 0.25 and 0.75.
    static void fillRowsAndColumns(HDRImage &image) {
      for (auto row = Zero<ASizeT>(); row < 4; ++row) {
        for (auto column = Zero<ASizeT>(); column < 2; ++column) {
          image[row * 2 + column] =
              Float4{static_cast<Float>(row), static_cast<Float>(column), 0.0f, 1.0f};
        }
      }
    }

    TEST_METHOD(sampleBilinearRepeatBlendsTheEdgesWithEachOther) {
      HDRImage image{Size2{2, 4}};
      fillRowsAndColumns(image);

      const auto top = image.sampleBilinear(Float4{0.25f, 1.0f, 0.0f, 0.0f});
      const auto bottom = image.sampleBilinear(Float4{0.25f, 0.0f, 0.0f, 0.0f});
      const auto topRow = image.sampleBilinear(Float4{0.25f, 0.875f, 0.0f, 0.0f});
      const auto beyond = image.sampleBilinear(Float4{1.25f, -0.625f, 0.0f, 0.0f});

      Assert::AreEqual(1.5f, x(top), 1e-5f, L"v = 1: half row 3, half row 0", LINE_INFO());
      Assert::AreEqual(1.5f, x(bottom), 1e-5f, L"v = 0: the same seam", LINE_INFO());
      Assert::AreEqual(3.0f, x(topRow), 1e-5f, L"row 3's center", LINE_INFO());
      Assert::AreEqual(1.0f, x(beyond), 1e-5f, L"v = -0.625 is row 1", LINE_INFO());
      Assert::AreEqual(0.0f, y(beyond), 1e-5f, L"u = 1.25 is column 0", LINE_INFO());
    }

    TEST_METHOD(sampleBilinearClampVKeepsThePolesApart) {
      HDRImage image{Size2{2, 4}};
      fillRowsAndColumns(image);

      const auto top = image.sampleBilinear(Float4{0.25f, 1.0f, 0.0f, 0.0f}, TextureWrap::ClampV);
      const auto nearTop = image.sampleBilinear(Float4{0.25f, 0.9375f, 0.0f, 0.0f}, TextureWrap::ClampV);
      const auto justBelowTop = image.sampleBilinear(Float4{0.25f, 0.99f, 0.0f, 0.0f}, TextureWrap::ClampV);
      const auto bottom = image.sampleBilinear(Float4{0.25f, 0.0f, 0.0f, 0.0f}, TextureWrap::ClampV);
      const auto nearBottom = image.sampleBilinear(Float4{0.25f, 0.0625f, 0.0f, 0.0f}, TextureWrap::ClampV);
      const auto belowBottom = image.sampleBilinear(Float4{0.25f, -0.1f, 0.0f, 0.0f}, TextureWrap::ClampV);
      const auto beyondTop = image.sampleBilinear(Float4{0.25f, 1.5f, 0.0f, 0.0f}, TextureWrap::ClampV);
      const auto between = image.sampleBilinear(Float4{0.25f, 0.5f, 0.0f, 0.0f}, TextureWrap::ClampV);
      const auto rowCenter = image.sampleBilinear(Float4{0.25f, 0.625f, 0.0f, 0.0f}, TextureWrap::ClampV);

      Assert::AreEqual(3.0f, x(top), 1e-6f, L"v = 1 is the top row", LINE_INFO());
      Assert::AreEqual(3.0f, x(nearTop), 1e-6f, L"above the top row's center", LINE_INFO());
      Assert::AreEqual(3.0f, x(justBelowTop), 1e-6f, L"just below v = 1", LINE_INFO());
      Assert::AreEqual(0.0f, x(bottom), 1e-6f, L"v = 0 is the bottom row", LINE_INFO());
      Assert::AreEqual(0.0f, x(nearBottom), 1e-6f, L"below the bottom row's center", LINE_INFO());
      Assert::AreEqual(0.0f, x(belowBottom), 1e-6f, L"clamped below", LINE_INFO());
      Assert::AreEqual(3.0f, x(beyondTop), 1e-6f, L"clamped above", LINE_INFO());
      Assert::AreEqual(1.5f, x(between), 1e-5f, L"rows 1 and 2 interpolate", LINE_INFO());
      Assert::AreEqual(2.0f, x(rowCenter), 1e-5f, L"row 2's center", LINE_INFO());
    }

    TEST_METHOD(sampleBilinearClampVStillWrapsU) {
      HDRImage image{Size2{2, 4}};
      fillRowsAndColumns(image);

      const auto seam = image.sampleBilinear(Float4{0.0f, 0.625f, 0.0f, 0.0f}, TextureWrap::ClampV);
      const auto lastColumn = image.sampleBilinear(Float4{0.75f, 0.625f, 0.0f, 0.0f}, TextureWrap::ClampV);
      const auto beyond = image.sampleBilinear(Float4{1.25f, 0.625f, 0.0f, 0.0f}, TextureWrap::ClampV);

      Assert::AreEqual(0.5f, y(seam), 1e-5f, L"last and first column blend", LINE_INFO());
      Assert::AreEqual(1.0f, y(lastColumn), 1e-5f, L"column 1's center", LINE_INFO());
      Assert::AreEqual(0.0f, y(beyond), 1e-5f, L"u = 1.25 is column 0", LINE_INFO());
      Assert::AreEqual(2.0f, x(seam), 1e-5f, L"row 2", LINE_INFO());
    }

    TEST_METHOD(sampleBilinearClampKeepsBothAxesAtTheEdges) {
      HDRImage image{Size2{2, 4}};
      fillRowsAndColumns(image);

      const auto left = image.sampleBilinear(Float4{0.0f, 0.625f, 0.0f, 0.0f}, TextureWrap::Clamp);
      const auto right = image.sampleBilinear(Float4{1.0f, 0.625f, 0.0f, 0.0f}, TextureWrap::Clamp);
      const auto farLeft = image.sampleBilinear(Float4{-3.0f, 0.625f, 0.0f, 0.0f}, TextureWrap::Clamp);
      const auto topRight = image.sampleBilinear(Float4{1.0f, 1.0f, 0.0f, 0.0f}, TextureWrap::Clamp);
      const auto between = image.sampleBilinear(Float4{0.5f, 0.5f, 0.0f, 0.0f}, TextureWrap::Clamp);

      Assert::AreEqual(0.0f, y(left), 1e-6f, L"u = 0 is column 0", LINE_INFO());
      Assert::AreEqual(1.0f, y(right), 1e-6f, L"u = 1 is column 1", LINE_INFO());
      Assert::AreEqual(0.0f, y(farLeft), 1e-6f, L"clamped left", LINE_INFO());
      Assert::AreEqual(2.0f, x(left), 1e-5f, L"row 2", LINE_INFO());
      Assert::AreEqual(3.0f, x(topRight), 1e-6f, L"top row", LINE_INFO());
      Assert::AreEqual(1.0f, y(topRight), 1e-6f, L"last column", LINE_INFO());
      Assert::AreEqual(0.5f, y(between), 1e-5f, L"columns interpolate", LINE_INFO());
      Assert::AreEqual(1.5f, x(between), 1e-5f, L"rows interpolate", LINE_INFO());
    }

    TEST_METHOD(sampleBilinearStaysWithinTheImageForAnyCoordinates) {
      HDRImage image{Size2{2, 4}};
      fillRowsAndColumns(image);
      const auto infinity = std::numeric_limits<Float>::infinity();
      const auto nan = std::numeric_limits<Float>::quiet_NaN();

      for (const auto wrap : {TextureWrap::Repeat, TextureWrap::ClampV, TextureWrap::Clamp}) {
        // Huge coordinates lose their fraction but still read texels of the image
        for (const auto coordinate : {1e30f, -1e30f, 3e9f, -3e9f}) {
          const auto sample = image.sampleBilinear(Float4{coordinate, coordinate, 0.0f, 0.0f}, wrap);
          Assert::IsTrue(x(sample) >= 0.0f && x(sample) <= 3.0f, L"a row", LINE_INFO());
          Assert::IsTrue(y(sample) >= 0.0f && y(sample) <= 1.0f, L"a column", LINE_INFO());
        }
        // NaN and infinite coordinates give no meaningful color, but must not read outside the image
        for (const auto coordinate : {nan, infinity, -infinity}) {
          image.sampleBilinear(Float4{coordinate, coordinate, 0.0f, 0.0f}, wrap);
          image.sampleBilinear(Float4{0.25f, coordinate, 0.0f, 0.0f}, wrap);
        }
      }
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
