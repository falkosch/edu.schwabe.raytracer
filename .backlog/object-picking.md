# Object picking

## Motivation

The interaction modes (Object/Light) currently always affect the last object or light in the scene. Users
should be able to click to select a specific scene object or light, then drag to manipulate it.

## Proposed improvement

- Ray-cast from the clicked screen pixel through the scene using the camera's view frustum
- Find the nearest intersection and identify the hit `SceneObject`
- Store the selected object/light index and highlight it visually (e.g., in the metrics panel)
- Drag operations apply to the selected object/light instead of always the last one
- For light selection, pick the light nearest to the intersection point
- Show detailed per-object info in the metrics panel for the selected object (e.g., mesh KD-tree depth/node count, face
  count, bounding box size)
