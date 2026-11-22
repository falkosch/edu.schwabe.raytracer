#include "tests/ShadingScene.h"
#include "../stdafx.h"

#include "tests/SceneObjects.h"

#include <raytracing/shading/shaders/ConcatenatingShader.h>
#include <raytracing/shading/shaders/EnvironmentShader.h>
#include <raytracing/shading/shaders/IntersectionNormalShader.h>

#include <array>
#include <functional>

namespace raytracerui
{
    namespace
    {
        using MaterialShader = ObjectShader::MaterialShader;
        using MultiplyingMaterialShader = ConcatenatingShader<
            ObjectShader, FacetIntersection, Float4, std::multiplies<Float4>, MaterialShader, MaterialShader>;

        // MultiplyByConstShader and ConcatenatingShader don't own their inner shaders, and scene objects are destroyed
        // on every scene switch: the inner shaders live as long as the resources they sample.
        const MaterialShader& marbleShader(Resources& resources)
        {
            static const HDRImageShader marble{*resources.getPPM("marble")};
            return marble;
        }

        const MaterialShader& checkerShader(Resources& resources)
        {
            static const HDRImageShader checker{*resources.getPPM("checker")};
            return checker;
        }
    }

    void ShadingScene::setup(Scene& scene, Resources& resources)
    {
        // Debevec's light probe of St. Peter's as an angular map: the background and the mirror sphere show it
        scene.setBackgroundShader(std::make_unique<EnvironmentShader>(
            *resources.getPPM("env_spherical_stpeters"), EnvironmentProjection::AngularMap
        ));
        scene.setAmbientLight(RGBS(0.03f, 0.03f, 0.03f));

        auto key = std::make_unique<LightInfo>(LightInfo::blackbody(6000.f, 0.5f));
        key->position = Float4(-2.5f, 3.f, 3.f, 1.f);
        key->attenuationFactors = Float4(1.f, 0.f, 0.f, 1.f);
        scene.getLights().push_back(std::move(key));
        auto fill = std::make_unique<LightInfo>(LightInfo::d65(0.15f));
        fill->position = Float4(3.f, 1.f, 2.5f, 1.f);
        fill->attenuationFactors = Float4(1.f, 0.f, 0.f, 1.f);
        scene.getLights().push_back(std::move(fill));

        auto& objects = scene.getSceneObjects();
        const auto paper = SceneMaterial::matte(Float4(0.8f, 0.8f, 0.8f, 0.f));
        const auto satin = SceneMaterial::plastic(Float4(0.8f, 0.8f, 0.8f, 0.f), 0.04f, 0.3f);
        constexpr auto RADIUS = 0.25f;
        constexpr std::array COLUMNS{-0.96f, -0.32f, 0.32f, 0.96f};
        constexpr auto BACK_Z = -1.2f;
        constexpr auto BACK_Y = -0.45f + RADIUS;
        constexpr auto FRONT_Z = -0.2f;
        constexpr auto FRONT_Y = -1.f + RADIUS;

        // Floor with the checker scaled, rotated by 30 degrees and shifted
        auto floor = createSceneObject(
            "floor, transformed checker", std::make_unique<Plane>(), paper,
            {Float3(1.f, 1.f, 1.f), Float3(-90.f, 0.f, 0.f), Float3(0.f, -1.f, 0.f)}
        );
        floor->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("checker")));
        floor->scaleTexture(Float2(0.5f, 0.5f));
        floor->rotateTexture(30.f);
        floor->translateTexture(Float2(0.1f, 0.2f));
        objects.push_back(std::move(floor));
        objects.push_back(createSceneObject(
            "step", std::make_unique<Box>(), paper,
            {Float3(1.4f, 0.275f, 0.35f), Float3(0.f, 0.f, 0.f), Float3(0.f, -0.725f, BACK_Z)}
        ));

        // Back row on the step: sRGB and linear earth, mirror sphere, normals as colors
        const auto ball = [](const Float x, const Float y, const Float z)
        {
            return Placement{Float3(RADIUS, RADIUS, RADIUS), Float3(0.f, 0.f, 0.f), Float3(x, y, z)};
        };
        auto srgbEarth = createSceneObject(
            "earth, sRGB", std::make_unique<Sphere>(), satin, ball(COLUMNS[0], BACK_Y, BACK_Z)
        );
        // The map spans the sphere once: its rows end at the poles
        srgbEarth->setDiffusionShader(
            std::make_unique<HDRImageShader>(*resources.getPPM("earth_ice_free"), TextureWrap::ClampV)
        );
        objects.push_back(std::move(srgbEarth));
        auto linearEarth = createSceneObject(
            "earth, linear", std::make_unique<Sphere>(), satin, ball(COLUMNS[1], BACK_Y, BACK_Z)
        );
        linearEarth->setDiffusionShader(
            std::make_unique<HDRImageShader>(*resources.getLinearPPM("earth_ice_free"), TextureWrap::ClampV)
        );
        objects.push_back(std::move(linearEarth));
        objects.push_back(createSceneObject(
            "mirror sphere", std::make_unique<Sphere>(), SceneMaterial::metal(Float4(0.95f, 0.95f, 0.95f, 0.f), 0.05f),
            ball(COLUMNS[2], BACK_Y, BACK_Z)
        ));
        const auto* teapot = resources.getMesh("teapot");
        const auto teapotScale = fitMeshScale(*teapot, 2.f * RADIUS * 1.2f);
        auto normals = createSceneObject(
            "teapot, normals as colors", teapot, SceneMaterial::emissive(Zero<Float4>()),
            {Float3(teapotScale), Float3(0.f, -30.f, 0.f), Float3(COLUMNS[3], BACK_Y - 0.08f, BACK_Z)}
        );
        normals->setEmittanceShader(std::make_unique<IntersectionNormalShader>());
        objects.push_back(std::move(normals));

        // Front row: constant color, marble times blue, checker times marble, transformed checker on a box
        const auto cube = [](const Float x, const Float rotation)
        {
            return Placement{
                Float3(0.8f * RADIUS, 0.8f * RADIUS, 0.8f * RADIUS), Float3(0.f, rotation, 0.f),
                Float3(x, -1.f + 0.8f * RADIUS, FRONT_Z)
            };
        };
        objects.push_back(createSceneObject(
            "constant color", std::make_unique<Sphere>(),
            SceneMaterial::plastic(Float4(0.7f, 0.15f, 0.1f, 0.f), 0.04f, 0.3f), ball(COLUMNS[0], FRONT_Y, FRONT_Z)
        ));
        auto tinted = createSceneObject("marble times blue", std::make_unique<Box>(), satin, cube(COLUMNS[1], 25.f));
        tinted->setDiffusionShader(std::make_unique<Resources::MultiplyByConstMaterialShader>(
            Float4(0.4f, 0.6f, 1.f, 1.f), marbleShader(resources)
        ));
        objects.push_back(std::move(tinted));
        auto product = createSceneObject(
            "checker times marble", std::make_unique<Box>(), satin, cube(COLUMNS[2], -25.f)
        );
        product->setDiffusionShader(std::make_unique<MultiplyingMaterialShader>(
            std::multiplies<Float4>{}, checkerShader(resources), marbleShader(resources)
        ));
        objects.push_back(std::move(product));
        auto transformed = createSceneObject(
            "box, transformed checker", std::make_unique<Box>(), satin, cube(COLUMNS[3], 40.f)
        );
        transformed->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("checker")));
        transformed->scaleTexture(Float2(2.f, 2.f));
        transformed->rotateTexture(45.f);
        transformed->translateTexture(Float2(0.25f, 0.f));
        objects.push_back(std::move(transformed));
    }
}
