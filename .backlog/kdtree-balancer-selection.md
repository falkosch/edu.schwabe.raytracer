# KD-tree balancer selection

## Motivation

The KD-tree balancer strategy is currently hardcoded in `main.cpp` (`FixedIterationsSAHKDTreeBalancer`).
Multiple balancer implementations exist (SAH variants, median, arithmetic mean, max axis, rotating axis,
brute force SAH, sampling) but there is no way to switch between them at runtime.

## Proposed improvement

Expose a balancer selection combo box in the ImGui configuration panel. Changing the balancer should rebuild
the scene's culling tree and trigger a re-render.
