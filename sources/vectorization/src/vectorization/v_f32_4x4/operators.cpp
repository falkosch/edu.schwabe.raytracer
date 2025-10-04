#include "vectorization/v_f32_4x4.h"

namespace vectorization {
  v_f32_4x4 operator+(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX + b.soaX, a.soaY + b.soaY, a.soaZ + b.soaZ, a.soaW + b.soaW);
  }

  v_f32_4x4 operator-(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX - b.soaX, a.soaY - b.soaY, a.soaZ - b.soaZ, a.soaW - b.soaW);
  }

  v_f32_4x4 operator*(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX * b.soaX, a.soaY * b.soaY, a.soaZ * b.soaZ, a.soaW * b.soaW);
  }

  v_f32_4x4 operator/(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX / b.soaX, a.soaY / b.soaY, a.soaZ / b.soaZ, a.soaW / b.soaW);
  }

  v_f32_4x4 operator%(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX % b.soaX, a.soaY % b.soaY, a.soaZ % b.soaZ, a.soaW % b.soaW);
  }

  v_f32_4x4 operator&(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX & b.soaX, a.soaY & b.soaY, a.soaZ & b.soaZ, a.soaW & b.soaW);
  }

  v_f32_4x4 operator|(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX | b.soaX, a.soaY | b.soaY, a.soaZ | b.soaZ, a.soaW | b.soaW);
  }

  v_f32_4x4 operator^(const v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    return v_f32_4x4(a.soaX ^ b.soaX, a.soaY ^ b.soaY, a.soaZ ^ b.soaZ, a.soaW ^ b.soaW);
  }

  v_f32_4x4 operator+=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX += b.soaX;
    a.soaY += b.soaY;
    a.soaZ += b.soaZ;
    a.soaW += b.soaW;
    return a;
  }

  v_f32_4x4 operator-=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX -= b.soaX;
    a.soaY -= b.soaY;
    a.soaZ -= b.soaZ;
    a.soaW -= b.soaW;
    return a;
  }

  v_f32_4x4 operator*=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX *= b.soaX;
    a.soaY *= b.soaY;
    a.soaZ *= b.soaZ;
    a.soaW *= b.soaW;
    return a;
  }

  v_f32_4x4 operator/=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX /= b.soaX;
    a.soaY /= b.soaY;
    a.soaZ /= b.soaZ;
    a.soaW /= b.soaW;
    return a;
  }

  v_f32_4x4 operator%=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX %= b.soaX;
    a.soaY %= b.soaY;
    a.soaZ %= b.soaZ;
    a.soaW %= b.soaW;
    return a;
  }

  v_f32_4x4 operator&=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX &= b.soaX;
    a.soaY &= b.soaY;
    a.soaZ &= b.soaZ;
    a.soaW &= b.soaW;
    return a;
  }

  v_f32_4x4 operator|=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX |= b.soaX;
    a.soaY |= b.soaY;
    a.soaZ |= b.soaZ;
    a.soaW |= b.soaW;
    return a;
  }

  v_f32_4x4 operator^=(v_f32_4x4 &a, const v_f32_4x4 &b) noexcept {
    a.soaX ^= b.soaX;
    a.soaY ^= b.soaY;
    a.soaZ ^= b.soaZ;
    a.soaW ^= b.soaW;
    return a;
  }
}
