#include "RaytracerUI.h"
#include "stdafx.h"

#include <logging.h>
#include <string>

static const auto Log = logging::scope("UI");

namespace raytracerui
{
  RaytracerUI::RaytracerUI(Raytracer& raytracerIn, const RaytraceParameters& parametersIn, ASizeT& fastPreviewSizeIn)
    : parameters(parametersIn), screenSize(One<Int2>()), fastPreviewSize(fastPreviewSizeIn), showMapIndex(),
      disableFastPreview(), frameCount(), outputHDR(), output(),
      initialViewMatrix(parametersIn.camera->getViewMatrix()), raytracer(&raytracerIn)
  {
    this->parameters.observer = this;
  }

  RaytracerUI::~RaytracerUI() = default;

  void RaytracerUI::reshape(const Int2& newSize)
  {
    Log.info([w = x(newSize), h = y(newSize)]
    {
      return "Resizing output to " + std::to_string(w) + "x" + std::to_string(h);
    });
    screenSize = newSize;
    parameters.camera->setProjection(
      parameters.perspectiveFOV, convert<Float2>(newSize), parameters.perspectiveZPlanes
    );
    triggerRaytracing(true);
  }

  void RaytracerUI::triggerRaytracing(const bool fastPreview)
  {
    const Int2 clampedSize = max(screenSize, One<Int2>());
    if (fastPreview && !disableFastPreview)
    {
      parameters.resolution = Size2(fastPreviewSize, (fastPreviewSize * y(clampedSize)) / x(clampedSize));
    }
    else
    {
      parameters.resolution = convert<Size2>(clampedSize);
    }
    raytracer->trigger(parameters);
  }

  void RaytracerUI::notifyUpdate(const RaytraceConfiguration& configuration)
  {
    lastStatistics = configuration.statistics;
    lastRenderDuration = configuration.durationSeconds;

    const auto& s = configuration.statistics;
    Log.info([pr = s.primaryRays, mr = s.missedPrimaryRays]
    {
      return "primaryRays " + std::to_string(pr) + "/" + std::to_string(mr);
    });
    Log.info([sr = s.secondaryRays, mr = s.missedSecondaryRays]
    {
      return "secondaryRays " + std::to_string(sr) + "/" + std::to_string(mr);
    });
    Log.info([sr = s.shadowRays, mr = s.missedShadowRays]
    {
      return "shadowRays " + std::to_string(sr) + "/" + std::to_string(mr);
    });
    Log.info([osr = s.objectShadowRays, omr = s.objectMissedShadowRays]
    {
      return "objectShadowRays " + std::to_string(osr) + "/" + std::to_string(omr);
    });

    outputHDR = selectOutputImage(configuration);
    output = std::make_unique<Bitmap>(*outputHDR);

    ++frameCount;
    if (frameCount == 1)
    {
      Log.info([] { return "Saving ray-traced image ..."; });
      output->saveAsPNG("ray-traced.png");
      Log.info([] { return "ray-traced.png"; });
    }
  }

  std::shared_ptr<const HDRImage> RaytracerUI::selectOutputImage(const RaytraceConfiguration& configuration) const
  {
    const std::array outputs{configuration.image, configuration.timingMap, configuration.depthMap};
    return outputs.at(this->showMapIndex);
  }
}
