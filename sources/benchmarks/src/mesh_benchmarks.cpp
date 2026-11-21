#include "mesh_benchmarks.h"

#include <logging.h>

#include <raytracing/common/StatisticsCookie.h>
#include <raytracing/geometry/forms/meshes/Mesh.h>
#include <raytracing/geometry/forms/meshes/MeshGeometryNode.h>
#include <raytracing/geometry/partitioning/KDTreeRoot.h>
#include <raytracing/geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.h>
#include <raytracing/geometry/partitioning/treetraverser/NaiveKDTreeTraverser.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <intrin.h>
#include <limits>
#include <memory>
#include <numbers>
#include <random>
#include <string>
#include <vector>

static const auto Log = logging::scope("Benchmark");

using namespace vectorization;
using namespace primitives;
using namespace raytracer;

namespace benchmarks
{
    namespace
    {
        constexpr ASizeT CoherentResolution{512};
        constexpr ASizeT IncoherentRayCount{CoherentResolution * CoherentResolution};
        constexpr std::mt19937::result_type RandomSeed{0x5eed1234u};

        // keeps the timed passes from being optimized away
        volatile Float sink{};

        enum class Query { Nearest, Any };

        const char* nameOf(const Query query)
        {
            return query == Query::Nearest ? "nearest" : "any";
        }

        std::string fixed(const double value, const int precision)
        {
            std::array<char, 64> buffer{};
            std::snprintf(buffer.data(), buffer.size(), "%.*f", precision, value);
            return buffer.data();
        }

        struct RaySet
        {
            std::string name;
            std::vector<RayCast> rayCasts;
        };

        // Pinhole camera on the -z side of the mesh, aimed at a grid on the front face of the bounding box, so every ray
        // enters the root bounding box. Backface culling as for primary rays (CULLING_ORIENTATION = -1).
        RaySet createCoherentRays(const AxisAlignedBoundingBox& bounding)
        {
            const auto minimum = bounding.minimum;
            const auto size = extents(bounding);
            const auto diagonal = length3(size);
            const auto boxCenter = center(bounding);
            const auto eye = Float4(x(boxCenter), y(boxCenter), z(minimum) - 2.0f * diagonal, 1.0f);

            auto rays = RaySet{.name = "coherent", .rayCasts = {}};
            rays.rayCasts.reserve(CoherentResolution * CoherentResolution);

            const auto step = 1.0f / static_cast<Float>(CoherentResolution);
            for (auto row = ASizeT{0}; row < CoherentResolution; ++row)
            {
                for (auto column = ASizeT{0}; column < CoherentResolution; ++column)
                {
                    const auto u = (static_cast<Float>(column) + 0.5f) * step;
                    const auto v = (static_cast<Float>(row) + 0.5f) * step;
                    const auto target =
                        Float4(x(minimum) + u * x(size), y(minimum) + v * y(size), z(minimum), 1.0f);
                    const auto toTarget = target - eye;
                    rays.rayCasts.emplace_back(
                        Ray(eye, normalize3(toTarget)), cullingOrientationToMask(-1), Zero<Size2>(),
                        length3(toTarget) + diagonal
                    );
                }
            }
            return rays;
        }

        // Random origins inside the bounding box and uniformly distributed directions, without culling: a stand-in for
        // secondary and shadow rays. Fixed seed, so every run traces the same rays.
        RaySet createIncoherentRays(const AxisAlignedBoundingBox& bounding)
        {
            const auto minimum = bounding.minimum;
            const auto size = extents(bounding);
            const auto diagonal = length3(size);

            auto generator = std::mt19937{RandomSeed};
            auto uniform = [&generator] { return static_cast<Float>(generator() >> 8) * (1.0f / 16777216.0f); };

            auto rays = RaySet{"incoherent", {}};
            rays.rayCasts.reserve(IncoherentRayCount);

            for (auto i = ASizeT{0}; i < IncoherentRayCount; ++i)
            {
                const auto origin = Float4(
                    x(minimum) + uniform() * x(size), y(minimum) + uniform() * y(size),
                    z(minimum) + uniform() * z(size), 1.0f
                );
                const auto cosTheta = 2.0f * uniform() - 1.0f;
                const auto sinTheta = std::sqrt(std::max(0.0f, 1.0f - cosTheta * cosTheta));
                const auto phi = 2.0f * std::numbers::pi_v<Float> * uniform();
                const auto direction = Float4(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta, 0.0f);
                rays.rayCasts.emplace_back(Ray(origin, direction), cullingOrientationToMask(0), Zero<Size2>(),
                                           diagonal);
            }
            return rays;
        }

