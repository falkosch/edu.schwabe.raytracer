#include "stdafx.h"

#include "tests/CornellBoxScene.h"
#include "tests/DragonScene.h"
#include "tests/ProceduralScene.h"
#include "tests/TestScene1.h"
#include "tests/TestScene2.h"

#include "Benchmarks.h"
#include "ManualTests.h"
#include "OpenGLWindowsRaytracerUI.h"

#include <logging.h>

#include <algorithm>
#include <memory>
#include <string>

#include <windows.h>

// MAX_TRACE_DEPTH >= 969 causes stack overflows
// MAX_TRACE_DEPTH > 31 for most scenes has no significant visual change anymore
#ifdef NDEBUG
constexpr ASizeT FAST_PREVIEW_SIZE = 512;
constexpr ASizeT MAX_TRACE_DEPTH = 31;
#else
const ASizeT FAST_PREVIEW_SIZE = 64;
const ASizeT MAX_TRACE_DEPTH = 0;
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

static const auto Log = logging::scope("Main");

namespace raytracerui {
  LRESULT runRaytracerUI() {
    Resources resources{};
    Scene scene{
        std::make_unique<NaiveKDTreeTraverser<SceneIntersection>>(), std::make_unique<FixedIterationsSAHKDTreeBalancer>()
    };

    CornellBoxScene::setup(scene, resources);
    // TestScene1::setup(scene, resources);
    // TestScene2::setup(scene, resources);
    // DragonScene::setup(scene, resources);
    // ProceduralScene<3, 10>::setup(scene, resources);

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

    auto ui = std::make_unique<OpenGLWindowsRaytracerUI>(raytracer, parameters, FAST_PREVIEW_SIZE);
    WPARAM returnCode{};
    try {
      returnCode = ui->run();
    } catch (const std::exception &exception) {
      std::string what = exception.what();
      Log.error([what] { return what; });
    } catch (...) {
      Log.error([] { return "non standard exception occurred"; });
    }

    return static_cast<LRESULT>(returnCode);
  }
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  logging::Logger::instance().setLogFile("raytracer.log");
  logging::Logger::instance().start();

  std::array<TCHAR, MAX_PATH + 1> currentPath{0};
  GetCurrentDirectory(MAX_PATH, currentPath.data());

  std::wstring wpath(currentPath.data());
  std::string path(wpath.size(), '\0');
  std::transform(wpath.begin(), wpath.end(), path.begin(), [](wchar_t c) { return static_cast<char>(c); });
  Log.info([path] { return "Working directory: " + path; });

  raytracerui::ManualTests()();
  // raytracerui::Benchmarks()();

  auto result = static_cast<int>(raytracerui::runRaytracerUI());
  logging::Logger::instance().stop();
  return result;
}
