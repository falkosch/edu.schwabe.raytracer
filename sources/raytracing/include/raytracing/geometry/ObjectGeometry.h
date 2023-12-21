#pragma once

#include "Intersectable.h"
#include "TransformInfo.h"
#include "partitioning/GeometryNode.h"

namespace raytracer {
  using namespace vectorization;

  class ObjectGeometry : public GeometryNode {
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
