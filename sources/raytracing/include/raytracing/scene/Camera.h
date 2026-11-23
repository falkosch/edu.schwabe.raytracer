#pragma once

#include "../geometry/TransformInfo.h"

namespace raytracer
{
  using namespace vectorization;

  // A perspective camera looking along -z of its view space, +y up. Its conventions:
  //  - The view frustum's "top left" corners and "right" and "bottom" directions name the image rows the way they are
  //    stored, not as they show: images keep the bottom row first (HDRImage, Bitmap), so image row 0 at the "top left"
  //    lies at the bottom of the view, and the "bottom" direction points up (+y in view space). Row 0 is the bottom row
  //    of the saved PNG and of the displayed image.
  //  - translate moves the scene in view space, the camera by the opposite: translate(v) puts a reset camera at -v.
  //  - rotate turns about the camera's position, by degrees about the axes of its view space: x about the y axis (yaw,
  //    the mouse's horizontal drag), y about the x axis (pitch) and z about the z axis (roll).
  class Camera
  {
    MFloat4x4 projectionMatrix, viewMatrix;

    Float4 worldPosition;

    // View frustum near-plane top left anchor point and near-plane extents (see the conventions above)
    Float4 vfNearTopLeft, vfNearRightDirection, vfNearBottomDirection;

    // View frustum far-plane top left anchor point and near-plane extents (see the conventions above)
    Float4 vfFarTopLeft, vfFarRightDirection, vfFarBottomDirection;

    void updateView();

  public:
    Camera();

    virtual ~Camera();

    void setProjection(const Float& fov, const Float2& screenSize, const Float2& projectionPlanes);

    void resetView();

    const MFloat4x4& getViewMatrix() const;

    void setViewMatrix(const MFloat4x4& matrix);

    // Turns the camera about its position: x is the yaw, y the pitch and z the roll, in degrees
    void rotate(const Float3& yawPitchRoll);

    // Moves the scene by viewTranslation in view space, so the camera moves by -viewTranslation
    void translate(const Float3& viewTranslation);

    void scale(const Float3& scale);

    const Float4 getWorldPosition() const;

    const Float4 getVFNearTopLeft() const;

    const Float4 getVFNearRightDirection() const;

    const Float4 getVFNearBottomDirection() const;

    const Float4 getVFFarTopLeft() const;

    const Float4 getVFFarRightDirection() const;

    const Float4 getVFFarBottomDirection() const;
  };
}
