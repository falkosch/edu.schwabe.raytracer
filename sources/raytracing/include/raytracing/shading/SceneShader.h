#pragma once

#include "../geometry/SceneGeometry.h"

#include "BackgroundQuery.h"
#include "LightInfo.h"
#include "LightShading.h"
#include "PerLightShadowCache.h"
#include "SceneShaderContainment.h"
#include "Shader.h"

#include <vector>

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class SceneShader : public SceneGeometry, public Shader<SceneShaderContainment, SceneIntersection, LightShading>
    {
    public:
        using BackgroundShader = Shader<SceneShader, BackgroundQuery, spectral::Spectrum>;
        using LightsCollection = std::vector<std::unique_ptr<LightInfo>>;

        SceneShader();

        explicit SceneShader(std::unique_ptr<const KDTreeTraverser<SceneIntersection>> treeTraverser);

        ~SceneShader() override;

        const BackgroundShader* getBackgroundShader() const;
        void setBackgroundShader(std::unique_ptr<const BackgroundShader> value);

        RGBS getAmbientLight() const;
        void setAmbientLight(const RGBS& value);

        const LightsCollection& getLights() const;
        LightsCollection& getLights();

        // Samples the lighting of a facet in the scene.
        spectral::Spectrum sampleBackground(const Float4& rayDirection, Float heroLambda) const;

        // Samples the lighting of a facet in the scene.
        LightShading
        sample(const SceneShaderContainment& containment, const SceneIntersection& intersection) const override;

        // Computes the lighting of a facet in the scene.
        LightShading sampleLighting(
            const Raytrace& incidentRay, const Float4& adaptedVisibilityCutoff,
            Float roughness, const spectral::SpectralVector& F0,
            const SceneIntersection& intersection, PerLightShadowCache::ShadowCacheType& shadowCache,
            StatisticsCookie& statistics
        ) const;

        // Scale visibilityCutoff according to the current visibilityIndex, which allows us to filter out unnecessary
        // lighting that doesn't add visible changes to the scene.
        static Float4 adaptedVisibilityCutoff(Float visibilityCutoff, Float visibilityIndex);

    private:
        std::unique_ptr<const BackgroundShader> backgroundShader;

        RGBS ambientLight;
        LightsCollection lights;

        Float4 computeLitAreaFraction(
            const RayCast& shadowRay, const SceneIntersection& intersection, ASizeT lightIndex,
            PerLightShadowCache::ShadowCacheType& shadowCache, StatisticsCookie& statistics
        ) const;

        // Calculates the diffuse intensity according to Lambert's cosine law (Lambertian reflection). Vector
        // lightDirection is the direction from the facet to the light source. Vector normal is the normal-vector
        // at the facet.
        static Float4 lambertDiffuseIntensity(const Float4& lightDirection, const Float4& normal);

        // Calculates the attenuated diffuse intensity. Vector attenuationFactors contain the light attenuation
        // over distance in empty space-based parameters with z-component as quadratic (Inverse-square-law; generally
        // for ideal point lights), y-component as linear (for spotlights) and x-component as a constant (planar lights)
        // falloff factor.
        static Float4 attenuateDiffuseIntensity(
            const Float4& attenuationFactors, const Float4& lightDistance, const Float4& diffuseIntensity
        );

        // Calculates the specular intensity according to Phong's lighting model but assumes a pre-given incident
        // vector that is the reflected vector of the incident vector. Vector lightDirection is the direction from
        // the facet to the light source. Vector shininess determines the smoothness of the facet.
        static Float4 phongSpecularIntensityPerReflectedIncident(
            const Float4& reflectedIncidentDirection, const Float4& lightDirection, const Float4& shininess
        );
    };
}
