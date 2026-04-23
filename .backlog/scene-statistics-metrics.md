# Scene statistics in metrics panel

## Motivation

The metrics panel (F2) shows ray counts and frame duration but no information about the scene itself.

## Proposed improvement

Add to the metrics panel:

- Scene object count (`scene->getSceneObjects().size()`)
- Light count (`scene->getLights().size()`)
- Total triangle/face count (sum across all mesh objects)
- KD-tree depth and node count
