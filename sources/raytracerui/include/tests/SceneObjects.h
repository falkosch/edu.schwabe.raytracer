#pragma once

#include "Placement.h"
#include "SceneMaterial.h"

#include <memory>
#include <string>

namespace raytracerui {
  using namespace vectorization;
  using namespace raytracer;

  // A scene object owning its form (Sphere, Box, Plane)
  std::unique_ptr<SceneObject> createSceneObject(
      std::string id, std::unique_ptr<const Form> form, const SceneMaterial &material, const Placement &placement
  );

  // A scene object borrowing its form, e.g. a mesh owned by Resources
  std::unique_ptr<SceneObject> createSceneObject(
      std::string id, const Form *form, const SceneMaterial &material, const Placement &placement
  );

  // Scale that fits the mesh's largest extent into the given size (meshes load with their smallest extent as 1)
  Float fitMeshScale(const Form &mesh, Float size);
}
