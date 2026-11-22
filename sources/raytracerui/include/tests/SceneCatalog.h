#pragma once

#include "SceneEntry.h"

#include <optional>
#include <span>
#include <string_view>

namespace raytracerui {
  // All scenes in the order of the scene dropdown; the first one is the default.
  std::span<const SceneEntry> sceneCatalog() noexcept;

  // Index of the scene with the given id in sceneCatalog().
  std::optional<ASizeT> findScene(std::string_view id) noexcept;
}
