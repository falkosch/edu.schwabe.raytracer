#include "vectorization/v_ui32_4.h"

#include <limits>

namespace vectorization {
  template <>
  v_ui32_4 MaskAll<v_ui32_4>() noexcept {
    return MaskAll<v_ui32_4::PackedType>();
  }

  template <>
  v_ui32_4 MaskX<v_ui32_4>() noexcept {
    return _mm_castps_si128(MaskX<PackedFloat4_128>());
  }

  template <>
  v_ui32_4 MaskXY<v_ui32_4>() noexcept {
    return _mm_castps_si128(MaskXY<PackedFloat4_128>());
  }

  template <>
  v_ui32_4 MaskXYZ<v_ui32_4>() noexcept {
    return _mm_castps_si128(MaskXYZ<PackedFloat4_128>());
  }

  template <>
  v_ui32_4 MaskXYZW<v_ui32_4>() noexcept {
    return _mm_castps_si128(MaskXYZW<PackedFloat4_128>());
  }

  template <>
  v_ui32_4 MaskY<v_ui32_4>() noexcept {
    return _mm_castps_si128(MaskY<PackedFloat4_128>());
  }

  template <>
  v_ui32_4 MaskYZ<v_ui32_4>() noexcept {
    return _mm_castps_si128(MaskYZ<PackedFloat4_128>());
  }

  template <>
  v_ui32_4 MaskYZW<v_ui32_4>() noexcept {
    return _mm_castps_si128(MaskYZW<PackedFloat4_128>());
  }

  template <>
  v_ui32_4 MaskZ<v_ui32_4>() noexcept {
    return _mm_castps_si128(MaskZ<PackedFloat4_128>());
  }

  template <>
  v_ui32_4 MaskZW<v_ui32_4>() noexcept {
    return _mm_castps_si128(MaskZW<PackedFloat4_128>());
  }

  template <>
  v_ui32_4 MaskW<v_ui32_4>() noexcept {
    return _mm_castps_si128(MaskW<PackedFloat4_128>());
  }

  template <>
  v_ui32_4 OneW<v_ui32_4>() noexcept {
    return _mm_srli_si128(One<v_ui32_4>().components, VectorSizes::Z * sizeof(v_ui32_4::ValueType));
  }

  template <>
  v_ui32_4 Zero<v_ui32_4>() noexcept {
    return Zero<v_ui32_4::PackedType>();
  }

  template <>
  v_ui32_4 One<v_ui32_4>() noexcept {
    return _mm_srli_epi32(
        MaskAll<v_ui32_4::PackedType>(), sizeof(v_ui32_4::ValueType) * std::numeric_limits<unsigned char>::digits - 1
    );
  }

  template <>
  v_ui32_4 Two<v_ui32_4>() noexcept {
    return _mm_slli_epi32(
        _mm_srli_epi32(
            MaskAll<v_ui32_4::PackedType>(),
            sizeof(v_ui32_4::ValueType) * std::numeric_limits<unsigned char>::digits - 1
        ),
        1
    );
  }
}
