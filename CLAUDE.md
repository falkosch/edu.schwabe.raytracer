# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

SSE/AVX-vectorized raytracer written in C++23. Windows-only UI (Win32 API + OpenGL + ImGui). Educational/hobby project.
Uses OpenMP for parallelization. Exceptions are globally disabled (`-fno-exceptions` / stripped `/EH`); targets that
need them (e.g. mesh loading) opt in via `target_compile_options`.

Compiler flags: `/arch:SSE4.2 /fp:fast` (MSVC), `/arch:AVX2` (clang-cl), `-march=native` (GCC/Clang).
The vectorization library auto-bumps its intrinsics level when the compiler defines `__AVX__`/`__AVX2__`.

## Build Commands

### Windows (Visual Studio)

Open with Visual Studio using CMake integration (CMakeSettings.json provided). Dependencies via vcpkg (GLEW, ImGui).
Requires `VCPKG_DIR` environment variable pointing to the directory containing `vcpkg.exe` (also add to `PATH`).

### CMake directly (Windows/MSVC with Ninja)

Use Ninja, not Visual Studio `.vcxproj` generator. Build directories are `build-release` / `build-debug` (not
`cmake-build-*`).

**Configure (Release):**

```bash
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=C:/Users/falko/AppData/Local/Programs/CLion/bin/ninja/win/x64/ninja.exe -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_TOOLCHAIN_FILE="$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake" -G Ninja -S D:\edu.schwabe.raytracer -B D:\edu.schwabe.raytracer\build-release
```

**Configure (Debug):**

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=C:/Users/falko/AppData/Local/Programs/CLion/bin/ninja/win/x64/ninja.exe -DCMAKE_TOOLCHAIN_FILE="$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake" -G Ninja -S D:\edu.schwabe.raytracer -B D:\edu.schwabe.raytracer\build-debug
```

**Build:**

```bash
cmake --build build-release --target raytracerui
cmake --build build-release --target sources_and_tests
```

**Test:**

```bash
vstest.console.exe build-release/tests/vectorization.native-test/vectorization.native-test.dll
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
AxisAlignedBoundingBox, BoundingSphere, Facet (Havel triangle test), FacetEdges (Moeller-Trumbore), SplittingPlane.

### raytracing (`sources/raytracing/`)

Core engine. Rendering pipeline: ACEScg-linear working space, GGX microfacet BRDF with VNDF importance sampling,
Schlick-Fresnel reflectance, Beer-Lambert absorption. View transform: ACEScg -> XYZ -> sRGB-linear -> AgX -> sRGB.

- **Scene**: Scene, SceneObject, Camera, Resources (OFF mesh loading, HDR/PPM textures)
- **Forms**: Sphere, Box, Plane, Mesh (triangle mesh with KD-tree acceleration)
- **KD-Tree**: Balancers (BruteForceSAH, FixedIterationsSAH, Median, ArithmeticMean, MaxAxis, RotatingAxis) and
  traversers (Naive recursive, Voxelization-based)
- **Shading**: SceneShader (per-light Lambert diffuse + Phong specular + attenuation + shadow cache),
  ObjectShader (7 material channels: diffusion, reflectance, specular, roughness, transmittance, refractionEta,
  emittance), concrete shaders (Const, HDRImage, EnvironmentMap, IntersectionNormal, NoiseGeneratorMap)
- **Color**: sRGB/ACEScg/Rec.2020 primaries, Bradford chromatic adaptation, AgX and ACES filmic view transforms
- **Utilities**: RGBS color type, Bitmap, HDRImage, Perlin/Simplex noise, StatisticsCookie, RaytracerPackets

### raytracerui (`sources/raytracerui/`)

Win32+OpenGL+ImGui application. Predefined scenes: CornellBox (default), Dragon, Procedural, TestScene1/2, TestLight.
Runtime-configurable: KD-tree balancer, trace depth, super-sampling, ray packet size, FOV, interaction mode
(Camera/Object/Light), display mode (image/timing/depth). ImGui panels toggled via F1-F4.

### logging (`sources/logging/`)

Thread-safe logging with dedicated worker thread. LogScope produces lazy-evaluated LogEntries; Logger singleton
routes to stdout/stderr/file/DisplayBuffer. DisplayBuffer is a 10k-line circular buffer for UI display.

### benchmarks (`sources/benchmarks/`)

RDTSC-based intersection micro-benchmarks. Pins to single core, measures cycles/intersection for AABB, Sphere,
Plane, and Mesh facet intersections across 4 ray directions.

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

## CMake Submodules

The `cmake/` directory is a git submodule containing shared CMake modules (including `Catch2Tests.cmake`). Clone with
`--recurse-submodules`.
