#pragma once

#include "native.h"
//#include "../v_ui8_16.h"
//#include "../v_ui8_32.h"
//#include "../v_i8_16.h"
//#include "../v_i8_32.h"
//#include "../v_ui16_8.h"
//#include "../v_ui16_16.h"
//#include "../v_i16_8.h"
//#include "../v_i16_16.h"
#include "../v_ui32_4.h"
//#include "../v_ui32_8.h"
//#include "../m_ui32_4x4.h"
#include "../v_i32_4.h"
//#include "../v_i32_8.h"
//#include "../m_i32_4x4.h"
#include "../v_f32_4.h"
//#include "../v_f32_8.h"
//#include "../v_f32_4x4.h"
#include "../m_f32_4x4.h"
#include "../v_ui64_2.h"
//#include "../v_ui64_4.h"
//#include "../v_i64_2.h"
//#include "../v_i64_4.h"
//#include "../v_f64_2.h"
//#include "../v_f64_4.h"

namespace vectorization
{
    template <>
    const v_i32_4 convert<v_i32_4>(const bool) noexcept;

    template <>
    const v_ui32_4 convert<v_ui32_4>(const bool) noexcept;

    template <>
    const v_i32_4 convert<v_i32_4>(const bool, const bool) noexcept;

    template <>
    const v_ui32_4 convert<v_ui32_4>(const bool, const bool) noexcept;

    template <typename U>
    const U convert(const v_ui32_4 & v) noexcept;

    template <>
    const v_f32_4 convert<v_f32_4>(const v_ui32_4 & v) noexcept;

    template <typename U>
    const U convert(const v_i32_4 & v) noexcept;

    template <>
    const v_f32_4 convert<v_f32_4>(const v_i32_4 & v) noexcept;

    template <>
    const v_ui64_2 convert<v_ui64_2>(const v_i32_4 & v) noexcept;

    template <typename U>
    const U convert(const v_f32_4 & v) noexcept;

    template <>
    const v_i32_4 convert<v_i32_4>(const v_f32_4 & v) noexcept;

    template <>
    const v_f32_4 convert<v_f32_4>(const v_f32_4 & v) noexcept;

    template <>
    const v_ui64_2 convert<v_ui64_2>(const v_f32_4 & v) noexcept;

    template <typename U>
    const U convert(const v_ui64_2 & v) noexcept;

    template <>
    const v_f32_4 convert<v_f32_4>(const v_ui64_2 & v) noexcept;
}
