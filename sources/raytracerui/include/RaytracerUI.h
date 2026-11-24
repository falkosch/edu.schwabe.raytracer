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

        // exposure of the displayed image in stops: the HDR image is scaled by 2^exposure before the view transform
        Float exposure{};

        // the least super-sampling factor of a full render (Enter), against the aliasing of one sample per pixel
        ASizeT fullRenderSuperSamplingFactor{};

        // whether full renders are denoised too (parameters.denoise is the fast previews')
        bool fullRenderDenoise{};

        // the run of the full render that is the first image (setFullFirstRender), 0 without; earlier runs are ignored
        ASizeT firstImageRunId{};

        ASizeT lastFullRenderRunId{};

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

        void setExposure(Float stops);

        void setFullRenderSuperSamplingFactor(ASizeT factor);

        void setFullRenderDenoise(bool denoise);

        // makes a full render the first image (saved as ray-traced.png), instead of the first fast preview
        void setFullFirstRender();

        void notifyUpdate(const RaytraceConfiguration& configuration) override;

        std::shared_ptr<const HDRImage> selectOutputImage(const RaytraceConfiguration& configuration) const;
    };
}
