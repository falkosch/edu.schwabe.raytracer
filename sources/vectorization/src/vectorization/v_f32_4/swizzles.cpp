#include "vectorization/v_f32_4/swizzles.h"

namespace vectorization {
  v_f32_4 xxxx(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(v);
  }

  v_f32_4 yyyy(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(v);
  }

  v_f32_4 zzzz(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
  }

  v_f32_4 wwww(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(v);
  }

  v_f32_4 xxxz(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::Z>(v);
  }

  v_f32_4 xxyy(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(v);
  }

  v_f32_4 xxzz(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(v);
  }

  v_f32_4 xyxy(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v);
  }

  v_f32_4 xzzz(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
  }

  v_f32_4 yxxy(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(v);
  }

  v_f32_4 yxwz(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(v);
  }

  v_f32_4 yyww(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(v);
  }

  v_f32_4 yzxw(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::X, VectorIndices::W>(v);
  }

  v_f32_4 yzwx(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::X>(v);
  }

  v_f32_4 zxyw(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::X, VectorIndices::Y, VectorIndices::W>(v);
  }

  v_f32_4 zzyy(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(v);
  }

  v_f32_4 zzww(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v);
  }

  v_f32_4 zwxy(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(v);
  }

  v_f32_4 zwzw(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v);
  }

  v_f32_4 wxyz(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::X, VectorIndices::Y, VectorIndices::Z>(v);
  }

  v_f32_4 wzyx(const v_f32_4 &v) noexcept {
    return swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(v);
  }
}
