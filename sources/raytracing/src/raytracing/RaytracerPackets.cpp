#include "raytracing/RaytracerPackets.h"
#include "../stdafx.h"

namespace raytracer {
  Size2 calculateSuperSamplingLengths(const Size2::ValueType superSamplingFactor) {
    const auto superSamplingLength = superSamplingFactor + One<Size2::ValueType>();
    return Size2(superSamplingLength, superSamplingLength * superSamplingLength);
  }

  Size2 calculatePacketLengths(const Size2::ValueType rayPacketSize, const Size2 &superSamplingLength) {
    return Size2(max(One<Size2::ValueType>(), (rayPacketSize + One<Size2::ValueType>()) / x(superSamplingLength)));
  }

  Size2 calculatePacketCounts(const Size2 &resolution, const Size2 &packetLength) {
    const auto packetCountXY = (resolution - One<Size2>()) / packetLength + One<Size2>();
    // overwrite the vertical packet count with the total packet count
    return replaceY(packetCountXY, x(packetCountXY) * y(packetCountXY));
  }

  RaytracerPackets::RaytracerPackets(const RaytraceConfiguration &configuration) {
    const auto inverseResolution = reciprocal(convert<Float4>(configuration.resolution));

    // x-increments
    nearRightDir = configuration.camera->getVFNearRightDirection() * xxxx(inverseResolution);
    farRightDir = configuration.camera->getVFFarRightDirection() * xxxx(inverseResolution);

    // y-increments
    nearBottomDir = configuration.camera->getVFNearBottomDirection() * yyyy(inverseResolution);
    farBottomDir = configuration.camera->getVFFarBottomDirection() * yyyy(inverseResolution);

    // Super-sampling anti-aliasing parameters
    ssLength = calculateSuperSamplingLengths(configuration.superSamplingFactor);

    const auto ssInverseFactor = Float4(reciprocal(static_cast<Float>(x(ssLength))));
    ssAvg = ssInverseFactor * ssInverseFactor;
    ssNearRightDir = nearRightDir * ssInverseFactor;
    ssFarRightDir = farRightDir * ssInverseFactor;
    ssNearBottomDir = nearBottomDir * ssInverseFactor;
    ssFarBottomDir = farBottomDir * ssInverseFactor;

    // add an offset to the top-left-point to correct sample-positioning
    nearTopLeft = configuration.camera->getVFNearTopLeft() + Half<Float4>() * (ssNearRightDir + ssNearBottomDir);
    farTopLeft = configuration.camera->getVFFarTopLeft() + Half<Float4>() * (ssFarRightDir + ssFarBottomDir);

    // Calculate packet boundaries
    packetLength = calculatePacketLengths(configuration.rayPacketSize, ssLength);
    packetCount = calculatePacketCounts(configuration.resolution, packetLength);
  }

  int RaytracerPackets::getPacketCount() const {
    return static_cast<int>(y(packetCount));
  }

  Size2 RaytracerPackets::packetStartOf(const int packet) const {
    return packetStartOf(static_cast<Size2::ValueType>(packet));
  }

  Size2 RaytracerPackets::packetStartOf(const Size2::ValueType packet) const {
    const auto count = x(packetCount);
    return Size2((packet % count) * x(packetLength), (packet / count) * y(packetLength));
  }

  Size2 RaytracerPackets::clampPacketLength(const Size2 &resolution, const Size2 &packetXYStart) const {
    // clamp packet length
    const auto minPacketLength = min(packetLength, resolution - packetXYStart);
    // overwrite vertical packet length with the total packet length
    return replaceY(minPacketLength, x(minPacketLength) * y(minPacketLength));
  }

  Size2::ValueType RaytracerPackets::packetPixelsCount(const Size2 minPacketLength) {
    return y(minPacketLength);
  }

  Float4 RaytracerPackets::pixelNearTopLeft(const Float4 &pixelCoordinates) const {
    return nearTopLeft + nearRightDir * xxxx(pixelCoordinates) + nearBottomDir * yyyy(pixelCoordinates);
  }

  Float4 RaytracerPackets::pixelFarTopLeft(const Float4 &pixelCoordinates) const {
    return farTopLeft + farRightDir * xxxx(pixelCoordinates) + farBottomDir * yyyy(pixelCoordinates);
  }

  Ray RaytracerPackets::setupRayOfSampleInPixel(
      const Size2::ValueType sample, const Float4 &pixelNearTopLeft, const Float4 &pixelFarTopLeft
  ) const {
    const auto xSSLength = x(ssLength);
    const auto sampleCoords = convert<Float4>(Size2(sample % xSSLength, sample / xSSLength));
    const auto origin = pixelNearTopLeft + ssNearRightDir * xxxx(sampleCoords) + ssNearBottomDir * yyyy(sampleCoords);
    return Ray(
        origin,
        normalize(pixelFarTopLeft - origin + ssFarRightDir * xxxx(sampleCoords) + ssFarBottomDir * yyyy(sampleCoords))
    );
  }

  void RaytracerPackets::samplePixel(Float4 *const pixel, const Float4 &sample) const {
    store(Float4(pixel) + sample * ssAvg, pixel);
  }

  Float4 RaytracerPackets::superSampledPixelDepth(const Float4 &sampledDepth) const {
    return log3(xy_zw(sampledDepth, sampledDepth * ssAvg));
  }

  Size2::ValueType RaytracerPackets::pixelSubSamplesCount() const {
    return y(ssLength);
  }

  Size2 RaytracerPackets::coordsOfPixel(
      const Size2::ValueType pixelInPacket, const Size2 &packetStart, const Size2 &minPacketLength
  ) {
    const auto xMinPacketLength = x(minPacketLength);
    return packetStart + Size2(pixelInPacket % xMinPacketLength, pixelInPacket / xMinPacketLength);
  }

  Size2::ValueType RaytracerPackets::imageIndexOfPixel(const Size2 &coordsOfPixel, const Size2 &resolution) {
    return x(coordsOfPixel) + y(coordsOfPixel) * x(resolution);
  }

  Float4 RaytracerPackets::samplePixelDepth(const Float4 &sampledPixelDepth, const Float4 &depth) {
    return xz_xz(xx_yy(min(depth, sampledPixelDepth), max(depth, sampledPixelDepth)), depth);
  }
}
