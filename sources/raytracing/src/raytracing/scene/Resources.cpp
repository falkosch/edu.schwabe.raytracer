#include "raytracing/scene/Resources.h"
#include "../../stdafx.h"

#include "raytracing/common/HDRImage.h"
#include "raytracing/geometry/partitioning/treebalancer/ArithmeticMeanKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/FixedIterationsSAHKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/MaxAxisKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/MedianKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/RotatingAxisKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treetraverser/NaiveKDTreeTraverser.h"
#include "raytracing/geometry/partitioning/treetraverser/VoxelizationKDTreeTraverser.h"

#include <algorithm>
#include <cassert>

namespace raytracer {
  Resources::Resources() : meshes(), textures(), revertedNormalsCheckList() {
    meshes["triangle"] = Mesh::buildTriangleMesh();
    meshes["plane"] = Mesh::buildPlaneMesh();
    meshes["cube"] = Mesh::buildCubeMesh();

    // revertedNormalsCheckList.push_back("meshes/cone.off");
    std::sort(revertedNormalsCheckList.begin(), revertedNormalsCheckList.end());
  }

  Resources::~Resources() = default;

  bool Resources::hasRevertedNormals(const std::string& identifier) const
  {
      return std::binary_search(revertedNormalsCheckList.cbegin(), revertedNormalsCheckList.cend(), identifier);
  }

  Mesh *Resources::getMesh(const std::string &identifier) {
    auto found = meshes.find(identifier);
    if (found == meshes.end()) {
      const std::string file = "meshes/" + identifier + ".off";
      found = meshes.find(file);

      if (found == meshes.end()) {
        auto traverser = std::make_unique<NaiveKDTreeTraverser<FacetIntersection>>();
        // auto traverser = std::make_unique<VoxelizationKDTreeTraverser<FacetIntersection>>();

        // auto balancer = std::make_unique<RotatingAxisKDTreeBalancer>();
        // auto balancer = std::make_unique<MaxAxisKDTreeBalancer>();
        // auto balancer = std::make_unique<MedianKDTreeBalancer>();
        // auto balancer = std::make_unique<ArithmeticMeanKDTreeBalancer>();
        // auto balancer = std::make_unique<FixedIterationsSAHKDTreeBalancer>(5);
        auto balancer = std::make_unique<BruteForceSAHKDTreeBalancer>();

        auto newMesh = Mesh::loadFromOffFile(file, hasRevertedNormals(file), std::move(traverser), std::move(balancer));
        auto *ptr = newMesh.get();
        meshes[file] = std::move(newMesh);
        return ptr;
      }
    }
    return found->second.get();
  }

  HDRImage *Resources::getPPM(const std::string &identifier) {
    auto found = textures.find(identifier);
    if (found == textures.end()) {
      const std::string file = "textures/" + identifier + ".ppm";
      found = textures.find(file);

      if (found == textures.end()) {
        const auto loadedBitmap = Bitmap::loadPPM(file);
        assert(loadedBitmap);

        auto image = std::make_unique<HDRImage>(*loadedBitmap);
        auto *ptr = image.get();
        textures[file] = std::move(image);
        return ptr;
      }
    }
    return found->second.get();
  }
}
