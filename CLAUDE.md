# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

SSE/AVX-vectorized Whitted-style raytracer written in C++. Windows-only UI (Win32 API + OpenGL). Educational/hobby
project.

## Build Commands

### Windows (Visual Studio)

Open with Visual Studio using CMake integration (CMakeSettings.json provided). Dependencies via vcpkg (Catch2, GLEW).

### Linux / GCC (cross-compiling for Windows)

```bash
# Install dependencies (Conan)
./install-dependencies-with-conan.sh

# Build (defaults: Release, x64, target=sources_and_tests)
./build-with-local-cc.sh

# Run tests
./test-with-local-cc.sh

# Run the UI executable
./run-with-local-cc.sh
```

Environment variables for build scripts (set before running, see `_build-env.sh`):

- `BUILD_TYPE` (default: Release)
- `TARGET` (default: sources_and_tests)
- `ARCH_TYPE` (default: x64)
- `OUTPUT_DIR` (default: build/{ARCH_TYPE}-local-cc-{BUILD_TYPE})

### CMake directly

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release .
cmake --build build --target sources_and_tests
ctest --test-dir build
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

## Testing

- Framework: Catch2 v3.5
- Tests live in `tests/vectorization.native-test/` — covers all vectorization operations
- On Windows/VS: tests build as a shared library (.dll) for VS Test Explorer
- On Linux: tests build as executables, run via `test-with-local-cc.sh` or `ctest`

## Code Style

- Formatting: `.clang-format` (LLVM-based, 120-column limit)
- Linting: `.clang-tidy` (clang-diagnostic-*, clang-analyzer-*)
- Compiler warnings: `/W4` (MSVC), `-Wall -Wextra -pedantic` (GCC)
- Defines always set: `UNICODE`, `_UNICODE`, `WIN32_LEAN_AND_MEAN`, `NOMINMAX`

## CI

- **GitHub Actions** (`.github/workflows/build-and-release.yml`): Ubuntu runner, Conan deps, GCC cross-compile, uploads
  release artifacts
- **Jenkins** (`Jenkinsfile`): Docker-based, includes SonarQube analysis and quality gate

## Data

Mesh and texture data must be unpacked from `data/data.zip` into the `data/` folder before running. The UI executable
must be run from or with access to the `data/` directory.

## CMake Submodules

The `cmake/` directory is a git submodule containing shared CMake modules (including `Catch2Tests.cmake`). Clone with
`--recurse-submodules`.
