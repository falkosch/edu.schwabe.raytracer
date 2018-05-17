#include "vectorization/conversions.h"
#include "../stdafx.h"

namespace vectorization
{

#pragma region convert(bool)
    //{ convert(bool)

    template <>
    const v_i32_4 convert<v_i32_4>(const bool v)
    {
        return v_i32_4(boolMask<v_i32_4::ValueType>(v));
    }

    template <>
    const v_ui32_4 convert<v_ui32_4>(const bool v)
    {
        return v_ui32_4(convert<v_i32_4>(v).components);
    }

    //}
#pragma endregion

#pragma region convert(bool, bool)
    //{ convert(bool, bool)

    template <>
    const v_i32_4 convert<v_i32_4>(const bool a, const bool b)
    {
        return v_i32_4(boolMask<v_i32_4::ValueType>(a), boolMask<v_i32_4::ValueType>(b));
    }

    template <>
    const v_ui32_4 convert<v_ui32_4>(const bool a, const bool b)
    {
        return v_ui32_4(convert<v_i32_4>(a, b).components);
    }

    //}
#pragma endregion

#pragma region convert(Int_32)
    //{ convert(Int_32)

    template <>
    const Float_32 convert<Float_32>(const Int_32 v)
    {
        return _mm_cvtss_f32(_mm_cvt_si2ss(Zero<PackedFloat4_128>(), v));
    }

    template <>
    const Float_64 convert<Float_64>(const Int_32 v)
    {
        return _mm_cvtsd_f64(_mm_cvtsi32_sd(Zero<PackedFloat2_128>(), v));
    }

    //}
#pragma endregion

#pragma region convert(Int_64)
    //{ convert(Int_64)

    template <>
    const Float_32 convert<Float_32>(const Int_64 v)
    {
#ifdef ARCH_X64
        return _mm_cvtss_f32(_mm_cvtsi64_ss(Zero<PackedFloat4_128>(), v));
#else
        // let the compiler decide because "_mm_cvtsi64_ss" is not available on 32 bit x86
        return static_cast<Float_32>(v);
#endif
    }

    template <>
    const Float_64 convert<Float_64>(const Int_64 v)
    {
#ifdef ARCH_X64
        return _mm_cvtsd_f64(_mm_cvtsi64_sd(Zero<PackedFloat2_128>(), v));
#else
        // let the compiler decide because "_mm_cvtsi64_sd" is not available on 32 bit x86
        return static_cast<Float_64>(v);
#endif
    }

    //}
#pragma endregion

#pragma region convert(Float_32)
    //{ convert(Int_32)

    template <>
    const Int_32 convert<Int_32>(const Float_32 v)
    {
        return _mm_cvtss_si32(_mm_set_ss(v));
    }

    template <>
    const Int_64 convert<Int_64>(const Float_32 v)
    {
#ifdef ARCH_X64
        return _mm_cvtss_si64(_mm_set_ss(v));
#else
        // let the compiler decide because "_mm_cvtss_si64" is not available on 32 bit x86
        return static_cast<Int_64>(v);
#endif
    }

    //}
#pragma endregion

#pragma region convert(Float_64)
    //{ convert(Float_64)

    template <>
    const Int_32 convert<Int_32>(const Float_64 v)
    {
        return _mm_cvtsd_si32(_mm_set_sd(v));
    }

    template <>
    const Int_64 convert<Int_64>(const Float_64 v)
    {
#ifdef ARCH_X64
        return _mm_cvtsd_si64(_mm_set_sd(v));
#else
        // let the compiler decide because "_mm_cvtsd_si64" is not available on 32 bit x86
        return static_cast<Int_64>(v);
#endif
    }

    //}
#pragma endregion

#pragma region convert(UTVector)
    //{ convert(UTVector)

