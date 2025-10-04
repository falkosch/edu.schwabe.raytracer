#include "raytracing/scene/Camera.h"
#include "../../stdafx.h"

namespace raytracer {
  Camera::Camera()
      : projectionMatrix(Identity<Float44>()), viewMatrix(Zero<Float44>()), worldPosition(), vfNearTopLeft(),
        vfNearRightDirection(), vfNearBottomDirection(), vfFarTopLeft(), vfFarRightDirection(), vfFarBottomDirection() {
    resetView();
  }

  Camera::~Camera() = default;

  void Camera::setProjection(const Float &fov, const Float2 &screenSize, const Float2 &projectionPlanes) {
    projectionMatrix = perspectiveFov(fov, x(screenSize), y(screenSize), x(projectionPlanes), y(projectionPlanes));
    updateView();
  }

  void Camera::resetView() {
    viewMatrix = Identity<Float44>();
    updateView();
  }

  void Camera::translate(const Float3 &translationIn) {
    const Float44 transViewMatrix = transpose(this->viewMatrix);
    this->viewMatrix =
        transpose(replaceRow<VectorIndices::W>(transViewMatrix, transViewMatrix.row3 + convert<Float4>(translationIn)));
    updateView();
  }

  void Camera::rotate(const Float3 &rotationIn) {
    const Float44 rotateMatrix = vectorization::rotate(
        vectorization::rotate(
            vectorization::rotate(Identity<Float44>(), z(rotationIn), OneZ<Float4>()), y(rotationIn),
            OneX<Float4>()
        ), // Switch of X and Y rotation vectors is okay
        x(rotationIn), OneY<Float4>()
    );
    this->viewMatrix = rotateMatrix * this->viewMatrix;
    updateView();
  }

  void Camera::scale(const Float3 &scaleIn) {
    const Float44 scaleMatrix = vectorization::scale(Identity<Float44>(), convert<Float4>(scaleIn));
    this->viewMatrix = this->viewMatrix * scaleMatrix;
    updateView();
  }

  const Float4 Camera::getWorldPosition() const {
    return worldPosition;
  }

  const Float4 Camera::getVFNearTopLeft() const {
    return vfNearTopLeft;
  }

  const Float4 Camera::getVFNearRightDirection() const {
    return vfNearRightDirection;
  }

  const Float4 Camera::getVFNearBottomDirection() const {
    return vfNearBottomDirection;
  }

  const Float4 Camera::getVFFarTopLeft() const {
    return vfFarTopLeft;
  }

  const Float4 Camera::getVFFarRightDirection() const {
    return vfFarRightDirection;
  }

  const Float4 Camera::getVFFarBottomDirection() const {
    return vfFarBottomDirection;
  }

  void Camera::updateView() {
    const Float44 tViewMatrix = this->viewMatrix;
    const Float44 tInverseViewProjectionMatrix = inverse(this->projectionMatrix * tViewMatrix);

    this->worldPosition = inverse(tViewMatrix) * OneW<Float4>();

    // extents of the viewport
    const Float4 NearZ = OneZ<Float4>();
    const Float4 FarZ = Zero<Float4>();
    const Float4 viewportExtends = Two<Float4>();

    const Float4 viewportOffset = viewportExtends * Half<Float4>();
    const Float4 viewportScale = Two<Float4>() / viewportExtends;
    const Float4 viewport = xy_xy(viewportOffset, viewportScale);

    const Float4 nearTopLeft = unproject(NearZ, tInverseViewProjectionMatrix, viewport);
    const Float4 farTopLeft = unproject(FarZ, tInverseViewProjectionMatrix, viewport);

    this->vfNearTopLeft = nearTopLeft;
    this->vfFarTopLeft = farTopLeft;
    this->vfNearRightDirection =
        unproject(x_yzw(viewportExtends, NearZ), tInverseViewProjectionMatrix, viewport) - nearTopLeft;
    this->vfNearBottomDirection =
        unproject(blend<true, false, true, true>(viewportExtends, NearZ), tInverseViewProjectionMatrix, viewport)
        - nearTopLeft;
    this->vfFarRightDirection =
        unproject(x_yzw(viewportExtends, FarZ), tInverseViewProjectionMatrix, viewport) - farTopLeft;
    this->vfFarBottomDirection =
        unproject(blend<true, false, true, true>(viewportExtends, FarZ), tInverseViewProjectionMatrix, viewport)
        - farTopLeft;
  }
}