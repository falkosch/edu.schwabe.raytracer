#include "raytracing_tests.h"

#include <raytracing/ATrousDenoiser.h>
#include <raytracing/shading/sampling.h>

#include <cmath>

namespace raytracer::test {
  TEST_CLASS(ATrousDenoiserTest) {
    static constexpr int Size = 64;

    // A surface for each half of the image (left, right): its normal, depth, albedo and brightness
    struct Half {
      Float4 normal;
      Float depth;
      Float albedo;
      Float brightness;
    };

    // An image of the two halves, each grey of its brightness with noise of +-amplitude (a hash per pixel), and the
    // guides of their surfaces
    struct TwoSurfaces {
      HDRImage image{Size2(Size, Size)};
      HDRImage guides{Size2(Size, Size)};
      HDRImage albedos{Size2(Size, Size)};

      TwoSurfaces(const Half &left, const Half &right, const Float amplitude) {
        for (auto py = 0; py < Size; ++py) {
          for (auto px = 0; px < Size; ++px) {
            const auto index = static_cast<ASizeT>(py * Size + px);
            const auto &half = px < Size / 2 ? left : right;
            const auto noise = x(sequenceOffset2D(deriveSeed(17u, static_cast<UInt_32>(index)))) * 2.0f - 1.0f;
            image[index] = zeroW(Float4(half.brightness + amplitude * noise));
            guides[index] = Float4(x(half.normal), y(half.normal), z(half.normal), half.depth);
            albedos[index] = zeroW(Float4(half.albedo));
          }
        }
      }
    };

    static Half plane(const Float brightness, const Float albedo = 0.5f) {
      return Half{Float4(0.0f, 0.0f, -1.0f, 0.0f), 2.0f, albedo, brightness};
    }

    // The mean and the standard deviation of the red channel of the columns from x0 to x1 (excluded)
    static std::pair<Float, Float> columns(const HDRImage &image, const int x0, const int x1) {
      auto sum = 0.0;
      auto squares = 0.0;
      for (auto py = 0; py < Size; ++py) {
        for (auto px = x0; px < x1; ++px) {
          const auto value = static_cast<double>(x(image[static_cast<ASizeT>(py * Size + px)]));
          sum += value;
          squares += value * value;
        }
      }
      const auto count = static_cast<double>(Size * (x1 - x0));
      const auto mean = sum / count;
      return {static_cast<Float>(mean), static_cast<Float>(std::sqrt(squares / count - mean * mean))};
    }

    static void denoise(TwoSurfaces &scene) {
      ATrousDenoiser(scene.guides, scene.albedos).denoise(scene.image);
    }

  public:
    TEST_METHOD(noiseOfASurfaceFadesAndItsMeanStays) {
      auto scene = TwoSurfaces(plane(0.5f), plane(0.5f), 0.2f);
      const auto [mean, spread] = columns(scene.image, 0, Size);
      denoise(scene);
      const auto [denoisedMean, denoisedSpread] = columns(scene.image, 0, Size);
      Assert::IsTrue(denoisedSpread < 0.2f * spread, L"less noise", LINE_INFO());
      Assert::AreEqual(mean, denoisedMean, 0.01f * mean, L"mean", LINE_INFO());
    }

    TEST_METHOD(surfacesOfOtherNormalsDontBleed) {
      auto right = plane(0.8f);
      right.normal = Float4(-1.0f, 0.0f, 0.0f, 0.0f);
      auto scene = TwoSurfaces(plane(0.2f), right, 0.1f);
      denoise(scene);
      // the columns next to the edge keep their side's brightness
      Assert::AreEqual(0.2f, columns(scene.image, Size / 2 - 2, Size / 2).first, 0.02f, L"left", LINE_INFO());
      Assert::AreEqual(0.8f, columns(scene.image, Size / 2, Size / 2 + 2).first, 0.02f, L"right", LINE_INFO());
    }

    TEST_METHOD(surfacesOfOtherDepthsDontBleed) {
      auto right = plane(0.8f);
      right.depth = 2.5f;
      auto scene = TwoSurfaces(plane(0.2f), right, 0.1f);
      denoise(scene);
      Assert::AreEqual(0.2f, columns(scene.image, Size / 2 - 2, Size / 2).first, 0.02f, L"left", LINE_INFO());
      Assert::AreEqual(0.8f, columns(scene.image, Size / 2, Size / 2 + 2).first, 0.02f, L"right", LINE_INFO());
    }

    TEST_METHOD(surfacesOfOtherAlbedosDontBleed) {
      auto scene = TwoSurfaces(plane(0.2f, 0.2f), plane(0.8f, 0.8f), 0.1f);
      denoise(scene);
      Assert::AreEqual(0.2f, columns(scene.image, Size / 2 - 2, Size / 2).first, 0.02f, L"left", LINE_INFO());
      Assert::AreEqual(0.8f, columns(scene.image, Size / 2, Size / 2 + 2).first, 0.02f, L"right", LINE_INFO());
    }

    TEST_METHOD(pixelsWithoutSurfaceStayAndDontBleed) {
      auto background = plane(0.9f);
      background.depth = 0.0f;
      auto scene = TwoSurfaces(plane(0.2f), background, 0.1f);
      const auto before = scene.image[static_cast<ASizeT>(Size / 2)];
      denoise(scene);
      Assert::IsTrue(allTrue(scene.image[static_cast<ASizeT>(Size / 2)] == before), L"background", LINE_INFO());
      Assert::AreEqual(0.2f, columns(scene.image, Size / 2 - 2, Size / 2).first, 0.02f, L"surface", LINE_INFO());
    }
  };
}
