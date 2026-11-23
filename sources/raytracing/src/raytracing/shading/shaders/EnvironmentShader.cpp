#include "raytracing/shading/shaders/EnvironmentShader.h"
#include "../../../stdafx.h"

#include "raytracing/shading/SpectralContext.h"
#include "raytracing/shading/shaders/environment_coordinates.h"

namespace raytracer
{
    EnvironmentShader::EnvironmentShader(
        const HDRImage& imageIn, const EnvironmentProjection projectionIn, const Float horizonRadiusIn
    )
        : environmentImage(&imageIn), projection(projectionIn), horizonRadius(horizonRadiusIn),
          // A latitude-longitude map's rows end at the poles, the other projections fill a disc that ends at the
          // image's border
          wrap(projectionIn == EnvironmentProjection::LatitudeLongitude ? TextureWrap::ClampV : TextureWrap::Clamp)
    {
    }

    EnvironmentShader::~EnvironmentShader() = default;

    spectral::Spectrum EnvironmentShader::sample(const SceneShader&, const BackgroundQuery& query) const
    {
        const Float4 rgbPixel =
            environmentImage->sampleBilinear(environmentCoordinates(projection, query.direction, horizonRadius), wrap);
        return query.spectralContext.fromRGB(rgbPixel);
    }
}