    template <typename U, ASizeT Size, typename T>
    const U convert(const UTVector<Size, T> & v)
    {
        typedef typename U::ValueType OT;
        U t;
        const ASizeT m = min(U::SIZE, Size);
        ASizeT i = VectorIndices::X;
        for (; i < m; ++i)
            t[i] = v[i];
        for (; i < U::SIZE; ++i)
            t[i] = Zero<OT>();
        return t;
    }

    //}
#pragma endregion

#pragma region convert(UTVector<X, T>)
    //{ convert(UTVector<X, T>)

    template <typename U, typename T>
    const U convert(const UTVector<VectorSizes::X, T> & v)
    {
        typedef typename U::ValueType OT;
        U t;
        t.setX(static_cast<OT>(x(v)));
        for (ASizeT i = VectorIndices::Y; i < U::SIZE; ++i)
            t[i] = Zero<OT>();
        return t;
    }

    template <>
    const v_ui32_4 convert<v_ui32_4>(const UTVector<VectorSizes::X, v_ui32_4::ValueType> & v)
    {
        return blend<false, true, true, true>(_mm_load_si128(reinterpret_cast<const v_ui32_4::PackedType * const>(&v)), Zero<v_ui32_4>());
    }

    template <>
    const v_i32_4 convert<v_i32_4>(const UTVector<VectorSizes::X, v_i32_4::ValueType> & v)
    {
        return blend<false, true, true, true>(_mm_load_si128(reinterpret_cast<const v_i32_4::PackedType * const>(&v)), Zero<v_i32_4>());
    }

    template <>
    const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::X, v_f32_4::ValueType> & v)
    {
        return _mm_load_ss(v.components);
    }

    template <>
    const v_ui64_2 convert<v_ui64_2>(const UTVector<VectorSizes::X, v_ui64_2::ValueType> & v)
    {
        return blend<false, true>(_mm_load_si128(reinterpret_cast<const v_ui64_2::PackedType * const>(&v)), Zero<v_ui64_2::PackedType>());
    }

    //}
#pragma endregion

#pragma region convert(UTVector<Y, T>)
    //{ convert(UTVector<Y, T>)

    template <typename U, typename T>
    const U convert(const UTVector<VectorSizes::Y, T> & v)
    {
        typedef typename U::ValueType OT;
        U t;
        t.setX(static_cast<OT>(x(v)));
        t.setY(static_cast<OT>(y(v)));
        for (ASizeT i = VectorIndices::Z; i < U::SIZE; ++i)
            t[i] = Zero<OT>();
        return t;
    }

    template const UTVector<VectorSizes::Y, Float_32> convert(const UTVector<VectorSizes::Y, Int_32> & v);

    template <>
    const v_ui32_4 convert<v_ui32_4>(const UTVector<VectorSizes::Y, v_ui32_4::ValueType> & v)
    {
        return blend<false, false, true, true>(_mm_load_si128(reinterpret_cast<const v_ui32_4::PackedType * const>(&v)), Zero<v_ui32_4>());
    }

    template <>
    const v_i32_4 convert<v_i32_4>(const UTVector<VectorSizes::Y, v_i32_4::ValueType> & v)
    {
        return blend<false, false, true, true>(_mm_load_si128(reinterpret_cast<const v_i32_4::PackedType * const>(&v)), Zero<v_i32_4>());
    }

    template <>
    const v_ui64_2 convert<v_ui64_2>(const UTVector<VectorSizes::Y, v_i32_4::ValueType> & v)
    {
        return v_ui64_2(static_cast<v_ui64_2::ValueType>(x(v)), static_cast<v_ui64_2::ValueType>(y(v)));
    }

    template <>
    const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::Y, v_f32_4::ValueType> & v)
    {
        return xy_zw(_mm_load_ps(v.components), Zero<v_f32_4>());
    }

    template <>
    const v_ui64_2 convert<v_ui64_2>(const UTVector<VectorSizes::Y, v_ui64_2::ValueType> & v)
    {
        return v_ui64_2(_mm_load_si128(reinterpret_cast<const v_ui64_2::PackedType * const>(&v)));
    }


    //}