        struct LeafStatistics
        {
            ASizeT leaves{}, emptyLeaves{}, references{};
            ASizeT minSize{std::numeric_limits<ASizeT>::max()}, maxSize{};
            ASizeT paddedReferences4{}, paddedReferences8{};
            // leaf sizes 1-4, 5-8, 9-16, 17-32, 33-64, >64
            std::array<ASizeT, 6> histogram{};
        };

        ASizeT roundUp(const ASizeT value, const ASizeT multiple)
        {
            return (value + multiple - 1) / multiple * multiple;
        }

        // walks the links from the root, so only reachable leaves count
        void collectLeafStatistics(const KDTreeRoot& graph, const KDTreeLink link, LeafStatistics& statistics)
        {
            if (!KDTreeRoot::isLeaf(link))
            {
                collectLeafStatistics(graph, graph.childLinks[link].childA, statistics);
                collectLeafStatistics(graph, graph.childLinks[link].childB, statistics);
                return;
            }

            ++statistics.leaves;
            const auto size = graph.geometryOf(link).size();
            if (size == 0)
            {
                ++statistics.emptyLeaves;
                return;
            }

            statistics.references += size;
            statistics.minSize = std::min(statistics.minSize, size);
            statistics.maxSize = std::max(statistics.maxSize, size);
            statistics.paddedReferences4 += roundUp(size, 4);
            statistics.paddedReferences8 += roundUp(size, 8);

            auto bucket = ASizeT{0};
            for (auto upper = ASizeT{4}; bucket < statistics.histogram.size() - 1 && size > upper; upper *= 2)
            {
                ++bucket;
            }
            ++statistics.histogram[bucket];
        }

        void logTreeStatistics(const std::string& meshFile, const Mesh& mesh)
        {
            const auto* graph = mesh.getGraph();
            if (!graph)
            {
                Log.warn([meshFile] { return meshFile + ": mesh has no KD-tree"; });
                return;
            }

            auto statistics = LeafStatistics();
            collectLeafStatistics(*graph, graph->rootLink, statistics);

            const auto facets = mesh.getFaceCount();
            const auto filledLeaves = statistics.leaves - statistics.emptyLeaves;
            const auto references = static_cast<double>(statistics.references);

            Log.info([=, depth = graph->depth(), nodes = graph->nodeCount()]
            {
                return meshFile + ": " + std::to_string(facets) + " facets, KD-tree depth " + std::to_string(depth) +
                    ", " +
                    std::to_string(nodes) + " nodes, " + std::to_string(statistics.leaves) + " leaves (" +
                    std::to_string(statistics.emptyLeaves) + " empty)";
            });
            Log.info([=]
            {
                return "  leaf facet references " + std::to_string(statistics.references) + " (duplication " +
                    fixed(references / static_cast<double>(facets), 3) + "x), leaf size min/mean/max " +
                    std::to_string(statistics.minSize) + "/" + fixed(references / static_cast<double>(filledLeaves), 2)
                    + "/" +
                    std::to_string(statistics.maxSize);
            });
            Log.info([=]
            {
                const auto& h = statistics.histogram;
                return "  leaf sizes 1-4: " + std::to_string(h[0]) + ", 5-8: " + std::to_string(h[1]) + ", 9-16: " +
                    std::to_string(h[2]) + ", 17-32: " + std::to_string(h[3]) + ", 33-64: " + std::to_string(h[4]) +
                    ", >64: " + std::to_string(h[5]);
            });
            Log.info([=]
            {
                return "  padded to blocks of 4: " + std::to_string(statistics.paddedReferences4) + " lanes (" +
                    fixed(static_cast<double>(statistics.paddedReferences4) / references, 3) + "x), blocks of 8: " +
                    std::to_string(statistics.paddedReferences8) + " lanes (" +
                    fixed(static_cast<double>(statistics.paddedReferences8) / references, 3) + "x)";
            });
        }

