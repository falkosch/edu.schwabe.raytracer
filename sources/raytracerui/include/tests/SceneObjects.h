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

  // A spherical area light with its emitter: an emissive sphere of the radiance (what the camera sees), marked as the
  // light's emitter, and a light at its center that lights the scene as the sphere does, with the intensity
  // radiance * pi * radius^2 and quadratic falloff
  void addSphereLight(Scene &scene, std::string id, const Float4 &center, Float radius, const Float4 &radiance);

  // A box light with its emitter: an emissive box of the radiance (what the camera sees, on all its faces) with the
  // half extents, rotated by the angles in degrees (as Placement::rotation) about its center, marked as the light's
  // emitter, and a box light of the same placement whose emitting faces (LightInfo::faceBit) light the scene with that
  // radiance
  void addBoxLight(
      Scene &scene, std::string id, const Float4 &center, const Float3 &halfExtents, const Float3 &rotation,
      const Float4 &radiance, UInt_32 emittingFaces = LightInfo::AllFaces
  );
}
