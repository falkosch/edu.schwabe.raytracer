#include "stdafx.h"

#include "tests/SceneCatalog.h"

#include "OpenGLWindowsRaytracerUI.h"

#include <logging.h>

#include <algorithm>
#include <csignal>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>

#include <crtdbg.h>
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

// distribution ray tracing: the most reflection rays per glossy hit and shadow rays per area light (sphere or box
// light), 0 for Whitted
constexpr ASizeT GLOSSY_SAMPLES = 16;
constexpr ASizeT SPHERE_LIGHT_SAMPLES = 16;

// diffuse interreflection (one bounce): the most rays a diffuse hit gathers the light of other surfaces with, in place
// of the ambient light; 0 for none
constexpr ASizeT DIFFUSE_SAMPLES = 16;

// many-light sampling: the most point lights whose shadow rays a diffuse ray's hit traces, picked by their light where
// a scene has more (0 for all)
constexpr ASizeT MANY_LIGHT_SAMPLES = 16;

// against fireflies: the most radiance of one glossy reflection ray or diffuse ray (0 for no limit)
constexpr Float GLOSSY_CLAMP = 4.0f;

// adaptive sampling: the most further rounds of sub-samples a pixel takes where its neighbours' sub-samples spread (the
// standard error of their mean in display values above the threshold); 0 rounds is off. It needs 2 x 2 sub-samples
// (full renders).
constexpr ASizeT ADAPTIVE_ROUNDS = 3;
constexpr Float ADAPTIVE_THRESHOLD = 2.0f / 255.0f;

// denoising, guided by the normals, depths and albedos of the pixels' surfaces: of fast previews (one sample per pixel)
// and of full renders (Enter), which it would cost glossy detail
constexpr bool DENOISE = true;
constexpr bool FULL_RENDER_DENOISE = false;

// irradiance caching: diffuse hits interpolate records of the irradiance instead of tracing diffuse rays
constexpr bool IRRADIANCE_CACHING = true;

// the least super-sampling factor of a full render (Enter)
#ifdef NDEBUG
constexpr ASizeT FULL_RENDER_SUPER_SAMPLING_FACTOR = 2;
#else
constexpr ASizeT FULL_RENDER_SUPER_SAMPLING_FACTOR = 0;
#endif

constexpr Float MAX_DISTANCE = std::numeric_limits<Float>::max();

constexpr Int CULLING_ORIENTATION = -1;

const Float VISIBILITY_CUTOFF = reciprocal(512.0f);

// n => (n+1)x(n+1) image-pixels/packet
constexpr ASizeT RAY_PACKET_SIZE = 15;

// Command-line options: --scene=<id> picks the start scene (ids in tests/SceneCatalog.cpp), --exposure=<stops> scales
// the displayed image by 2^stops, --max-trace-depth=<n>, --glossy-samples=<n>, --sphere-light-samples=<n>,
// --diffuse-samples=<n>, --many-light-samples=<n> and --super-sampling=<n> replace MAX_TRACE_DEPTH, GLOSSY_SAMPLES,
// SPHERE_LIGHT_SAMPLES, DIFFUSE_SAMPLES, MANY_LIGHT_SAMPLES and SUPER_SAMPLING_FACTOR,
// --full-render renders the first image like Enter does (at least FULL_RENDER_SUPER_SAMPLING_FACTOR), --seed=<n> picks
// another sample pattern of the distribution ray tracing (0 is the default), --adaptive-rounds=<n> and
// --adaptive-threshold=<x> replace ADAPTIVE_ROUNDS and ADAPTIVE_THRESHOLD, --denoise=<0|1>,
// --full-render-denoise=<0|1> and --irradiance-caching=<0|1> replace DENOISE, FULL_RENDER_DENOISE and
// IRRADIANCE_CACHING, --exit-after-render renders without showing the window and exits once the first image is saved
// as ray-traced.png.
constexpr std::string_view SCENE_OPTION = "--scene=";
constexpr std::string_view EXPOSURE_OPTION = "--exposure=";
constexpr std::string_view MAX_TRACE_DEPTH_OPTION = "--max-trace-depth=";
constexpr std::string_view GLOSSY_SAMPLES_OPTION = "--glossy-samples=";
constexpr std::string_view SPHERE_LIGHT_SAMPLES_OPTION = "--sphere-light-samples=";
constexpr std::string_view DIFFUSE_SAMPLES_OPTION = "--diffuse-samples=";
constexpr std::string_view MANY_LIGHT_SAMPLES_OPTION = "--many-light-samples=";
constexpr std::string_view SEED_OPTION = "--seed=";
constexpr std::string_view SUPER_SAMPLING_OPTION = "--super-sampling=";
constexpr std::string_view GLOSSY_CLAMP_OPTION = "--glossy-clamp=";
constexpr std::string_view REGULARIZE_PATHS_OPTION = "--regularize-paths";
constexpr std::string_view VISIBILITY_CUTOFF_OPTION = "--visibility-cutoff=";
constexpr std::string_view ADAPTIVE_ROUNDS_OPTION = "--adaptive-rounds=";
constexpr std::string_view ADAPTIVE_THRESHOLD_OPTION = "--adaptive-threshold=";
constexpr std::string_view DENOISE_OPTION = "--denoise=";
constexpr std::string_view FULL_RENDER_DENOISE_OPTION = "--full-render-denoise=";
constexpr std::string_view IRRADIANCE_CACHING_OPTION = "--irradiance-caching=";
constexpr std::string_view FULL_RENDER_OPTION = "--full-render";
constexpr std::string_view EXIT_AFTER_RENDER_OPTION = "--exit-after-render";

