#pragma once

#include <vectorization.h>

namespace raytracer {
  using namespace vectorization;

  // forward declaration
  class SceneShader;

  /*
   * Struct for the fixed raytrace parameters.
   */
  struct RaytraceBaseParameters {
    // parameters used within raytracing
    Float visibilityCutoff;
    Float maxDistance;
    ASizeT maxTraceDepth;
    SceneShader *sceneShader;
    Int cullingOrientation;

    // Distribution ray tracing: the most reflection rays of a glossy surface per hit, and the most shadow rays to an
    // area light (a sphere or box light) per intersection. Fewer are traced the less they add to the pixel. 0 is
    // Whitted ray tracing: one mirror ray (rough surfaces keep their highlights), one shadow ray to the area light's
    // nearest point (hard shadows).
    ASizeT glossySamples;
    ASizeT sphereLightSamples;

    // Diffuse interreflection (one bounce): the most rays a diffuse hit gathers the light of other surfaces with,
    // fewer the less it adds to the pixel. They replace the ambient light at that hit; their own hits keep it. 0 is
    // off: the ambient light stands in for the interreflection everywhere.
    ASizeT diffuseSamples;

    // Many-light sampling at the hits of diffuse rays (and further along their paths): the most point lights (and
    // sphere lights without shadow rays) whose shadow rays such a hit traces. With more lights, it picks that many by
    // their unshadowed diffuse light, unbiased (SceneShader::sampleLighting). 0 takes every light.
    ASizeT manyLightSamples;

    // Picks the sample pattern of the distribution ray tracing: 0 is the default, other values render the same image
    // with other samples (e.g. to measure its noise). Whitted ray tracing doesn't sample and ignores it.
    UInt_32 seedOffset;

    // The most radiance one glossy reflection ray or diffuse ray adds: a brighter one is scaled down to it, keeping its
    // colour. It keeps single rays that find a small, bright spot (a light through a mirror or glass) from dominating
    // their pixel ("fireflies"), but biases: very bright glossy reflections and interreflections get darker
    // (raytracerui's 4 darkens caustics and glossy reflections of lights by up to 6 percentage points more than
    // unclamped). 0 is off.
    Float glossyClamp;

    // After a glossy reflection or a diffuse ray, the mirror reflections and refractions further along the path don't
    // show the emissive objects of area lights either (Raytrace::glossy carries on). It removes the glitter of glossy
    // -> glass or mirror -> light paths and the speckles of diffuse -> glass or mirror -> light paths (caustics), but
    // biases: the light along them goes missing.
    bool regularizePaths;

    // Adaptive sampling: after every pixel's first round of sub-samples, a pixel whose neighbours' sub-samples spread
    // so much that the standard error of a mean of theirs exceeds adaptiveThreshold takes further rounds, as many as
    // bring that error below it (k rounds divide it by sqrt(k + 1)), up to adaptiveRounds; each on the grid shifted
    // within its cells and at hero wavelengths shifted within their strata. The neighbours judge a pixel, not its own
    // sub-samples: those would bias it (AdaptiveRounds). The spread is that of the sub-samples' luminance in display
    // values (y / (1 + y), encoded with the exponent 1 / 2.2), so 1/255 is about one step of an 8-bit image.
    // 0 rounds is off; a pixel needs 2 x 2 sub-samples at least, and Whitted ray tracing (no glossy, area light and
    // diffuse samples) doesn't sample adaptively.
    ASizeT adaptiveRounds;
    Float adaptiveThreshold;

    // Denoises the image once rendered (ATrousDenoiser), guided by the normal, depth and diffuse albedo of each pixel's
    // first hit that isn't a smooth mirror or glass (PixelFeatures). It blurs noise within surfaces, not across their
    // edges, but biases: fine detail of the lighting (sharp shadows and highlights, reflected detail) blurs too.
    // Whitted ray tracing doesn't sample and isn't denoised.
    bool denoise;

    // Irradiance caching (IrradianceCache, Ward et al. 1988): before the pixels, records of the irradiance that
    // diffuse rays gather, 16 times diffuseSamples rays each (at least 256, each limited to 16 times their mean), at
    // primary hits on grids of 32 down to 4 pixels where the records don't cover them fully yet, per sub-sample of the
    // first round (each has its own wavelengths); their radii clamped by their neighbours'. Diffuse hits the records
    // cover fully take their irradiance instead of tracing diffuse rays, partly covered ones blend it with their rays'
    // by the coverage; the others and the further rounds of adaptive sampling trace them. Interpolation errors are
    // smooth; rare bright paths (caustics through mirrors and glass, highlights) are left to the hits' own rays.
    bool irradianceCaching;

    RaytraceBaseParameters();
  };
}
