# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

SSE/AVX-vectorized Whitted-style raytracer written in C++23. Windows-only UI (Win32 API + OpenGL). Educational/hobby
project. Uses OpenMP for parallelization. Key compiler flags: `/arch:AVX2 /fp:fast` (MSVC), `-march=native` (GCC).

## Build Commands

### Windows (Visual Studio)

Open with Visual Studio using CMake integration (CMakeSettings.json provided). Dependencies via vcpkg (GLEW).
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

Four libraries with a strict dependency chain:

```
raytracerui (executable, Win32+OpenGL)
    └── raytracing (static lib)
            ├── primitives (static lib)
            │       └── vectorization (static lib)
            └── vectorization
```

### vectorization (`sources/vectorization/`)

SIMD wrapper library over SSE4/AVX/FMA intrinsics. Core types:

- `v_f32_4` — 4-wide float vector (128-bit SSE)
- `v_i32_4`, `v_ui32_4` — 4-wide integer vectors
- `v_ui64_2` — 2-wide uint64 vector
- `m_f32_4x4` — 4x4 float matrix

Operations are split into many small files by category (accessors, blends, swizzles, selects, constants, math).
SIMD width suffixes: `128d` (2×f64), `128s` (4×f32), `256d` (4×f64), `256s` (8×f32), `128i`/`256i` (integer).
Third-party transcendental math in `3rdparty/` (sse_mathfun.h, avx_mathfun.h).

### primitives (`sources/primitives/`)

Geometric types: Ray, RayCast, AxisAlignedBoundingBox, BoundingSphere, Facet, FacetEdges, SplittingPlane.

### raytracing (`sources/raytracing/`)

Core engine with subsystems:

- **Scene**: Scene, SceneObject, Camera, Resources
- **Forms**: Sphere, Box, Plane, Mesh (with texture coords)
- **KD-Tree partitioning**: Multiple balancer strategies (SAH variants, median, arithmetic mean, etc.) and traversers (
  naive, voxelization)
- **Shading**: BRDF, Schlick-Fresnel refraction, Beer-Lambert, Phong specular. Shader types: environment, HDR image,
  intersection normal
- **Utilities**: Bitmap, HDRImage, Perlin/Simplex noise, statistics

### raytracerui (`sources/raytracerui/`)

Win32 application entry point (`src/main.cpp`). Predefined test scenes: CornellBox, Dragon, Procedural, TestScene1/2,
TestLight. Configuration constants defined in main.cpp (FAST_PREVIEW_SIZE, MAX_TRACE_DEPTH, RAY_PACKET_SIZE).

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
- Test file naming mirrors source: `component_128d.cpp` → `component_128d_test.cpp`

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

Baseline numbers incorporate:

- CMake Unity compilation mode
- LTCG fix (MSVC Whole Program Optimization now working correctly)
- FMA3 intrinsics
- Packed arithmetic functions refactoring
- Switching to BruteForceSAHKDTreeBalancer as default
- Implement sweep-based SAH KD-tree partitioning
- RGBS color type refactoring
- Use precompiled headers in builds

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
