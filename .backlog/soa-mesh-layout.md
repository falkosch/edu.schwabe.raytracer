# Convert mesh data from AoS to SoA layout

## Motivation

`Mesh` stores `std::vector<Facet>` as Array-of-Structures — each `Facet` packs 3 vertices (v0, v1, v2)
contiguously. This layout prevents SIMD batch intersection (`simd-batch-triangle-test.md`), because loading
the same component (e.g. all v0.x values) across 4 triangles requires gathering from non-contiguous memory.

The same concern applies to `planeNormals`, `flatNormals`, `smoothNormals`, `facetEdges`, and `texCoords` —
all currently stored as AoS vectors in `Mesh`.

The hit-test data is done: plane normals in blocks of 4, in KD-tree leaf order, see `DONE/soa-mesh-layout.md`. The
other per-facet arrays stay AoS by decision. The point below remains.

## Open points

- `MeshGeometryNode` still owns a `Facet` copy (48 B per facet; dragon2 about 4.8 MB). The leaf loops no longer read
  the nodes; only the balancers use the copy (`includeInBounding`, `overlaps`). An index into `Mesh::facets` would
  do.

## Key files

- `sources/raytracing/include/raytracing/geometry/forms/meshes/MeshGeometryNode.h` — owns a `Facet` copy; consider
  replacing with index-only reference into the SoA arrays
- `sources/raytracing/src/raytracing/geometry/forms/meshes/facets.cpp` — `computeFacets`, `computeNormals`,
  `computeNodesAndGraph`, `computeFacetIntersection`
