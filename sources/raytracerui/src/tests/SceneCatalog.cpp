#include "tests/SceneCatalog.h"
#include "../stdafx.h"

#include "tests/CornellBoxScene.h"
#include "tests/FormsScene.h"
#include "tests/LightingScene.h"
#include "tests/MaterialsScene.h"
#include "tests/MeshesScene.h"
#include "tests/ProceduralScene.h"
#include "tests/ShadingScene.h"
#include "tests/ShowcaseScene.h"

#include <array>

namespace raytracerui
{
    namespace
    {
        constexpr std::array scenes{
            SceneEntry{"cornell-box", "Cornell Box", &CornellBoxScene::setup},
            SceneEntry{"procedural", "Procedural", &ProceduralScene<>::setup},
            SceneEntry{"forms", "Forms", &FormsScene::setup},
            SceneEntry{"lighting", "Lighting", &LightingScene::setup},
            SceneEntry{"shading", "Shading", &ShadingScene::setup},
            SceneEntry{"materials", "Materials & Noise", &MaterialsScene::setup},
            SceneEntry{"meshes", "Meshes", &MeshesScene::setup},
            SceneEntry{"showcase", "Showcase", &ShowcaseScene::setup},
        };
    }

    std::span<const SceneEntry> sceneCatalog() noexcept
    {
        return scenes;
    }

    std::optional<ASizeT> findScene(const std::string_view id) noexcept
    {
        for (auto index = Zero<ASizeT>(); index < scenes.size(); ++index)
        {
            if (id == scenes[index].id)
            {
                return index;
            }
        }
        return std::nullopt;
    }
}
