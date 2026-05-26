# Bug: Scene KD-tree AABB-ray overlap test falsely rejects rays

## Sequencing

- **Type:** bug
- **Level:** 1 (core raytracing)
- **Depends on:** *(none)*
- **Blocks:** DragonScene rendering, any scene with few finite objects

## Symptom

The DragonScene does not render the dragon mesh. The timing map shows mesh KD-tree
traversal structure in some screen regions (top-left, top-right) but gaps in others
(center, bottom), indicating the scene-level AABB-ray overlap test falsely rejects
rays that should intersect the dragon's bounding box.

Ray statistics confirm:

- `secondaryRays 0/0` — no reflections/refractions traced
- `shadowRays 1310720/1310720` — all shadow rays miss (only the background Plane is hit)
- The dragon mesh (100400 faces, 50184 vertices) loads and builds its internal KD-tree
  correctly (confirmed by using it in CornellBox)

## Reproduction

1. Set `DragonScene::setup(scene, resources)` as the active scene in `main.cpp`
2. Build and run — the dragon is invisible, only the checker background Plane renders
3. Switch the timing map display (`showMapIndex = 1`) — the dragon's bounding area shows
   partial/fragmented traversal instead of a connected region

The bug is resolution-sensitive: at 512×271 (16:9) the gaps are clearly visible.

## Root cause (confirmed)

The bug is in `NaiveKDTreeTraverser::findNearestIntersection` at the root AABB check:

```cpp
if (!overlaps(rayCast, root.rootBounding)) {
    return rayCast.maxDistance;  // ← falsely rejects valid rays
}
```

**Proof:** bypassing the scene KD-tree entirely (using brute-force
`findNearestIntersection(finiteSceneObjectsAsGeometryNodes, ...)` instead) renders
the dragon correctly with 164981 secondary rays.

The `overlaps(const RayCast&, const AxisAlignedBoundingBox&)` function in
`primitives/src/primitives/AxisAlignedBoundingBox.cpp:202` uses a slab-based
intersection test. The probable cause is in the W component of the AABB:

- `AxisAlignedBoundingBox::transform()` sets `boxOut.minimum.w = 1.0` and
  `boxOut.maximum.w = 1.0` (from the translation extraction at line 99-101)
- `computeBoxIntersectionCoefficients()` computes `(box.min - ray.origin) * ray.reciprocalDirection`
  for ALL 4 components including W
- When `ray.direction.w = 0`, `ray.reciprocalDirection.w = ±inf`
- `(1.0 - 1.0) * inf = 0 * inf = NaN` in IEEE 754
- NaN propagates through SSE `min`/`max` operations with platform-dependent behavior

Although the final swizzle pattern (`xy_xy`, `xz_xz`) appears to exclude W from the
slab test, the intermediate `min(pMin, pMax)` and `max(pMin, pMax)` operations on the
full 4-wide vector may produce NaN in the W lane. SSE `_mm_min_ps`/`_mm_max_ps` have
implementation-defined behavior when one operand is NaN — the result depends on operand
order, which can vary with compiler optimizations.

The bug manifests only when the scene root AABB has a non-trivial W component (from
the transform function), which happens for all transformed scene objects. In CornellBox
(8 objects), the scene KD-tree has inner nodes whose child AABBs use the dual-box
`overlaps(rayCast, a, b)` overload (line 158) which has a different code path. The
single-box `overlaps` at line 202 is only used for the ROOT check. With 8 objects, the
root AABB is large enough that most rays pass the root check regardless of NaN
artifacts. With 1 object (DragonScene), the root AABB is tight and the NaN-induced
false rejections become visible.

## Attempted fixes (2026-05-26, did not resolve)

1. **`transform()` W=0**: Changed `One<Float4>()` to `Zero<Float4>()` in the translation
   extraction swizzle so the AABB carries W=0 after transform. Dragon still invisible.
2. **`computeBoxIntersectionCoefficients` zeroW**: Zeroed W on all inputs (box min/max,
   ray origin, reciprocal direction) in both the single-box and dual-box overloads.
   Dragon still invisible.

Both fixes targeted the W-component NaN hypothesis. Since neither resolved the symptom,
the root cause described above is either incomplete or incorrect. **Further investigation
is needed** — start by instrumenting the actual overlap/intersection calls for the
DragonScene root AABB to confirm whether the slab test is truly the rejection site, or
whether the issue is elsewhere (e.g. KD-tree construction, scene graph building, mesh
bounding box computation, or the brute-force bypass test being stale).

## Affected files

- `sources/primitives/src/primitives/AxisAlignedBoundingBox.cpp` — `overlaps` and/or
  `computeBoxIntersectionCoefficients` and/or `transform`
- `sources/raytracing/include/raytracing/geometry/partitioning/treetraverser/NaiveKDTreeTraverser.h`
  — root AABB check at line 98
- `sources/raytracerui/src/tests/DragonScene.cpp` — test case for validation

## Notes

- The DragonScene materials were also accidentally rewritten in commit `1594e2f`
  ("cleanups, fix typos and grammar"): marble diffusion was removed, reflectance
  reduced from 25% to 13%, earth_iced transmittance replaced with marble. The scene
  file has been restored to its original material values (adapted for the new pipeline)
  as part of the linear-pipeline work.
- The background Plane at z=-2 is NOT the issue — the camera at z=+3.2 looks toward -Z,
  placing the dragon (z~0) between the camera and the Plane (z=-2).
