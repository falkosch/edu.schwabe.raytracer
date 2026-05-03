#include "vectorization/v_f64_4/constants.h"

namespace vectorization {
  template <>
  v_f64_4 Zero<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Zero<v_f64_4::PackedType>();
#else
    return {{Zero<v_f64_4::HalfPackedType>(), Zero<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 One<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return One<v_f64_4::PackedType>();
#else
    return {{One<v_f64_4::HalfPackedType>(), One<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 Two<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Two<v_f64_4::PackedType>();
#else
    return {{Two<v_f64_4::HalfPackedType>(), Two<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 Half<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Half<v_f64_4::PackedType>();
#else
    return {{Half<v_f64_4::HalfPackedType>(), Half<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 NegativeOne<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return NegativeOne<v_f64_4::PackedType>();
#else
    return {{NegativeOne<v_f64_4::HalfPackedType>(), NegativeOne<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 NegativeTwo<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return NegativeTwo<v_f64_4::PackedType>();
#else
    return {{NegativeTwo<v_f64_4::HalfPackedType>(), NegativeTwo<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 NegativeZero<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return NegativeZero<v_f64_4::PackedType>();
#else
    return {{NegativeZero<v_f64_4::HalfPackedType>(), NegativeZero<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 Epsilon<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Epsilon<v_f64_4::PackedType>();
#else
    return {{Epsilon<v_f64_4::HalfPackedType>(), Epsilon<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 Infinity<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Infinity<v_f64_4::PackedType>();
#else
    return {{Infinity<v_f64_4::HalfPackedType>(), Infinity<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 NegativeInfinity<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return NegativeInfinity<v_f64_4::PackedType>();
#else
    return {{NegativeInfinity<v_f64_4::HalfPackedType>(), NegativeInfinity<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 NotANumber<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return NotANumber<v_f64_4::PackedType>();
#else
    return {{NotANumber<v_f64_4::HalfPackedType>(), NotANumber<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 OneHalf<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return OneHalf<v_f64_4::PackedType>();
#else
    return {{OneHalf<v_f64_4::HalfPackedType>(), OneHalf<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 Pi<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Pi<v_f64_4::PackedType>();
#else
    return {{Pi<v_f64_4::HalfPackedType>(), Pi<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 ReciprocalPi<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return ReciprocalPi<v_f64_4::PackedType>();
#else
    return {{ReciprocalPi<v_f64_4::HalfPackedType>(), ReciprocalPi<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 DegreeToRadian<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return DegreeToRadian<v_f64_4::PackedType>();
#else
    return {{DegreeToRadian<v_f64_4::HalfPackedType>(), DegreeToRadian<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 RadianToDegree<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return RadianToDegree<v_f64_4::PackedType>();
#else
    return {{RadianToDegree<v_f64_4::HalfPackedType>(), RadianToDegree<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 RadianToUniform<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return RadianToUniform<v_f64_4::PackedType>();
#else
    return {{RadianToUniform<v_f64_4::HalfPackedType>(), RadianToUniform<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 SelfOcclusionEpsilon<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return SelfOcclusionEpsilon<v_f64_4::PackedType>();
#else
    return {{SelfOcclusionEpsilon<v_f64_4::HalfPackedType>(), SelfOcclusionEpsilon<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 Sin45<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return Sin45<v_f64_4::PackedType>();
#else
    return {{Sin45<v_f64_4::HalfPackedType>(), Sin45<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 MaskAll<v_f64_4>() noexcept {
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
    return MaskAll<v_f64_4::PackedType>();
#else
    return {{MaskAll<v_f64_4::HalfPackedType>(), MaskAll<v_f64_4::HalfPackedType>()}};
#endif
  }

  template <>
  v_f64_4 MaskNone<v_f64_4>() noexcept {
    return Zero<v_f64_4>();
  }
}
