#include "tests/MeshesScene.h"
#include "../stdafx.h"

#include "tests/SceneObjects.h"

#include <array>
#include <string_view>

namespace raytracerui
{
    namespace
    {
        constexpr auto COLUMNS = 6;
        constexpr auto CELL = 0.45f;
        constexpr auto SIZE = 0.38f;
        constexpr auto Z = -0.3f;
    }

    void MeshesScene::setup(Scene& scene, Resources& resources)
    {
        scene.setBackgroundShader(std::make_unique<RGBBackgroundShader>(RGBS::black()));
        scene.setAmbientLight(RGBS(0.02f, 0.02f, 0.02f));

        auto key = std::make_unique<LightInfo>(LightInfo::blackbody(5000.f, 0.4f));
        key->position = Float4(-2.f, 2.5f, 3.5f, 1.f);
        key->attenuationFactors = Float4(1.f, 0.f, 0.f, 1.f);
        scene.getLights().push_back(std::move(key));
        auto fill = std::make_unique<LightInfo>(LightInfo::blackbody(8000.f, 0.12f));
        fill->position = Float4(2.5f, 0.f, 3.f, 1.f);
        fill->attenuationFactors = Float4(1.f, 0.f, 0.f, 1.f);
        scene.getLights().push_back(std::move(fill));

        auto& objects = scene.getSceneObjects();
        objects.push_back(createSceneObject(
            "backdrop", std::make_unique<Plane>(), SceneMaterial::matte(Float4(0.18f, 0.18f, 0.2f, 0.f)),
            {Float3(1.f, 1.f, 1.f), Float3(0.f, 0.f, 0.f), Float3(0.f, 0.f, -1.5f)}
        ));

        // Mesh and its rotation to stand upright and face the camera
        struct MeshPose
        {
            std::string_view name;
            Float3 rotation;
        };
        const std::array meshes{
            MeshPose{"auto_high", Float3(0.f, 90.f, 0.f)}, MeshPose{"auto_low", Float3(0.f, 0.f, 0.f)},
            MeshPose{"bunny_high", Float3(0.f, 0.f, 0.f)}, MeshPose{"bunny_low", Float3(0.f, 0.f, 0.f)},
            MeshPose{"camel_head", Float3(0.f, 0.f, 0.f)}, MeshPose{"chair", Float3(0.f, 0.f, 0.f)},
            MeshPose{"chessfigure", Float3(0.f, 0.f, 0.f)}, MeshPose{"cone", Float3(0.f, 0.f, 0.f)},
            MeshPose{"cow", Float3(0.f, 0.f, 0.f)}, MeshPose{"cowboyhat", Float3(0.f, 0.f, 0.f)},
            MeshPose{"cup", Float3(0.f, 0.f, 0.f)}, MeshPose{"cup_fluted", Float3(0.f, 0.f, 0.f)},
            MeshPose{"dragon1", Float3(-90.f, 0.f, 0.f)}, MeshPose{"dragon2", Float3(0.f, 0.f, 0.f)},
            MeshPose{"eight", Float3(0.f, 0.f, 0.f)}, MeshPose{"europemap", Float3(0.f, 0.f, 0.f)},
            MeshPose{"heptoroid", Float3(-60.f, 0.f, 0.f)}, MeshPose{"mannequin", Float3(-90.f, 0.f, 0.f)},
            MeshPose{"pistol", Float3(0.f, 0.f, 0.f)}, MeshPose{"spaceshuttle", Float3(0.f, 0.f, 0.f)},
            MeshPose{"sphere_high", Float3(0.f, 0.f, 0.f)}, MeshPose{"sphere_low", Float3(0.f, 0.f, 0.f)},
            MeshPose{"spikey_sphere", Float3(0.f, 0.f, 0.f)}, MeshPose{"tank", Float3(0.f, 90.f, 0.f)},
            MeshPose{"teapot", Float3(0.f, 0.f, 0.f)}, MeshPose{"teapot_branch", Float3(0.f, 0.f, 0.f)},
            MeshPose{"teapot_clean", Float3(0.f, 0.f, 0.f)}, MeshPose{"teapot_gem", Float3(0.f, 0.f, 0.f)},
            MeshPose{"teapot_hex", Float3(0.f, 0.f, 0.f)}, MeshPose{"teapot_honey", Float3(0.f, 0.f, 0.f)},
            MeshPose{"teapot_leaf", Float3(0.f, 0.f, 0.f)}, MeshPose{"temple", Float3(0.f, 0.f, 0.f)},
            MeshPose{"tire", Float3(0.f, 90.f, 0.f)},
        };

        const auto clay = SceneMaterial::plastic(Float4(0.6f, 0.57f, 0.53f, 0.f), 0.04f, 0.5f);
        const auto rows = (static_cast<int>(meshes.size()) + COLUMNS - 1) / COLUMNS;
        for (auto index = 0; index < static_cast<int>(meshes.size()); ++index)
        {
            const auto& [name, rotation] = meshes[static_cast<ASizeT>(index)];
            const auto* mesh = resources.getMesh(std::string{name});
            const auto column = index % COLUMNS;
            const auto row = index / COLUMNS;
            const Float3 position{
                (static_cast<Float>(column) - 0.5f * static_cast<Float>(COLUMNS - 1)) * CELL,
                (0.5f * static_cast<Float>(rows - 1) - static_cast<Float>(row)) * CELL, Z
            };
            objects.push_back(createSceneObject(
                std::string{name}, mesh, clay, {Float3(fitMeshScale(*mesh, SIZE)), rotation, position}
            ));
        }
    }
}
