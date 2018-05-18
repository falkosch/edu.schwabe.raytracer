#include "raytracing/geometry/ObjectGeometry.h"
#include "../../stdafx.h"

namespace raytracer
{

    ObjectGeometry::ObjectGeometry()
        :
        modelMatrix(),
        transposeInverseModelMatrix(),
        transformInfo()
    { }

    ObjectGeometry::~ObjectGeometry() { }

    void ObjectGeometry::resetModelMatrix()
    {
        transformInfo = TransformInfo();
        updateModelMatrix();
    }

    void ObjectGeometry::rotate(const Float3 & rotationIn)
    {
        transformInfo.rotation += rotationIn;
        updateModelMatrix();
    }

    void ObjectGeometry::translate(const Float3 & translationIn)
    {
        transformInfo.translation += translationIn;
        updateModelMatrix();
    }

    void ObjectGeometry::scale(const Float3 & scaleIn)
    {
        transformInfo.scale *= scaleIn;
        updateModelMatrix();
    }

    void ObjectGeometry::updateModelMatrix()
    {
        // transforms are applied in reverse order, like in OGL!
        const Float44 tModelMatrix = vectorization::scale(
            vectorization::rotate(
            vectorization::rotate(
            vectorization::rotate(
            vectorization::translate(Identity<Float44>(), convert<Float4>(transformInfo.translation)),
            z(transformInfo.rotation), OneZ<Float4>()),
            y(transformInfo.rotation), OneY<Float4>()),
            x(transformInfo.rotation), OneX<Float4>()),
            convert<Float4>(transformInfo.scale));
        this->modelMatrix = tModelMatrix;
        this->transposeInverseModelMatrix = transpose(inverse(tModelMatrix));
        updateBounding();
    }

}
