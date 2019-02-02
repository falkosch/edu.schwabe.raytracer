#include "RaytracerUI.h"
#include "stdafx.h"

#include <iostream>

namespace raytracerui
{

	RaytracerUI::RaytracerUI(
		Raytracer & raytracerIn,
		const RaytraceParameters & parametersIn,
		const ASizeT & fastPreviewSizeIn)
		:
		parameters(parametersIn),
		screenSize(One<Int2>()),
		fastPreviewSize(fastPreviewSizeIn),
		showMapIndex(),
		disableFastPreview(),
		outputHDR(),
		output(),
		raytracer(&raytracerIn)
	{
		this->parameters.observer = this;
	}

	RaytracerUI::~RaytracerUI()
	{
		if (outputHDR)
		{
			delete outputHDR;
			outputHDR = nullptr;
		}
		if (output)
		{
			delete output;
			output = nullptr;
		}
	}

	void RaytracerUI::reshape(const Int2 & newSize)
	{
		std::cout << "Resizing output to " << x(newSize) << "x" << y(newSize) << std::endl;
		screenSize = newSize;
		parameters.camera->setProjection(
			parameters.perspectiveFOV,
			convert<Float2>(newSize),
			parameters.perspectiveZPlanes
		);
		triggerRaytracing(true);
	}

	void RaytracerUI::triggerRaytracing(const bool fastPreview)
	{
		const Int2 clampedSize = max(screenSize, One<Int2>());
		if (fastPreview & !disableFastPreview) {
			parameters.resolution = Size2(fastPreviewSize, (fastPreviewSize * y(clampedSize)) / x(clampedSize));
		}
		else {
			parameters.resolution = convert<Size2>(clampedSize);
		}
		raytracer->trigger(parameters);
	}

	void RaytracerUI::notifyUpdate(const RaytraceConfiguration & configuration)
	{
		std::cout << "primaryRays " << configuration.statistics.primaryRays << "/" << configuration.statistics.missedPrimaryRays << std::endl;
		std::cout << "secondaryRays " << configuration.statistics.secondaryRays << "/" << configuration.statistics.missedSecondaryRays << std::endl;
		std::cout << "shadowRays " << configuration.statistics.shadowRays << "/" << configuration.statistics.missedShadowRays << std::endl;

		if (outputHDR) delete outputHDR;
		if (output) delete output;

		outputHDR = selectOutputImage(configuration);
		output = new Bitmap(*outputHDR);
		if (outputHDR != configuration.image) delete configuration.image;
		if (outputHDR != configuration.depthMap && configuration.depthMap) delete configuration.depthMap;
		if (outputHDR != configuration.timingMap && configuration.timingMap) delete configuration.timingMap;
	}

	const HDRImage*const RaytracerUI::selectOutputImage(const RaytraceConfiguration & configuration) const
	{
		switch (showMapIndex)
		{
		default: case static_cast<ASizeT>(0) :
			return configuration.image;
			case static_cast<ASizeT>(1) :
				return configuration.timingMap;
				case static_cast<ASizeT>(2) :
					return configuration.depthMap;
		}
	}

}
