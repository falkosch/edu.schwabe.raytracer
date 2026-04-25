# Convert mesh data from AoS to SoA layout

## Motivation

`Mesh` stores `std::vector<Facet>` as Array-of-Structures — each `Facet` packs 3 vertices (v0, v1, v2)
contiguously. This layout prevents SIMD batch intersection (`simd-batch-triangle-test.md`), because loading
the same component (e.g. all v0.x values) across 4 triangles requires gathering from non-contiguous memory.

The same concern applies to `planeNormals`, `flatNormals`, `smoothNormals`, `facetEdges`, and `texCoords` —
all currently stored as AoS vectors in `Mesh`.

## Layout options (must be benchmarked before committing)

The right layout is not obvious without measurement. Single-ray traversal currently visits facets in
KD-tree leaf order (effectively random), so a pure component-wise SoA layout will *gather* on every
facet access and likely regress the single-ray path until the batch-intersection follow-up lands.
Different layouts trade single-ray cost against batch-intersection feasibility.

### Path A — Full component-wise SoA (max SIMD-batch friendly)

Replace each `std::vector<Facet>` with 12 separate `std::vector<float>` (`v0x`, `v0y`, `v0z`, `v0w`,
`v1x`, ..., `v2w`). Same expansion for `planeNormals` (12 floats per facet), `flatNormals` (4),
`smoothNormals` (12), `facetEdges` (8), `texCoords` (12).

- Pros: contiguous `_mm_load_ps` of 4 consecutive facets' v0.x — directly unlocks batch intersection
- Cons: every single-facet access becomes a 12-component gather; more cache lines touched per facet
  in the SISD path; intersection helper signatures balloon (or need a wrapper struct)
- Open question: is the regression on the SISD path tolerable until the batch path is in place?
  Need a benchmark before/after with the existing `nearestIntersectionHavel` loop.

### Path B — Slot-wise SoA (`v0[]`, `v1[]`, `v2[]` arrays of `Float4`)

Keep `Float4` storage but split each `Facet` field into three separate arrays. Cheap to implement,
nearly zero churn to call sites that take `Float4` by value.

- Pros: minimal code change; no per-facet gather cost; still aligned 16-byte loads
- Cons: does NOT allow contiguous `_mm_load_ps(&v0x[base])` of 4 facets' v0.x — i.e. it does not
  actually unlock the batch-intersection use case the parent task names
- This path has no real benefit unless paired with a batched re-layout step; document and probably
  reject after measurement.

### Path C — Block-SoA (a.k.a. AoSoA, SIMD-width tiles)

Group facets into blocks of 4 (SSE) or 8 (AVX). Within a block the data is component-wise SoA
(`v0x[0..3]` packed as one `Float4`, etc.); across blocks it's an array. Equivalent to storing the
mesh as `std::vector<FacetBlock4>` where `FacetBlock4` holds 12× `Float4` (one lane per facet).

- Pros: contiguous SIMD load *within* a block (batch-intersection friendly); only one cache line
  per block in the SISD path (better locality than Path A); easier to retrofit alignment
- Cons: KD-tree leaves reference facets by global index; inside a block we'd need partial-mask
  intersection if the leaf only references a subset; reordering facets by KD-tree leaf
  improves locality but complicates `MeshGeometryNode::index`
- Probably the most interesting option to benchmark; matches what `embree` and similar use.

### Path D — Hybrid: keep AoS `Facet`, add SoA mirror only where batch loads need it

Leave `std::vector<Facet>` etc. in place for the SISD path. Add separate SoA mirror arrays
(component-wise or block-SoA) populated alongside, used only by the batch traverser when it lands.

- Pros: zero regression on the SISD path; the SoA layout is purely additive; can be implemented
  per-field as needed (e.g. only `planeNormals` SoA initially)
- Cons: roughly 2× memory for the mirrored fields; bookkeeping to keep mirror in sync on
  scene mutation; "two ways to do it" cognitive load
- The conservative choice if benchmarks show Path A regresses too much.

## Cross-cutting design choices (independent of layout path)

### `Facet` / `FacetNormals` / `FacetEdges` struct lifetime

These structs are used outside `Mesh`: `Plane.h::baseVectors`, `MeshGeometryNode::facet`,
`computeNodesAndGraph` constructs `MeshGeometryNode` from a `Facet`, and single-facet intersection
helpers (`nearestIntersectionHavel`, `nearestIntersectionMoeller`) take them by const ref. They
should stay; SoA paths add `gather*(soa, i)` helpers that reconstruct an AoS struct on demand for
SISD callers.

### Encapsulation of SoA fields

12+ parallel `std::vector<float>` members on `Mesh` is unwieldy. Wrap them in a small
`MeshFacetSoA` (and `FacetNormalsSoA`, `FacetEdgesSoA`, etc.) struct so `computeNormals` and
`computeFacetIntersection` parameter lists don't explode. Independent of which path is chosen.

### KD-tree leaf access pattern is the hot constraint

The SISD intersection loop in `Mesh.cpp::findNearestIntersection` accesses facets via
`MeshGeometryNode::index` — the order is determined by KD-tree traversal, not by index. Any layout
that assumes "consecutive indices touched together" (Path C in particular) needs the KD-tree
balancer to preserve or restore that locality, e.g. by reordering facets so that leaf-adjacent
facets are also block-adjacent.

## Recommended sequencing

1. **Microbenchmark first** — extract a minimal harness around the current `nearestIntersectionHavel`
   loop on `meshes/teapot.off` (already in the regression baseline: 0.067s for 512×512 first frame).
   Establish a stable baseline.
2. **Prototype Path A on `planeNormals` only** (the hottest field — 3 `Float4` per facet, used in
   every Havel test). Measure SISD-path regression in isolation.
3. **If regression < ~5%**, expand Path A to remaining fields; proceed to `simd-batch-triangle-test.md`.
4. **If regression too large**, prototype Path C (block-SoA, blocks of 4) on `planeNormals`. Same
   measurement.
5. **If both regress unacceptably**, fall back to Path D and document that SoA is mirror-only.
6. Each benchmark result should land as a comment in this file before the next path is attempted.

## Key files

- `sources/raytracing/include/raytracing/geometry/forms/meshes/Mesh.h` — member vector layout
- `sources/raytracing/src/raytracing/geometry/forms/meshes/Mesh.cpp` — SISD intersection loops, `clear`, `setupMesh`
- `sources/raytracing/src/raytracing/geometry/forms/meshes/facets.cpp` — `computeFacets`, `computeNormals`,
  `computeNodesAndGraph`, `computeFacetIntersection`
- `sources/raytracing/src/raytracing/geometry/forms/meshes/textureCoordinates.cpp` — `computeTexCoordsOrtho`,
  `computeTexCoordsSpherical`
- `sources/raytracing/include/raytracing/geometry/forms/meshes/MeshGeometryNode.h` — owns a `Facet` copy; consider
  replacing with index-only reference into the SoA arrays
- `sources/primitives/include/primitives/Facet.h` — keep struct, possibly add gather constructor
- `sources/primitives/include/primitives/FacetEdges.h` — keep struct