        // Counts what the leaf loops get to see, then forwards to the mesh's own leaf intersection. Only used in the
        // untimed counting pass, so the timed path stays the production one.
        class CountingLeafTraverser final : public GeometryNodesTraverser<FacetIntersection>
        {
        public:
            explicit CountingLeafTraverser(const Mesh& mesh) : mesh(mesh)
            {
            }

            Float findNearestIntersection(
                const GeometryNodeSpan geometryNodes, const RayCast& rayCast,
                const FacetIntersection* originIntersection,
                FacetIntersection& intersectionOut
            ) const override
            {
                ++leafVisits;
                facetsInVisitedLeaves += geometryNodes.size();
                return mesh.findNearestIntersection(geometryNodes, rayCast, originIntersection, intersectionOut);
            }

            Float findAnyIntersection(
                const GeometryNodeSpan geometryNodes, const RayCast& rayCast,
                const FacetIntersection* originIntersection,
                FacetIntersection& intersectionOut
            ) const override
            {
                ++leafVisits;
                facetsInVisitedLeaves += geometryNodes.size();
                return mesh.findAnyIntersection(geometryNodes, rayCast, originIntersection, intersectionOut);
            }

            mutable ASizeT leafVisits{};
            mutable ASizeT facetsInVisitedLeaves{};

        private:
            const Mesh& mesh;
        };

        struct QueryResult
        {
            ASizeT hits{};
            // nearest only: which facet was hit and at which distance; any-hit results depend on the traversal order
            ASizeT facetIndexSum{};
            ASizeT hitsWithoutFacet{};
            double distanceSum{};
            ASizeT kdTreeNodesVisited{};
            ASizeT leafVisits{};
            ASizeT facetsInVisitedLeaves{};
        };

        Float trace(const Query query, const Mesh& mesh, const RayCast& rayCast, FacetIntersection& intersection)
        {
            return query == Query::Nearest
                       ? mesh.findNearestIntersection(rayCast, nullptr, intersection)
                       : mesh.findAnyIntersection(rayCast, nullptr, intersection);
        }

        // Untimed: results through the production entry point (plus KD-tree node visits), then the leaf counters through
        // the counting wrapper.
        QueryResult countQuery(
            const Query query, const Mesh& mesh, const KDTreeTraverser<FacetIntersection>& traverser,
            const std::vector<RayCast>& rayCasts
        )
        {
            auto result = QueryResult();
            auto intersection = FacetIntersection();

            auto statistics = StatisticsCookie();
            StatisticsCookie::current = &statistics;
            for (const auto& rayCast : rayCasts)
            {
                const auto distance = trace(query, mesh, rayCast, intersection);
                if (distance < rayCast.maxDistance)
                {
                    ++result.hits;
                    if (query == Query::Nearest)
                    {
                        if (const auto* facetNode = dynamic_cast<const MeshGeometryNode*>(intersection.node))
                        {
                            result.facetIndexSum += facetNode->index;
                        }
                        else
                        {
                            ++result.hitsWithoutFacet;
                        }
                        result.distanceSum += distance;
                    }
                }
            }
            StatisticsCookie::current = nullptr;
            result.kdTreeNodesVisited = statistics.kdTreeNodesVisited;

            const auto counting = CountingLeafTraverser(mesh);
            const auto& graph = *mesh.getGraph();
            for (const auto& rayCast : rayCasts)
            {
                if (query == Query::Nearest)
                {
                    traverser.findNearestIntersection(counting, graph, rayCast, nullptr, intersection);
                }
                else
                {
                    traverser.findAnyIntersection(counting, graph, rayCast, nullptr, intersection);
                }
            }
            result.leafVisits = counting.leafVisits;
            result.facetsInVisitedLeaves = counting.facetsInVisitedLeaves;

            return result;
        }

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

        struct Timing
        {
            // Per ray. Core cycles are TSC cycles scaled by the core clock measured around each repetition.
            // best: sum over the chunks of each chunk's fastest repetition; median/max: whole passes
            double bestCoreCycles{}, medianCoreCycles{}, maxCoreCycles{};
            double medianTscCycles{};
            double medianNanoseconds{};
            // estimated core clock during the timed passes
            double medianCoreGigahertz{};
        };

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

        Timing timeQuery(const Query query, const Mesh& mesh, const std::vector<RayCast>& rayCasts,
                         const ASizeT repetitions)
        {
            return query == Query::Nearest
                       ? timeQuery<Query::Nearest>(mesh, rayCasts, repetitions)
                       : timeQuery<Query::Any>(mesh, rayCasts, repetitions);
        }

