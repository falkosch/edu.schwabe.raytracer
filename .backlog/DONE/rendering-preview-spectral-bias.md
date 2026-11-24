# The fast preview's fixed wavelengths

## Motivation

Hero-wavelength sampling (Wilkie 2014) estimates each pixel's colour from 8 wavelengths per path, 50 nm apart
(`DELTA_LAMBDA`), shifted by a hero wavelength. It is an unbiased estimate of the spectral integral only while the
hero varies: then the wavelengths cover the visible range across the samples, and their error is noise that averages
out. The raytracer builds one `SpectralContext` per sub-sample and frame, shared by all pixels
(`createSpectralContexts` in `Raytracer.cpp`): the sub-samples of a pixel stratify one 50 nm step, which is fine from
2 x 2 sub-samples on (32 and more wavelengths, `DONE/rendering-distribution-noise.md`, section 8, "Hero wavelengths").

The fast preview has one sub-sample: every pixel of every preview takes the same 8 wavelengths (405, 455, ..., 755 nm).
That is a fixed quadrature, a systematic error rather than noise: it never averages out, and saturated colours come
out wrong in every pixel alike. Measured 2026-11-24 in the Cornell box (mean display values, without ambient light):
the red wall's red channel 154.0 in the preview against 145.1 from 2 x 2 sub-samples on (+6 %), the green wall's red
channel 20.9 against 54.1 (more saturated). Before the strata fix, the preview used 380, 430, ..., 730 nm and erred the
other way (with the Cornell box's ambient light of that day: red wall 138.6 against 153.7, green wall 77.8 against
59.0). Previews and full renders show different colours of the same scene, which the user noticed (super-sampling 2
brightened the red and green walls).

## Scope

### 1. Vary the hero across the preview's pixels

- With fewer than 8 sub-samples (`n`), each round takes 8 strata of the wavelength step in place of `n`: sub-sample `s`
  of a pixel takes the stratum `s * (8 / n) + p`, `p` from the pixel (a pattern that puts all `8 / n` offsets into
  every small neighbourhood, e.g. an interleaved or blue-noise index). Colour errors turn into per-pixel noise that a
  small neighbourhood averages; the denoiser smooths it in previews.
- The spectral contexts: 8 per round in place of `n` (built once per frame; cheap).

### 2. What depends on the shared contexts

- Irradiance caching (`IrradianceCache`): each spectral context has records of its own. With the contexts varying per
  pixel, a context's records would come from 1 in 8 pixels: either records for every context at each candidate
  (8 times their cost), or records whose irradiance spectrum another context can read (interpolated across the
  wavelength step from neighbouring records of other contexts).
- The denoiser's albedo guide (`PixelFeatures`): computed with the pixel's own wavelengths, it would vary from pixel
  to pixel on saturated surfaces and stop the filter there. Convert the diffusion with fixed wavelengths, or compare
  albedos by ratio.
- Adaptive sampling and full renders keep their strata (already enough wavelengths).

### 3. Constraints

- Whitted previews change too (they share the fixed wavelengths today).
- Images stay deterministic (the pattern is a function of the pixel).
- Measure: the preview's colours against the references (the Cornell box's red and green walls, the Materials chart)
  and its noise, with and without the denoiser and the cache.

### Resolution

Done 2026-11-24, for denoised images:

- **Pixel contexts** (`Raytracer.cpp`): with fewer than 8 sub-samples in a denoised image, each sub-sample's stratum
  splits into 8 / n (`contextsPerSubSample`), and a pixel takes one by Roberts' R2 sequence over the pixels
  (`pixelContext`: every 5 x 5 pixels, the denoiser's first pass, take all 8; every 3 x 3 at least 6). Context
  `round * n * perSubSample + s * perSubSample + c` is sub-sample s's context c; adaptive rounds take theirs the same
  way. Full renders (3 x 3 and more) keep one context per sub-sample: byte-identical to before.
- **Not where nothing denoises** (Whitted ray tracing, `denoise` off): a lattice ((x + 3 y) mod 8, all 8 in every
  3 x 3 pixels) showed as diagonal stripes on the Cornell box's red wall there, R2 as a fine checker; the UI enlarges
  the 512-pixel preview about 4 times. Those keep the fixed quadrature (the red wall's red 169.6 against 161.8).
- **Irradiance caching** (`IrradianceCache`): the contexts form sets, one per sub-sample, with records of their own.
  A record keeps its context (`IrradianceRecord::context`, by a hash of its pixel: the pixels' contexts would give a
  grid's pixels the same few), and a query of another context of the set takes it at its own wavelengths, linear
  between the record's (`spectral::shiftWavelengths`, the outer lanes held). The weighted sums are kept per context
  and shifted once per query; with one context per set nothing shifts.
- **Albedo guide**: the diffusion's RGB (`ObjectShader::sampleDiffusion`) in place of its spectrum at the pixel's
  wavelengths, the same for every context.
- Tests: `SpectrumTest::shiftWavelengthsInterpolatesBetweenNeighbouringLanes`,
  `IrradianceCacheTest::aSetsContextsReadEachOthersRecordsAtTheirWavelengths`,
  `RaytracerTest::denoisedPreviewPixelsVaryTheirWavelengths` (a saturated red's mean with 1 sub-sample within 0.5 % of
  4 x 4; undenoised, every pixel the same).

The Cornell box's walls, mean display values over 4 seeds (red, green, blue):

| Render                   | Red wall           | Green wall         |
|--------------------------|--------------------|--------------------|
| Reference                | 161.8, 23.5, 23.5  | 59.0, 132.3, 24.3  |
| Preview before           | 169.6, 26.8, 26.8  | 22.5, 129.9, 22.1  |
| Preview now              | 160.0, 22.7, 22.7  | 58.9, 130.0, 23.4  |
| Full render (unchanged)  | 160.6, 22.9, 23.0  | 57.1, 129.8, 23.3  |

The previews' RMS error against the references (`DONE/rendering-distribution-noise.md`, "Measuring"; before / now):
Cornell rough mirror 5.55 / 5.15, teapot 8.94 / 8.66, ceiling 4.38 / 4.14, back wall 2.66 / 2.09, box side
5.64 / 4.63; Showcase bunny 6.09 / 5.88, dragon 19.71 / 19.93, floor 6.87 / 7.11 (-4.7 -> -5.1 %); Materials metals
21.01 / 20.93; Lighting sphere 3.27 / 1.63, wall top 2.48 / 2.26, floor 2.96 / 1.08. The noise falls a little
(the albedo guide no longer stops the filter where the colours vary). The Cornell box's preview keeps its 9791
records; the root `CLAUDE.md` has its counters (since with the irradiance cache's half normal term, 9551 records).

## Key files

- `sources/raytracing/src/raytracing/Raytracer.cpp` (`createSpectralContexts`, `constructPackedRaytracesList`, the
  irradiance records)
- `sources/raytracing/src/raytracing/IrradianceCache.cpp`, `ATrousDenoiser.cpp`
- `sources/raytracing/src/raytracing/shading/spectral/wavelengths.cpp`

## Related backlog items

- `DONE/rendering-distribution-noise.md`, section 8: the strata fix, irradiance caching, the denoiser
- `illumination-spectral-docs.md`: documents hero sampling across sub-samples

## Commits

- `1d485c1` feat(raytracing): bounce diffuse light, cache irradiance, denoise previews
