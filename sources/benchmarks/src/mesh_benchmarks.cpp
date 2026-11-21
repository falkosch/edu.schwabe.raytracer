#include "mesh_benchmarks.h"

#include "leaf_statistics.h"
#include "query.h"
#include "query_result.h"
#include "ray_set.h"
#include "timing.h"

#include <logging.h>

#include <raytracing/geometry/forms/meshes/Mesh.h>
#include <raytracing/geometry/partitioning/KDTreeRoot.h>
#include <raytracing/geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.h>
#include <raytracing/geometry/partitioning/treetraverser/WideKDTreeTraverser.h>

#include <array>
#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>
#include <system_error>
#include <vector>

static const auto Log = logging::scope("Benchmark");

using namespace vectorization;
using namespace primitives;
using namespace raytracer;

namespace benchmarks
{
    namespace
    {
        std::string fixed(const double value, const int precision)
        {
            std::array<char, 64> buffer{};
            std::snprintf(buffer.data(), buffer.size(), "%.*f", precision, value);
            return buffer.data();
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

            Log.info([=, depth = graph->depth(), nodes = graph->nodeCount(), wideNodes = graph->wideNodes.size()]
            {
                return meshFile + ": " + std::to_string(facets) + " facets, KD-tree depth " + std::to_string(depth) +
                    ", " +
                    std::to_string(nodes) + " nodes, " + std::to_string(statistics.leaves) + " leaves (" +
                    std::to_string(statistics.emptyLeaves) + " empty), " + std::to_string(wideNodes) + " wide nodes";
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
                meshFile, false, std::make_unique<WideKDTreeTraverser<FacetIntersection>>(),
                std::make_unique<BruteForceSAHKDTreeBalancer>()
            );
            if (!mesh || mesh->getFaceCount() == 0 || !mesh->getGraph())
            {
                Log.error([meshFile] { return "Failed to load " + meshFile + " with a KD-tree"; });
                return;
            }

            logTreeStatistics(meshFile, *mesh);

            const auto traverser = WideKDTreeTraverser<FacetIntersection>();
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

