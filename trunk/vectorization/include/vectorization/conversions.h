#pragma once

#include "vectorization/utvector.h"
//#include "vectorization/v_ui8_16.h"
//#include "vectorization/v_ui8_32.h"
//#include "vectorization/v_i8_16.h"
//#include "vectorization/v_i8_32.h"
//#include "vectorization/v_ui16_8.h"
//#include "vectorization/v_ui16_16.h"
//#include "vectorization/v_i16_8.h"
//#include "vectorization/v_i16_16.h"
#include "vectorization/v_ui32_4.h"
//#include "vectorization/v_ui32_8.h"
//#include "vectorization/m_ui32_4x4.h"
#include "vectorization/v_i32_4.h"
//#include "vectorization/v_i32_8.h"
//#include "vectorization/m_i32_4x4.h"
#include "vectorization/v_f32_4.h"
//#include "vectorization/v_f32_8.h"
//#include "vectorization/v_f32_4x4.h"
#include "vectorization/m_f32_4x4.h"
#include "vectorization/v_ui64_2.h"
//#include "vectorization/v_ui64_4.h"
//#include "vectorization/v_i64_2.h"
//#include "vectorization/v_i64_4.h"
//#include "vectorization/v_f64_2.h"
//#include "vectorization/v_f64_4.h"

namespace vectorization
{

#pragma region convert(bool)
    //{ convert(bool)

    template <typename U>
    const U convert(const bool);

    template <>
    const v_i32_4 convert<v_i32_4>(const bool);

    template <>
    const v_ui32_4 convert<v_ui32_4>(const bool);

    //}
#pragma endregion

#pragma region convert(bool, bool)
    //{ convert(bool, bool)

    template <typename U>
    const U convert(const bool, const bool);

    template <>
    const v_i32_4 convert<v_i32_4>(const bool, const bool);

    template <>
    const v_ui32_4 convert<v_ui32_4>(const bool, const bool);

    //}
#pragma endregion

#pragma region convert(Int_32)
    //{ convert(Int_32)

    template <typename U>
    const U convert(const Int_32 v);

    template <>
    const Float_32 convert<Float_32>(const Int_32 v);

    template <>
    const Float_64 convert<Float_64>(const Int_32 v);

    //}
#pragma endregion

#pragma region convert(Int_64)
    //{ convert(Int_64)

    template <typename U>
    const U convert(const Int_64 v);

    template <>
    const Float_32 convert<Float_32>(const Int_64 v);

    template <>
    const Float_64 convert<Float_64>(const Int_64 v);

    //}
#pragma endregion

#pragma region convert(Float_32)
    //{ convert(Float_32)

    template <typename U>
    const U convert(const Float_32 v);

    template <>
    const Int_32 convert<Int_32>(const Float_32 v);

    template <>
    const Int_64 convert<Int_64>(const Float_32 v);

    //}
#pragma endregion

#pragma region convert(Float_64)
    //{ convert(Float_64)

    template <typename U>
    const U convert(const Float_64 v);

    template <>
    const Int_32 convert<Int_32>(const Float_64 v);

    template <>
    const Int_64 convert<Int_64>(const Float_64 v);

    //}
#pragma endregion

#pragma region convert(UTVector)
    //{ convert(UTVector)

    template <typename U, ASizeT Size, typename T>
    const U convert(const UTVector<Size, T> & v);

    //}
#pragma endregion

#pragma region convert(UTVector<X, T>)
    //{ convert(UTVector<X, T>)

    template <typename U, typename T>
    const U convert(const UTVector<VectorSizes::X, T> & v);

    template <>
    const v_ui32_4 convert<v_ui32_4>(const UTVector<VectorSizes::X, v_ui32_4::ValueType> & v);

    template <>
    const v_i32_4 convert<v_i32_4>(const UTVector<VectorSizes::X, v_i32_4::ValueType> & v);

    template <>
    const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::X, v_f32_4::ValueType> & v);

    template <>
    const v_ui64_2 convert<v_ui64_2>(const UTVector<VectorSizes::X, v_ui64_2::ValueType> & v);

    //}
#pragma endregion

#pragma region convert(UTVector<Y, T>)
    //{ convert(UTVector<Y, T>)

    template <typename U, typename T>
    const U convert(const UTVector<VectorSizes::Y, T> & v);

    template <>
    const v_ui32_4 convert<v_ui32_4>(const UTVector<VectorSizes::Y, v_ui32_4::ValueType> & v);

    template <>
    const v_i32_4 convert<v_i32_4>(const UTVector<VectorSizes::Y, v_i32_4::ValueType> & v);

    template <>
    const v_ui64_2 convert<v_ui64_2>(const UTVector<VectorSizes::Y, v_i32_4::ValueType> & v);

    template <>
    const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::Y, v_f32_4::ValueType> & v);

    template <>
    const v_ui64_2 convert<v_ui64_2>(const UTVector<VectorSizes::Y, v_ui64_2::ValueType> & v);

    //}
#pragma endregion

#pragma region convert(UTVector<Z, T>)
    //{ convert(UTVector<Z, T>)

    template <typename U, typename T>
    const U convert(const UTVector<VectorSizes::Z, T> & v);

    template <>
    const v_ui32_4 convert<v_ui32_4>(const UTVector<VectorSizes::Z, v_ui32_4::ValueType> & v);

    template <>
    const v_i32_4 convert<v_i32_4>(const UTVector<VectorSizes::Z, v_i32_4::ValueType> & v);

    template <>
    const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::Z, v_f32_4::ValueType> & v);

    //}