#pragma endregion

#pragma region convert(UTVector<Z, T>)
    //{ convert(UTVector<Z, T>)

    template <typename U, typename T>
    const U convert(const UTVector<VectorSizes::Z, T> & v)
    {
        typedef typename U::ValueType OT;
        U t;
        t.setX(static_cast<OT>(x(v)));
        t.setY(static_cast<OT>(y(v)));
        t.setZ(static_cast<OT>(z(v)));
        for (ASizeT i = VectorIndices::W; i < U::SIZE; ++i)
            t[i] = Zero<OT>();
        return t;
    }

    template <>
    const v_ui32_4 convert<v_ui32_4>(const UTVector<VectorSizes::Z, v_ui32_4::ValueType> & v)
    {
        return zeroW(v_ui32_4(_mm_load_si128(reinterpret_cast<const v_ui32_4::PackedType * const>(&v))));
    }

    template <>
    const v_i32_4 convert<v_i32_4>(const UTVector<VectorSizes::Z, v_i32_4::ValueType> & v)
    {
        return zeroW(v_i32_4(_mm_load_si128(reinterpret_cast<const v_i32_4::PackedType * const>(&v))));
    }

    template <>
    const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::Z, v_f32_4::ValueType> & v)
    {
        return zeroW(_mm_load_ps(v.components));
    }

    //}
#pragma endregion

#pragma region convert(UTVector<W, T>)
    //{ convert(UTVector<W, T>)

    template <typename U, typename T>
    const U convert(const UTVector<VectorSizes::W, T> & v)
    {
        typedef typename U::ValueType OT;
        U t;
        t.setX(static_cast<OT>(x(v)));
        t.setY(static_cast<OT>(y(v)));
        t.setZ(static_cast<OT>(z(v)));
        t.setW(static_cast<OT>(w(v)));
        for (ASizeT i = VectorIndices::AboveW; i < U::SIZE; ++i)
            t[i] = Zero<OT>();
        return t;
    }

    template <>
    const v_ui32_4 convert<v_ui32_4>(const UTVector<VectorSizes::W, v_ui32_4::ValueType> & v)
    {
        return _mm_load_si128(reinterpret_cast<const v_ui32_4::PackedType * const>(&v));
    }

    template <>
    const v_i32_4 convert<v_i32_4>(const UTVector<VectorSizes::W, v_i32_4::ValueType> & v)
    {
        return _mm_load_si128(reinterpret_cast<const v_i32_4::PackedType * const>(&v));
    }

    template <>
    const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::W, v_f32_4::ValueType> & v)
    {
        return _mm_load_ps(v.components);
    }

    //}
#pragma endregion

#pragma region convert(v_ui32_4)
    //{ convert(v_ui32_4)

    template <>
    const UTVector<VectorSizes::X, v_ui32_4::ValueType> convert<UTVector<VectorSizes::X, v_ui32_4::ValueType>>(const v_ui32_4 & v)
    {
        UTVector<VectorSizes::X, v_ui32_4::ValueType> t;
        _mm_store_si128(reinterpret_cast<v_ui32_4::PackedType * const>(t.components), v.components);
        return t;
    }

    template <>
    const UTVector<VectorSizes::Y, v_ui32_4::ValueType> convert<UTVector<VectorSizes::Y, v_ui32_4::ValueType>>(const v_ui32_4 & v)
    {
        UTVector<VectorSizes::Y, v_ui32_4::ValueType> t;
        _mm_store_si128(reinterpret_cast<v_ui32_4::PackedType * const>(t.components), v.components);
        return t;
    }

    template <>
    const UTVector<VectorSizes::Z, v_ui32_4::ValueType> convert<UTVector<VectorSizes::Z, v_ui32_4::ValueType>>(const v_ui32_4 & v)
    {
        UTVector<VectorSizes::Z, v_ui32_4::ValueType> t;
        _mm_store_si128(reinterpret_cast<v_ui32_4::PackedType * const>(t.components), v.components);
        return t;
    }

    template <>
    const UTVector<VectorSizes::W, v_ui32_4::ValueType> convert<UTVector<VectorSizes::W, v_ui32_4::ValueType>>(const v_ui32_4 & v)
    {
        UTVector<VectorSizes::W, v_ui32_4::ValueType> t;
        _mm_store_si128(reinterpret_cast<v_ui32_4::PackedType * const>(t.components), v.components);
        return t;
    }

    //}
