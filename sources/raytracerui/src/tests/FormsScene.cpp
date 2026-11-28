#include "tests/FormsScene.h"
#include "../stdafx.h"

#include "tests/SceneObjects.h"

#include <raytracing/shading/shaders/IntersectionNormalShader.h>

namespace raytracerui
{
    namespace
    {
        // A form twice, side by side, offset left and right of the placement: on the left with the checker texture,
        // on the right with its surface normals as the diffuse color (IntersectionNormalShader: each component of the
        // world-space normal mapped from [-1, 1] to [0, 1])
        template <typename MakeForm>
        void addPair(
            Scene::SceneList& objects, Resources& resources, const std::string& id, const MakeForm& makeForm,
            const SceneMaterial& material, const Placement& placement, const Float offset
        )
        {
            auto left = placement;
            left.translation = placement.translation - Float3(offset, 0.f, 0.f);
            auto checkered = createSceneObject(id, makeForm(), material, left);
            checkered->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("checker")));
            objects.push_back(std::move(checkered));

            auto right = placement;
            right.translation = placement.translation + Float3(offset, 0.f, 0.f);
            auto normals = createSceneObject(id + ", normals as colors", makeForm(), material, right);
            normals->setDiffusionShader(std::make_unique<IntersectionNormalShader>());
            objects.push_back(std::move(normals));
        }
    }

    void FormsScene::setup(Scene& scene, Resources& resources)
    {
        scene.setBackgroundShader(std::make_unique<RGBBackgroundShader>(RGBS(0.02f, 0.025f, 0.035f)));
        scene.setAmbientLight(RGBS::black());

        // Light values times pi: a white surface facing a light reflects its irradiance / pi (Lambert)
        // Neutral key light from the upper left, a directional light (without falloff, hard shadows)
        auto key = std::make_unique<LightInfo>(LightInfo::blackbody(5500.f, 0.45f * Pi<Float>()));
        key->direction = normalize3(Float4(-2.5f, 3.f, 3.f, 0.f));
        scene.getLights().push_back(std::move(key));

        // Cooler, weaker fill from the right
        auto fill = std::make_unique<LightInfo>(LightInfo::d65(0.12f * Pi<Float>()));
        fill->direction = normalize3(Float4(3.f, 1.f, 2.5f, 0.f));
        scene.getLights().push_back(std::move(fill));

        auto& objects = scene.getSceneObjects();
        const auto paper = SceneMaterial::matte(Float4(0.8f, 0.8f, 0.8f, 0.f));
        const auto glossy = SceneMaterial::plastic(Float4(0.8f, 0.8f, 0.8f, 0.f), 0.04f, 0.2f);

        // Planes: the tiled floor faces up, the plain back wall faces the camera and shows the shadows
        auto floor = createSceneObject(
            "floor plane", std::make_unique<Plane>(), paper,
            {Float3(1.f, 1.f, 1.f), Float3(-90.f, 0.f, 0.f), Float3(0.f, -1.f, 0.f)}
        );
        floor->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("seamless_floor_concrete")));
        floor->scaleTexture(Float2(0.5f, 0.5f));
        objects.push_back(std::move(floor));
        objects.push_back(createSceneObject(
            "back wall plane", std::make_unique<Plane>(), paper,
            {Float3(1.f, 1.f, 1.f), Float3(0.f, 0.f, 0.f), Float3(0.f, 0.f, -2.f)}
        ));

        // Back row: analytic sphere, upright box, mesh sphere
        // A shelf of boxes: board on two legs
        const auto wood = SceneMaterial::plastic(Float4(0.45f, 0.3f, 0.18f, 0.f), 0.04f, 0.4f);
        objects.push_back(createSceneObject(
            "shelf board", std::make_unique<Box>(), wood,
            {Float3(1.5f, 0.04f, 0.35f), Float3(0.f, 0.f, 0.f), Float3(0.f, -0.25f, -1.1f)}
        ));
        for (const auto x : {-1.3f, 1.3f})
        {
            objects.push_back(createSceneObject(
                "shelf leg", std::make_unique<Box>(), wood,
                {Float3(0.04f, 0.355f, 0.3f), Float3(0.f, 0.f, 0.f), Float3(x, -0.645f, -1.1f)}
            ));
        }

        // Back row on the shelf (its top at y = -0.21), in pairs: analytic sphere, upright box, mesh sphere
        constexpr auto shelfOffset = 0.22f;
        const auto sphere = [] { return std::make_unique<Sphere>(); };
        const auto box = [] { return std::make_unique<Box>(); };
        addPair(
            objects, resources, "sphere", sphere, glossy,
            {Float3(0.19f, 0.19f, 0.19f), Float3(0.f, 0.f, 0.f), Float3(-1.f, -0.02f, -1.1f)}, shelfOffset
        );
        addPair(
            objects, resources, "box", box, glossy,
            {Float3(0.14f, 0.14f, 0.14f), Float3(0.f, 30.f, 0.f), Float3(0.f, -0.07f, -1.1f)}, shelfOffset
        );
        const auto* meshSphere = resources.getMesh("sphere_high");
        const auto meshSphereScale = fitMeshScale(*meshSphere, 0.38f);
        addPair(
            objects, resources, "mesh sphere", [meshSphere] { return meshSphere; }, glossy,
            {Float3(meshSphereScale), Float3(0.f, 0.f, 0.f), Float3(1.f, -0.02f, -1.1f)}, shelfOffset
        );

        // Front row on the floor, in pairs: ellipsoid, tilted cube standing on a corner, low-poly mesh
        constexpr auto floorOffset = 0.24f;
        addPair(
            objects, resources, "ellipsoid", sphere, glossy,
            {Float3(0.2f, 0.1f, 0.125f), Float3(0.f, 0.f, 15.f), Float3(-0.85f, -0.89f, -0.1f)}, floorOffset
        );
        addPair(
            objects, resources, "tilted cube", box, glossy,
            {Float3(0.12f, 0.12f, 0.12f), Float3(35.26f, 45.f, 0.f), Float3(0.f, -0.79f, -0.1f)}, floorOffset
        );
        const auto* cone = resources.getMesh("cone");
        const auto coneScale = fitMeshScale(*cone, 0.36f);
        addPair(
            objects, resources, "cone mesh", [cone] { return cone; }, glossy,
            {Float3(coneScale), Float3(0.f, 0.f, 0.f), Float3(0.85f, -0.82f, -0.1f)}, floorOffset
        );
    }
}
