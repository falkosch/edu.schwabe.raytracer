# Convert mesh data from AoS to SoA layout

## Motivation

`Mesh` stores `std::vector<Facet>` as Array-of-Structures — each Facet packs 3 vertices (v0, v1, v2)
contiguously. This layout prevents SIMD batch intersection (`simd-batch-triangle-test.md`), because loading
the same component (e.g. all v0.x values) across 4 triangles requires gathering from non-contiguous memory.

## Proposed fix

Convert to Structure-of-Arrays: separate arrays for each vertex component. For example:

```
// AoS (current):  [v0 v1 v2] [v0 v1 v2] [v0 v1 v2] ...
// SoA (proposed):  v0x: [f0 f1 f2 f3 ...], v0y: [...], v0z: [...], v1x: [...], ...
```

This enables contiguous SIMD-width loads across multiple triangles for batch intersection.

The same conversion applies to `planeNormals`, `flatNormals`, `smoothNormals`, `facetEdges`, and
`texCoords` — all currently stored as AoS vectors in `Mesh`.

## Scope

- `Mesh.h` — change member vectors from `std::vector<Facet>` etc. to SoA arrays
- `facets.cpp` — adapt `computeFacets`, `computeNormals`, `computeNodesAndGraph`
- `Mesh.cpp` — adapt intersection loops, `computeFacetIntersection`
- `MeshGeometryNode` — may need to change how it references facet data

## Key files

- `sources/raytracing/include/raytracing/geometry/forms/meshes/Mesh.h`
- `sources/raytracing/src/raytracing/geometry/forms/meshes/Mesh.cpp`
- `sources/raytracing/src/raytracing/geometry/forms/meshes/facets.cpp`
- `sources/primitives/include/primitives/Facet.h`
