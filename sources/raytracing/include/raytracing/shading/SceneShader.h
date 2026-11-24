#pragma once

#include "../geometry/SceneGeometry.h"

#include "BackgroundQuery.h"
#include "LightInfo.h"
#include "LightShading.h"
#include "PerLightShadowCache.h"
#include "SceneShaderContainment.h"
#include "Shader.h"
#include "SpectralContext.h"

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

        // Precomputes the spectral data of a hero wavelength, including this scene's ambient light and light spectra.
        SpectralContext createSpectralContext(Float heroLambda) const;

        // Samples the lighting of a facet in the scene.
        spectral::Spectrum sampleBackground(const Float4& rayDirection, const SpectralContext& spectralContext) const;

        // Samples the lighting of a facet in the scene.
        LightShading
        sample(const SceneShaderContainment& containment, const SceneIntersection& intersection) const override;

        // Computes the lighting of a facet in the scene, with up to sphereLightSamples shadow rays to an area light (a
        // sphere or box light). glossyRays is the number of glossy reflection rays the intersection traces (0 for
        // none): with shadow rays, the highlights of area lights weigh them against those rays by multiple importance
        // sampling, which add the light where they find the emitter (Raytracer::traceGlossyReflection).
        LightShading sampleLighting(
            const Raytrace& incidentRay, const Float4& adaptedVisibilityCutoff,
            Float roughness, const spectral::SpectralVector& F0,
            const SceneIntersection& intersection, ASizeT sphereLightSamples, ASizeT glossyRays,
            PerLightShadowCache::ShadowCacheType& shadowCache, StatisticsCookie& statistics
        ) const;

        // Scale visibilityCutoff by the ray's importance (Raytrace::importance), which allows us to filter out
        // unnecessary lighting that doesn't add visible changes to the scene.
        static Float4 adaptedVisibilityCutoff(Float visibilityCutoff, Float importance);

        // The number of shadow rays to an area light: up to maxSamples, fewer the less the intersection adds to the
        // pixel (the incident ray's visibility index)
        static ASizeT areaLightSamples(ASizeT maxSamples, Float visibilityIndex);

        // How sampleLighting's shadow rays estimate the highlight of an area light towards one of its points (toPoint,
        // relative to the intersection), for a glossy reflection ray that found the light's emitter there:
        // - emits: whether the point is the light's, on its sphere or on one of its faces that emit towards the
        //   intersection (an emitter that doesn't match its light is no light)
        // - pdf: the density per solid angle of the shadow rays' directions there, 0 where sampleLighting doesn't
        //   sample the light's highlight (a sphere whose center is below the horizon, a light below the cutoff, an
        //   intersection seen from inside its object, which gets no highlight)
        // - radiance: the light's radiance as the shadow rays take it
        struct LightPointDensity
        {
            bool emits;
            Float pdf;
            spectral::Spectrum radiance;
        };

        LightPointDensity lightPointDensity(
            const LightInfo& light, const Raytrace& incidentRay, const Float4& adaptedVisibilityCutoff,
            const SceneIntersection& intersection, const Float4& toPoint
        ) const;

    private:
        std::unique_ptr<const BackgroundShader> backgroundShader;

        RGBS ambientLight;
        LightsCollection lights;

        // The fraction of the light that reaches the intersection along the shadow ray, per wavelength
        spectral::SpectralVector computeLitAreaFraction(
            const RayCast& shadowRay, const SceneIntersection& intersection, ASizeT lightIndex,
            const SpectralContext& spectralContext, PerLightShadowCache::ShadowCacheType& shadowCache,
            StatisticsCookie& statistics
        ) const;

        // The lit fraction along one shadow ray to the center of the cap of a sphere light (LightInfo::radius) that is
        // visible from the intersection: a hard shadow, without samples
        spectral::SpectralVector computeLitSphereFraction(
            RayCast shadowRay, const SceneIntersection& intersection, ASizeT lightIndex, const LightInfo& light,
            const SpectralContext& spectralContext, PerLightShadowCache::ShadowCacheType& shadowCache,
            StatisticsCookie& statistics
        ) const;

        // Adds the lighting of a sphere light (LightInfo::radius) to lighting, with up to maxSamples shadow rays
        // (SphereLightCap): the light from its center (an intensity with its attenuation, exact while the sphere is
        // above the horizon) times the mean lit fraction of the rays. Rough surfaces get its highlight from the same
        // rays, the GGX BRDF towards their points with the sphere's radiance (light sampling, the intensity spread
        // over the sphere's cross section), weighed against the glossyRays by the power heuristic.
        void sampleSphereLight(
            const Raytrace& incidentRay, const Float4& adaptedVisibilityCutoff, Float roughness,
            const spectral::SpectralVector& F0, const SceneIntersection& intersection, ASizeT lightIndex,
            const LightInfo& light, ASizeT maxSamples, ASizeT glossyRays,
            PerLightShadowCache::ShadowCacheType& shadowCache, StatisticsCookie& statistics, LightShading& lighting
        ) const;

        // Adds the lighting of a box light (LightInfo::halfExtents) to lighting: the exact irradiance of its faces that
        // the intersection sees (BoxLightFaces) times their lit fraction, from up to maxSamples shadow rays to points
        // spread evenly over their area, fewer the less the intersection adds to the pixel. The lit fraction weighs
        // each ray by the irradiance from its point (the ratio estimator of Heitz et al. 2018), so only penumbrae are
        // noisy. Rough surfaces get the light's highlight from the same rays: the GGX BRDF towards their points, lit
        // ones only, averaged over the visible area (light sampling: no bias, exact shadows), weighed against the
        // glossyRays by the power heuristic. With 0, one ray to the faces' nearest point (a hard shadow), and each
        // face's highlight from its representative point (Karis 2013), the lobe widened by the face's size: 0.26 to
        // 1.8 times the integrated BRDF for large faces.
        void sampleBoxLight(
            const Raytrace& incidentRay, const Float4& adaptedVisibilityCutoff, Float roughness,
            const spectral::SpectralVector& F0, const SceneIntersection& intersection, ASizeT lightIndex,
            const LightInfo& light, ASizeT maxSamples, ASizeT glossyRays,
            PerLightShadowCache::ShadowCacheType& shadowCache, StatisticsCookie& statistics, LightShading& lighting
        ) const;

        // Follows a shadow ray through transmissive objects up to the light: each object lets the Fresnel transmission
        // of its entry and exit pass (Schlick) and absorbs along the straight chord between them (Beer-Lambert). The
        // ray isn't refracted, so there are no caustics. An opaque object blocks the light.
        spectral::SpectralVector transmitShadowRay(
            const RayCast& shadowRay, const SceneIntersection& intersection, ASizeT lightIndex,
            const SpectralContext& spectralContext, PerLightShadowCache::ShadowCacheType& shadowCache
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
    };
}
