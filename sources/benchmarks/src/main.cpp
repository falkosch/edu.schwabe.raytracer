#include "intersection_benchmarks.h"
#include "mesh_benchmarks.h"

#include <intrin.h>
#include <logging.h>
#include <windows.h>

#include <array>
#include <charconv>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

static const auto Log = logging::scope("Benchmark");

using namespace vectorization;
using namespace primitives;

namespace
{
    UInt_64 measureCPUClockFrequency()
    {
        LARGE_INTEGER frequency, startTime, stopTime;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&startTime);
        stopTime.QuadPart = startTime.QuadPart + (frequency.QuadPart << 2);

        const auto start = __rdtsc();
        while (startTime.QuadPart < stopTime.QuadPart)
        {
            QueryPerformanceCounter(&startTime);
        }
        const auto stop = __rdtsc();

        return (stop - start) >> 2;
    }

    // Pins the thread to the given logical processor, or to the first one the process may use. Returns the index.
    int pinToSingleCore(const int requestedCore)
    {
        const auto thisProcess = GetCurrentProcess();
        DWORD_PTR processAffinityMask{}, systemAffinityMask{};
        GetProcessAffinityMask(thisProcess, &processAffinityMask, &systemAffinityMask);

        auto core = 0;
        DWORD_PTR singleCoreMask = 1;
        if (requestedCore >= 0)
        {
            core = requestedCore;
            singleCoreMask = DWORD_PTR{1} << requestedCore;
        }
        else
        {
            while (singleCoreMask && !(singleCoreMask & processAffinityMask))
            {
                singleCoreMask <<= 1;
                ++core;
            }
        }

        SetThreadAffinityMask(GetCurrentThread(), singleCoreMask);
        return core;
    }

    struct Options
    {
        bool runIntersections = true;
        bool runMeshes = true;
        ASizeT meshRepetitions = 15;
        int core = -1;
        std::vector<std::string> meshFiles;
    };

    constexpr auto Usage = "usage: benchmarks [--data=<dir>] [--only=intersections|meshes] [--repetitions=<n>] "
        "[--core=<n>] [--mesh=<file>]...";

    template <typename T>
    bool parseNumber(const std::string_view value, T& out)
    {
        const auto [end, error] = std::from_chars(value.data(), value.data() + value.size(), out);
        return error == std::errc() && end == value.data() + value.size();
    }

    // Returns false and logs the reason on invalid arguments. --data changes the working directory, because the mesh
    // files are resolved relative to it (like in the UI).
    bool parseOptions(const int argc, char* argv[], Options& options)
    {
        constexpr std::string_view dataPrefix = "--data=";
        constexpr std::string_view repetitionsPrefix = "--repetitions=";
        constexpr std::string_view corePrefix = "--core=";
        constexpr std::string_view meshPrefix = "--mesh=";

        for (auto i = 1; i < argc; ++i)
        {
            const auto argument = std::string_view(argv[i]);
            if (argument.starts_with(dataPrefix))
            {
                const auto directory = std::filesystem::path(argument.substr(dataPrefix.size()));
                auto error = std::error_code();
                std::filesystem::current_path(directory, error);
                if (error)
                {
                    Log.error([d = directory.string(), m = error.message()]
                    {
                        return "Cannot change to data directory " + d + ": " + m;
                    });
                    return false;
                }
            }
            else if (argument == "--only=intersections")
            {
                options.runMeshes = false;
            }
            else if (argument == "--only=meshes")
            {
                options.runIntersections = false;
            }
            else if (argument.starts_with(repetitionsPrefix))
            {
                if (!parseNumber(argument.substr(repetitionsPrefix.size()), options.meshRepetitions) ||
                    options.meshRepetitions == 0)
                {
                    Log.error([a = std::string(argument)] { return "Invalid repetition count: " + a; });
                    return false;
                }
            }
            else if (argument.starts_with(corePrefix))
            {
                DWORD_PTR processAffinityMask{}, systemAffinityMask{};
                GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemAffinityMask);
                if (!parseNumber(argument.substr(corePrefix.size()), options.core) || options.core < 0 ||
                    options.core >= static_cast<int>(sizeof(DWORD_PTR) * 8) ||
                    !(processAffinityMask & (DWORD_PTR{1} << options.core)))
                {
                    Log.error([a = std::string(argument)] { return "Invalid or unavailable core: " + a; });
                    return false;
                }
            }
            else if (argument.starts_with(meshPrefix))
            {
                options.meshFiles.emplace_back(argument.substr(meshPrefix.size()));
            }
            else
            {
                Log.error([a = std::string(argument)] { return "Unknown argument " + a + "\n" + Usage; });
                return false;
            }
        }

        if (options.meshFiles.empty())
        {
            options.meshFiles = {"meshes/teapot.off", "meshes/dragon2.off"};
        }
        return true;
    }
}

int main(const int argc, char* argv[])
{
    logging::Logger::instance().start();

    auto options = Options();
    if (!parseOptions(argc, argv, options))
    {
        logging::Logger::instance().stop();
        return 1;
    }

    // less preemption and fewer migrations of the timed thread
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
    const auto core = pinToSingleCore(options.core);
    Log.info([core] { return "Pinned to logical processor " + std::to_string(core); });

    const auto cpuFrequency = measureCPUClockFrequency();
    Log.info([cpuFrequency] { return "CPU clock frequency: " + std::to_string(cpuFrequency) + " Hz"; });

    constexpr ASizeT iterations{100000000};

    const std::array rayCasts = {
        RayCast(
            Ray(Float4(0.f, 0.f, -2.f, 1.f), Float4(0.f, 0.f, 1.f, 0.f)), cullingOrientationToMask(-1), Zero<Size2>(),
            5.f
        ),
        RayCast(
            Ray(Float4(0.f, 0.f, 2.f, 1.f), Float4(0.f, 0.f, -1.f, 0.f)), cullingOrientationToMask(-1), Zero<Size2>(),
            5.f
        ),
        RayCast(
            Ray(Float4(3.f, 0.f, 0.f, 1.f), Float4(-1.f, 0.f, 0.f, 0.f)), cullingOrientationToMask(-1), Zero<Size2>(),
            5.f
        ),
        RayCast(
            Ray(Float4(0.f, 2.f, 0.f, 1.f), Float4(0.f, -1.f, 0.f, 0.f)), cullingOrientationToMask(-1), Zero<Size2>(),
            5.f
        )
    };

    if (options.runIntersections)
    {
        benchmarks::runIntersectionBenchmarks(rayCasts, iterations);
    }

    if (options.runMeshes)
    {
        benchmarks::runMeshBenchmarks(options.meshFiles, options.meshRepetitions);
    }

    logging::Logger::instance().stop();
    return 0;
}
