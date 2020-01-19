#include "raytracing/scene/Resources.h"
#include "../../stdafx.h"

#include "raytracing/geometry/partitioning/treetraverser/NaiveKDTreeTraverser.h"
#include "raytracing/geometry/partitioning/treetraverser/VoxelizationKDTreeTraverser.h"
#include "raytracing/geometry/partitioning/treebalancer/RotatingAxisKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/MaxAxisKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/MedianKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/ArithmeticMeanKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/FixedIterationsSAHKDTreeBalancer.h"
#include "raytracing/geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.h"
#include "raytracing/common/HDRImage.h"

#include <algorithm>
#include <cassert>

namespace raytracer
{
    Resources::Resources()
        :
        meshes(),
        textures(),
        revertedNormalsCheckList() {
        Mesh * basicMesh = new Mesh();
        basicMesh->buildTriangleMesh();
        meshes["triangle"] = basicMesh;

        basicMesh = new Mesh();
        basicMesh->buildPlaneMesh();
        meshes["plane"] = basicMesh;

        basicMesh = new Mesh();
        basicMesh->buildCubeMesh();
        meshes["cube"] = basicMesh;

        //revertedNormalsCheckList.push_back("meshes/cone.off");
        std::sort(revertedNormalsCheckList.begin(), revertedNormalsCheckList.end());
    }

    Resources::~Resources() { }

    const bool Resources::hasRevertedNormals(const std::string & identifier) const {
        return std::binary_search(
            revertedNormalsCheckList.cbegin(),
            revertedNormalsCheckList.cend(),
            identifier);
    }

    Mesh * const Resources::getMesh(const std::string & identifier) {
        std::map<std::string, Mesh *>::const_iterator found = meshes.find(identifier);
        if (found == meshes.cend()) {
            const std::string file = "meshes/" + identifier + ".off";
            found = meshes.find(file);

            if (found == meshes.cend()) {
                const KDTreeTraverser<FacetIntersection> * const traverser = new NaiveKDTreeTraverser<FacetIntersection>();
                //const KDTreeTraverser<FacetIntersection>*const traverser = new VoxelizationKDTreeTraverser<FacetIntersection>();

                //const KDTreeBalancer*const balancer = new RotatingAxisKDTreeBalancer();
                //const KDTreeBalancer*const balancer = new MaxAxisKDTreeBalancer();
                //const KDTreeBalancer*const balancer = new MedianKDTreeBalancer();
                //const KDTreeBalancer*const balancer = new ArithmeticMeanKDTreeBalancer();
                const KDTreeBalancer * const balancer = new FixedIterationsSAHKDTreeBalancer(5);
                //const KDTreeBalancer*const balancer = new BruteForceSAHKDTreeBalancer();

                Mesh * const newMesh = Mesh::loadFromOffFile(file, hasRevertedNormals(file), traverser, balancer);
                meshes[file] = newMesh;
                return newMesh;
            }
        }
        return found->second;
    }

    HDRImage * const Resources::getPPM(const std::string & identifier) {
        std::map<std::string, HDRImage *>::const_iterator found = textures.find(identifier);
        if (found == textures.cend()) {
            const std::string file = "textures/" + identifier + ".ppm";
            found = textures.find(file);

            if (found == textures.cend()) {
                const Bitmap * const loadedBitmap = Bitmap::loadPPM(file);
                assert(loadedBitmap);

                HDRImage * const image = new HDRImage(*loadedBitmap);
                textures[file] = image;

                delete(loadedBitmap);
                return image;
            }
        }
        return found->second;
    }
}