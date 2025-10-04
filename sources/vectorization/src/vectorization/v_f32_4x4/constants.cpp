#include "vectorization/v_f32_4x4.h"

#include "vectorization/constants.h"

namespace vectorization {
  template <>
  const v_f32_4x4 Zero<v_f32_4x4>() noexcept {
    return v_f32_4x4(Zero<v_f32_4x4::PackedType>());
  }
}
