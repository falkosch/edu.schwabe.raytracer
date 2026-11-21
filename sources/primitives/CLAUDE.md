# Primitives Library

Ray-casting geometric primitives with SIMD-accelerated intersection tests. Static library depending on `vectorization`.

## Types

| Type                     | Purpose                                                                         |
|--------------------------|---------------------------------------------------------------------------------|
| `Ray`                    | Origin + direction + precomputed reciprocal direction                           |
| `RayCast`                | Ray + culling mask + origin IDs (self-occlusion) + max distance                 |
| `AxisAlignedBoundingBox` | Min/max AABB with ray-box slab test (Geimer & Mueller), box-box overlap         |
| `BoundingSphere`         | Center (XYZ) + radius (W) with geometric ray-sphere test                        |
| `Facet`                  | 3-vertex triangle; Havel et al. ray-triangle test via precomputed plane normals |
| `FacetEdges`             | Edge vectors (v1-v0, v2-v0); Moeller-Trumbore ray-triangle test                 |
| `FacetLanes`             | Lane vector of the facet block test (`Float8`): one lane per facet              |
| `FacetNormalsBlock`      | Plane normals of `FacetLanes::SIZE` facets, component-wise; Havel block test    |
| `FacetBlockRay`          | Ray origin and direction, each component broadcast to all lanes                 |
| `FacetBlockTest`         | Block test result: Havel determinants per lane and the bit mask of hit lanes    |
| `SplittingPlane`         | Normal + signed distance; used for KD-tree spatial partitioning                 |

## Key APIs

All intersection functions follow the pattern:

- `bool overlaps(rayCast, shape)` -- boolean hit test
- `Float nearestIntersection(rayCast, shape, originId)` -- returns distance or max-float on miss

`AxisAlignedBoundingBox` additionally provides: `extendBy`, `clamp`, `transform` (Arvo's Graphics Gems method),
`surfaceArea`, `center`, `halfLengths`.

`RayCast` provides culling helpers (`backfaceCulled`, `frontfaceCulled`, `notCulled`, `cullingOrientation`) and
self-occlusion detection via `originIds` with configurable epsilon.

Triangle intersection returns `Float4{u, v, distance}` (barycentric coordinates).

`intersectionsHavel(block, ray, maxDistance, candidates, cullBack, cullFront)` runs `nearestIntersectionHavel`'s steps
for all facets of a `FacetNormalsBlock` at once and returns a `FacetBlockTest`; `texCoordsAndDistance(test, lane)`
gives a lane's `Float4{u, v, distance, distance}`. Its dot products are explicit FMA chains, so results can differ from
the single-facet test in the last bits. `Mesh` (raytracing) packs its KD-tree leaves into these blocks.

## Typedefs

`FacetTexCoords4` and `FacetNormals` are aliases for `Facet`, documenting semantic usage (texture coordinates
vs. precomputed plane normals).
