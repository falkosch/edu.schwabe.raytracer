# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

SSE/AVX-vectorized raytracer written in C++23. Windows-only UI (Win32 API + OpenGL + ImGui). Educational/hobby project.
Uses OpenMP for parallelization. Exceptions are globally disabled (`-fno-exceptions` / stripped `/EH`); targets that
need them (e.g. mesh loading) opt in via `target_compile_options`.

Compiler flags: `/std:c++23 /arch:AVX2 /fp:fast` (MSVC; the root `CMakeLists.txt` overrides CMake's `-std:c++latest`
mapping for MSVC ≥ 19.52), `/arch:AVX2` (clang-cl), `-march=native` (GCC/Clang).
The vectorization library auto-bumps its intrinsics level when the compiler defines `__AVX__`/`__AVX2__`.

## Build Commands

All configurations live in `CMakePresets.json` (CMake ≥ 4.0, Ninja generator, vcpkg manifest mode for GLEW and
ImGui). The `VCPKG_DIR` environment variable must point to the directory containing `vcpkg.exe`, which should also be
on `PATH`. Visual Studio and CLion both read the presets.

| Preset          | Compiler | Build directory       |
|-----------------|----------|-----------------------|
| `debug`         | MSVC     | `build-debug`         |
| `release`       | MSVC     | `build-release`       |
| `clang-release` | clang-cl | `build-clang-release` |

All presets use the `x64-windows-static` triplet. The root `CMakeLists.txt` chooses the CRT to match the triplet
(`/MT` / `/MTd` for `*-static`, `/MD` / `/MDd` otherwise). Use Ninja, not the Visual Studio `.vcxproj` generator, and
the `build-*` directories, not `cmake-build-*`.

Run these from a VS developer shell (`vcvars64.bat`), which puts `cl`, `clang-cl`, CMake and Ninja on `PATH`. With a
CMake that isn't on that `PATH` (for example CLion's bundled one), add
`-DCMAKE_MAKE_PROGRAM=C:/Users/falko/AppData/Local/Programs/CLion/bin/ninja/win/x64/ninja.exe` to the configure step.

**Configure and build:**

```bash
cmake --preset release
cmake --build --preset release                        # all targets, test DLLs included
cmake --build --preset release --target raytracerui   # or: benchmarks, sources (both), <library>.native-test
```

**Test:**

```bash
vstest.console.exe build-release/tests/vectorization.native-test/vectorization.native-test.dll
vstest.console.exe build-release/tests/primitives.native-test/primitives.native-test.dll
vstest.console.exe build-release/tests/raytracing.native-test/raytracing.native-test.dll
```

## Architecture

Six libraries with dependency chain:

```
raytracerui (executable, Win32+OpenGL+ImGui)
    └── raytracing (static lib)
            ├── primitives (static lib)
            │       └── vectorization (static lib)
            │               └── logging (static lib)
            └── vectorization
benchmarks (executable) ─── raytracing, logging
```

### vectorization (`sources/vectorization/`)

SIMD wrapper library over SSE4.2/AVX2/FMA intrinsics. See `sources/vectorization/CLAUDE.md` for full details.

Mid-tier vector types: `v_f32_4`, `v_f32_8`, `v_f64_2`, `v_f64_4`, `v_i32_4`, `v_i32_8`, `v_i64_2`, `v_i64_4`,
`v_ui32_4`, `v_ui32_8`, `v_ui64_2`, `v_ui64_4`, `m_f32_4x4`.

API tier aliases (`Float4`, `MFloat4x4`, etc.) default to single precision; define
`VECTORIZATION_HIGH_PRECISION` to switch to double.

### primitives (`sources/primitives/`)

Ray-casting primitives with SIMD intersection tests. Types: Ray, RayCast (ray + culling + self-occlusion),
AxisAlignedBoundingBox, BoundingSphere, Facet (Havel triangle test), FacetEdges (Moeller-Trumbore), SplittingPlane,
and the facet block test (FacetLanes, FacetNormalsBlock, FacetBlockRay, FacetBlockTest: Havel for 8 facets at once).

### raytracing (`sources/raytracing/`)

Core engine. Rendering pipeline: hero-wavelength spectral sampling (Wilkie 2014), GGX microfacet BRDF with VNDF
importance sampling, spectral Schlick-Fresnel reflectance, Beer-Lambert absorption. View transform:
spectrum -> XYZ -> sRGB-linear -> AgX -> sRGB.

- **Scene**: Scene, SceneObject, Camera, Resources (OFF mesh loading, HDR/PPM textures)
- **Forms**: Sphere, Box, Plane, Mesh (triangle mesh with KD-tree acceleration)
- **KD-Tree**: Balancers (BruteForceSAH, FixedIterationsSAH, Median, ArithmeticMean, MaxAxis, RotatingAxis) and
  traversers (Naive with an explicit stack, Wide over the tree's eight-wide view for meshes and the scene,
  Voxelization debug view) over a flat-array tree (`KDTreeRoot`)
