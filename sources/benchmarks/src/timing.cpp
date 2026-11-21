#include "timing.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <intrin.h>
#include <limits>

namespace benchmarks
{
    namespace
    {
        // keeps the timed passes from being optimized away
        volatile Float sink{};

        // Rays per timed chunk: a few milliseconds each, short enough that many chunks escape interrupts and other
        // processes, so the per-chunk minimum over the repetitions filters that noise out.
        constexpr ASizeT ChunkSize{4096};

        // Times every chunk of rays separately (TSC ticks per chunk).
        template <Query query>
        ARCH_NOINLINE void tracePass(
            const Mesh& mesh, const std::vector<RayCast>& rayCasts, std::vector<std::uint64_t>& chunkTicks
        )
        {
            auto intersection = FacetIntersection();
            auto distanceSum = Zero<Float>();
            for (auto chunk = ASizeT{0}; chunk < chunkTicks.size(); ++chunk)
            {
                const auto begin = chunk * ChunkSize;
                const auto end = std::min(begin + ChunkSize, rayCasts.size());
                const auto start = __rdtsc();
                for (auto i = begin; i < end; ++i)
                {
                    distanceSum += trace(query, mesh, rayCasts[i], intersection);
                }
                chunkTicks[chunk] = __rdtsc() - start;
            }
            sink = sink + distanceSum;
        }

        volatile std::uint64_t multiplierSource{0x9E3779B97F4A7C15ull};
        volatile std::uint64_t integerSink{};

        // Core clock relative to the TSC, from a dependent chain of multiply + xor: 3 + 1 cycles latency per step on
        // current Intel and AMD cores (the shift runs in parallel). The xor keeps the compiler from merging multiplies. The
        // TSC ticks at a fixed rate, but turbo and power management change the core clock, so raw TSC cycles drift between
        // runs. An interrupt only makes a piece of the chain slower, so the fastest of several short pieces is used.
        ARCH_NOINLINE double coreCyclesPerTscCycle()
        {
            constexpr std::uint64_t pieces{8};
            constexpr std::uint64_t pieceLength{1u << 19};
            const auto multiplier = multiplierSource | 1u;
            auto value = multiplier;

            auto fastestTicks = std::numeric_limits<std::uint64_t>::max();
            for (auto piece = std::uint64_t{0}; piece < pieces; ++piece)
            {
                const auto start = __rdtsc();
                for (auto i = std::uint64_t{0}; i < pieceLength; ++i)
                {
                    value = (value * multiplier) ^ (value >> 7);
                }
                fastestTicks = std::min(fastestTicks, __rdtsc() - start);
            }

            integerSink = value;
            return 4.0 * static_cast<double>(pieceLength) / static_cast<double>(fastestTicks);
        }

        double median(std::vector<double> values)
        {
            std::ranges::sort(values);
            return values[values.size() / 2];
        }

        template <Query query>
        Timing timeQuery(const Mesh& mesh, const std::vector<RayCast>& rayCasts, const ASizeT repetitions)
        {
            const auto rayCount = static_cast<double>(rayCasts.size());
            const auto chunkCount = (rayCasts.size() + ChunkSize - 1) / ChunkSize;

            std::vector<std::uint64_t> chunkTicks(chunkCount);
            std::vector<double> bestChunkCoreCycles(chunkCount, std::numeric_limits<double>::max());

            // warm-up
            tracePass<query>(mesh, rayCasts, chunkTicks);

            std::vector<double> coreCycles, tscCycles, nanoseconds, coreGigahertz;
            for (auto repetition = ASizeT{0}; repetition < repetitions; ++repetition)
            {
                const auto ratioBefore = coreCyclesPerTscCycle();
                const auto wallStart = std::chrono::steady_clock::now();
                tracePass<query>(mesh, rayCasts, chunkTicks);
                const auto wallStop = std::chrono::steady_clock::now();
                const auto ratio = 0.5 * (ratioBefore + coreCyclesPerTscCycle());

                auto tsc = 0.0;
                for (auto chunk = ASizeT{0}; chunk < chunkCount; ++chunk)
                {
                    const auto ticks = static_cast<double>(chunkTicks[chunk]);
                    tsc += ticks;
                    bestChunkCoreCycles[chunk] = std::min(bestChunkCoreCycles[chunk], ticks * ratio);
                }

                const auto wallNanoseconds = std::chrono::duration<double, std::nano>(wallStop - wallStart).count();
                tscCycles.push_back(tsc / rayCount);
                coreCycles.push_back(tsc * ratio / rayCount);
                nanoseconds.push_back(wallNanoseconds / rayCount);
                coreGigahertz.push_back(tsc * ratio / wallNanoseconds);
            }

            auto bestCoreCycles = 0.0;
            for (const auto cycles : bestChunkCoreCycles)
            {
                bestCoreCycles += cycles;
            }

            return Timing{
                bestCoreCycles / rayCount,
                median(coreCycles),
                std::ranges::max(coreCycles),
                median(tscCycles),
                median(nanoseconds),
                median(coreGigahertz)
            };
        }
    }

    Timing timeQuery(const Query query, const Mesh& mesh, const std::vector<RayCast>& rayCasts,
                     const ASizeT repetitions)
    {
        return query == Query::Nearest
                   ? timeQuery<Query::Nearest>(mesh, rayCasts, repetitions)
                   : timeQuery<Query::Any>(mesh, rayCasts, repetitions);
    }
}
