#pragma once

#include "type.h"

namespace vectorization {

  template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
  v_ui64_4 swizzle(const v_ui64_4 &v) noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX2
    return _mm256_permute4x64_epi64(v.components, _MM_SHUFFLE(W, Z, Y, X));
#else
    return v_ui64_4(v[X], v[Y], v[Z], v[W]);
#endif
  }

  template <>
  v_ui64_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_ui64_4 &v) noexcept;

  v_ui64_4 xxxx(const v_ui64_4 &v) noexcept;
  v_ui64_4 yyyy(const v_ui64_4 &v) noexcept;
  v_ui64_4 zzzz(const v_ui64_4 &v) noexcept;
  v_ui64_4 wwww(const v_ui64_4 &v) noexcept;
}