- **Shading**: SceneShader (per-light Lambert diffuse + Phong specular + attenuation + shadow cache),
  ObjectShader (7 material channels: diffusion, reflectance, specular, roughness, transmittance, refractionEta,
  emittance), concrete shaders (Const, HDRImage, EnvironmentMap, IntersectionNormal, NoiseGeneratorMap)
- **Color**: sRGB/Rec.2020 primaries, Bradford chromatic adaptation, AgX and ACES filmic view transforms
- **Utilities**: RGBS color type, Bitmap, HDRImage, Perlin/Simplex noise, StatisticsCookie, RaytracerPackets

### raytracerui (`sources/raytracerui/`)

Win32+OpenGL+ImGui application. Predefined scenes (dropdown, or `--scene=<id>`): CornellBox (default), TestScene1/2,
Dragon, Procedural, and the test scenes Forms, Lighting, Shading, Materials & Noise, Meshes and the Showcase.
`--exit-after-render` closes the window once the first image is saved as `ray-traced.png`.
Runtime-configurable: KD-tree balancer, trace depth, super-sampling, ray packet size, FOV, interaction mode
(Camera/Object/Light), display mode (image/timing/depth). ImGui panels toggled via F1-F4.

### logging (`sources/logging/`)

Thread-safe logging with dedicated worker thread. LogScope produces lazy-evaluated LogEntries; Logger singleton
routes to stdout/stderr/file/DisplayBuffer. DisplayBuffer is a 10k-line circular buffer for UI display.

### benchmarks (`sources/benchmarks/`)

RDTSC-based intersection micro-benchmarks. Pins to single core, measures cycles/intersection for AABB, Sphere,
Plane, and Mesh facet intersections across 4 ray directions.

Mesh benchmarks (`mesh_benchmarks.cpp`) time KD-tree traversal plus the leaf loop on `meshes/teapot.off` and
`meshes/dragon2.off` (coherent and incoherent ray sets, nearest and any hit) and log leaf statistics, per-ray counters
and hit checksums. Options: `--data=<dir>` (working directory for the mesh files), `--only=intersections|meshes`,
`--repetitions=<n>`, `--core=<n>`, `--mesh=<file>` (repeatable). Compare variants by the median "best" core cycles/ray
over 5 alternating runs each; see `.backlog/DONE/mesh-traversal-benchmark.md` for the baseline and its noise.

## Threading Model

The `Raytracer` class runs rendering on a background `std::jthread` worker thread. The UI thread triggers renders via
`Raytracer::trigger()`, which is non-blocking. Render completion is marshalled back to the UI thread via
`PostMessage(WM_RENDER_COMPLETE)`.

**Enqueue work pattern**: `Raytracer::enqueueWork(std::function<void()>)` stores a callback under the mutex. The worker
thread executes pending work *before* each trace, on the same thread. This serializes scene modifications (e.g. KD-tree
rebuilds) with rendering — no concurrent access, no UI blocking. Rapid enqueues overwrite previous pending work, so
only the last one takes effect. Use this pattern for any scene mutation that must not race with rendering.

## Testing

- Tests live in `tests/vectorization.native-test/` — covers all vectorization operations
- **Windows/VS**: Uses MS CppUnitTest (`<CppUnitTest.h>`). Tests build as a shared library (.dll) for VS Test Explorer.
  Uses `TEST_CLASS(NameTest)` / `TEST_METHOD(camelCaseAction)` macros, assertions via `Assert::AreEqual()` etc.
- Test file naming mirrors source: `component_128d.cpp` -> `component_128d_test.cpp`
- Buffers handed to SIMD loads/stores or pointer constructors (`v_f32_4{arr.data()}`) must be declared
  `alignas(XMM_ALIGNMENT)` / `alignas(YMM_ALIGNMENT)`. MSVC usually tolerates misalignment, but clang crashes on it.
- `tests/*/src/test_module.cpp` holds each DLL's `TEST_MODULE_INITIALIZE`: Debug asserts go to stderr instead of a
  modal dialog, and the raytracing tests pin the OpenMP runtime, which the test executor would otherwise unload
  under live worker threads

### Regression Tests

Normal logs right after start:

