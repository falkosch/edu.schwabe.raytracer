#include "vectorization/constants.h"
#include "../stdafx.h"

#include <limits.h>

namespace vectorization
{

#pragma region Mask...()
    //{ Mask...()

    template <typename V>
    const V MaskAll()
    {
        return static_cast<V>(static_cast<V>(0) - static_cast<V>(1));
    }

    template const bool MaskAll<bool>();
    template const long MaskAll<long>();
    template const unsigned long MaskAll<unsigned long>();
    template const Int_8 MaskAll<Int_8>();
    template const UInt_8 MaskAll<UInt_8>();
    template const Int_16 MaskAll<Int_16>();
    template const UInt_16 MaskAll<UInt_16>();
    template const Int_32 MaskAll<Int_32>();
    template const UInt_32 MaskAll<UInt_32>();
    template const Int_64 MaskAll<Int_64>();
    template const UInt_64 MaskAll<UInt_64>();

    template <>
    const PackedInts_128 MaskAll<PackedInts_128>()
    {
        return _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128());
    }

    template <>
    const PackedFloat4_128 MaskAll<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(MaskAll<PackedInts_128>());
    }

    template <>
    const PackedFloat4_128 MaskX<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_srli_si128(MaskAll<PackedInts_128>(), VectorSizes::Z * sizeof(Float_32)));
    }

    template <>
    const PackedFloat4_128 MaskXY<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_srli_si128(MaskAll<PackedInts_128>(), VectorSizes::Y * sizeof(Float_32)));
    }

    template <>
    const PackedFloat4_128 MaskXYZ<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_srli_si128(MaskAll<PackedInts_128>(), VectorSizes::X * sizeof(Float_32)));
    }

    template <>
    const PackedFloat4_128 MaskXYZW<PackedFloat4_128>()
    {
        return MaskAll<PackedFloat4_128>();
    }

    template <>
    const PackedFloat4_128 MaskY<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_slli_si128(
            _mm_castps_si128(MaskX<PackedFloat4_128>()),
            VectorSizes::X * sizeof(Float_32)));
    }

    template <>
    const PackedFloat4_128 MaskYZ<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_slli_si128(
            _mm_castps_si128(MaskXY<PackedFloat4_128>()),
            VectorSizes::X * sizeof(Float_32)));
    }

    template <>
    const PackedFloat4_128 MaskYZW<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_slli_si128(MaskAll<PackedInts_128>(), VectorSizes::X * sizeof(Float_32)));
    }

    template <>
    const PackedFloat4_128 MaskZ<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_srli_si128(
            _mm_castps_si128(MaskW<PackedFloat4_128>()),
            VectorSizes::X * sizeof(Float_32)));
    }

    template <>
    const PackedFloat4_128 MaskZW<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_slli_si128(MaskAll<PackedInts_128>(), VectorSizes::Y * sizeof(Float_32)));
    }

    template <>
    const PackedFloat4_128 MaskW<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_slli_si128(MaskAll<PackedInts_128>(), VectorSizes::Z * sizeof(Float_32)));
    }

    template <>
    const PackedFloat2_128 MaskAll<PackedFloat2_128>()
    {
        return _mm_castsi128_pd(MaskAll<PackedInts_128>());
    }

    template <>
    const PackedFloat2_128 MaskX<PackedFloat2_128>()
    {
        return _mm_castsi128_pd(_mm_srli_si128(MaskAll<PackedInts_128>(), VectorSizes::X * sizeof(Float_64)));
    }

    template <>
    const PackedFloat2_128 MaskXY<PackedFloat2_128>()
    {
        return MaskAll<PackedFloat2_128>();
    }

    template <>
    const PackedFloat2_128 MaskY<PackedFloat2_128>()
    {
        return _mm_castsi128_pd(_mm_slli_si128(MaskAll<PackedInts_128>(), VectorSizes::X * sizeof(Float_64)));
    }

    template <>
    const Float_32 MaskAll<Float_32>()
    {
        return _mm_cvtss_f32(MaskAll<PackedFloat4_128>());
    }

    template <>
    const Float_64 MaskAll<Float_64>()
    {
        return _mm_cvtsd_f64(MaskAll<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region Zero
    //{ Zero

    template <typename T>
    const T Zero<T>()
    {
        return T();
    }

    template const bool Zero<bool>();
    template const long Zero<long>();
    template const unsigned long Zero<unsigned long>();
    template const Int_8 Zero<Int_8>();
    template const UInt_8 Zero<UInt_8>();
    template const Int_16 Zero<Int_16>();
    template const UInt_16 Zero<UInt_16>();
    template const Int_32 Zero<Int_32>();
    template const UInt_32 Zero<UInt_32>();
    template const Int_64 Zero<Int_64>();
    template const UInt_64 Zero<UInt_64>();

    template <>
    const PackedInts_128 Zero<PackedInts_128>()
    {
        return _mm_setzero_si128();
    }

    template <>
    const PackedFloat4_128 Zero<PackedFloat4_128>()
    {
        return _mm_setzero_ps();
    }

    template <>
    const PackedFloat2_128 Zero<PackedFloat2_128>()
    {
        return _mm_setzero_pd();
    }

    template <>
    const Float_32 Zero<Float_32>()
    {
        return _mm_cvtss_f32(Zero<PackedFloat4_128>());
    }

    template <>
    const Float_64 Zero<Float_64>()
    {
        return _mm_cvtsd_f64(Zero<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region Half
    //{ Half

    template <>
    const PackedFloat4_128 Half<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(MaskAll<PackedInts_128>(), 26), 2));
    }

    template <>
    const PackedFloat2_128 Half<PackedFloat2_128>()
    {
        return _mm_castsi128_pd(_mm_srli_epi64(_mm_slli_epi64(MaskAll<PackedInts_128>(), 55), 2));
    }

    template <>
    const Float_32 Half<Float_32>()
    {
        return _mm_cvtss_f32(Half<PackedFloat4_128>());
    }

    template <>
    const Float_64 Half<Float_64>()
    {
        return _mm_cvtsd_f64(Half<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region One
    //{ One

    template <typename T>
    const T One<T>()
    {
        return static_cast<T>(1);
    }

    template const bool One<bool>();
    template const long One<long>();
    template const unsigned long One<unsigned long>();
    template const Int_8 One<Int_8>();
    template const UInt_8 One<UInt_8>();
    template const Int_16 One<Int_16>();
    template const UInt_16 One<UInt_16>();
    template const Int_32 One<Int_32>();
    template const UInt_32 One<UInt_32>();
    template const Int_64 One<Int_64>();
    template const UInt_64 One<UInt_64>();

    template <>
    const PackedFloat4_128 One<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(MaskAll<PackedInts_128>(), 25), 2));
    }

    template <>
    const PackedFloat2_128 One<PackedFloat2_128>()
    {
        return _mm_castsi128_pd(_mm_srli_epi64(_mm_slli_epi64(MaskAll<PackedInts_128>(), 54), 2));
    }

    template <>
    const Float_32 One<Float_32>()
    {
        return _mm_cvtss_f32(One<PackedFloat4_128>());
    }

    template <>
    const Float_64 One<Float_64>()
    {
        return _mm_cvtsd_f64(One<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region OneX
    //{ OneX

    template <>
    const PackedFloat4_128 OneX<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_srli_si128(
            _mm_castps_si128(One<PackedFloat4_128>()),
            VectorSizes::Z * sizeof(Float_32)));
    }

    template <>
    const PackedFloat2_128 OneX<PackedFloat2_128>()
    {
        return _mm_castsi128_pd(_mm_srli_si128(
            _mm_castpd_si128(One<PackedFloat2_128>()),
            VectorSizes::X * sizeof(Float_64)));
    }

    //}
