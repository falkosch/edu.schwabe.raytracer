#pragma once

#include "type.h"

namespace vectorization
{
    v_ui32_8 andnot(const v_ui32_8& a, const v_ui32_8& b) noexcept;

    v_ui32_8 sqr(const v_ui32_8& v) noexcept;

    v_ui32_8 min(const v_ui32_8& a, const v_ui32_8& b) noexcept;
    v_ui32_8 max(const v_ui32_8& a, const v_ui32_8& b) noexcept;

    v_ui32_8 clamp(const v_ui32_8& v, const v_ui32_8& l, const v_ui32_8& m) noexcept;

    bool anyTrue(const v_ui32_8& v) noexcept;
    bool anyFalse(const v_ui32_8& v) noexcept;
    bool allTrue(const v_ui32_8& v) noexcept;
    bool allFalse(const v_ui32_8& v) noexcept;
}
