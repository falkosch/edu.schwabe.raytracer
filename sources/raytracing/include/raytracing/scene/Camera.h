#pragma once

#include "../geometry/TransformInfo.h"

namespace raytracer
{
    using namespace vectorization;

    class Camera
    {
        Float44 projectionMatrix, viewMatrix;

        Float4 worldPosition;

        // View frustum near-plane top left anchor point and near-plane extents
        Float4 vfNearTopLeft, vfNearRightDirection, vfNearBottomDirection;

        // View frustum far-plane top left anchor point and near-plane extents
        Float4 vfFarTopLeft, vfFarRightDirection, vfFarBottomDirection;

        void updateView();

    public:

        Camera();

        virtual ~Camera();

        void setProjection(const Float & fov, const Float2 & screenSize, const Float2 & projectionPlanes);

        void resetView();

        void rotate(const Float3 & rotation);

        void translate(const Float3 & translation);

        void scale(const Float3 & scale);

        const Float4 getWorldPosition() const;

        const Float4 getVFNearTopLeft() const;

        const Float4 getVFNearRightDirection() const;

        const Float4 getVFNearBottomDirection() const;

        const Float4 getVFFarTopLeft() const;

        const Float4 getVFFarRightDirection() const;

        const Float4 getVFFarBottomDirection() const;
    };
}
