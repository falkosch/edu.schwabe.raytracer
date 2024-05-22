#include "vectorization/m_f32_4x4.h"

#include "vectorization/constants.h"

namespace vectorization {
  template <>
  const m_f32_4x4 Zero<m_f32_4x4>() noexcept {
    return m_f32_4x4(Zero<m_f32_4x4::PackedType>());
  }

  template <>
  const m_f32_4x4 Identity<m_f32_4x4>() noexcept {
    return m_f32_4x4();
  }
}
