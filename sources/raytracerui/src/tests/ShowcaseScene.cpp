#include "tests/ShowcaseScene.h"
#include "../stdafx.h"

#include "tests/SceneObjects.h"

namespace raytracerui
{
    namespace
    {
        constexpr auto FLOOR_Y = -1.f;

        // Scales every light of the scene: the ambient light, the lights and their emitters
        constexpr auto LIGHT_SCALE = 0.85f;

        void addLight(Scene& scene, LightInfo light, const Float4& position, const Float4& attenuationFactors)
        {
            light.position = position;
            light.attenuationFactors = attenuationFactors;
            scene.getLights().push_back(std::make_unique<LightInfo>(light));
        }

        // A mesh standing on the floor, fitted to the given size
        std::unique_ptr<SceneObject> standingMesh(
            Resources& resources, const std::string& name, const SceneMaterial& material, const Float size,
            const Float x, const Float z, const Float rotationY
        )
        {
            const auto* mesh = resources.getMesh(name);
            const auto scale = fitMeshScale(*mesh, size);
            const auto halfHeight = Half<Float>() * scale * y(extents(mesh->getBounding()));
            return createSceneObject(
                name, mesh, material,
                {Float3(scale), Float3(0.f, rotationY, 0.f), Float3(x, FLOOR_Y + halfHeight, z)}
            );
        }
    }

    void ShowcaseScene::setup(Scene& scene, Resources& resources)
    {
        scene.setBackgroundShader(std::make_unique<RGBBackgroundShader>(RGBS::black()));
        scene.setAmbientLight(RGBS(0.01f * LIGHT_SCALE, 0.01f * LIGHT_SCALE, 0.012f * LIGHT_SCALE));

        // Warm softbox from the upper left (a sphere light: soft shadows, seen round in the chrome sphere), a cooler
        // rectangular softbox of 1.2 x 0.8 from the upper right as a fill (a box light whose front face, its local -y,
        // turns towards the dragon: seen as a rectangle next to the round one), cool rim from behind on the right,
        // cool glow on the backdrop. Quadratic falloff keeps the far backdrop dark.
        addSphereLight(
            scene, "softbox", Float4(-2.f, 2.5f, 2.5f, 1.f), 0.6f, Float4(14.f, 11.f, 8.f, 0.f) * Float4(LIGHT_SCALE)
        );
        addBoxLight(
            scene, "fill softbox", Float4(2.f, 2.3f, 2.3f, 1.f), Float3(0.6f, 0.02f, 0.4f), Float3(50.9f, 35.5f, 0.f),
            Float4(5.f, 5.5f, 6.5f, 0.f) * Float4(LIGHT_SCALE), LightInfo::faceBit(1, false)
        );
        const Float4 quadratic{0.f, 0.f, 1.f, 1.f};
        addLight(scene, LightInfo::blackbody(9000.f, 3.f * LIGHT_SCALE), Float4(2.5f, 1.5f, -1.2f, 1.f), quadratic);
        addLight(scene, LightInfo::blackbody(12000.f, 0.3f * LIGHT_SCALE), Float4(0.f, 0.f, -2.1f, 1.f), quadratic);

        auto& objects = scene.getSceneObjects();

        // Black lacquer floor: barely diffuse, Fresnel reflections that grow towards grazing angles
        objects.push_back(createSceneObject(
            "lacquer floor", std::make_unique<Plane>(),
            SceneMaterial::plastic(Float4(0.03f, 0.03f, 0.035f, 0.f), 0.05f, 0.1f),
            {Float3(1.f, 1.f, 1.f), Float3(-90.f, 0.f, 0.f), Float3(0.f, FLOOR_Y, 0.f)}
        ));
        objects.push_back(createSceneObject(
            "backdrop", std::make_unique<Plane>(), SceneMaterial::matte(Float4(0.08f, 0.085f, 0.1f, 0.f)),
            {Float3(1.f, 1.f, 1.f), Float3(0.f, 0.f, 0.f), Float3(0.f, 0.f, -2.5f)}
        ));

        // Amber glass dragon in the middle: absorption tints the thick parts darker
        objects.push_back(standingMesh(
            resources, "dragon2",
            SceneMaterial::dielectric(RefractionIndices::Glass, Float4(0.9f, 0.35f, 0.1f, 0.f), 0.05f), 1.9f, 0.05f,
            -0.7f, -25.f
        ));

        // Gold bunny on the left
        objects.push_back(standingMesh(
            resources, "bunny_high", SceneMaterial::metal(Float4(1.f, 0.78f, 0.34f, 0.f), 0.2f), 0.6f, -0.8f, 0.35f,
            25.f
        ));

        // Chrome sphere on a marble pedestal on the right
        constexpr auto PEDESTAL_HALF_HEIGHT = 0.28f;
        auto pedestal = createSceneObject(
            "marble pedestal", std::make_unique<Box>(), SceneMaterial::plastic(Float4(0.8f, 0.8f, 0.8f, 0.f), 0.04f, 0.15f),
            {Float3(0.18f, PEDESTAL_HALF_HEIGHT, 0.18f), Float3(0.f, 30.f, 0.f),
             Float3(0.85f, FLOOR_Y + PEDESTAL_HALF_HEIGHT, 0.f)}
        );
        pedestal->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("marble")));
        objects.push_back(std::move(pedestal));
        constexpr auto CHROME_RADIUS = 0.2f;
        objects.push_back(createSceneObject(
            "chrome sphere", std::make_unique<Sphere>(), SceneMaterial::metal(Float4(0.95f, 0.95f, 0.95f, 0.f), 0.02f),
            {Float3(CHROME_RADIUS, CHROME_RADIUS, CHROME_RADIUS), Float3(0.f, 0.f, 0.f),
             Float3(0.85f, FLOOR_Y + 2.f * PEDESTAL_HALF_HEIGHT + CHROME_RADIUS, 0.f)}
        ));

        // Diamond in front
        constexpr auto DIAMOND_RADIUS = 0.13f;
        objects.push_back(createSceneObject(
            "diamond", std::make_unique<Sphere>(),
            SceneMaterial::dielectric(RefractionIndices::Diamond, Float4(10.f, 10.f, 10.f, 0.f), 0.02f),
            {Float3(DIAMOND_RADIUS, DIAMOND_RADIUS, DIAMOND_RADIUS), Float3(0.f, 0.f, 0.f),
             Float3(0.1f, FLOOR_Y + DIAMOND_RADIUS, 0.6f)}
        ));
    }
}
