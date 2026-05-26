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

## Typedefs

`FacetTexCoords4` and `FacetNormals` are aliases for `Facet`, documenting semantic usage (texture coordinates
vs. precomputed plane normals).
