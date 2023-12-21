#pragma once

#include <primitives.h>
#include <vectorization.h>

#include <vector>

namespace raytracer {
  const primitives::AxisAlignedBoundingBox computeBounding(const std::vector<vectorization::Float4> &vertices) noexcept;

  const primitives::AxisAlignedBoundingBox computeStandardMesh(std::vector<vectorization::Float4> &vertices) noexcept;
}
