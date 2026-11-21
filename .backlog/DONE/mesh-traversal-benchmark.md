# Mesh KD-tree traversal benchmark

## Motivation

Three open items need a stable before/after measurement of the mesh hot path (KD-tree traversal plus the leaf
intersection loop): `soa-mesh-layout.md` (step 1 of its sequencing asks for exactly this harness),
`flat-array-kdtree-nodes.md` and `simd-batch-triangle-test.md`. `kdtree-leaf-packed-facets.md` needs it to choose
between its options.

The existing `benchmarkMeshFacet` in `sources/benchmarks/src/intersection_benchmarks.cpp` can't serve as that
baseline:

- It uses `Mesh::buildTriangleMesh()`: one triangle, built without traverser or balancer, so
  `Mesh::findNearestIntersection` takes the brute-force fallback over `nodes`. KD-tree traversal and the leaf loop
  of a real mesh are never exercised.
- It fires the same 4 rays 100M times. The data fits in L1 and the branches are perfectly predictable, so it can't
  show the cache and branch effects that the flat-array and SoA work target.

The full-frame regression render (`CLAUDE.md`, 512×512 first frame) includes shading, shadow-ray caching and OpenMP
scheduling. A few-percent change in traversal or leaf cost drowns in that noise.

## Proposed fix

1. Load `meshes/teapot.off` the way `Resources::getMesh` does (`Resources.cpp:44-56`):
   `NaiveKDTreeTraverser<FacetIntersection>` + `BruteForceSAHKDTreeBalancer`. The benchmark executable has no data
   path handling yet; take the `data/` directory from the command line or require it as working directory, like the
   UI. Optionally add `meshes/dragon2.off` as a larger mesh.