        void logQuery(const Query query, const RaySet& rays, const Timing& timing, const QueryResult& result)
        {
            const auto rayCount = static_cast<double>(rays.rayCasts.size());
            const auto label = std::string(nameOf(query)) + " " + rays.name;

            Log.info([=]
            {
                return label + ": " + fixed(timing.bestCoreCycles, 1) + " / " + fixed(timing.medianCoreCycles, 1) +
                    " / " +
                    fixed(timing.maxCoreCycles, 1) + " core cycles/ray (best/median/max); median " +
                    fixed(timing.medianTscCycles, 1) + " TSC cycles/ray, " + fixed(timing.medianNanoseconds, 1) +
                    " ns/ray at " +
                    fixed(timing.medianCoreGigahertz, 2) + " GHz";
            });
            Log.info([=]
            {
                auto line = "  per ray: " + fixed(static_cast<double>(result.kdTreeNodesVisited) / rayCount, 2) +
                    " KD-tree nodes, " + fixed(static_cast<double>(result.leafVisits) / rayCount, 2) + " leaves, " +
                    fixed(static_cast<double>(result.facetsInVisitedLeaves) / rayCount, 2) +
                    " facets in visited leaves; " +
                    std::to_string(result.hits) + " hits";
                if (query == Query::Nearest)
                {
                    line += ", facet index sum " + std::to_string(result.facetIndexSum) + ", distance sum " +
                        fixed(result.distanceSum, 3);
                    if (result.hitsWithoutFacet > 0)
                    {
                        line += ", " + std::to_string(result.hitsWithoutFacet) + " hits without facet";
                    }
                }
                return line;
            });
        }

        // Same traverser and balancer as Resources::getMesh. VoxelizationKDTreeTraverser is left out on purpose: it is a
        // debug view that intersects the leaf boxes and never runs the leaf facet tests.
        void runMeshBenchmark(const std::string& meshFile, const ASizeT repetitions)
        {
            const auto mesh = Mesh::loadFromOffFile(
                meshFile, false, std::make_unique<NaiveKDTreeTraverser<FacetIntersection>>(),
                std::make_unique<BruteForceSAHKDTreeBalancer>()
            );
            if (!mesh || mesh->getFaceCount() == 0 || !mesh->getGraph())
            {
                Log.error([meshFile] { return "Failed to load " + meshFile + " with a KD-tree"; });
                return;
            }

            logTreeStatistics(meshFile, *mesh);

            const auto traverser = NaiveKDTreeTraverser<FacetIntersection>();
            for (const auto& rays : {
                     createCoherentRays(mesh->getBounding()), createIncoherentRays(mesh->getBounding())
                 })
            {
                auto nearestHits = ASizeT{0};
                for (const auto query : {Query::Nearest, Query::Any})
                {
                    const auto result = countQuery(query, *mesh, traverser, rays.rayCasts);
                    const auto timing = timeQuery(query, *mesh, rays.rayCasts, repetitions);
                    logQuery(query, rays, timing, result);

                    if (query == Query::Nearest)
                    {
                        nearestHits = result.hits;
                    }
                    else if (result.hits != nearestHits)
                    {
                        Log.warn([name = rays.name, nearestHits, anyHits = result.hits]
                        {
                            return name + ": nearest found " + std::to_string(nearestHits) + " hits, any found " +
                                std::to_string(anyHits);
                        });
                    }
                }
            }
        }
    }

    void runMeshBenchmarks(const std::vector<std::string>& meshFiles, const ASizeT repetitions)
    {
        Log.info([repetitions]
        {
            return "Mesh benchmarks: " + std::to_string(CoherentResolution) + "x" + std::to_string(CoherentResolution) +
                " coherent and " + std::to_string(IncoherentRayCount) + " incoherent rays, " + std::to_string(
                    repetitions) +
                " timed repetitions each";
        });

        for (const auto& meshFile : meshFiles)
        {
            auto error = std::error_code();
            if (!std::filesystem::exists(meshFile, error))
            {
                Log.warn([meshFile]
                {
                    return "Skipping " + meshFile + ": file not found (run from data/ or pass --data=<dir>)";
                });
                continue;
            }
            runMeshBenchmark(meshFile, repetitions);
        }
    }
}
