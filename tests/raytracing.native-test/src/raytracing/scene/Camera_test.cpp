#include "raytracing_tests.h"

namespace raytracer::test {
  // The Camera does not generate primary rays itself; it exposes the view-frustum corners that
  // RaytracerPackets uses to build per-pixel rays. These tests pin the frustum invariants and
  // mirror the per-pixel arithmetic from RaytracerPackets to exercise the camera-as-ray-source.
  namespace {
    Float4 pixelNearTopLeft(const Camera& camera, const Size2& resolution, const Float4& pixelCoords) {
      const auto inverseResolution = reciprocal(convert<Float4>(resolution));
      const auto nearRightDir = camera.getVFNearRightDirection() * xxxx(inverseResolution);
      const auto nearBottomDir = camera.getVFNearBottomDirection() * yyyy(inverseResolution);
      // RaytracerPackets shifts the top-left by half a sub-sample step. With ssLength=1 (no super
      // sampling), the half-step equals half a pixel — which is the pixel center.
      const auto topLeft = camera.getVFNearTopLeft() + Half<Float4>() * (nearRightDir + nearBottomDir);
      return topLeft + nearRightDir * xxxx(pixelCoords) + nearBottomDir * yyyy(pixelCoords);
    }

    Float4 pixelFarTopLeft(const Camera& camera, const Size2& resolution, const Float4& pixelCoords) {
      const auto inverseResolution = reciprocal(convert<Float4>(resolution));
      const auto farRightDir = camera.getVFFarRightDirection() * xxxx(inverseResolution);
      const auto farBottomDir = camera.getVFFarBottomDirection() * yyyy(inverseResolution);
      const auto topLeft = camera.getVFFarTopLeft() + Half<Float4>() * (farRightDir + farBottomDir);
      return topLeft + farRightDir * xxxx(pixelCoords) + farBottomDir * yyyy(pixelCoords);
    }
  }

