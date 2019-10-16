#pragma once

#include "type.h"
#include "../swizzles.h"

namespace vectorization
{
    template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
    // generic swizzle
    inline const v_f32_4 swizzle(const v_f32_4 & v) noexcept {
        return swizzle<X, Y, Z, W>(v.components);
    }

    const v_f32_4 xxxx(const v_f32_4 & v) noexcept;

    const v_f32_4 yyyy(const v_f32_4 & v) noexcept;

    const v_f32_4 zzzz(const v_f32_4 & v) noexcept;

    const v_f32_4 wwww(const v_f32_4 & v) noexcept;

    const v_f32_4 xxxz(const v_f32_4 & v) noexcept;

    const v_f32_4 xxyy(const v_f32_4 & v) noexcept;

    const v_f32_4 xxzz(const v_f32_4 & v) noexcept;

    const v_f32_4 xyxy(const v_f32_4 & v) noexcept;

    const v_f32_4 xzzz(const v_f32_4 & v) noexcept;

    const v_f32_4 yxxy(const v_f32_4 & v) noexcept;

    const v_f32_4 yxwz(const v_f32_4 & v) noexcept;

    const v_f32_4 yyww(const v_f32_4 & v) noexcept;

    const v_f32_4 yzxw(const v_f32_4 & v) noexcept;

    const v_f32_4 yzwx(const v_f32_4 & v) noexcept;

    const v_f32_4 zxyw(const v_f32_4 & v) noexcept;

    const v_f32_4 zzyy(const v_f32_4 & v) noexcept;

    const v_f32_4 zzww(const v_f32_4 & v) noexcept;

    const v_f32_4 zwxy(const v_f32_4 & v) noexcept;

    const v_f32_4 zwzw(const v_f32_4 & v) noexcept;

    const v_f32_4 wxyz(const v_f32_4 & v) noexcept;

    const v_f32_4 wzyx(const v_f32_4 & v) noexcept;
}
