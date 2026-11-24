#pragma once

#include <raytracing.h>

namespace raytracerui {
  using namespace vectorization;
  using namespace raytracer;

  // The seven material channels of a scene object as constants. brdf::applyBRDF combines them per hit:
  //   emittance + diffusion * (ambient radiance + diffuse irradiance / pi) + GGX highlights (F0 = specular, roughness)
  //   + mix(transmitted, reflectance * mirrored ray, Fresnel)
  // - roughness widens the highlights only; the mirrored ray stays sharp.
  // - transmittance is the absorption length per channel (exp(-depth / transmittance)); zero is opaque.
  // - emittance colors the object itself but doesn't light other objects.
  struct SceneMaterial {
    Float4 diffusion;
    Float4 reflectance;
    Float4 specular;
    Float4 roughness;
    Float4 transmittance;
    Float4 refractionEta;
    Float4 emittance;

    // Lambertian surface without highlights
    static SceneMaterial matte(const Float4 &color);

    // Diffuse color under a clear coat: highlights and grazing reflections with the given F0
    static SceneMaterial plastic(const Float4 &color, Float specular, Float roughness);

    // Mirror with the given reflectance (F0) as color, no diffusion
    static SceneMaterial metal(const Float4 &color, Float roughness);

    // Refracting medium entered from vacuum; transmittance is its absorption length per channel
    static SceneMaterial dielectric(Float refractionIndex, const Float4 &transmittance, Float roughness);

    // Self-lit surface
    static SceneMaterial emissive(const Float4 &color);

    void applyTo(SceneObject &object) const;
  };
}
