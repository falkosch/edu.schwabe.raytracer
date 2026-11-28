#include "tests/MaterialsScene.h"
#include "../stdafx.h"

#include "tests/SceneObjects.h"

#include <raytracing/common/noise/PerlinNoiseGenerator.h>
#include <raytracing/common/noise/SimplexNoiseGenerator.h>
#include <raytracing/shading/shaders/NoiseGeneratorMap.h>

#include <array>
#include <string>

namespace raytracerui
{
    namespace
    {
        constexpr auto RADIUS = 0.16f;
        constexpr std::array COLUMNS{-1.2f, -0.8f, -0.4f, 0.f, 0.4f, 0.8f, 1.2f};
        constexpr std::array ROWS{0.72f, 0.24f, -0.24f, -0.72f};
        constexpr auto Z = -0.6f;
        // Long absorption length: clear media
        const Float4 CLEAR{10.f, 10.f, 10.f, 0.f};
        // The noise's lattice cells per sphere: u runs once around it, v from pole to pole, half as far, so twice the
        // cells along u keep them as wide as tall (at the equator)
        const Float2 NOISE_SCALE{8.f, 4.f};

        // NoiseGeneratorMap doesn't own its generator, and scene objects are destroyed on every scene switch
        const NoiseGenerator& perlin()
        {
            static const PerlinNoiseGenerator generator{42};
            return generator;
        }

        const NoiseGenerator& simplex()
        {
            static const SimplexNoiseGenerator generator{42};
            return generator;
        }

        void addSphere(
            Scene& scene, const std::string& id, const SceneMaterial& material, const ASizeT column, const ASizeT row
        )
        {
            scene.getSceneObjects().push_back(createSceneObject(
                id, std::make_unique<Sphere>(), material,
                {Float3(RADIUS, RADIUS, RADIUS), Float3(0.f, 0.f, 0.f), Float3(COLUMNS[column], ROWS[row], Z)}
            ));
        }

        template <ASizeT Octaves>
        std::unique_ptr<const ObjectShader::MaterialShader> noise(const NoiseGenerator& generator)
        {
            return std::make_unique<NoiseGeneratorMap<Octaves>>(generator);
        }

        void addNoiseSphere(
            Scene& scene, const std::string& id, std::unique_ptr<const ObjectShader::MaterialShader> shader,
            const ASizeT column
        )
        {
            addSphere(scene, id, SceneMaterial::plastic(Float4(1.f, 1.f, 1.f, 0.f), 0.04f, 0.4f), column, 3);
            auto& sphere = *scene.getSceneObjects().back();
            sphere.setDiffusionShader(std::move(shader));
            sphere.scaleTexture(NOISE_SCALE);
        }
    }

