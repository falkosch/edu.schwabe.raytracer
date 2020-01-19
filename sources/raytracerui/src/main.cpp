#include "stdafx.h"

#include "tests/CornellBoxScene.h"
#include "tests/TestScene1.h"
#include "tests/TestScene2.h"
#include "tests/ProceduralScene.h"
#include "tests/DragonScene.h"

#include "OpenGLWindowsRaytracerUI.h"
#include "ManualTests.h"
#include "Benchmarks.h"

#include <iostream>
#include <string>
#include <memory>

#include <Windows.h>

// MAX_TRACE_DEPTH >= 969 causes stackoverflows
// MAX_TRACE_DEPTH > 31 for most scenes has no significant visual change anymore
#ifdef NDEBUG
const ASizeT FAST_PREVIEW_SIZE = 512;
const ASizeT MAX_TRACE_DEPTH = 31;
#else
const ASizeT FAST_PREVIEW_SIZE = 64;
const ASizeT MAX_TRACE_DEPTH = 0;
#endif

const Float PERSPECTIVE_FOV = 45.f;

const Float2 PERSPECTIVE_Z_PLANES = Float2(0.01f, 2.0f);

const Float2 PERSPECTIVE_Z_PLANE_EXTENDS = Float2(2.0f);

const Float3 CAMERA_INIT_TRANSLATION = Float3(0.0f, 0.0f, -3.2f);

// x => x * x * screen-pixels == image-pixels
const Float SAMPLING_FACTOR = 1.0f;

// n => (n+1)x(n+1) samples/image-pixel
const ASizeT SUPER_SAMPLING_FACTOR = 0;

const Float MAX_DISTANCE = std::numeric_limits<Float>::max();

const Int CULLING_ORIENTATION = -1;

const Float VISIBILITY_CUTOFF = reciprocal(512.0f);

// n => (n+1)x(n+1) image-pixels/packet
const ASizeT RAY_PACKET_SIZE = 15;

namespace raytracerui
{
    const LRESULT runRaytracerUI() {
        Resources resources{ };
        Scene scene{
            new NaiveKDTreeTraverser<SceneIntersection>(),
            new FixedIterationsSAHKDTreeBalancer()
        };

        CornellBoxScene::setup(scene, resources);
        //TestScene1::setup(scene, resources);
        //TestScene2::setup(scene, resources);
        //DragonScene::setup(scene, resources);
        //ProceduralScene<3, 10>::setup(scene, resources);

        scene.buildSceneGraph();

        Camera camera{ };
        camera.setProjection(PERSPECTIVE_FOV, PERSPECTIVE_Z_PLANE_EXTENDS, PERSPECTIVE_Z_PLANES);
        camera.translate(CAMERA_INIT_TRANSLATION);

        RaytraceParameters parameters{ };
        parameters.visibilityCutoff = VISIBILITY_CUTOFF;
        parameters.maxDistance = MAX_DISTANCE;
        parameters.maxTraceDepth = MAX_TRACE_DEPTH;
        parameters.sceneShader = &scene;
        parameters.cullingOrientation = CULLING_ORIENTATION;
        parameters.perspectiveZPlanes = PERSPECTIVE_Z_PLANES;
        parameters.perspectiveFOV = PERSPECTIVE_FOV;
        parameters.samplingFactor = SAMPLING_FACTOR;
        parameters.supersamplingFactor = SUPER_SAMPLING_FACTOR;
        parameters.rayPacketSize = RAY_PACKET_SIZE;
        parameters.camera = &camera;

        Raytracer raytracer{ };

        auto ui = std::make_unique<WindowsRaytracerUI>(raytracer, parameters, FAST_PREVIEW_SIZE);
        WPARAM returnCode{ };
        try {
            returnCode = ui->run();
        } catch (const std::exception & exception) {
            std::cerr << exception.what() << std::endl;
        } catch (...) {
            std::cerr << "non standard exception occured" << std::endl;
        }

        return returnCode;
    }
}

int __cdecl main() {
    std::array<TCHAR, MAX_PATH + 1> currentPath{ 0 };
    GetCurrentDirectory(MAX_PATH, currentPath.data());

    std::wcout << "Working directory: " << std::wstring(currentPath.data()) << std::endl;

    raytracerui::ManualTests()();
    //raytracerui::Benchmarks()();

    return static_cast<int>(raytracerui::runRaytracerUI());
}