# Enabler: introduce a named RGBS type for illumination values

## Sequencing

- **Type:** enabler (pre-enabler — all illumination improvement items depend on this)
- **Level:** 0 (prerequisite)
- **Order:** 0
- **Depends on:** *(none)*
- **Blocks:** `illumination-rgb-linear-pipeline.md`,
  `illumination-unpolarized-spectral.md`,
  `illumination-polarized-spectral.md`,
  `illumination-wave-optics.md`

## Motivation

Illumination values throughout the raytracer are currently bare `Float4` — the same type
used for positions, normals, directions, material parameters, and everything else. There is
no type-level distinction between "this Float4 carries radiance (RGB + shared scale)" and
"this Float4 is a surface normal". Introducing a named `RGBS` wrapper creates a single
semantic seam that later levels can swap (RGBS → Spectrum → PolarizedSpectrum) without
hunting for which `Float4` fields carry illumination.

This is a pure refactoring enabler: the new type wraps `Float4` with identical layout and
arithmetic, so there is no behavioral or performance change.

## Storage layout

```cpp
// sources/raytracing/include/raytracing/common/RGBS.h
//
// R, G, B = color channels;  S = shared scale (W component of the underlying Float4)
// Layout is identical to Float4 (16 B, SSE-aligned).
struct RGBS {
    Float4 value;

    // Implicit or explicit conversion from/to Float4 so existing call sites compile
    // unchanged during the incremental rollout.
    // ...arithmetic operators forwarding to Float4 ops...
};
```

The wrapper should be thin enough that the compiler elides it entirely. Start with implicit
`Float4 → RGBS` conversion to keep the diff small; tighten to explicit once all call sites
are migrated.

## Scope

### 1. Define `RGBS` type (new header + source)

- `sources/raytracing/include/raytracing/common/RGBS.h`
- Arithmetic: `+`, `-`, `*`, `*=` (element-wise), scalar multiply. Forward to `Float4`.
- Named constructors: `RGBS::black()`, `RGBS::white()`, `RGBS(r, g, b, s)`.
- Accessor: `.rgb()` (returns `Float4` with W zeroed), `.scale()` (returns W).

### 2. Replace `Float4` with `RGBS` in illumination-carrying fields

The following fields carry radiance, color, or spectral coefficients and should become
`RGBS`. Fields that carry geometric or scalar parameters (shininess, refractionEta,
transmittance coefficient, depth, directions) stay as `Float4`.

| Type                            | Field(s) → RGBS                                              | Stays Float4                                                    |
|---------------------------------|--------------------------------------------------------------|-----------------------------------------------------------------|
| `SurfaceShading`                | `diffusion`, `reflectance`, `specular`, `emittance`          | `shininess`, `transmittance`, `refractionEta`                   |
| `LightShading`                  | `ambient`, `diffuse`, `specular`, `reflected`, `transmitted` | —                                                               |
| `BRDFParameters`                | `reflectanceCoefficient`, `fractionTransmitted`              | `transmissionDirection` (Float4), `viewDistance` (Float scalar) |
| `IlluminatedIntersection`       | `color`                                                      | `depth`                                                         |
| `LightInfo`                     | `emittance`, `glare`                                         | `position`, `attenuationFactors`                                |
| `SceneShader`                   | `ambientLight`                                               | —                                                               |
| `Raytracer::applyBRDF`          | return type                                                  | —                                                               |
| `Shader<..., Float4>` outputs   | color/radiance outputs                                       | non-color outputs                                               |
| `SceneShader::sampleBackground` | return type                                                  | —                                                               |

**Note on `MaterialShader` typedef:** `ObjectShader` defines a single typedef
`Shader<ObjectShader, FacetIntersection, Float4>` shared by all 7 material shader slots.
During the incremental rollout with implicit `Float4 → RGBS` conversion, this typedef stays
as-is: shaders keep returning `Float4`, and conversion happens at the assignment boundary in
`ObjectShader::sample()`. Once all call sites are migrated, split into `ColorMaterialShader`
(for diffusion, reflectance, specular, emittance) and `ParameterMaterialShader` (for
shininess, transmittance, refractionEta). `IntersectionNormalShader` outputs normals, not
radiance — it must use `ParameterMaterialShader`.

**Note on `refractionEta`:** stays `Float4` for a layout reason — its 4 components pack
`(η_incident, η_transmitted, η_prev_incident, η_prev_transmitted)`, used via component
swizzles (`xxxx`, `zzzz`, `wwww`) in `schlickFresnel`. This is a packed parameter encoding,
not an XYZW or RGBS pattern.

### 3. Update call sites

Every function that reads or writes the fields above must accept/return `RGBS` instead of
`Float4`. With implicit conversion the initial diff is small — most call sites compile
unchanged. Tighten conversion boundaries incrementally.

Key files to touch:

- `sources/raytracing/include/raytracing/shading/SurfaceShading.h`
- `sources/raytracing/include/raytracing/shading/LightShading.h`
- `sources/raytracing/include/raytracing/shading/LightInfo.h` (`emittance`, `glare`)
- `sources/raytracing/include/raytracing/shading/SceneShader.h` (`ambientLight`)
- `sources/raytracing/include/raytracing/shading/ObjectShader.h` (MaterialShader typedef)
- `sources/raytracing/include/raytracing/BRDFParameters.h`
- `sources/raytracing/include/raytracing/IlluminatedIntersection.h`
- `sources/raytracing/include/raytracing/Raytracer.h` (`applyBRDF` signature)
- `sources/raytracing/src/raytracing/Raytracer.cpp` (`applyBRDF`, `schlickFresnel`,
  `fresnelReflectance`, shading accumulation)
- `sources/raytracing/src/raytracing/shading/SceneShader.cpp` (lighting accumulation)
- `sources/raytracing/src/raytracing/shading/ObjectShader.cpp` (material sampling)
- `sources/raytracing/include/raytracing/shading/shaders/*.h` (EnvironmentShader,
  HDRImageShader — return types)

### 4. Tests

- `sizeof(RGBS) == sizeof(Float4)` — no layout overhead.
- Arithmetic: `RGBS(1,2,3,1) + RGBS(4,5,6,1) == RGBS(5,7,9,2)` etc.
- Round-trip: `RGBS → Float4 → RGBS` is identity.
- Existing `vectorization.native-test` and `raytracing.native-test` suites pass unchanged
  (the wrapper is transparent).

## Non-degradation guarantee

- `RGBS` has identical layout and codegen as `Float4` — no register pressure, no extra
  copies, no alignment change.
- Regression baseline (0.07s on teapot, ray counters) must remain unchanged.
- No new runtime cost; the type boundary is compile-time only.

## Key files (new)

- `sources/raytracing/include/raytracing/common/RGBS.h`
- `sources/raytracing/src/raytracing/common/RGBS.cpp`
- `tests/raytracing.native-test/src/raytracing/common/RGBS_test.cpp`