#pragma endregion

#pragma region convert(v_i32_4)
    //{ convert(v_i32_4)

    template <>
    const UTVector<VectorSizes::X, v_i32_4::ValueType> convert<UTVector<VectorSizes::X, v_i32_4::ValueType>>(const v_i32_4 & v)
    {
        UTVector<VectorSizes::X, v_i32_4::ValueType> t;
        _mm_store_si128(reinterpret_cast<v_i32_4::PackedType * const>(t.components), v.components);
        return t;
    }

    template <>
    const UTVector<VectorSizes::Y, v_i32_4::ValueType> convert<UTVector<VectorSizes::Y, v_i32_4::ValueType>>(const v_i32_4 & v)
    {
        UTVector<VectorSizes::Y, v_i32_4::ValueType> t;
        _mm_store_si128(reinterpret_cast<v_i32_4::PackedType * const>(t.components), v.components);
        return t;
    }

    template <>
    const UTVector<VectorSizes::Z, v_i32_4::ValueType> convert<UTVector<VectorSizes::Z, v_i32_4::ValueType>>(const v_i32_4 & v)
    {
        UTVector<VectorSizes::Z, v_i32_4::ValueType> t;
        _mm_store_si128(reinterpret_cast<v_i32_4::PackedType * const>(t.components), v.components);
        return t;
    }

    template <>
    const UTVector<VectorSizes::W, v_i32_4::ValueType> convert<UTVector<VectorSizes::W, v_i32_4::ValueType>>(const v_i32_4 & v)
    {
        UTVector<VectorSizes::W, v_i32_4::ValueType> t;
        _mm_store_si128(reinterpret_cast<v_i32_4::PackedType * const>(t.components), v.components);
        return t;
    }

    template <>
    const v_f32_4 convert<v_f32_4>(const v_i32_4 & v)
    {
        return _mm_cvtepi32_ps(v.components);
    }

    //}
#pragma endregion

#pragma region convert(v_f32_4)
    //{ convert(v_f32_4)

    template <>
    const UTVector<VectorSizes::X, v_f32_4::ValueType> convert<UTVector<VectorSizes::X, v_f32_4::ValueType>>(const v_f32_4 & v)
    {
        UTVector<VectorSizes::X, v_f32_4::ValueType> t;
        _mm_store_ps(t.components, v.components);
        return t;
    }

    template <>
    const UTVector<VectorSizes::Y, v_f32_4::ValueType> convert<UTVector<VectorSizes::Y, v_f32_4::ValueType>>(const v_f32_4 & v)
    {
        UTVector<VectorSizes::Y, v_f32_4::ValueType> t;
        _mm_store_ps(t.components, v.components);
        return t;
    }

    template <>
    const UTVector<VectorSizes::Z, v_f32_4::ValueType> convert<UTVector<VectorSizes::Z, v_f32_4::ValueType>>(const v_f32_4 & v)
    {
        UTVector<VectorSizes::Z, v_f32_4::ValueType> t;
        _mm_store_ps(t.components, v.components);
        return t;
    }

    template <>
    const UTVector<VectorSizes::W, v_f32_4::ValueType> convert<UTVector<VectorSizes::W, v_f32_4::ValueType>>(const v_f32_4 & v)
    {
        UTVector<VectorSizes::W, v_f32_4::ValueType> t;
        _mm_store_ps(t.components, v.components);
        return t;
    }

    template <>
    const UTVector<VectorSizes::Y, UInt_32> convert<UTVector<VectorSizes::Y, UInt_32>>(const v_f32_4 & v)
    {
        const v_i32_4 t = convert<v_i32_4>(v);
        return UTVector<VectorSizes::Y, UInt_32>(
            static_cast<UInt_32>(x(t)),
            static_cast<UInt_32>(y(t)));
    }

    template <>
    const v_i32_4 convert<v_i32_4>(const v_f32_4 & v)
    {
        return _mm_cvtps_epi32(v.components);
    }

    template <>
    const v_f32_4 convert<v_f32_4>(const v_f32_4 & v)
    {
        return v;
    }

    template <>
    const v_ui64_2 convert<v_ui64_2>(const v_f32_4 & v)
    {
#ifdef ARCH_X64
        return v_ui64_2(
            static_cast<v_ui64_2::ValueType>(_mm_cvtss_si64(v.components)),
            static_cast<v_ui64_2::ValueType>(_mm_cvtss_si64(yyww(v).components)));
#else
        return v_ui64_2(
            static_cast<v_ui64_2::ValueType>(x(v)),
            static_cast<v_ui64_2::ValueType>(y(v)));
#endif
    }

    //}