static const auto Log = logging::scope("Main");

namespace
{
    // the number after the option's name
    ASizeT parseCount(const std::string& argument, const std::string_view option)
    {
        const auto count = argument.substr(option.size());
        return static_cast<ASizeT>(std::strtoul(count.c_str(), nullptr, 10));
    }
}

namespace raytracerui
{
    // Unattended runs (--exit-after-render) have nobody to answer the modal dialog of a failed assert or debug CRT
    // check: print the message to stderr instead, write the log and abort.
    void reportFailuresWithoutDialogs()
    {
        _set_error_mode(_OUT_TO_STDERR);
        _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
        _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
        std::signal(SIGABRT, [](int)
        {
            Log.error([] { return "Aborted by a failed assertion or runtime check, see stderr"; });
            logging::Logger::instance().stop();
        });
    }

    LRESULT runRaytracerUI(
        const ASizeT sceneIndex, const Float exposure, const RaytraceBaseParameters& tracing,
        const ASizeT superSampling, const bool fullRender, const bool fullRenderDenoise, const bool exitAfterRender
    )
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
        parameters.visibilityCutoff = tracing.visibilityCutoff;
        parameters.maxDistance = MAX_DISTANCE;
        parameters.maxTraceDepth = tracing.maxTraceDepth;
        parameters.glossySamples = tracing.glossySamples;
        parameters.sphereLightSamples = tracing.sphereLightSamples;
        parameters.diffuseSamples = tracing.diffuseSamples;
        parameters.manyLightSamples = tracing.manyLightSamples;
        parameters.seedOffset = tracing.seedOffset;
        parameters.glossyClamp = tracing.glossyClamp;
        parameters.regularizePaths = tracing.regularizePaths;
        parameters.adaptiveRounds = tracing.adaptiveRounds;
        parameters.adaptiveThreshold = tracing.adaptiveThreshold;
        parameters.denoise = tracing.denoise;
        parameters.irradianceCaching = tracing.irradianceCaching;
        parameters.sceneShader = &scene;
        parameters.cullingOrientation = CULLING_ORIENTATION;
        parameters.perspectiveZPlanes = PERSPECTIVE_Z_PLANES;
        parameters.perspectiveFOV = PERSPECTIVE_FOV;
        parameters.samplingFactor = SAMPLING_FACTOR;
        parameters.superSamplingFactor = superSampling;
        parameters.rayPacketSize = RAY_PACKET_SIZE;
        parameters.camera = &camera;

        Raytracer raytracer{};

