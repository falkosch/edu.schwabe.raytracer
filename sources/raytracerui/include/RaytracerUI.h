#pragma once

#include <raytracing.h>

#include <memory>

namespace raytracerui
{
    using namespace vectorization;
    using namespace raytracer;

    class RaytracerUI : public RaytraceObserver
    {
    protected:
        RaytraceParameters parameters;

        Size2 screenSize;

        Size2 fastPreviewSize;

        ASizeT showMapIndex;

        bool disableFastPreview;

        ASizeT frameCount;

        StatisticsCookie lastStatistics;

        Float_64 lastRenderDuration{};

        std::shared_ptr<const HDRImage> outputHDR;

        std::unique_ptr<const Bitmap> output;

        MFloat4x4 initialViewMatrix;

        Raytracer* raytracer;

        virtual void reshape(const Size2& newSize);

    public:
        RaytracerUI(Raytracer& raytracer, const RaytraceParameters& parameters, const Size2& fastPreviewSize);

        ~RaytracerUI() override;

        virtual void triggerRaytracing(bool fastPreview);

        void notifyUpdate(const RaytraceConfiguration& configuration) override;

        std::shared_ptr<const HDRImage> selectOutputImage(const RaytraceConfiguration& configuration) const;
    };
}
