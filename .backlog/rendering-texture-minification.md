# Texture minification: mipmaps and ray footprints

## Motivation

`HDRImage::sampleBilinear` blends the four texels around a point. That suits magnification, where a texel covers
several pixels. Under minification many texels fall into one pixel (distant planes, small objects, textures scaled
up, curved mirrors), and the four texels a sample picks are an arbitrary part of that footprint: textures alias.
The Forms scene's checker floor and back wall and the Materials scene's checker backdrop sparkle and form moire at
a distance, and the Shading scene's mirror sphere minifies the St. Peter's light probe. Super-sampling only hides it,
at a high cost (`DONE/rendering-test-scene-findings.md`, 5.7: "Distant checker planes and mirror rims sparkle").

Goal: filter each texture lookup over the ray's footprint, so a pixel averages the texels it covers.

## Scope

### 1. Mipmaps

- `HDRImage` builds a mip chain once per image (2x2 box filter or better; odd sizes round down). The borders follow
  the `TextureWrap` of the lookup: `Repeat` wraps, `Clamp` and `ClampV` keep the edges. HDR values are filtered as they
  are (sRGB-linear), the extra memory is a third of the image.
- A trilinear lookup: the level from log2 of the footprint in texels, blending the two nearest levels. Anisotropic
  filtering (several samples along the footprint's major axis, or EWA) as a later step, for the grazing floors.

### 2. A footprint per ray

- Ray differentials (Igehy 1999): each ray carries the derivatives of its origin and direction across the pixel,
  propagated through reflection and refraction. At a hit they project onto the tangent plane and give the derivatives
  of the texture coordinates, which needs dP/du and dP/dv of the forms (sphere, box faces, plane, mesh facets).
- Or ray cones (Akenine-Moeller et al. 2019, "Texture Level of Detail Strategies for Real-Time Ray Tracing"): one
  spread angle per ray, widened by the surface curvature at reflections. Cheaper and isotropic.
- Decide how glossy reflection rays (`traceGlossyReflection`, their lobe width) and super-sampling (the sub-pixel
  spacing) widen the footprint.
- The footprint reaches the shaders through `FacetIntersection` / `SceneIntersection`, scaled by the object's texture
  transform like the texture coordinates themselves.

### 3. Environment maps

- The background lookup's footprint is the ray's spread, mapped through the projection's Jacobian
  (`environment_coordinates.h`). The pole of a latitude-longitude map and the rim of a mirror ball need care.

### 4. Measure

- Forms (checker floor, back wall), Materials (checker backdrop), Shading (mirror sphere, floor): compare against a
  heavily super-sampled reference, in the fast preview and the full render. Record the render time.

### 5. Constraints

- A switch to turn the filtering off (e.g. in `RaytraceBaseParameters`), which renders as today.
- Images stay deterministic.
- Tests: mip levels average their parent texels, the level selection, the footprint of a plane at a known distance
  and angle against its analytic value.

## Key files

- `sources/raytracing/include/raytracing/common/HDRImage.h`, `sources/raytracing/src/raytracing/common/HDRImage.cpp`
- `sources/raytracing/src/raytracing/shading/shaders/HDRImageShader.cpp`, `EnvironmentShader.cpp`,
  `environment_coordinates.cpp`
- `sources/raytracing/include/raytracing/geometry/FacetIntersection.h`, `SceneIntersection.h`
- `sources/raytracing/include/raytracing/shading/Raytrace.h` (per-ray state)
- `sources/raytracing/src/raytracing/RaytracerPackets.cpp`, `scene/Camera.cpp` (primary rays and their pixel spacing),
  `Raytracer.cpp` (secondary rays)