#pragma endregion

#pragma region convert(v_ui64_2)
    //{ convert(v_ui64_2)

    template <>
    const UTVector<VectorSizes::X, v_ui64_2::ValueType> convert<UTVector<VectorSizes::X, v_ui64_2::ValueType>>(const v_ui64_2 & v)
    {
        UTVector<VectorSizes::X, v_ui64_2::ValueType> t;
        _mm_store_si128(reinterpret_cast<v_ui64_2::PackedType * const>(t.components), v.components);
        return t;
    }

    template <>
    const UTVector<VectorSizes::Y, v_ui64_2::ValueType> convert<UTVector<VectorSizes::Y, v_ui64_2::ValueType>>(const v_ui64_2 & v)
    {
        UTVector<VectorSizes::Y, v_ui64_2::ValueType> t;
        _mm_store_si128(reinterpret_cast<v_ui64_2::PackedType * const>(t.components), v.components);
        return t;
    }

    template <>
    const UTVector<VectorSizes::Z, v_ui64_2::ValueType> convert<UTVector<VectorSizes::Z, v_ui64_2::ValueType>>(const v_ui64_2 & v)
    {
        return UTVector<VectorSizes::Z, v_ui64_2::ValueType>(x(v), y(v));
    }

    template <>
    const UTVector<VectorSizes::W, v_ui64_2::ValueType> convert<UTVector<VectorSizes::W, v_ui64_2::ValueType>>(const v_ui64_2 & v)
    {
        return UTVector<VectorSizes::W, v_ui64_2::ValueType>(x(v), y(v));
    }

    template <>
    const UTVector<VectorSizes::Y, Int_32> convert<UTVector<VectorSizes::Y, Int_32>>(const v_ui64_2 & v)
    {
        return UTVector<VectorSizes::Y, Int_32>(static_cast<Int_32>(x(v)), static_cast<Int_32>(y(v)));
    }

    template <>
    const UTVector<VectorSizes::Y, Float_32> convert<UTVector<VectorSizes::Y, Float_32>>(const v_ui64_2 & v)
    {
        return UTVector<VectorSizes::Y, Float_32>(
            convert<Float_32>(static_cast<Int_64>(x(v))),
            convert<Float_32>(static_cast<Int_64>(y(v))));
    }

    template <>
    const v_f32_4 convert<v_f32_4>(const v_ui64_2 & v)
    {
        return v_f32_4(static_cast<v_f32_4::ValueType>(x(v)), static_cast<v_f32_4::ValueType>(y(v)));
    }

    //}
#pragma endregion

}
