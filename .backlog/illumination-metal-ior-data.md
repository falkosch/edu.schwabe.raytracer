# Real metal IOR n(λ)/k(λ) tables

## Sequencing

- **Type:** extension
- **Level:** 2 (unpolarized spectral)
- **Depends on:** Level 2 unpolarized spectral pipeline (shipped — per-λ Schlick-Fresnel
  dispatch over `MetalConstants` already in place)
- **Blocks:** nothing

## Motivation

`sources/raytracing/src/raytracing/shading/spectral/metal_ior.cpp` currently returns
wavelength-independent placeholder values per metal:

```cpp
case Metal::Cu: return {0.27f, 3.60f};
case Metal::Au: return {0.18f, 3.00f};
...
```

With placeholders, all 8 spectral lanes get the same `(n, k)`, so the per-λ Schlick-Fresnel
collapses to a single Fresnel value. Gold and copper end up looking like neutral mirrors
with no characteristic warm tint. The spectral pipeline is in place; only the data is
missing.

The acceptance criterion from the original unpolarized-spectral implementation —
*"Gold (Au) and copper (Cu) metals look distinctly yellow and reddish-pink without any
manual color tweaking — purely from the IOR tables"* — is unmet until real data is loaded.

## Scope

Replace the placeholder constants with real wavelength-dependent n(λ)/k(λ) for the existing
`Metal` enum (Cu, Au, Al, Fe, Ag), tabulated at 5nm step over 380–780nm, looked up via
linear interpolation — same pattern as `cmf.cpp` and `illuminants.cpp`.

**Data source:** Mitsuba's [`mitsuba-data/ior/`](https://github.com/mitsuba-renderer/mitsuba-data)
repository (BSD 3-Clause). Files like `Au.eta.spd`, `Au.k.spd` contain wavelength-value
pairs from published measurements:

- Cu/Au/Ag: Johnson & Christy 1972
- Al: Rakic 1998
- Fe: Johnson & Christy 1974 or similar

Convert to `constexpr Float[81]` arrays at 5nm step. No external runtime data files;
compile-time embed only.

## Implementation notes

- Drop the now-unused `wavelengths` parameter omission: with real tables, the parameter
  is consumed via lookup.
- Keep the `MetalConstants` placeholder path removed; replace `metalIOR()` with a per-metal
  table lookup that broadcasts to all 8 lanes.

## Tests

- `metalIOR(Au, wavelengthsAt(589))` returns n ≈ 0.18, k ≈ 3.07 (Johnson & Christy 1972 value)
  within 1% tolerance.
- `metalIOR(Cu, wavelengthsAt(580))` has lower n than at 450 (copper's characteristic dip
  in the green-yellow region).
- Schlick-Fresnel at normal incidence applied to gold yields a reflectance spectrum that
  integrates to a yellow chromaticity (Y > 0.5, x > 0.4, y > 0.4 in xyY).

## Out of scope

- Adding new metals beyond the existing enum (Cu, Au, Al, Fe, Ag). Easy follow-up if needed.
- Complex-IOR-aware Fresnel formula. Current Schlick approximation is kept; for full accuracy
  the Fresnel call would need to take the complex (n, k) and use the proper conducting
  formula. File as a separate item if the visual delta matters.

## Key files

- `sources/raytracing/src/raytracing/shading/spectral/metal_ior.cpp` — replace placeholder
  with real tables and lookup
- `tests/raytracing.native-test/src/raytracing/shading/spectral/metal_ior_test.cpp` —
  update tests to check against published values
