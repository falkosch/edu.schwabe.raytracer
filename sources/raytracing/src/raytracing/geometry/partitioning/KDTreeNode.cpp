#include "raytracing/geometry/partitioning/KDTreeNodeChilds.h"
#include "../../../stdafx.h"

namespace raytracer
{
    KDTreeNode::KDTreeNode()
        :
        geometryNodes(),
        childs() { }

    KDTreeNode::~KDTreeNode() {
        clear();
    }

    void KDTreeNode::clear() {
        if (childs) {
            delete childs;
            childs = nullptr;
        }
        if (geometryNodes) {
            delete geometryNodes;
            geometryNodes = nullptr;
        }
    }

    const bool KDTreeNode::isNonEmptyLeaf() const {
        return geometryNodes && !geometryNodes->empty();
    }

    void KDTreeNode::grow(
        const AxisAlignedBoundingBox & leftBounding,
        PGeometryNodeList & leftGeometryNodes,
        const AxisAlignedBoundingBox & rightBounding,
        PGeometryNodeList & rightGeometryNodes
    ) {
        clear();
        childs = new KDTreeNodeChilds();
        childs->boundingA = leftBounding;
        childs->boundingB = rightBounding;
        childs->childA.geometryNodes = &leftGeometryNodes;
        childs->childB.geometryNodes = &rightGeometryNodes;
    }
}