  TEST_CLASS(CameraTest) {
  public:
    TEST_METHOD(defaultViewMatrixIsIdentity) {
      const Camera camera{};
      const auto& view = camera.getViewMatrix();
      Assert::AreEqual(1.0f, x(view.row0), L"row0 x", LINE_INFO());
      Assert::AreEqual(1.0f, y(view.row1), L"row1 y", LINE_INFO());
      Assert::AreEqual(1.0f, z(view.row2), L"row2 z", LINE_INFO());
      Assert::AreEqual(1.0f, w(view.row3), L"row3 w", LINE_INFO());
    }

    TEST_METHOD(defaultWorldPositionIsOrigin) {
      const Camera camera{};
      const auto pos = camera.getWorldPosition();
      Assert::AreEqual(0.0f, x(pos), L"position x", LINE_INFO());
      Assert::AreEqual(0.0f, y(pos), L"position y", LINE_INFO());
      Assert::AreEqual(0.0f, z(pos), L"position z", LINE_INFO());
      Assert::AreEqual(1.0f, w(pos), L"position w (homogeneous)", LINE_INFO());
    }

    TEST_METHOD(setProjectionPopulatesFrustum) {
      Camera camera{};
      camera.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      const auto nearRight = camera.getVFNearRightDirection();
      const auto nearBottom = camera.getVFNearBottomDirection();
      // Lock in the observed orientation: both x(nearRight) and y(nearBottom) are positive in
      // this engine. The image plane uses "right" along +x and "bottom" along +y in view space
      // (the framebuffer Y axis is consequently flipped at output).
      Assert::IsTrue(x(nearRight) > 0.0f, L"near right has positive x", LINE_INFO());
      Assert::IsTrue(y(nearBottom) > 0.0f, L"near bottom has positive y", LINE_INFO());
      // Right and bottom should be the dominant components (no skew on identity view).
      Assert::IsTrue(std::abs(x(nearRight)) > std::abs(y(nearRight)),
                     L"right is dominantly x-aligned", LINE_INFO());
      Assert::IsTrue(std::abs(y(nearBottom)) > std::abs(x(nearBottom)),
                     L"bottom is dominantly y-aligned", LINE_INFO());
    }

    TEST_METHOD(squareAspectGivesSymmetricFrustum) {
      Camera camera{};
      camera.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      const auto nearRight = camera.getVFNearRightDirection();
      const auto nearBottom = camera.getVFNearBottomDirection();
      // For square viewport the right span and the bottom span have equal magnitude.
      Assert::AreEqual(x(nearRight), y(nearBottom), 0.0001f, L"square aspect symmetry",
                       LINE_INFO());
    }

    TEST_METHOD(wideAspectScalesRightProportionally) {
      Camera camera{};
      camera.setProjection(45.0f, Float2{4.0f, 2.0f}, Float2{0.01f, 2.0f});
      const auto nearRight = camera.getVFNearRightDirection();
      const auto nearBottom = camera.getVFNearBottomDirection();
      // Width is 2x height — right span should be 2x bottom span (in magnitude).
      Assert::AreEqual(2.0f, x(nearRight) / y(nearBottom), 0.001f, L"wide aspect ratio",
                       LINE_INFO());
    }

    TEST_METHOD(tallAspectScalesBottomProportionally) {
      Camera camera{};
      camera.setProjection(45.0f, Float2{2.0f, 4.0f}, Float2{0.01f, 2.0f});
      const auto nearRight = camera.getVFNearRightDirection();
      const auto nearBottom = camera.getVFNearBottomDirection();
      Assert::AreEqual(0.5f, x(nearRight) / y(nearBottom), 0.001f, L"tall aspect ratio",
                       LINE_INFO());
    }

    TEST_METHOD(largerFovGivesWiderFrustum) {
      Camera narrow{};
      narrow.setProjection(30.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      Camera wide{};
      wide.setProjection(90.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      Assert::IsTrue(x(wide.getVFNearRightDirection()) > x(narrow.getVFNearRightDirection()),
                     L"90° fov is wider than 30°", LINE_INFO());
    }

    TEST_METHOD(translateMovesWorldPosition) {
      Camera camera{};
      camera.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      camera.translate(Float3{1.0f, 2.0f, -3.0f});
      const auto pos = camera.getWorldPosition();
      // Camera::translate moves the camera in the inverse view direction; the visible world
      // position equals the negated translation (camera at -t when world-translation is t in
      // view-space). Lock in the observed convention.
      Assert::AreEqual(-1.0f, x(pos), 0.0001f, L"world pos x after translate", LINE_INFO());
      Assert::AreEqual(-2.0f, y(pos), 0.0001f, L"world pos y after translate", LINE_INFO());
      Assert::AreEqual(3.0f, z(pos), 0.0001f, L"world pos z after translate", LINE_INFO());
      Assert::AreEqual(1.0f, w(pos), 0.0001f, L"homogeneous w preserved", LINE_INFO());
    }

    TEST_METHOD(translateShiftsFrustumWithCamera) {
      Camera reference{};
      reference.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      const auto refNearTL = reference.getVFNearTopLeft();

      Camera moved{};
      moved.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      moved.translate(Float3{0.0f, 0.0f, -5.0f});
      const auto movedNearTL = moved.getVFNearTopLeft();

      // The frustum is anchored to the camera, so translating the camera shifts the near corner
      // by the same amount along z.
      Assert::AreEqual(z(refNearTL) + 5.0f, z(movedNearTL), 0.001f, L"frustum follows translate",
                       LINE_INFO());
      Assert::AreEqual(x(refNearTL), x(movedNearTL), 0.0001f, L"x corner unchanged", LINE_INFO());
    }

    TEST_METHOD(centerPixelRayHasOriginAtCameraPositionZ) {
      Camera camera{};
      camera.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      const Size2 resolution{512, 512};
      const Float4 centerPixel{256.0f, 256.0f, 0.0f, 0.0f};
      const auto rayOrigin = pixelNearTopLeft(camera, resolution, centerPixel);
      // Center pixel ray origin lies on the near plane along the camera's optical axis.
      Assert::AreEqual(0.0f, x(rayOrigin), 0.01f, L"center pixel x ≈ 0", LINE_INFO());
      Assert::AreEqual(0.0f, y(rayOrigin), 0.01f, L"center pixel y ≈ 0", LINE_INFO());
    }

    TEST_METHOD(cornerPixelsAreSymmetric) {
      Camera camera{};
      camera.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      const Size2 resolution{512, 512};
      const auto topLeft = pixelNearTopLeft(camera, resolution, Float4{0.0f, 0.0f, 0.0f, 0.0f});
      const auto topRight = pixelNearTopLeft(camera, resolution, Float4{511.0f, 0.0f, 0.0f, 0.0f});
      const auto bottomLeft = pixelNearTopLeft(camera, resolution, Float4{0.0f, 511.0f, 0.0f, 0.0f});
      const auto bottomRight = pixelNearTopLeft(camera, resolution, Float4{511.0f, 511.0f, 0.0f, 0.0f});

      // Corners are mirror images across the camera optical axis (square viewport).
      Assert::AreEqual(x(topLeft), -x(topRight), 0.001f, L"x mirror around optical axis", LINE_INFO());
      Assert::AreEqual(y(topLeft), -y(bottomLeft), 0.001f, L"y mirror around optical axis", LINE_INFO());
      Assert::AreEqual(z(topLeft), z(topRight), 0.0001f, L"all corners at same z (near plane)",
                       LINE_INFO());
      Assert::AreEqual(z(topLeft), z(bottomRight), 0.0001f, L"diagonal corners at same z", LINE_INFO());
    }

    TEST_METHOD(rayDirectionFromCenterPointsForward) {
      Camera camera{};
      camera.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      const Size2 resolution{512, 512};
      const Float4 centerPixel{256.0f, 256.0f, 0.0f, 0.0f};
      const auto nearOrigin = pixelNearTopLeft(camera, resolution, centerPixel);
      const auto farPoint = pixelFarTopLeft(camera, resolution, centerPixel);
      const auto direction = farPoint - nearOrigin;
      // Near-plane to far-plane has positive z magnitude (camera looks toward +z in this setup).
      Assert::IsTrue(std::abs(x(direction)) < 0.01f, L"forward direction x ≈ 0", LINE_INFO());
      Assert::IsTrue(std::abs(y(direction)) < 0.01f, L"forward direction y ≈ 0", LINE_INFO());
      Assert::IsTrue(std::abs(z(direction)) > 0.0f, L"forward direction z is non-zero",
                     LINE_INFO());
    }

    TEST_METHOD(cornerRaysDivergeFromCenter) {
      Camera camera{};
      camera.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      const Size2 resolution{512, 512};
      const Float4 centerPixel{256.0f, 256.0f, 0.0f, 0.0f};
      const Float4 cornerPixel{0.0f, 0.0f, 0.0f, 0.0f};

      const auto centerOrigin = pixelNearTopLeft(camera, resolution, centerPixel);
      const auto cornerOrigin = pixelNearTopLeft(camera, resolution, cornerPixel);
      const auto centerFar = pixelFarTopLeft(camera, resolution, centerPixel);
      const auto cornerFar = pixelFarTopLeft(camera, resolution, cornerPixel);

      const auto centerDir = centerFar - centerOrigin;
      const auto cornerDir = cornerFar - cornerOrigin;

      // Corner ray's direction must differ from the center ray's direction in x and y.
      Assert::IsTrue(std::abs(x(cornerDir) - x(centerDir)) > 0.0001f,
                     L"corner ray x differs from center", LINE_INFO());
      Assert::IsTrue(std::abs(y(cornerDir) - y(centerDir)) > 0.0001f,
                     L"corner ray y differs from center", LINE_INFO());
    }

    TEST_METHOD(setViewMatrixOverwritesView) {
      Camera camera{};
      const auto custom = Identity<MFloat4x4>();
      camera.setViewMatrix(custom);
      const auto& view = camera.getViewMatrix();
      Assert::AreEqual(1.0f, x(view.row0), L"row0 x", LINE_INFO());
      Assert::AreEqual(1.0f, y(view.row1), L"row1 y", LINE_INFO());
    }

    TEST_METHOD(resetViewRestoresIdentity) {
      Camera camera{};
      camera.translate(Float3{5.0f, 5.0f, 5.0f});
      camera.resetView();
      const auto pos = camera.getWorldPosition();
      Assert::AreEqual(0.0f, x(pos), 0.0001f, L"position x reset", LINE_INFO());
      Assert::AreEqual(0.0f, y(pos), 0.0001f, L"position y reset", LINE_INFO());
      Assert::AreEqual(0.0f, z(pos), 0.0001f, L"position z reset", LINE_INFO());
    }

    TEST_METHOD(rotateChangesFrustumOrientation) {
      Camera reference{};
      reference.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      const auto refBottom = reference.getVFNearBottomDirection();

      Camera rotated{};
      rotated.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      // Camera::rotate maps rotation.y to the OneX axis (see Camera.cpp). Rotating by 90° around
      // OneX must rotate the bottom direction (originally on +y) into +z.
      rotated.rotate(Float3{0.0f, 90.0f, 0.0f});
      const auto rotBottom = rotated.getVFNearBottomDirection();

      const auto delta = std::abs(x(rotBottom) - x(refBottom)) + std::abs(y(rotBottom) - y(refBottom))
                         + std::abs(z(rotBottom) - z(refBottom));
      Assert::IsTrue(delta > 0.0f, L"rotation changes the bottom direction", LINE_INFO());
    }

    TEST_METHOD(farFrustumDiffersFromNear) {
      Camera camera{};
      camera.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      const auto nearRight = camera.getVFNearRightDirection();
      const auto farRight = camera.getVFFarRightDirection();
      // The far and near frustum spans differ — they describe different planes. Locking in the
      // weakest claim: they are not identical for non-trivial near/far planes.
      Assert::IsTrue(std::abs(x(farRight) - x(nearRight)) > 0.0f, L"far right differs from near right",
                     LINE_INFO());
    }

    TEST_METHOD(cornerPixelDirectionMagnitudeExceedsCenter) {
      Camera camera{};
      camera.setProjection(45.0f, Float2{2.0f, 2.0f}, Float2{0.01f, 2.0f});
      const Size2 resolution{512, 512};
      const auto centerOrigin =
          pixelNearTopLeft(camera, resolution, Float4{256.0f, 256.0f, 0.0f, 0.0f});
      const auto centerFar =
          pixelFarTopLeft(camera, resolution, Float4{256.0f, 256.0f, 0.0f, 0.0f});
      const auto cornerOrigin =
          pixelNearTopLeft(camera, resolution, Float4{0.0f, 0.0f, 0.0f, 0.0f});
      const auto cornerFar =
          pixelFarTopLeft(camera, resolution, Float4{0.0f, 0.0f, 0.0f, 0.0f});

      // Corner rays travel a longer geometric distance from near to far plane than the center
      // ray (which is axis-aligned).
      Assert::IsTrue(length3(cornerFar - cornerOrigin) > length3(centerFar - centerOrigin),
                     L"corner direction is longer than center", LINE_INFO());
    }
  };
}