    void MaterialsScene::setup(Scene& scene, Resources& resources)
    {
        scene.setBackgroundShader(std::make_unique<RGBBackgroundShader>(RGBS(0.02f, 0.02f, 0.025f)));
        scene.setAmbientLight(RGBS::black());

        // Light values times pi: a white surface facing a light reflects its irradiance / pi (Lambert). Key and fill
        // are point lights in front of the wall behind the camera, falling off as the inverse square: their intensities
        // light the chart's center (0, 0, -0.6) as they did without falloff (the squared distances 23.2 and 16.1).
        auto key = std::make_unique<LightInfo>(LightInfo::blackbody(5500.f, 0.55f * 23.2f * Pi<Float>()));
        key->position = Float4(-2.f, 2.5f, 3.f, 1.f);
        scene.getLights().push_back(std::move(key));
        auto fill = std::make_unique<LightInfo>(LightInfo::d65(0.15f * 16.1f * Pi<Float>()));
        fill->position = Float4(2.5f, -0.5f, 2.5f, 1.f);
        scene.getLights().push_back(std::move(fill));

        // Checker backdrop for the reflections and refractions to show
        auto backdrop = createSceneObject(
            "backdrop", std::make_unique<Plane>(), SceneMaterial::matte(Float4(0.7f, 0.7f, 0.7f, 0.f)),
            {Float3(1.f, 1.f, 1.f), Float3(0.f, 0.f, 0.f), Float3(0.f, 0.f, -2.f)}
        );
        backdrop->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("checker")));
        backdrop->scaleTexture(Float2(0.5f, 0.5f));
        scene.getSceneObjects().push_back(std::move(backdrop));
        // Lit wall behind the camera, so that the mirrors reflect something; it faces the scene
        auto studioWall = createSceneObject(
            "wall behind the camera", std::make_unique<Plane>(), SceneMaterial::matte(Float4(0.6f, 0.6f, 0.6f, 0.f)),
            {Float3(1.f, 1.f, 1.f), Float3(0.f, 180.f, 0.f), Float3(0.f, 0.f, 4.f)}
        );
        studioWall->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("seamless_floor_concrete")));
        studioWall->scaleTexture(Float2(0.5f, 0.5f));
        scene.getSceneObjects().push_back(std::move(studioWall));

        // Row 1: metal roughness from mirror to fully rough
        const Float4 silver{0.95f, 0.93f, 0.88f, 0.f};
        constexpr std::array roughnesses{0.f, 0.05f, 0.1f, 0.2f, 0.4f, 0.7f, 1.f};
        for (auto column = Zero<ASizeT>(); column < roughnesses.size(); ++column)
        {
            addSphere(
                scene, "metal, roughness " + std::to_string(roughnesses[column]),
                SceneMaterial::metal(silver, roughnesses[column]), column, 0
            );
        }

        // Row 2: every refraction index entered from vacuum
        const std::array indices{
            RefractionIndices::Caesium, RefractionIndices::AirSTP, RefractionIndices::Aluminium,
            RefractionIndices::Tabaxir, RefractionIndices::Water, RefractionIndices::Glass, RefractionIndices::Diamond
        };
        for (auto column = Zero<ASizeT>(); column < indices.size(); ++column)
        {
            addSphere(
                scene, "dielectric, index " + std::to_string(indices[column]),
                SceneMaterial::dielectric(indices[column], CLEAR, 0.05f), column, 1
            );
        }

        // Row 3: matte, rough and glossy plastic, gold, copper, absorbing glass, emitter
        const Float4 red{0.7f, 0.12f, 0.08f, 0.f};
        addSphere(scene, "matte red", SceneMaterial::matte(red), 0, 2);
        addSphere(scene, "rough red plastic", SceneMaterial::plastic(red, 0.04f, 0.5f), 1, 2);
        addSphere(scene, "glossy red plastic", SceneMaterial::plastic(red, 0.04f, 0.05f), 2, 2);
        addSphere(scene, "gold", SceneMaterial::metal(Float4(1.f, 0.78f, 0.34f, 0.f), 0.15f), 3, 2);
        addSphere(scene, "copper", SceneMaterial::metal(Float4(0.95f, 0.64f, 0.54f, 0.f), 0.15f), 4, 2);
        addSphere(
            scene, "blue absorbing glass",
            SceneMaterial::dielectric(RefractionIndices::Glass, Float4(0.1f, 0.25f, 0.8f, 0.f), 0.05f), 5, 2
        );
        addSphere(scene, "emitter", SceneMaterial::emissive(Float4(0.05f, 0.25f, 0.3f, 0.f)), 6, 2);

        // Row 4: noise generators by octaves, and noise as roughness
        addNoiseSphere(scene, "Perlin, 1 octave", noise<1>(perlin()), 0);
        addNoiseSphere(scene, "Perlin, 4 octaves", noise<4>(perlin()), 1);
        addNoiseSphere(scene, "Perlin, 8 octaves", noise<8>(perlin()), 2);
        addNoiseSphere(scene, "Simplex, 1 octave", noise<1>(simplex()), 3);
        addNoiseSphere(scene, "Simplex, 4 octaves", noise<4>(simplex()), 4);
        addNoiseSphere(scene, "Simplex, 8 octaves", noise<8>(simplex()), 5);
        addSphere(scene, "metal, Simplex roughness", SceneMaterial::metal(silver, 0.f), 6, 3);
        auto& roughMetal = *scene.getSceneObjects().back();
        roughMetal.setRoughnessShader(noise<8>(simplex()));
        roughMetal.scaleTexture(NOISE_SCALE);
    }
}