```
[0.000] - INFO Epsilon : Machine epsilon for float is 1.19209e-07
[0.000] - INFO Epsilon : Machine epsilon for double is 2.22045e-16
[0.000] - INFO Main : Working directory: D:\edu.schwabe.raytracer\data
[0.010] - INFO Mesh : Loaded 36961 bytes from file meshes/teapot.off
[0.023] - INFO KDTree : Built KD-tree for 1452 nodes in 28232885 cycles
[0.023] - INFO Mesh : loaded meshes/teapot.off: 726 vertices, 1452 faces
[0.023] - INFO Scene : Building culling tree for scene ...
[0.023] - INFO KDTree : Built KD-tree for 8 nodes in 164 cycles
[0.023] - INFO Scene : done
[0.040] - INFO UI : Resizing output to 512x512
[0.042] - INFO Raytracer : Raytrace 1 (512x512):
[0.081] - INFO Raytracer : Duration: 0.0389018s
[0.185] - INFO UI : primaryRays 262144/0
[0.185] - INFO UI : secondaryRays 121821/17871
[0.185] - INFO UI : shadowRays 253286/253005
[0.185] - INFO UI : objectShadowRays 231066/199568
[0.185] - INFO UI : Saving ray-traced image ...
[0.196] - INFO UI : ray-traced.png
```

Baseline numbers incorporate: CMake Unity compilation, LTCG, FMA3 intrinsics, BruteForceSAH default balancer,
sweep-based SAH partitioning, RGBS color type, precompiled headers.

### Rendering a scene

Use this to check any change that affects the image, and look at the result:

```bash
cd data
../build-release/sources/raytracerui/raytracerui.exe --scene=<id> --exit-after-render
```

- The app renders the scene once at the fast-preview size (512x512 in release), saves `data/ray-traced.png`, logs
  the ray counters and exits. Read the PNG to judge it; compare `data/raytracer.log` with a baseline.
- Scene ids are listed in `sources/raytracerui/src/tests/SceneCatalog.cpp` (`cornell-box`, `forms`, `lighting`,
  `shading`, `materials`, `meshes`, `showcase`, ...). The test scenes cover forms, lights, shaders, materials and
  meshes, so pick the one the change touches; `cornell-box` is the regression baseline.
- The logger writes `raytracer.log` only when the app exits, so wait for the process to end before reading it. Each
  start overwrites both files.
- To compare against a commit, build that commit's `raytracerui` in a throwaway worktree with a short build directory
  (Windows path length) and render the same scene; byte-identical PNGs mean an unchanged image.
- Debug builds open a modal dialog on a failed assert, which blocks the run: prefer release, or expect the dialog.
- The `clang-release` executable needs LLVM's `bin` directory (`libomp.dll`) on `PATH`, e.g.
  `C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin`.

## Code Style

- Formatting: `.clang-format` (LLVM-based, 120-column limit)
- Linting: `.clang-tidy` (clang-diagnostic-*, clang-analyzer-*)
- Compiler warnings: `/W4` (MSVC)
- Defines always set: `UNICODE`, `_UNICODE`, `WIN32_LEAN_AND_MEAN`, `NOMINMAX`

## Data

Mesh and texture data must be unpacked from `data/data.zip` into the `data/` folder before running. The UI executable
must be run from or with access to the `data/` directory.

## Naming Conventions

- **Files and folders**: snake_case, lowercase (`replace_component_128s.h`, `blend_masked_128d.cpp`,
  `swizzled_blends/`, ...)
- Types/classes: PascalCase (`AxisAlignedBoundingBox`, `BoundingSphere`, ...)
- Constant builder functions: PascalCase (`MaskAll<T>()`, `One<T>()`, `OneW<T>()`, ...)
- Methods/functions: camelCase (`nearestIntersection()`, `replaceComponent()`, ...)
- SIMD vector types: `v_{type}_{width}` pattern (`v_f32_4`, `v_ui32_4`, ...)
- Namespaces match library names: `vectorization`, `primitives`, `raytracer`
- Commit messages: conventional commits (`fix:`, `feat:`, `refactor:`, `cleanup:`, `docs:`, `test:`)

## Precompiled Headers

Each target has a `src/stdafx.h` used as PCH via `target_precompile_headers`. The vectorization library itself has no
PCH (narrow per-file includes). Raytracing's stdafx.h contains `using namespace std;` — files that include Windows
headers (e.g. `PNGWriter.cpp`) must be excluded via `SKIP_PRECOMPILE_HEADERS` to avoid `std::byte` ambiguity.

## Development Rules

- DO NOT revert user changes — assume they are intentional cleanups. If there is a conflict you cannot
  resolve, ask the user for feedback instead of silently reverting.
- When editing a file, change ONLY what was explicitly requested. Do not remove comments, reformat
  surrounding code, or rewrite files when a targeted edit suffices.
- Outside the vectorization library, use API-tier type aliases (`Float`, `Float4`, `Float8`) instead of
  internal types (`Float_32`, `v_f32_4`, `v_f32_8`). This applies to source files, headers, and tests.
- Within a library's own source files, use narrow/specific includes (e.g.
  `#include "vectorization/functions/copysign.h"`),
  not broad aggregator includes. Broad includes can cause cyclic dependencies, especially within the vectorization
  library.
  Consuming libraries and executables (`#include <...>`) should use the barrel header (e.g.
  `#include <vectorization.h>`).
- Tests should include only the barrel header (`#include <vectorization.h>`), not narrow
  vectorization includes.