#pragma endregion

#pragma region OneY
    //{ OneW

    template <>
    const PackedFloat4_128 OneY<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_slli_si128(
            _mm_srli_si128(_mm_castps_si128(One<PackedFloat4_128>()), VectorSizes::Z * sizeof(Float_32)),
            VectorSizes::X * sizeof(Float_32)));
    }

    template <>
    const PackedFloat2_128 OneY<PackedFloat2_128>()
    {
        return _mm_castsi128_pd(_mm_slli_si128(
            _mm_castpd_si128(One<PackedFloat2_128>()),
            VectorSizes::X * sizeof(Float_64)));
    }

    //}
#pragma endregion

#pragma region OneZ
    //{ OneW

    template <>
    const PackedFloat4_128 OneZ<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_srli_si128(
            _mm_slli_si128(_mm_castps_si128(One<PackedFloat4_128>()), VectorSizes::Z * sizeof(Float_32)),
            VectorSizes::X * sizeof(Float_32)));
    }

    //}
#pragma endregion

#pragma region OneW
    //{ OneW

    template <>
    const PackedFloat4_128 OneW<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_slli_si128(
            _mm_castps_si128(One<PackedFloat4_128>()),
            VectorSizes::Z * sizeof(Float_32)));
    }

    //}
#pragma endregion

