#include "raytracing_tests.h"

#include <raytracing/shading/SceneShader.h>
#include <raytracing/shading/shaders/EnvironmentShader.h>
#include <raytracing/shading/spectral/conversion.h>
#include <raytracing/shading/spectral/wavelengths.h>

namespace raytracer::test
{
    class StubSceneShader final : public SceneShader
    {
    public:
        void buildSceneGraph() override
        {
        }
    };

    static const Float TEST_HERO = spectral::LAMBDA_MIN + spectral::LAMBDA_RANGE * Half<Float>();

    static BackgroundQuery query(const Float4& dir)
    {
        return BackgroundQuery{dir, TEST_HERO};
    }

    static Float4 toRGB(const spectral::Spectrum& s)
    {
        return spectral::xyzToDisplayRGB(spectral::spectrumToXYZ(s, spectral::makeXYZContext(TEST_HERO))).value;
    }

    TEST_CLASS (EnvironmentShaderTest)
    {
        public
        :
        TEST_METHOD(singlePixelImageProducesNonZeroSpectrum)
        {
            HDRImage image{Size2{1, 1}};
            image[0] = Float4{0.25f, 0.5f, 0.75f, 1.0f};
            const EnvironmentShader shader{image};
            const StubSceneShader sceneShader{};

            const auto result = shader.sample(sceneShader, query(Float4{0.0f, 1.0f, 0.0f, 0.0f}));
            const auto color = toRGB(result);

            Assert::IsTrue(x(color) > 0.0f, L"r nonzero", LINE_INFO());
            Assert::IsTrue(y(color) > 0.0f, L"g nonzero", LINE_INFO());
            Assert::IsTrue(z(color) > 0.0f, L"b nonzero", LINE_INFO());
            Assert::IsTrue(z(color) > x(color), L"blue > red for (0.25, 0.5, 0.75)", LINE_INFO());
        }

        TEST_METHOD(yComponentIsIgnored)
        {
            HDRImage image{Size2{2, 2}};
            image[0] = Float4{0.1f, 0.2f, 0.3f, 1.0f};
            image[1] = Float4{0.4f, 0.5f, 0.6f, 1.0f};
            image[2] = Float4{0.7f, 0.8f, 0.9f, 1.0f};
            image[3] = Float4{1.0f, 0.0f, 0.0f, 1.0f};
            const EnvironmentShader shader{image};
            const StubSceneShader sceneShader{};

            const auto a = toRGB(shader.sample(sceneShader, query(Float4{0.3f, -1.0f, 0.7f, 0.0f})));
            const auto b = toRGB(shader.sample(sceneShader, query(Float4{0.3f, 1.0f, 0.7f, 0.0f})));

            Assert::AreEqual(x(a), x(b), 0.01f, L"y=-1 vs y=1 r", LINE_INFO());
            Assert::AreEqual(y(a), y(b), 0.01f, L"y=-1 vs y=1 g", LINE_INFO());
            Assert::AreEqual(z(a), z(b), 0.01f, L"y=-1 vs y=1 b", LINE_INFO());
        }

        TEST_METHOD(blackBackgroundProducesZeroSpectrum)
        {
            HDRImage image{Size2{1, 1}};
            image[0] = Float4{0.0f, 0.0f, 0.0f, 0.0f};
            const EnvironmentShader shader{image};
            const StubSceneShader sceneShader{};

            const auto color = toRGB(shader.sample(sceneShader, query(Float4{0.0f, 0.0f, 0.0f, 0.0f})));

            Assert::AreEqual(0.0f, x(color), 1e-6f, L"r", LINE_INFO());
            Assert::AreEqual(0.0f, y(color), 1e-6f, L"g", LINE_INFO());
            Assert::AreEqual(0.0f, z(color), 1e-6f, L"b", LINE_INFO());
        }
    };
}
