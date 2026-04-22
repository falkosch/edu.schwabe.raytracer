# UI improvements for raytracerui

## Motivation

The current UI is minimal — a Win32+OpenGL window with basic mouse controls and stdout logging. Several
usability issues make interactive use cumbersome.

## Issues and proposed improvements

### 1. OpenMP blocks the UI thread

OpenMP parallelization shares the main thread, making the UI unresponsive during rendering. The raytracing
work should run on a background thread (or use OpenMP only on worker threads) so the UI remains interactive
during rendering.

### 2. Controls visualization

The UI should display available key and mouse controls as an overlay or panel, so users don't have to
guess what's available.

### 3. Game-style camera controls (WASD)

Replace the current modifier-key+mouse scheme with familiar 3D game controls:

| Current                            | Proposed                                               |
|------------------------------------|--------------------------------------------------------|
| SHIFT + mouse movement             | W / S — move forward / backward along camera direction |
| CTRL + mouse movement              | A / D — strafe left / right                            |
| LEFT MOUSE BUTTON + mouse movement | Free mouse movement — camera turn (yaw/pitch)          |

### 4. Camera / object / light mode switch

Add a mode switch (key toggle or UI button) between:

- **Camera mode** — WASD + mouse controls the camera
- **Object/Light pick & move mode** — click to select scene objects or lights, drag to move (like a 3D
  editor)

Currently, RIGHT MOUSE BUTTON + mouse movement moves only the first light — this should be generalized.

### 5. Configuration UI

Expose rendering parameters in the UI:

- Anti-aliasing strength
- Ray packet sizes
- Max trace depth
- Resolution / preview size
- KD-tree balancer selection

### 6. Save / export buttons

- Button to save the rendered scene to a file
- Support PNG export in addition to BMP (currently BMP only)

### 7. In-UI metrics display

In addition to stdout logging, show detailed metrics in the UI:

- Primary / secondary / shadow ray counts (hit / miss)
- Frame duration
- Scene statistics (object count, triangle count, tree depth)
- Informative text display — no charts needed

## Key files

- `sources/raytracerui/src/main.cpp` (configuration constants, scene setup)
- `sources/raytracerui/src/WindowsRaytracerUI.cpp` (Win32 message loop, input handling)
- `sources/raytracerui/src/OpenGLWindowsRaytracerUI.cpp` (OpenGL rendering)
- `sources/raytracerui/src/RaytracerUI.cpp` (raytrace orchestration)
- `sources/raytracerui/src/MessageLoopBasedUI.cpp` (message loop)
