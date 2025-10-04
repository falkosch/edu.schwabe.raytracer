#pragma once

#include "api.h"

#include "vectorization/constants.h"

namespace vectorization {
  template <>
  m_f32_4x4 Zero<m_f32_4x4>() noexcept;

  template <>
  m_f32_4x4 Identity<m_f32_4x4>() noexcept;
}
