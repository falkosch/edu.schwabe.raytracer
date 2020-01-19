#pragma once

#include <raytracing.h>

namespace raytracerui
{
    using namespace vectorization;
    using namespace raytracer;

    class RaytracerUI : public RaytraceObserver
    {
    protected:

        RaytraceParameters parameters;

        Int2 screenSize;

        ASizeT fastPreviewSize;

        ASizeT showMapIndex;

        bool disableFastPreview;

        const HDRImage * outputHDR;

        const Bitmap * output;

        Raytracer * raytracer;

        virtual void reshape(const Int2 & newSize);

    public:

        RaytracerUI(Raytracer & raytracer, const RaytraceParameters & parameters, const ASizeT & fastPreviewSize);

        virtual ~RaytracerUI();

        void triggerRaytracing(const bool fastPreview);

        void notifyUpdate(const RaytraceConfiguration & configuration);

        const HDRImage * const selectOutputImage(const RaytraceConfiguration & configuration) const;
    };
}