2. Generate a fixed, reproducible ray set once, before timing. Aim at `mesh->getBounding()` (`setupMesh` rescales
   and translates the vertices, so don't hard-code coordinates):
   - coherent rays: a primary-ray grid (e.g. 512×512) from a camera in front of the mesh
   - incoherent rays: random origins and directions through the bounding box, fixed seed (stand-in for secondary
     and shadow rays)
3. Time `findNearestIntersection` and `findAnyIntersection` (the shadow path) separately, as cycles per ray. Reuse
   the existing RDTSC + `pinToSingleCore` setup from `main.cpp`.
4. Report counters next to the cycles, so a change can be attributed to traversal or to the leaf loop:
   - KD-tree nodes visited per ray (`StatisticsCookie::kdTreeNodesVisited` via `StatisticsCookie::current`)
   - leaves and facets in visited leaves per ray: counted in an untimed pass through a `GeometryNodesTraverser`
     wrapper in the benchmark, so the production leaf loop stays unchanged
   - hit count and a checksum of hit distances, to catch functional regressions between layouts
5. Run several repetitions and report min and median. Record the baseline numbers in this file before any of the
   dependent items starts.

Only `NaiveKDTreeTraverser` is benchmarked. `VoxelizationKDTreeTraverser` is a debug view: it intersects the leaf
boxes and never calls the leaf loop, so its "hits" are leaf boxes and its timing says nothing about facets.
`flat-array-kdtree-nodes.md` ported it to the flat tree.

## Implementation

- `sources/benchmarks/src/mesh_benchmarks.cpp` and `include/mesh_benchmarks.h`, called from `main.cpp`. The only
  production change is the read-only accessor `Mesh::getGraph()` (like `SceneGeometry::getSceneGraph()`), needed for
  the leaf statistics.
- Options: `--data=<dir>`, `--only=intersections|meshes`, `--repetitions=<n>` (default 15), `--core=<n>`,
  `--mesh=<file>` (repeatable, default teapot and dragon2). Without data the mesh part is skipped with a warning.
- Timing: each pass is timed in chunks of 4096 rays. "best" sums each chunk's fastest repetition, which filters out
  interrupts; "median" is the median whole pass. TSC cycles are scaled to core cycles with a multiply/xor latency
  chain measured before and after every repetition (fastest of 8 pieces). The scaling is exact for compute-bound code
  and overcorrects memory stalls (fixed in ns), so runs at a lower clock read slightly lower.
- Process and thread priority are raised (`HIGH_PRIORITY_CLASS`, `THREAD_PRIORITY_HIGHEST`); this also applies to the
  intersection benchmarks.
- Checks: nearest and any hit must find the same number of hits per ray set (warning otherwise). Nearest-hit results
  are logged as a facet index sum (exact) and a distance sum (compare with a tolerance; a batched Havel test may
  round differently). Any-hit distances aren't summed because they depend on the traversal order.

## Baseline

i9-14900KF, logical processor 4 (P-core), Windows 11 with the Balanced power plan, MSVC 14.52 `release` preset, based
on `fc4c097`. Two sessions of ten runs each of `benchmarks.exe --data=<repo>/data --only=meshes --core=4`
(15 repetitions per run): "noisy" with the usual background load, "stable" with little other load. Core cycles per
ray. CV is the standard deviation over the ten runs divided by their mean; range is (worst - best) divided by the
median; change is stable vs. noisy.

"best" (sum of each chunk's fastest repetition), median over the ten runs:

| Mesh    | Query   | Rays       | Noisy  |   CV | Range | Stable |   CV | Range | Change |
|---------|---------|------------|-------:|-----:|------:|-------:|-----:|------:|-------:|
| teapot  | nearest | coherent   | 2218.8 | 0.4% |  1.2% | 2219.4 | 0.8% |  2.5% |  +0.0% |
| teapot  | any     | coherent   | 1351.8 | 1.3% |  4.6% | 1350.5 | 0.7% |  2.2% |  -0.1% |
| teapot  | nearest | incoherent | 4040.4 | 0.9% |  2.8% | 4028.6 | 0.8% |  2.2% |  -0.3% |
| teapot  | any     | incoherent | 2474.9 | 0.9% |  2.3% | 2469.3 | 0.8% |  2.0% |  -0.2% |
| dragon2 | nearest | coherent   | 2505.2 | 1.3% |  3.5% | 2528.5 | 0.8% |  2.5% |  +0.9% |
| dragon2 | any     | coherent   | 1623.3 | 2.3% |  7.0% | 1623.1 | 1.8% |  6.1% |  -0.0% |
| dragon2 | nearest | incoherent | 5151.5 | 2.7% |  8.7% | 4985.2 | 1.6% |  5.4% |  -3.2% |
| dragon2 | any     | incoherent | 3429.0 | 2.6% |  7.3% | 3313.0 | 3.4% | 12.2% |  -3.4% |

Median pass, median over the ten runs:

| Mesh    | Query   | Rays       | Noisy  |   CV | Range | Stable |   CV | Range | Change |
|---------|---------|------------|-------:|-----:|------:|-------:|-----:|------:|-------:|
| teapot  | nearest | coherent   | 2342.8 | 1.5% |  4.4% | 2330.9 | 1.3% |  4.1% |  -0.5% |
| teapot  | any     | coherent   | 1425.1 | 2.7% |  8.5% | 1433.0 | 1.5% |  4.9% |  +0.6% |
| teapot  | nearest | incoherent | 4222.4 | 1.7% |  4.6% | 4174.2 | 1.1% |  3.6% |  -1.1% |
| teapot  | any     | incoherent | 2607.6 | 1.7% |  6.1% | 2574.4 | 3.2% | 11.2% |  -1.3% |
| dragon2 | nearest | coherent   | 2644.6 | 2.3% |  6.4% | 2629.9 | 4.3% | 14.9% |  -0.6% |
| dragon2 | any     | coherent   | 1741.6 | 3.0% |  9.4% | 1695.5 | 4.6% | 15.4% |  -2.6% |
| dragon2 | nearest | incoherent | 5916.9 | 5.5% | 13.6% | 5473.0 | 9.3% | 31.9% |  -7.5% |
| dragon2 | any     | incoherent | 3847.8 | 5.5% | 15.5% | 3588.9 | 7.0% | 24.9% |  -6.7% |

Estimated core clock: noisy 4.85-5.45 GHz (medians 4.97-5.30), stable 4.83-5.52 GHz (medians 5.28-5.43). Wall-clock
ns/ray had a CV of 4-10% (noisy) and 1.3-12.7% (stable).

These timings are from the original code, before the flat-array refactor and the traversal fix; both comparisons are
in `DONE/flat-array-kdtree-nodes.md`.

Counters, hits and sums of the original code, identical in all twenty runs (the flat-array refactor keeps them):

| Mesh    | Query   | Rays       | Nodes | Leaves | Facets |   Hits | Facet index sum |
|---------|---------|------------|------:|-------:|-------:|-------:|----------------:|
| teapot  | nearest | coherent   | 10.37 |   1.96 |  55.18 | 116128 |        98652480 |
| teapot  | any     | coherent   |  8.37 |   1.31 |  36.72 | 116128 |                 |
| teapot  | nearest | incoherent | 10.73 |   2.22 |  61.91 | 123260 |       104809776 |
| teapot  | any     | incoherent |  8.65 |   1.42 |  39.98 | 123260 |                 |
| dragon2 | nearest | coherent   | 21.26 |   2.06 |  47.33 |  69837 |      3117523969 |
| dragon2 | any     | coherent   | 17.46 |   1.47 |  33.60 |  69837 |                 |
| dragon2 | nearest | incoherent | 20.00 |   2.07 |  50.74 |  42407 |      2044256667 |
| dragon2 | any     | incoherent | 17.41 |   1.49 |  35.45 |  42407 |                 |

After the traversal fix (no descent into a subtree whose child boxes the ray misses both), hits and sums stay the
same, the counters drop:

| Mesh    | Query   | Rays       | Nodes | Leaves | Facets |
|---------|---------|------------|------:|-------:|-------:|
| teapot  | nearest | coherent   |  7.49 |   0.95 |  29.06 |
| teapot  | any     | coherent   |  6.93 |   0.78 |  22.46 |
| teapot  | nearest | incoherent |  9.00 |   1.36 |  39.67 |
| teapot  | any     | incoherent |  7.44 |   0.87 |  25.44 |
| dragon2 | nearest | coherent   |  9.34 |   0.50 |  13.25 |
| dragon2 | any     | coherent   |  8.63 |   0.40 |  10.66 |
| dragon2 | nearest | incoherent |  9.08 |   0.64 |  17.13 |
| dragon2 | any     | incoherent |  7.60 |   0.31 |   8.63 |

Nodes, leaves and facets are per ray; facets counts all facets in the visited leaves (any hit counts the whole leaf
even when it stops early).
Nearest-hit distance sums: teapot 661367.878 (coherent) and 46461.251 (incoherent), dragon2 539927.274 and 17252.616.
Leaf statistics are in `kdtree-leaf-packed-facets.md`.

Noise: "best" is the stable metric. Its CV stays at 0.4-1.3% on teapot and 0.8-3.4% on dragon2 in both sessions
(stable dragon2 any/incoherent has one low outlier at 2991.6). The median pass reaches 9% CV, wall-clock ns/ray 13%.

Between the sessions, teapot's "best" moved by at most 0.3% and dragon2 coherent by at most 1%. Dragon2 incoherent got
3% faster with less background load: those rays miss the caches most, so they depend on what the other cores do to
the shared L3 and memory. The median pass moved more (up to 7.5%), because single slow passes go into it.

Compare variants by the median of "best" over 5 runs each, alternating the variants in one session instead of
against the numbers above. With a CV of up to ~2%, a 5% change is then about 4 standard errors of the difference;
for dragon2 any/incoherent (3.4%) use 10 runs each.

## Key files

- `sources/benchmarks/src/intersection_benchmarks.cpp`, `sources/benchmarks/include/intersection_benchmarks.h`
- `sources/benchmarks/src/main.cpp` — core pinning, RDTSC calibration, ray setup
- `sources/benchmarks/CMakeLists.txt`
- `sources/raytracing/src/raytracing/scene/Resources.cpp` — production mesh loading (traverser + balancer choice)
- `sources/raytracing/include/raytracing/common/StatisticsCookie.h`

## Related backlog items

- `soa-mesh-layout.md` — step 1 of its sequencing is this harness. Its 0.067s teapot baseline is out of date
  (`CLAUDE.md` now lists 0.0389s).
- `flat-array-kdtree-nodes.md`, `simd-batch-triangle-test.md`, `kdtree-leaf-packed-facets.md` — measured with this.
- `raytracing-testability.md` (#2) — on a mesh the KD-tree path is only reachable through `loadFromOffFile`; this
  harness hits the same restriction.

## Commits

- `5e14632` feat(benchmarks): add mesh KD-tree traversal benchmark