        // a render that exits shows no window
        auto ui = std::make_unique<
            OpenGLWindowsRaytracerUI>(raytracer, parameters, FAST_PREVIEW_SIZE, scene, resources, exitAfterRender);
        ui->setSceneIndex(static_cast<int>(sceneIndex));
        ui->setExposure(exposure);
        ui->setFullRenderSuperSamplingFactor(FULL_RENDER_SUPER_SAMPLING_FACTOR);
        ui->setFullRenderDenoise(fullRenderDenoise);
        if (fullRender)
        {
            ui->setFullFirstRender();
        }
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
    auto exposure = Zero<Float>();
    // the options for the ray tracing, the rest of the parameters is set up in runRaytracerUI
    RaytraceBaseParameters tracing{};
    tracing.visibilityCutoff = VISIBILITY_CUTOFF;
    tracing.maxTraceDepth = MAX_TRACE_DEPTH;
    tracing.glossySamples = GLOSSY_SAMPLES;
    tracing.sphereLightSamples = SPHERE_LIGHT_SAMPLES;
    tracing.diffuseSamples = DIFFUSE_SAMPLES;
    tracing.manyLightSamples = MANY_LIGHT_SAMPLES;
    tracing.glossyClamp = GLOSSY_CLAMP;
    tracing.adaptiveRounds = ADAPTIVE_ROUNDS;
    tracing.adaptiveThreshold = ADAPTIVE_THRESHOLD;
    tracing.denoise = DENOISE;
    tracing.irradianceCaching = IRRADIANCE_CACHING;
    auto superSampling = SUPER_SAMPLING_FACTOR;
    auto fullRender = false;
    auto fullRenderDenoise = FULL_RENDER_DENOISE;
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
        else if (argument.starts_with(EXPOSURE_OPTION))
        {
            const auto stops = argument.substr(EXPOSURE_OPTION.size());
            exposure = std::strtof(stops.c_str(), nullptr);
        }
        else if (argument.starts_with(MAX_TRACE_DEPTH_OPTION))
        {
            tracing.maxTraceDepth = parseCount(argument, MAX_TRACE_DEPTH_OPTION);
        }
        else if (argument.starts_with(GLOSSY_SAMPLES_OPTION))
        {
            tracing.glossySamples = parseCount(argument, GLOSSY_SAMPLES_OPTION);
        }
        else if (argument.starts_with(SPHERE_LIGHT_SAMPLES_OPTION))
        {
            tracing.sphereLightSamples = parseCount(argument, SPHERE_LIGHT_SAMPLES_OPTION);
        }
        else if (argument.starts_with(DIFFUSE_SAMPLES_OPTION))
        {
            tracing.diffuseSamples = parseCount(argument, DIFFUSE_SAMPLES_OPTION);
        }
        else if (argument.starts_with(MANY_LIGHT_SAMPLES_OPTION))
        {
            tracing.manyLightSamples = parseCount(argument, MANY_LIGHT_SAMPLES_OPTION);
        }
        else if (argument.starts_with(SEED_OPTION))
        {
            tracing.seedOffset = static_cast<UInt_32>(parseCount(argument, SEED_OPTION));
        }
        else if (argument.starts_with(GLOSSY_CLAMP_OPTION))
        {
            const auto limit = argument.substr(GLOSSY_CLAMP_OPTION.size());
            tracing.glossyClamp = std::strtof(limit.c_str(), nullptr);
        }
        else if (argument.starts_with(VISIBILITY_CUTOFF_OPTION))
        {
            const auto cutoff = argument.substr(VISIBILITY_CUTOFF_OPTION.size());
            tracing.visibilityCutoff = std::strtof(cutoff.c_str(), nullptr);
        }
        else if (argument == REGULARIZE_PATHS_OPTION)
        {
            tracing.regularizePaths = true;
        }
        else if (argument.starts_with(DENOISE_OPTION))
        {
            tracing.denoise = parseCount(argument, DENOISE_OPTION) != Zero<ASizeT>();
        }
        else if (argument.starts_with(FULL_RENDER_DENOISE_OPTION))
        {
            fullRenderDenoise = parseCount(argument, FULL_RENDER_DENOISE_OPTION) != Zero<ASizeT>();
        }
        else if (argument.starts_with(IRRADIANCE_CACHING_OPTION))
        {
            tracing.irradianceCaching = parseCount(argument, IRRADIANCE_CACHING_OPTION) != Zero<ASizeT>();
        }
        else if (argument.starts_with(ADAPTIVE_ROUNDS_OPTION))
        {
            tracing.adaptiveRounds = parseCount(argument, ADAPTIVE_ROUNDS_OPTION);
        }
        else if (argument.starts_with(ADAPTIVE_THRESHOLD_OPTION))
        {
            const auto threshold = argument.substr(ADAPTIVE_THRESHOLD_OPTION.size());
            tracing.adaptiveThreshold = std::strtof(threshold.c_str(), nullptr);
        }
        else if (argument.starts_with(SUPER_SAMPLING_OPTION))
        {
            superSampling = parseCount(argument, SUPER_SAMPLING_OPTION);
        }
        else if (argument == FULL_RENDER_OPTION)
        {
            fullRender = true;
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
    if (exitAfterRender)
    {
        raytracerui::reportFailuresWithoutDialogs();
    }

    const auto result = static_cast<int>(
        raytracerui::runRaytracerUI(
            sceneIndex, exposure, tracing, superSampling, fullRender, fullRenderDenoise, exitAfterRender
        )
    );
    logging::Logger::instance().stop();
    return result;
}
