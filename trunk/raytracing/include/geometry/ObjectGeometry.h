#pragma once

#include "geometry/Intersectable.h"
#include "geometry/partitioning/GeometryNode.h"
#include "geometry/TransformInfo.h"

namespace raytracer
{

    class ObjectGeometry : public GeometryNode
    {
    public:

        ObjectGeometry();

        virtual ~ObjectGeometry();

        void resetModelMatrix();

        void rotate(const Float3 &rotation);

        void translate(const Float3 &translation);

        void scale(const Float3 &scale);

    protected:

        Float44 modelMatrix, transposeInverseModelMatrix;

        TransformInfo transformInfo;

        void updateModelMatrix();

        virtual void updateBounding() = 0;
    };

}