#pragma endregion

#pragma region convert(UTVector<W, T>)
    //{ convert(UTVector<W, T>)

    template <typename U, typename T>
    const U convert(const UTVector<VectorSizes::W, T> & v);

    template <>
    const v_ui32_4 convert<v_ui32_4>(const UTVector<VectorSizes::W, v_ui32_4::ValueType> & v);

    template <>
    const v_i32_4 convert<v_i32_4>(const UTVector<VectorSizes::W, v_i32_4::ValueType> & v);

    template <>
    const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::W, v_f32_4::ValueType> & v);

    //}
#pragma endregion

#pragma region convert(v_ui32_4)
    //{ convert(v_ui32_4)

    template <typename U>
    const U convert(const v_ui32_4 & v);

    template <>
    const UTVector<VectorSizes::X, v_ui32_4::ValueType> convert<UTVector<VectorSizes::X, v_ui32_4::ValueType>>(const v_ui32_4 & v);

    template <>
    const UTVector<VectorSizes::Y, v_ui32_4::ValueType> convert<UTVector<VectorSizes::Y, v_ui32_4::ValueType>>(const v_ui32_4 & v);

    template <>
    const UTVector<VectorSizes::Z, v_ui32_4::ValueType> convert<UTVector<VectorSizes::Z, v_ui32_4::ValueType>>(const v_ui32_4 & v);

    template <>
    const UTVector<VectorSizes::W, v_ui32_4::ValueType> convert<UTVector<VectorSizes::W, v_ui32_4::ValueType>>(const v_ui32_4 & v);

    //}
#pragma endregion

#pragma region convert(v_i32_4)
    //{ convert(v_i32_4)

    template <typename U>
    const U convert(const v_i32_4 & v);

    template <>
    const UTVector<VectorSizes::X, v_i32_4::ValueType> convert<UTVector<VectorSizes::X, v_i32_4::ValueType>>(const v_i32_4 & v);

    template <>
    const UTVector<VectorSizes::Y, v_i32_4::ValueType> convert<UTVector<VectorSizes::Y, v_i32_4::ValueType>>(const v_i32_4 & v);

    template <>
    const UTVector<VectorSizes::Z, v_i32_4::ValueType> convert<UTVector<VectorSizes::Z, v_i32_4::ValueType>>(const v_i32_4 & v);

    template <>
    const UTVector<VectorSizes::W, v_i32_4::ValueType> convert<UTVector<VectorSizes::W, v_i32_4::ValueType>>(const v_i32_4 & v);

    template <>
    const v_f32_4 convert<v_f32_4>(const v_i32_4 & v);

    //}
#pragma endregion

#pragma region convert(v_f32_4)
    //{ convert(v_f32_4)

    template <typename U>
    const U convert(const v_f32_4 & v);

    template <>
    const UTVector<VectorSizes::X, v_f32_4::ValueType> convert<UTVector<VectorSizes::X, v_f32_4::ValueType>>(const v_f32_4 & v);

    template <>
    const UTVector<VectorSizes::Y, v_f32_4::ValueType> convert<UTVector<VectorSizes::Y, v_f32_4::ValueType>>(const v_f32_4 & v);

    template <>
    const UTVector<VectorSizes::Z, v_f32_4::ValueType> convert<UTVector<VectorSizes::Z, v_f32_4::ValueType>>(const v_f32_4 & v);

    template <>
    const UTVector<VectorSizes::W, v_f32_4::ValueType> convert<UTVector<VectorSizes::W, v_f32_4::ValueType>>(const v_f32_4 & v);

    template <>
    const UTVector<VectorSizes::Y, UInt_32> convert<UTVector<VectorSizes::Y, UInt_32>>(const v_f32_4 & v);

    template <>
    const v_i32_4 convert<v_i32_4>(const v_f32_4 & v);

    template <>
    const v_f32_4 convert<v_f32_4>(const v_f32_4 & v);

    template <>
    const v_ui64_2 convert<v_ui64_2>(const v_f32_4 & v);

    //}
#pragma endregion

#pragma region convert(v_ui64_2)
    //{ convert(v_ui64_2)

    template <typename U>
    const U convert(const v_ui64_2 & v);

    template <>
    const UTVector<VectorSizes::X, v_ui64_2::ValueType> convert<UTVector<VectorSizes::X, v_ui64_2::ValueType>>(const v_ui64_2 & v);

    template <>
    const UTVector<VectorSizes::Y, v_ui64_2::ValueType> convert<UTVector<VectorSizes::Y, v_ui64_2::ValueType>>(const v_ui64_2 & v);

    template <>
    const UTVector<VectorSizes::Z, v_ui64_2::ValueType> convert<UTVector<VectorSizes::Z, v_ui64_2::ValueType>>(const v_ui64_2 & v);

    template <>
    const UTVector<VectorSizes::W, v_ui64_2::ValueType> convert<UTVector<VectorSizes::W, v_ui64_2::ValueType>>(const v_ui64_2 & v);

    template <>
    const UTVector<VectorSizes::Y, Int_32> convert<UTVector<VectorSizes::Y, Int_32>>(const v_ui64_2 & v);

    template <>
    const UTVector<VectorSizes::Y, Float_32> convert<UTVector<VectorSizes::Y, Float_32>>(const v_ui64_2 & v);

    template <>
    const v_f32_4 convert<v_f32_4>(const v_ui64_2 & v);

    //}
#pragma endregion

}
