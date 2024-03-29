#include "vectorization/v_f32_4x4.h"

namespace vectorization {
#pragma region Constructors
  //{ Constructors

  v_f32_4x4::v_f32_4x4() noexcept : soaX(), soaY(), soaZ(), soaW() {
  }

  v_f32_4x4::v_f32_4x4(
      const PackedType &soaXIn, const PackedType &soaYIn, const PackedType &soaZIn, const PackedType &soaWIn
  ) noexcept
      : soaX(soaXIn), soaY(soaYIn), soaZ(soaZIn), soaW(soaWIn) {
  }

  v_f32_4x4::v_f32_4x4(
      const SOAVectorType &soaXIn, const SOAVectorType &soaYIn, const SOAVectorType &soaZIn, const SOAVectorType &soaWIn
  ) noexcept
      : soaX(soaXIn), soaY(soaYIn), soaZ(soaZIn), soaW(soaWIn) {
  }

  // broadcasting aos vector into its soa vectors
  v_f32_4x4::v_f32_4x4(const AOSVectorType &aos) noexcept : soaX(), soaY(), soaZ(), soaW() {
    auto pairXYXY = xyxy(aos);
    auto pairZWZW = zwzw(aos);
    soaX = xxzz(pairXYXY);
    soaY = yyww(pairXYXY);
    soaZ = xxzz(pairZWZW);
    soaW = yyww(pairZWZW);
  }

  v_f32_4x4::v_f32_4x4(const ValueType *const m) noexcept
      : soaX(m + VectorIndices::X * SIZE_SOA), soaY(m + VectorIndices::Y * SIZE_SOA),
        soaZ(m + VectorIndices::Z * SIZE_SOA), soaW(m + VectorIndices::W * SIZE_SOA) {
  }

  //}
#pragma endregion

#pragma region Accessors
  //{ Accessors

  template <>
  const v_f32_4x4::SOAVectorType soa<VectorIndices::X>(const v_f32_4x4 &m) noexcept {
    return m.soaX;
  }

  template <>
  const v_f32_4x4::SOAVectorType soa<VectorIndices::Y>(const v_f32_4x4 &m) noexcept {
    return m.soaY;
  }

  template <>
  const v_f32_4x4::SOAVectorType soa<VectorIndices::Z>(const v_f32_4x4 &m) noexcept {
    return m.soaZ;
  }

  template <>
  const v_f32_4x4::SOAVectorType soa<VectorIndices::W>(const v_f32_4x4 &m) noexcept {
    return m.soaW;
  }

  template <>
  void soa<VectorIndices::X>(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept {
    m.soaX = v;
  }

  template <>
  void soa<VectorIndices::Y>(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept {
    m.soaY = v;
  }

  template <>
  void soa<VectorIndices::Z>(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept {
    m.soaZ = v;
  }

  template <>
  void soa<VectorIndices::W>(v_f32_4x4 &m, const v_f32_4x4::SOAVectorType &v) noexcept {
    m.soaW = v;
  }

  //}
#pragma endregion

#pragma region Operators
  //{ Operators

  const v_f32_4x4 operator+(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX + b.soaX, a.soaY + b.soaY, a.soaZ + b.soaZ, a.soaW + b.soaW);
  }

  const v_f32_4x4 operator-(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX - b.soaX, a.soaY - b.soaY, a.soaZ - b.soaZ, a.soaW - b.soaW);
  }

  const v_f32_4x4 operator*(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX * b.soaX, a.soaY * b.soaY, a.soaZ * b.soaZ, a.soaW * b.soaW);
  }

  const v_f32_4x4 operator/(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX / b.soaX, a.soaY / b.soaY, a.soaZ / b.soaZ, a.soaW / b.soaW);
  }

  const v_f32_4x4 operator%(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX % b.soaX, a.soaY % b.soaY, a.soaZ % b.soaZ, a.soaW % b.soaW);
  }

  const v_f32_4x4 operator&(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX & b.soaX, a.soaY & b.soaY, a.soaZ & b.soaZ, a.soaW & b.soaW);
  }

  const v_f32_4x4 operator|(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX | b.soaX, a.soaY | b.soaY, a.soaZ | b.soaZ, a.soaW | b.soaW);
  }

  const v_f32_4x4 operator^(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX ^ b.soaX, a.soaY ^ b.soaY, a.soaZ ^ b.soaZ, a.soaW ^ b.soaW);
  }

  const v_f32_4x4 operator+=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX += b.soaX;
    a.soaY += b.soaY;
    a.soaZ += b.soaZ;
    a.soaW += b.soaW;
    return a;
  }

  const v_f32_4x4 operator-=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX -= b.soaX;
    a.soaY -= b.soaY;
    a.soaZ -= b.soaZ;
    a.soaW -= b.soaW;
    return a;
  }

  const v_f32_4x4 operator*=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX *= b.soaX;
    a.soaY *= b.soaY;
    a.soaZ *= b.soaZ;
    a.soaW *= b.soaW;
    return a;
  }

  const v_f32_4x4 operator/=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX /= b.soaX;
    a.soaY /= b.soaY;
    a.soaZ /= b.soaZ;
    a.soaW /= b.soaW;
    return a;
  }

  const v_f32_4x4 operator%=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX %= b.soaX;
    a.soaY %= b.soaY;
    a.soaZ %= b.soaZ;
    a.soaW %= b.soaW;
    return a;
  }

  const v_f32_4x4 operator&=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX &= b.soaX;
    a.soaY &= b.soaY;
    a.soaZ &= b.soaZ;
    a.soaW &= b.soaW;
    return a;
  }

  const v_f32_4x4 operator|=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX |= b.soaX;
    a.soaY |= b.soaY;
    a.soaZ |= b.soaZ;
    a.soaW |= b.soaW;
    return a;
  }

  const v_f32_4x4 operator^=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX ^= b.soaX;
    a.soaY ^= b.soaY;
    a.soaZ ^= b.soaZ;
    a.soaW ^= b.soaW;
    return a;
  }

  //}
#pragma endregion

#pragma region Special vector operations
  //{ Special vector operations

  const v_f32_4x4 transpose(const v_f32_4x4 &m) noexcept {
    // adapted from glm::detail::sse_transpose_ps
    // http://glm.g-truc.net/
    auto t0 = xy_xy(m.soaX, m.soaY);
    auto t2 = zw_zw(m.soaX, m.soaY);
    auto t1 = xy_xy(m.soaZ, m.soaW);
    auto t3 = zw_zw(m.soaZ, m.soaW);
    return v_f32_4x4(xz_xz(t0, t1), yw_yw(t0, t1), xz_xz(t2, t3), yw_yw(t2, t3));
  }

  //}
#pragma endregion
}