#pragma region OneHalf
    //{ OneHalf

    template <>
    const PackedFloat4_128 OneHalf<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(MaskAll<PackedInts_128>(), 24), 2));
    }

    template <>
    const PackedFloat2_128 OneHalf<PackedFloat2_128>()
    {
        return _mm_castsi128_pd(_mm_srli_epi64(_mm_slli_epi64(MaskAll<PackedInts_128>(), 53), 2));
    }

    template <>
    const Float_32 OneHalf<Float_32>()
    {
        return _mm_cvtss_f32(OneHalf<PackedFloat4_128>());
    }

    template <>
    const Float_64 OneHalf<Float_64>()
    {
        return _mm_cvtsd_f64(OneHalf<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region Two
    //{ Two

    template <typename T>
    const T Two<T>()
    {
        return static_cast<T>(2);
    }

    template const long Two<long>();
    template const unsigned long Two<unsigned long>();
    template const Int_8 Two<Int_8>();
    template const UInt_8 Two<UInt_8>();
    template const Int_16 Two<Int_16>();
    template const UInt_16 Two<UInt_16>();
    template const Int_32 Two<Int_32>();
    template const UInt_32 Two<UInt_32>();
    template const Int_64 Two<Int_64>();
    template const UInt_64 Two<UInt_64>();

    template <>
    const PackedFloat4_128 Two<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_srli_epi32(
            _mm_slli_epi32(MaskAll<PackedInts_128>(), sizeof(Int_32) * CHAR_BIT - 1),
            1));
    }

    template <>
    const PackedFloat2_128 Two<PackedFloat2_128>()
    {
        return _mm_castsi128_pd(_mm_srli_epi64(
            _mm_slli_epi64(MaskAll<PackedInts_128>(), sizeof(Int_64) * CHAR_BIT - 1),
            1));
    }

    template <>
    const Float_32 Two<Float_32>()
    {
        return _mm_cvtss_f32(Two<PackedFloat4_128>());
    }

    template <>
    const Float_64 Two<Float_64>()
    {
        return _mm_cvtsd_f64(Two<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region NegZero
    //{ NegZero

    template <>
    const PackedFloat4_128 NegZero<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_slli_epi32(
            MaskAll<PackedInts_128>(),
            sizeof(Int_32) * CHAR_BIT - 1));
    }

    template <>
    const PackedFloat2_128 NegZero<PackedFloat2_128>()
    {
        return _mm_castsi128_pd(_mm_slli_epi64(
            MaskAll<PackedInts_128>(),
            sizeof(Int_64) * CHAR_BIT - 1));
    }

    template <>
    const Float_32 NegZero<Float_32>()
    {
        return _mm_cvtss_f32(NegZero<PackedFloat4_128>());
    }

    template <>
    const Float_64 NegZero<Float_64>()
    {
        return _mm_cvtsd_f64(NegZero<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region NegOne
    //{ NegOne

    template <typename T>
    const T NegOne<T>()
    {
        return static_cast<T>(-1);
    }

    template const long NegOne<long>();
    template const Int_8 NegOne<Int_8>();
    template const Int_16 NegOne<Int_16>();
    template const Int_32 NegOne<Int_32>();
    template const Int_64 NegOne<Int_64>();

    template <>
    const PackedFloat4_128 NegOne<PackedFloat4_128>()
    {
        return _mm_or_ps(One<PackedFloat4_128>(), NegZero<PackedFloat4_128>());
    }

    template <>
    const PackedFloat2_128 NegOne<PackedFloat2_128>()
    {
        return _mm_or_pd(One<PackedFloat2_128>(), NegZero<PackedFloat2_128>());
    }

    template <>
    const Float_32 NegOne<Float_32>()
    {
        return _mm_cvtss_f32(NegOne<PackedFloat4_128>());
    }

    template <>
    const Float_64 NegOne<Float_64>()
    {
        return _mm_cvtsd_f64(NegOne<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region NegTwo
    //{ NegTwo

    template <typename T>
    const T NegTwo<T>()
    {
        return static_cast<T>(-2);
    }

    template const long NegTwo<long>();
    template const Int_8 NegTwo<Int_8>();
    template const Int_16 NegTwo<Int_16>();
    template const Int_32 NegTwo<Int_32>();
    template const Int_64 NegTwo<Int_64>();

    template <>
    const PackedFloat4_128 NegTwo<PackedFloat4_128>()
    {
        return _mm_castsi128_ps(_mm_slli_epi32(
            MaskAll<PackedInts_128>(),
            sizeof(Int_32) * CHAR_BIT - 2));
    }

    template <>
    const PackedFloat2_128 NegTwo<PackedFloat2_128>()
    {
        return _mm_castsi128_pd(_mm_slli_epi64(
            MaskAll<PackedInts_128>(),
            sizeof(Int_64) * CHAR_BIT - 2));
    }

    template <>
    const Float_32 NegTwo<Float_32>()
    {
        return _mm_cvtss_f32(NegTwo<PackedFloat4_128>());
    }

    template <>
    const Float_64 NegTwo<Float_64>()
    {
        return _mm_cvtsd_f64(NegTwo<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region Epsilon
    //{ Epsilon

    template <typename T>
    inline const T benchmarkMachineEpsilon() {
        T f = One<T>();
        T e = f;
        while (f + One<T>() != One<T>())
        {
            e = f;
            f *= Half<T>();
        }
        return e;
    }

    const PackedFloat4_128 Epsilon_PackedFloat4_128 = _mm_set_ps1(benchmarkMachineEpsilon<Float_32>());

    const PackedFloat2_128 Epsilon_PackedFloat2_128 = _mm_set1_pd(benchmarkMachineEpsilon<Float_64>());

    template <>
    const PackedFloat4_128 Epsilon<PackedFloat4_128>()
    {
        return Epsilon_PackedFloat4_128;
    }

    template <>
    const PackedFloat2_128 Epsilon<PackedFloat2_128>()
    {
        return Epsilon_PackedFloat2_128;
    }

    template <>
    const Float_32 Epsilon<Float_32>()
    {
        return _mm_cvtss_f32(Epsilon<PackedFloat4_128>());
    }

    template <>
    const Float_64 Epsilon<Float_64>()
    {
        return _mm_cvtsd_f64(Epsilon<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region SelfOcclusionEpsilon
    //{ Epsilon

#define SELF_OCCLUSION_EPSILON 0.001953125
    const PackedFloat4_128 SelfOcclusionEpsilon_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(SELF_OCCLUSION_EPSILON));
    const PackedFloat2_128 SelfOcclusionEpsilon_PackedFloat2_128 = _mm_set1_pd(SELF_OCCLUSION_EPSILON);
#undef SELF_OCCLUSION_EPSILON

    template <>
    const PackedFloat4_128 SelfOcclusionEpsilon<PackedFloat4_128>()
    {
        return SelfOcclusionEpsilon_PackedFloat4_128;
    }

    template <>
    const PackedFloat2_128 SelfOcclusionEpsilon<PackedFloat2_128>()
    {
        return SelfOcclusionEpsilon_PackedFloat2_128;
    }

    template <>
    const Float_32 SelfOcclusionEpsilon<Float_32>()
    {
        return _mm_cvtss_f32(SelfOcclusionEpsilon<PackedFloat4_128>());
    }

    template <>
    const Float_64 SelfOcclusionEpsilon<Float_64>()
    {
        return _mm_cvtsd_f64(SelfOcclusionEpsilon<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region Pi
    //{ Pi

#define C_PI 3.1415926535897932384626433832795
    const PackedFloat4_128 Pi_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(C_PI));
    const PackedFloat2_128 Pi_PackedFloat2_128 = _mm_set1_pd(C_PI);

    template <>
    const PackedFloat4_128 Pi<PackedFloat4_128>()
    {
        return Pi_PackedFloat4_128;
    }

    template <>
    const PackedFloat2_128 Pi<PackedFloat2_128>()
    {
        return Pi_PackedFloat2_128;
    }

    template <>
    const Float_32 Pi<Float_32>()
    {
        return _mm_cvtss_f32(Pi<PackedFloat4_128>());
    }

    template <>
    const Float_64 Pi<Float_64>()
    {
        return _mm_cvtsd_f64(Pi<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region ReciprocalPi
    //{ ReciprocalPi

#define RECIPROCAL_PI 1.0 / C_PI
    const PackedFloat4_128 ReciprocalPi_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(RECIPROCAL_PI));
    const PackedFloat2_128 ReciprocalPi_PackedFloat2_128 = _mm_set1_pd(RECIPROCAL_PI);
#undef RECIPROCAL_PI

    template <>
    const PackedFloat4_128 ReciprocalPi<PackedFloat4_128>()
    {
        return ReciprocalPi_PackedFloat4_128;
    }

    template <>
    const PackedFloat2_128 ReciprocalPi<PackedFloat2_128>()
    {
        return ReciprocalPi_PackedFloat2_128;
    }

    template <>
    const Float_32 ReciprocalPi<Float_32>()
    {
        return _mm_cvtss_f32(ReciprocalPi<PackedFloat4_128>());
    }

    template <>
    const Float_64 ReciprocalPi<Float_64>()
    {
        return _mm_cvtsd_f64(ReciprocalPi<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region Sin45
    //{ Sin45

#define SIN_45 0.70710678118654752440084436210485
    const PackedFloat4_128 Sin45_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(SIN_45));
    const PackedFloat2_128 Sin45_PackedFloat2_128 = _mm_set1_pd(SIN_45);
#undef SIN_45

    template <>
    const PackedFloat4_128 Sin45<PackedFloat4_128>()
    {
        return Sin45_PackedFloat4_128;
    }

    template <>
    const PackedFloat2_128 Sin45<PackedFloat2_128>()
    {
        return Sin45_PackedFloat2_128;
    }

    template <>
    const Float_32 Sin45<Float_32>()
    {
        return _mm_cvtss_f32(Sin45<PackedFloat4_128>());
    }

    template <>
    const Float_64 Sin45<Float_64>()
    {
        return _mm_cvtsd_f64(Sin45<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region RadianToUniform
    //{ RadianToUniform

#define RADIAN_TO_UNIFORM 0.5 / C_PI
    const PackedFloat4_128 RadianToUniform_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(RADIAN_TO_UNIFORM));
    const PackedFloat2_128 RadianToUniform_PackedFloat2_128 = _mm_set1_pd(RADIAN_TO_UNIFORM);
#undef RADIAN_TO_UNIFORM

    template <>
    const PackedFloat4_128 RadianToUniform<PackedFloat4_128>()
    {
        return RadianToUniform_PackedFloat4_128;
    }

    template <>
    const PackedFloat2_128 RadianToUniform<PackedFloat2_128>()
    {
        return RadianToUniform_PackedFloat2_128;
    }

    template <>
    const Float_32 RadianToUniform<Float_32>()
    {
        return _mm_cvtss_f32(RadianToUniform<PackedFloat4_128>());
    }

    template <>
    const Float_64 RadianToUniform<Float_64>()
    {
        return _mm_cvtsd_f64(RadianToUniform<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region RadianToDegree
    //{ RadianToDegree

#define RADIAN_TO_DEGREE 180.0 / C_PI
    const PackedFloat4_128 RadianToDegree_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(RADIAN_TO_DEGREE));
    const PackedFloat2_128 RadianToDegree_PackedFloat2_128 = _mm_set1_pd(RADIAN_TO_DEGREE);
#undef RADIAN_TO_DEGREE

    template <>
    const PackedFloat4_128 RadianToDegree<PackedFloat4_128>()
    {
        return RadianToDegree_PackedFloat4_128;
    }

    template <>
    const PackedFloat2_128 RadianToDegree<PackedFloat2_128>()
    {
        return RadianToDegree_PackedFloat2_128;
    }

    template <>
    const Float_32 RadianToDegree<Float_32>()
    {
        return _mm_cvtss_f32(RadianToDegree<PackedFloat4_128>());
    }

    template <>
    const Float_64 RadianToDegree<Float_64>()
    {
        return _mm_cvtsd_f64(RadianToDegree<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region DegreeToRadian
    //{ DegreeToRadian

#define DEGREE_TO_RADIAN C_PI / 180.0
    const PackedFloat4_128 DegreeToRadian_PackedFloat4_128 = _mm_set_ps1(static_cast<Float_32>(DEGREE_TO_RADIAN));
    const PackedFloat2_128 DegreeToRadian_PackedFloat2_128 = _mm_set1_pd(DEGREE_TO_RADIAN);
#undef DEGREE_TO_RADIAN

    template <>
    const PackedFloat4_128 DegreeToRadian<PackedFloat4_128>()
    {
        return DegreeToRadian_PackedFloat4_128;
    }

    template <>
    const PackedFloat2_128 DegreeToRadian<PackedFloat2_128>()
    {
        return DegreeToRadian_PackedFloat2_128;
    }

    template <>
    const Float_32 DegreeToRadian<Float_32>()
    {
        return _mm_cvtss_f32(DegreeToRadian<PackedFloat4_128>());
    }

    template <>
    const Float_64 DegreeToRadian<Float_64>()
    {
        return _mm_cvtsd_f64(DegreeToRadian<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region Infinity
    //{ Infinity

    template <>
    const PackedFloat4_128 Infinity<PackedFloat4_128>()
    {
        // erase fraction part (23 bits) and sign bit
        return _mm_castsi128_ps(_mm_srli_epi32(_mm_slli_epi32(MaskAll<PackedInts_128>(), 24), 1));
    }

    template <>
    const PackedFloat2_128 Infinity<PackedFloat2_128>()
    {
        // erase fraction part (52 bits) and sign bit
        return _mm_castsi128_pd(_mm_srli_epi64(_mm_slli_epi64(MaskAll<PackedInts_128>(), 53), 1));
    }

    template <>
    const Float_32 Infinity<Float_32>()
    {
        return _mm_cvtss_f32(Infinity<PackedFloat4_128>());
    }

    template <>
    const Float_64 Infinity<Float_64>()
    {
        return _mm_cvtsd_f64(Infinity<PackedFloat2_128>());
    }

    //}
#pragma endregion

#pragma region NegInfinity
    //{ NegInfinity

    template <>
    const PackedFloat4_128 NegInfinity<PackedFloat4_128>()
    {
        // erase fraction part (23 bits)
        return _mm_castsi128_ps(_mm_slli_epi32(MaskAll<PackedInts_128>(), 23));
    }

    template <>
    const PackedFloat2_128 NegInfinity<PackedFloat2_128>()
    {
        // erase double fraction part (52 bits)
        return _mm_castsi128_pd(_mm_slli_epi64(MaskAll<PackedInts_128>(), 52));
    }

    template <>
    const Float_32 NegInfinity<Float_32>()
    {
        return _mm_cvtss_f32(NegInfinity<PackedFloat4_128>());
    }

    template <>
    const Float_64 NegInfinity<Float_64>()
    {
        return _mm_cvtsd_f64(NegInfinity<PackedFloat2_128>());
    }

    //}
#pragma endregion

}
