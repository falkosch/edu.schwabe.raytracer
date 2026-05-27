#include "RaytracerUI.h"
#include "stdafx.h"

#include <logging.h>
#include <raytracing/shading/color/primaries.h>
#include <raytracing/shading/color/view_transform.h>
#include <string>

static const auto Log = logging::scope("UI");

namespace raytracerui
{
    RaytracerUI::RaytracerUI(Raytracer& raytracer, const RaytraceParameters& parameters, const Size2& fastPreviewSize)
        : parameters(parameters), screenSize(One<Size2>()), fastPreviewSize(fastPreviewSize), showMapIndex(),
          disableFastPreview(), frameCount(), outputHDR(), output(),
          initialViewMatrix(parameters.camera->getViewMatrix()), raytracer(&raytracer)
    {
        this->parameters.observer = this;
    }

    RaytracerUI::~RaytracerUI() = default;

    void RaytracerUI::reshape(const Size2& newSize)
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
        const auto clampedScreenSize = max(screenSize, One<Size2>());
        if (fastPreview && !disableFastPreview)
        {
            parameters.resolution = min(fastPreviewSize, clampedScreenSize);
        }
        else
        {
            parameters.resolution = clampedScreenSize;
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
        Log.info([kv = s.kdTreeNodesVisited, rom = s.sceneRootOverlapMisses]
        {
            return "kdTree nodesVisited=" + std::to_string(kv) + " rootOverlapMisses=" + std::to_string(rom);
        });

        outputHDR = selectOutputImage(configuration);

        if (showMapIndex == 0 && outputHDR)
        {
            static const auto acescgToSRGB = color::XYZToSRGBLinear() * color::ACEScgToXYZ();
            const auto resolution = outputHDR->getResolution();
            const auto count = static_cast<int>(x(resolution) * y(resolution));
            const auto displayImage = std::make_shared<HDRImage>(resolution);
#pragma omp parallel for
            for (int i = 0; i < count; ++i)
            {
                const auto pixel = acescgToSRGB * (*outputHDR)[static_cast<ASizeT>(i)];
                displayImage->getData()[i] = color::srgbEncode(color::agx(pixel));
            }
            outputHDR = displayImage;
        }

        output = std::make_unique<Bitmap>(*outputHDR);

        ++frameCount;
        if (frameCount == 1)
        {
            Log.info([] { return "Saving ray-traced image ..."; });
            const auto res = output->saveAsPNG("ray-traced.png");
            Log.info([res] { return "ray-traced.png saved: " + std::to_string(res); });
        }
    }

    std::shared_ptr<const HDRImage> RaytracerUI::selectOutputImage(const RaytraceConfiguration& configuration) const
    {
        const std::array outputs{configuration.image, configuration.timingMap, configuration.depthMap};
        return outputs.at(this->showMapIndex);
    }
}
