#include "vectorization/v_f32_8/constants.h"

namespace vectorization {
  template <>
  v_f32_8 Zero<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Zero<v_f32_8::PackedType>();
#else
    return {{Zero<v_f32_8::HalfPackedType>(), Zero<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 One<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return One<v_f32_8::PackedType>();
#else
    return {{One<v_f32_8::HalfPackedType>(), One<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 Two<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Two<v_f32_8::PackedType>();
#else
    return {{Two<v_f32_8::HalfPackedType>(), Two<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 Half<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Half<v_f32_8::PackedType>();
#else
    return {{Half<v_f32_8::HalfPackedType>(), Half<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 NegativeOne<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return NegativeOne<v_f32_8::PackedType>();
#else
    return {{NegativeOne<v_f32_8::HalfPackedType>(), NegativeOne<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 NegativeTwo<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return NegativeTwo<v_f32_8::PackedType>();
#else
    return {{NegativeTwo<v_f32_8::HalfPackedType>(), NegativeTwo<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 NegativeZero<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return NegativeZero<v_f32_8::PackedType>();
#else
    return {{NegativeZero<v_f32_8::HalfPackedType>(), NegativeZero<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 Epsilon<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Epsilon<v_f32_8::PackedType>();
#else
    return {{Epsilon<v_f32_8::HalfPackedType>(), Epsilon<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 Infinity<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Infinity<v_f32_8::PackedType>();
#else
    return {{Infinity<v_f32_8::HalfPackedType>(), Infinity<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 NegativeInfinity<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return NegativeInfinity<v_f32_8::PackedType>();
#else
    return {{NegativeInfinity<v_f32_8::HalfPackedType>(), NegativeInfinity<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 NotANumber<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return NotANumber<v_f32_8::PackedType>();
#else
    return {{NotANumber<v_f32_8::HalfPackedType>(), NotANumber<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 OneHalf<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return OneHalf<v_f32_8::PackedType>();
#else
    return {{OneHalf<v_f32_8::HalfPackedType>(), OneHalf<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 Pi<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Pi<v_f32_8::PackedType>();
#else
    return {{Pi<v_f32_8::HalfPackedType>(), Pi<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 ReciprocalPi<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return ReciprocalPi<v_f32_8::PackedType>();
#else
    return {{ReciprocalPi<v_f32_8::HalfPackedType>(), ReciprocalPi<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 DegreeToRadian<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return DegreeToRadian<v_f32_8::PackedType>();
#else
    return {{DegreeToRadian<v_f32_8::HalfPackedType>(), DegreeToRadian<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 RadianToDegree<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return RadianToDegree<v_f32_8::PackedType>();
#else
    return {{RadianToDegree<v_f32_8::HalfPackedType>(), RadianToDegree<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 RadianToUniform<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return RadianToUniform<v_f32_8::PackedType>();
#else
    return {{RadianToUniform<v_f32_8::HalfPackedType>(), RadianToUniform<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 SelfOcclusionEpsilon<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return SelfOcclusionEpsilon<v_f32_8::PackedType>();
#else
    return {{SelfOcclusionEpsilon<v_f32_8::HalfPackedType>(), SelfOcclusionEpsilon<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 Sin45<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Sin45<v_f32_8::PackedType>();
#else
    return {{Sin45<v_f32_8::HalfPackedType>(), Sin45<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 MaskAll<v_f32_8>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return MaskAll<v_f32_8::PackedType>();
#else
    return {{MaskAll<v_f32_8::HalfPackedType>(), MaskAll<v_f32_8::HalfPackedType>()}};
#endif
  }

  template <>
  v_f32_8 MaskNone<v_f32_8>() noexcept {
    return Zero<v_f32_8>();
  }
}
