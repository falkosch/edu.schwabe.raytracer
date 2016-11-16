#pragma once

#include "RaytraceConfiguration.h"

namespace raytracer
{

    // per worker thread cached raytracing information
    struct RaytracerPackets
    {
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

        ALIGNED_ALLOCATORS(__alignof(RaytracerPackets));

        RaytracerPackets(const RaytraceConfiguration & configuration);

        const int getPacketCount() const;

        const Size2 packetStartOf(const int packet) const;

        const Size2 packetStartOf(const Size2::ValueType packet) const;

        const Size2 clampedPacketLength(const Size2 & resolution, const Size2 & packetXYStart) const;

        const Float4 pixelNearTopLeft(const Float4 & fcoordsOfPixel) const;

        const Float4 pixelFarTopLeft(const Float4 & fcoordsOfPixel) const;

        const Ray setupRayOfSampleInPixel(const Size2::ValueType sample, const Float4 & pixelNearTL, const Float4 & pixelFarTL) const;

        void samplePixel(Float4 * const pixel, const Float4 & sample) const;

        const Float4 supersampledPixelDepth(const Float4 & sampledDepth) const;

        const Size2::ValueType pixelSubsamplesCount() const;

        static const Size2 coordsOfPixel(const Size2::ValueType pixelInPacket, const Size2 & packetStart, const Size2 & minPacketLength);

        static const Size2::ValueType packetPixelsCount(const Size2 minPacketLength);

        static const Size2::ValueType imageIndexOfPixel(const Size2 & coordsOfPixel, const Size2 & resolution);

        static const Float4 samplePixelDepth(const Float4 & sampledPixelDepth, const Float & depth);
    };

}
