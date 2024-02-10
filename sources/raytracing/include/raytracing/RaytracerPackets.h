#pragma once

#include "RaytraceConfiguration.h"

#include <primitives/Ray.h>
#include <vectorization.h>

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  // per worker thread cached raytracing information
  struct RaytracerPackets {
    Size2 packetCount;
    // x: pixels per each line in packet
    // y: total count of pixels in packet
    Size2 packetLength;
    // count of super samples in pixel
    Size2 ssLength;

    // top left origin of camera planes
    Float4 nearTopLeft, farTopLeft;
    // extends of the camera near plane
    Float4 nearRightDir, farRightDir;
    // extends of the camera far plane
    Float4 nearBottomDir, farBottomDir;
    // extends in super sampling spaces
    Float4 ssNearRightDir, ssFarRightDir;
    Float4 ssNearBottomDir, ssFarBottomDir;
    // super sample weight
    Float4 ssAvg;

    RaytracerPackets(const RaytraceConfiguration &configuration);

    [[nodiscard]] int getPacketCount() const;

    [[nodiscard]] Size2 packetStartOf(int packet) const;

    [[nodiscard]] Size2 packetStartOf(Size2::ValueType packet) const;

    [[nodiscard]] Size2 clampPacketLength(const Size2 &resolution, const Size2 &packetXYStart) const;

    [[nodiscard]] Float4 pixelNearTopLeft(const Float4 &pixelCoordinates) const;

    [[nodiscard]] Float4 pixelFarTopLeft(const Float4 &pixelCoordinates) const;

    [[nodiscard]] Ray setupRayOfSampleInPixel(
        Size2::ValueType sample, const Float4 &pixelNearTopLeft, const Float4 &pixelFarTopLeft
    ) const;

    void samplePixel(Float4 *pixel, const Float4 &sample) const;

    [[nodiscard]] Float4 superSampledPixelDepth(const Float4 &sampledDepth) const;

    [[nodiscard]] Size2::ValueType pixelSubSamplesCount() const;

    static Size2 coordsOfPixel(Size2::ValueType pixelInPacket, const Size2 &packetStart, const Size2 &minPacketLength);

    static Size2::ValueType packetPixelsCount(Size2 minPacketLength);

    static Size2::ValueType imageIndexOfPixel(const Size2 &coordsOfPixel, const Size2 &resolution);

    static Float4 samplePixelDepth(const Float4 &sampledPixelDepth, const Float4 &depth);
  };
}
