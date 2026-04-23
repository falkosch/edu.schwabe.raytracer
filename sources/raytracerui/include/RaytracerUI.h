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

    Int2 screenSize;

    ASizeT fastPreviewSize;

    ASizeT showMapIndex;

    bool disableFastPreview;

    ASizeT frameCount;

    StatisticsCookie lastStatistics;

    Float_64 lastRenderDuration{};

    std::unique_ptr<const HDRImage> outputHDR;

    std::unique_ptr<const Bitmap> output;

    Raytracer* raytracer;

    virtual void reshape(const Int2& newSize);

  public:
    RaytracerUI(Raytracer& raytracer, const RaytraceParameters& parameters, ASizeT& fastPreviewSize);

    ~RaytracerUI() override;

    virtual void triggerRaytracing(bool fastPreview);

    void notifyUpdate(const RaytraceConfiguration& configuration) override;

    const HDRImage* selectOutputImage(const RaytraceConfiguration& configuration) const;
  };
}
