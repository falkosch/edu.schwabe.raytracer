#include "intersection_benchmarks.h"

#include <intrin.h>
#include <logging.h>
#include <windows.h>

#include <array>
#include <string>

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

    void pinToSingleCore()
    {
        const auto thisProcess = GetCurrentProcess();
        DWORD_PTR processAffinityMask{}, systemAffinityMask{};
        GetProcessAffinityMask(thisProcess, &processAffinityMask, &systemAffinityMask);

        DWORD_PTR singleCoreMask = 1;
        while (singleCoreMask && !(singleCoreMask & processAffinityMask))
        {
            singleCoreMask <<= 1;
        }

        SetThreadAffinityMask(GetCurrentThread(), singleCoreMask);
    }
}

int main()
{
    logging::Logger::instance().start();
    pinToSingleCore();

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

    benchmarks::runIntersectionBenchmarks(rayCasts, iterations);

    logging::Logger::instance().stop();
    return 0;
}
