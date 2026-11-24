#pragma once

#include <raytracing.h>

namespace raytracerui {
  using namespace vectorization;
  using namespace raytracer;

  // The seven material channels of a scene object as constants. brdf::applyBRDF combines them per hit:
  //   emittance + diffusion * (ambient radiance + diffuse irradiance / pi) of what the specular lobe leaves
  //   + specular lobe (GGX of the roughness: highlights, mirror or glossy reflection)
  //   + transmitted * (1 - its Fresnel) * (1 - diffusion)
  // - the specular lobe's Fresnel reflectance (brdf::SpecularFresnel) is Schlick's of the specular channel (F0) on an
  //   opaque surface, the indices' (refractionEta) on a transmissive one, times the reflectance channel as the lobe's
  //   weight (at most 1; 1 is physical, 0 turns the lobe off).
  // - roughness widens the highlights and the reflections alike (glossy above brdf::MirrorRoughness).
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

    // Conductor with the given reflectance at normal incidence (F0) as color, no diffusion
    static SceneMaterial metal(const Float4 &color, Float roughness);

    // Refracting medium entered from vacuum; transmittance is its absorption length per channel
    static SceneMaterial dielectric(Float refractionIndex, const Float4 &transmittance, Float roughness);

    // Self-lit surface
    static SceneMaterial emissive(const Float4 &color);

    void applyTo(SceneObject &object) const;
  };
}
