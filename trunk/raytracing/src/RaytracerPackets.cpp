#include "stdafx.h"

#include "RaytracerPackets.h"

namespace raytracer
{

    inline const Size2 calculateSupersamplingLengths(const Size2::ValueType supersamplingFactor)
    {
        const Size2::ValueType supersamplingLength = supersamplingFactor + One<Size2::ValueType>();
        return Size2(supersamplingLength, supersamplingLength * supersamplingLength);
    }

    inline const Size2 calculatePacketLengths(const Size2::ValueType rayPacketSize, const Size2 & supersamplingLength)
    {
        return Size2(max(One<Size2::ValueType>(), (rayPacketSize + One<Size2::ValueType>()) / x(supersamplingLength)));
    }

    inline const Size2 calculatePacketCounts(const Size2 & resolution, const Size2 & packetLength)
    {
        const Size2 packetCountXY = (resolution - One<Size2>()) / packetLength + One<Size2>();
        // overwrite vertical packet count with the total packet count
        return replaceY(packetCountXY, x(packetCountXY) * y(packetCountXY));
    }

    RaytracerPackets::RaytracerPackets(const RaytraceConfiguration & configuration)
    {
        const Float4 fresolution = reciprocal(convert<Float4>(configuration.resolution));

        // x-increments
        nearRightDir = configuration.camera->getVFNearRightDirection() * xxxx(fresolution);
        farRightDir = configuration.camera->getVFFarRightDirection() * xxxx(fresolution);

        // y-increments
        nearBottomDir = configuration.camera->getVFNearBottomDirection() * yyyy(fresolution);
        farBottomDir = configuration.camera->getVFFarBottomDirection() * yyyy(fresolution);

        // Super-sampling anti-aliasing parameters
        ssLength = calculateSupersamplingLengths(configuration.supersamplingFactor);

        const Float4 ssInverseFactor = Float4(reciprocal(static_cast<Float>(x(ssLength))));
        ssAvg = ssInverseFactor * ssInverseFactor;
        ssNearRightDir = nearRightDir * ssInverseFactor;
        ssFarRightDir = farRightDir * ssInverseFactor;
        ssNearBottomDir = nearBottomDir * ssInverseFactor;
        ssFarBottomDir = farBottomDir * ssInverseFactor;

        // add an offset to top-left-point to correct sample-positioning
        nearTopLeft = configuration.camera->getVFNearTopLeft() + Half<Float4>() * (ssNearRightDir + ssNearBottomDir);
        farTopLeft = configuration.camera->getVFFarTopLeft() + Half<Float4>() * (ssFarRightDir + ssFarBottomDir);

        // Calculate packet boundaries
        packetLength = calculatePacketLengths(configuration.rayPacketSize, ssLength);
        packetCount = calculatePacketCounts(configuration.resolution, packetLength);
    }

    const int RaytracerPackets::getPacketCount() const
    {
        return static_cast<int>(y(packetCount));
    }

    const Size2 RaytracerPackets::packetStartOf(const int packet) const
    {
        return packetStartOf(static_cast<Size2::ValueType>(packet));
    }

    const Size2 RaytracerPackets::packetStartOf(const Size2::ValueType packet) const
    {
        const Size2::ValueType count = x(packetCount);
        return Size2(
            (packet % count) * x(packetLength),
            (packet / count) * y(packetLength));
    }

    const Size2 RaytracerPackets::clampedPacketLength(const Size2 & resolution, const Size2 & packetXYStart) const
    {
        // clamp packet length
        const Size2 minPacketLength = min(packetLength, resolution - packetXYStart);
        // overwrite vertical packet length with the total packet length
        return replaceY(minPacketLength, x(minPacketLength) * y(minPacketLength));
    }

    const Size2::ValueType RaytracerPackets::packetPixelsCount(const Size2 minPacketLength)
    {
        return y(minPacketLength);
    }

    const Float4 RaytracerPackets::pixelNearTopLeft(const Float4 & fcoordsOfPixel) const
    {
        return nearTopLeft + nearRightDir * xxxx(fcoordsOfPixel) + nearBottomDir * yyyy(fcoordsOfPixel);
    }

    const Float4 RaytracerPackets::pixelFarTopLeft(const Float4 & fcoordsOfPixel) const
    {
        return farTopLeft + farRightDir * xxxx(fcoordsOfPixel) + farBottomDir * yyyy(fcoordsOfPixel);
    }

    const Ray RaytracerPackets::setupRayOfSampleInPixel(const Size2::ValueType sample, const Float4 & pixelNearTL, const Float4 & pixelFarTL) const
    {
        const Size2::ValueType xSSLength = x(ssLength);
        const Float4 sampleCoords = convert<Float4>(Size2(sample % xSSLength, sample / xSSLength));
        const Float4 origin = pixelNearTL + ssNearRightDir * xxxx(sampleCoords) + ssNearBottomDir * yyyy(sampleCoords);
        return Ray(origin, normalize(pixelFarTL - origin + ssFarRightDir * xxxx(sampleCoords) + ssFarBottomDir * yyyy(sampleCoords)));
    }

    void RaytracerPackets::samplePixel(Float4 * const pixel, const Float4 & sample) const
    {
        store(Float4(pixel) + sample * ssAvg, pixel);
    }

    const Float4 RaytracerPackets::supersampledPixelDepth(const Float4 & sampledDepth) const
    {
        return log3(xy_zw(sampledDepth, sampledDepth * ssAvg));
    }

    const Size2::ValueType RaytracerPackets::pixelSubsamplesCount() const
    {
        return y(ssLength);
    }

    const Size2 RaytracerPackets::coordsOfPixel(const Size2::ValueType pixelInPacket, const Size2 & packetStart, const Size2 & minPacketLength)
    {
        const Size2::ValueType xMinPacketLength = x(minPacketLength);
        return packetStart + Size2(pixelInPacket % xMinPacketLength, pixelInPacket / xMinPacketLength);
    }

    const Size2::ValueType RaytracerPackets::imageIndexOfPixel(const Size2 & coordsOfPixel, const Size2 & resolution)
    {
        return x(coordsOfPixel) + y(coordsOfPixel) * x(resolution);
    }

    const Float4 RaytracerPackets::samplePixelDepth(const Float4 & sampledPixelDepth, const Float & depth)
    {
        const Float4 depthv = Float4(depth);
        return xz_xz(xx_yy(min(depthv, sampledPixelDepth), max(depthv, sampledPixelDepth)), depthv);
    }

}
