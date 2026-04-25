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

Normal stdout messages right after start on the first frame with ray hit and performance metrics:

```
Machine epsilon for float is 1.19209e-07
Machine epsilon for double is 2.22045e-16
Working directory: E:\edu.schwabe.raytracer\data
Loaded 36961 bytes from file meshes/teapot.off
loaded meshes/teapot.off: 726 vertices, 1452 faces
Building culling tree for scene ... done
Resizing output to 512x512
Raytrace 1 (512x512):
primaryRays 262144/0
secondaryRays 123863/17917
shadowRays 254010/253727
objectShadowRays 231858/200292
Duration: 0.116185s
```

New baseline output after using CMake Unity compilation mode:

```
D:\edu.schwabe.raytracer\cmake-build-release\sources\raytracerui\raytracerui.exe
Machine epsilon for float is 1.19209e-07
Machine epsilon for double is 2.22045e-16
Working directory: D:\edu.schwabe.raytracer\data
Loaded 36961 bytes from file meshes/teapot.off
loaded meshes/teapot.off: 726 vertices, 1452 faces
Building culling tree for scene ... done
Resizing output to 512x512
Raytrace 1 (512x512):
primaryRays 262144/0
secondaryRays 121805/17871
shadowRays 253279/252997
objectShadowRays 231073/199578
Duration: 0.0670916s
```

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

## CMake Submodules

The `cmake/` directory is a git submodule containing shared CMake modules (including `Catch2Tests.cmake`). Clone with
`--recurse-submodules`.
