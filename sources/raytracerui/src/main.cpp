#include "stdafx.h"

#include "tests/SceneCatalog.h"

#include "OpenGLWindowsRaytracerUI.h"

#include <logging.h>

#include <algorithm>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>

#include <windows.h>

// MAX_TRACE_DEPTH >= 969 causes stack overflows
// MAX_TRACE_DEPTH > 31 for most scenes has no significant visual change anymore
#ifdef NDEBUG
const Size2 FAST_PREVIEW_SIZE{512, 512};
constexpr ASizeT MAX_TRACE_DEPTH = 31;
#else
const Size2 FAST_PREVIEW_SIZE{64, 64};
constexpr ASizeT MAX_TRACE_DEPTH = 0;
#endif

constexpr Float PERSPECTIVE_FOV = 45.f;

const auto PERSPECTIVE_Z_PLANES = Float2(0.01f, 2.0f);

const auto PERSPECTIVE_Z_PLANE_EXTENDS = Float2(2.0f);

const auto CAMERA_INIT_TRANSLATION = Float3(0.0f, 0.0f, -3.2f);

// x => x * x * screen-pixels == image-pixels
constexpr Float SAMPLING_FACTOR = 1.0f;

// n => (n+1)x(n+1) samples/image-pixel
constexpr ASizeT SUPER_SAMPLING_FACTOR = 0;

constexpr Float MAX_DISTANCE = std::numeric_limits<Float>::max();

constexpr Int CULLING_ORIENTATION = -1;

const Float VISIBILITY_CUTOFF = reciprocal(512.0f);

// n => (n+1)x(n+1) image-pixels/packet
constexpr ASizeT RAY_PACKET_SIZE = 15;

// Command-line options: --scene=<id> picks the start scene (ids in tests/SceneCatalog.cpp), --exit-after-render
// closes the window once the first image is saved as ray-traced.png.
constexpr std::string_view SCENE_OPTION = "--scene=";
constexpr std::string_view EXIT_AFTER_RENDER_OPTION = "--exit-after-render";

static const auto Log = logging::scope("Main");

namespace raytracerui
{
    LRESULT runRaytracerUI(const ASizeT sceneIndex, const bool exitAfterRender)
    {
        Resources resources{};
        Scene scene{
            std::make_unique<WideKDTreeTraverser<SceneIntersection>>(), std::make_unique<BruteForceSAHKDTreeBalancer>()
        };

        sceneCatalog()[sceneIndex].setup(scene, resources);

        scene.buildSceneGraph();

        Camera camera{};
        camera.setProjection(PERSPECTIVE_FOV, PERSPECTIVE_Z_PLANE_EXTENDS, PERSPECTIVE_Z_PLANES);
        camera.translate(CAMERA_INIT_TRANSLATION);

        RaytraceParameters parameters{};
        parameters.visibilityCutoff = VISIBILITY_CUTOFF;
        parameters.maxDistance = MAX_DISTANCE;
        parameters.maxTraceDepth = MAX_TRACE_DEPTH;
        parameters.sceneShader = &scene;
        parameters.cullingOrientation = CULLING_ORIENTATION;
        parameters.perspectiveZPlanes = PERSPECTIVE_Z_PLANES;
        parameters.perspectiveFOV = PERSPECTIVE_FOV;
        parameters.samplingFactor = SAMPLING_FACTOR;
        parameters.superSamplingFactor = SUPER_SAMPLING_FACTOR;
        parameters.rayPacketSize = RAY_PACKET_SIZE;
        parameters.camera = &camera;

        Raytracer raytracer{};

        auto ui = std::make_unique<
            OpenGLWindowsRaytracerUI>(raytracer, parameters, FAST_PREVIEW_SIZE, scene, resources);
        ui->setSceneIndex(static_cast<int>(sceneIndex));
        ui->setExitAfterFirstImage(exitAfterRender);
        WPARAM returnCode{};
        try
        {
            returnCode = ui->run();
        }
        catch (const std::exception& exception)
        {
            std::string what = exception.what();
            Log.error([what] { return what; });
        }
        catch (...)
        {
            Log.error([] { return "non standard exception occurred"; });
        }

        return static_cast<LRESULT>(returnCode);
    }
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR commandLine, int)
{
    logging::Logger::instance().setLogFile("raytracer.log");
    logging::Logger::instance().start();

    std::array<TCHAR, MAX_PATH + 1> currentPath{0};
    GetCurrentDirectory(MAX_PATH, currentPath.data());

    std::wstring wpath(currentPath.data());
    std::string path(wpath.size(), '\0');
    std::transform(wpath.begin(), wpath.end(), path.begin(), [](wchar_t c) { return static_cast<char>(c); });
    Log.info([path] { return "Working directory: " + path; });

    auto sceneIndex = Zero<ASizeT>();
    auto exitAfterRender = false;
    std::istringstream arguments{commandLine};
    for (std::string argument; arguments >> argument;)
    {
        if (argument.starts_with(SCENE_OPTION))
        {
            const auto id = argument.substr(SCENE_OPTION.size());
            if (const auto found = raytracerui::findScene(id))
            {
                sceneIndex = *found;
            }
            else
            {
                Log.error([id] { return "Unknown scene '" + id + "', starting the default scene"; });
            }
        }
        else if (argument == EXIT_AFTER_RENDER_OPTION)
        {
            exitAfterRender = true;
        }
        else
        {
            Log.warn([argument] { return "Ignoring unknown argument '" + argument + "'"; });
        }
    }
    Log.info([sceneIndex] { return std::string{"Scene: "} + raytracerui::sceneCatalog()[sceneIndex].label; });

    const auto result = static_cast<int>(raytracerui::runRaytracerUI(sceneIndex, exitAfterRender));
    logging::Logger::instance().stop();
    return result;
}
