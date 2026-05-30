# Dispersion at refractive surfaces

## Sequencing

- **Type:** extension
- **Level:** 2 (unpolarized spectral)
- **Depends on:** Level 2 unpolarized spectral pipeline (shipped)
- **Blocks:** nothing

## Motivation

`Raytracer::traceTransmission` refracts the entire ray with a single scalar `refractionEta`,
so all 8 wavelengths follow the same refracted direction. Glass refracts but never
disperses; the canonical glass-prism rainbow is impossible to produce.

The acceptance criterion from the original unpolarized-spectral implementation —
*"A glass prism scene shows visible color separation (rainbow)"* — is unmet.

## Why this isn't trivial

The original unpolarized-spectral implementation §5 proposed a Monte Carlo trick: at refraction,
evaluate η(λ_hero), zero the other 7 spectral lanes, multiply the hero lane by 8 to preserve
the expected value of the MC estimator. Pixel-sample averaging reabsorbs the variance.

This only works in a renderer that **accumulates many samples per pixel**. The current
Whitted raytracer renders deterministically: 1 ray per pixel (or N stratified sub-samples
under `superSamplingFactor`, but N is small). With small N the "×8 hero lane" produces
extremely noisy refracted regions — the variance has nowhere to go.

## Options

### A. Spawn per-wavelength sub-rays at refraction (deterministic, costly)

At every refraction event, spawn 8 child rays, one per wavelength, each with its own
η(λ_k) and refracted direction. Accumulate the resulting per-λ radiance into the
corresponding spectrum lane.

- **Pro:** deterministic, no variance, single-sample pixel still renders a correct prism.
- **Con:** 8× ray count on refractive paths. For deeply nested refractive geometry
  (e.g. multi-bounce inside a glass mesh), cost compounds. KD-tree traversal is the
  dominant cost; this multiplies it on the affected paths.
- **Mitigation:** Hero-only refraction below a configurable visibility-index threshold so
  deep paths don't keep fanning out. Or cap recursion depth specifically for dispersive
  refraction (a separate counter from `maxTraceDepth`).

### B. Adopt path tracing / MC averaging (large change)

Switch the integrator from Whitted to a multi-sample path tracer with sufficient samples
per pixel for the ×8 hero trick to converge. Out of scope for this item, and explicitly
listed as "Out of scope" in the original unpolarized-spectral implementation.

### C. Status quo: no dispersion

Document the limitation and live without prism rainbows. The rest of the spectral pipeline
delivers the bulk of its value (metals, light-source character, monochromatic sources)
without dispersion.

## Recommended approach

**Option A**, deterministic per-wavelength sub-ray spawning, with a tunable cutoff so
recursive refractive paths don't explode. The 8× cost is bounded and predictable;
profile-driven cutoffs can keep regression scenes within budget while still rendering
single-refraction prisms correctly.

If profiling shows the cost is unacceptable even with cutoffs, fall back to Option C
and revisit when Option B becomes viable.

## Scope (when implemented)

- Sellmeier dispersion formula in `RefractionIndices.h` for common glass types (BK7,
  flint, dense flint) — produces η(λ) from coefficients.
- `refractionEtaShader` returns a `Float8` of per-λ η values instead of a scalar
  `Float4`.
- `Raytracer::traceTransmission` spawns up to 8 sub-rays, one per wavelength lane that
  passes a per-lane visibility/depth cutoff.
- Each sub-ray carries a single-lane Spectrum (other lanes zero) and accumulates back into
  the parent's transmitted Spectrum at the original lane index.
- Sellmeier coefficients for BK7 at minimum (the prism standard).

## Tests

- Glass prism scene (white light, wedge of glass) produces a visible RGB spread on the
  far wall — quantitatively, integrated radiance has measurably different chromaticities
  between left and right edges of the refracted band.
- Regression baseline (teapot in CornellBox) stays within ≤ 1.3× of pre-dispersion runtime
  (1 refraction event per primary ray inside the teapot at most).

## Acceptance criteria

- Glass prism visibly disperses white light into a rainbow.
- BK7 Sellmeier η at 656nm (red C-line) lower than at 486nm (blue F-line) by the textbook
  Abbe-number margin.
- No regression in non-refractive scenes (CornellBox without glass teapot, all other test
  scenes).

## Out of scope

- Path-tracing-based MC dispersion (Option B above).
- Wavelength-dependent absorption inside glass (Beer-Lambert already works per-λ; this
  item is purely about the refraction direction).
- Dispersion in non-glass materials (water, diamond — same machinery, just different
  Sellmeier coefficients; add as needed).

## Key files (when implemented)

- `sources/raytracing/include/raytracing/common/RefractionIndices.h` — add Sellmeier
- `sources/raytracing/src/raytracing/Raytracer.cpp` — per-λ refraction loop in
  `traceTransmission`
- `sources/raytracing/include/raytracing/shading/ObjectShader.h` — `refractionEtaShader`
  output type widens to `Float8`
- `sources/raytracerui/src/tests/PrismScene.cpp` (new) — regression scene for dispersion
