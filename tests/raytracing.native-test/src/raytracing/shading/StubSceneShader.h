#pragma once

#include "raytracing_tests.h"

#include <raytracing/shading/SceneShader.h>

namespace raytracer::test
{
    // A SceneShader without scene graph, for shaders that only need the scene shader's interface.
    class StubSceneShader final : public SceneShader
    {
    public:
        void buildSceneGraph() override
        {
        }
    };
}
