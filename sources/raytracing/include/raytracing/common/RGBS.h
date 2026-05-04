#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    struct RGBS
    {
        Float4 value;

        RGBS() noexcept;

        RGBS(const Float4& v) noexcept;

        RGBS(Float r, Float g, Float b) noexcept;

        static RGBS black() noexcept;

        static RGBS white() noexcept;

        Float4 rgb() const noexcept;

        RGBS operator+(const RGBS& other) const noexcept;

        RGBS operator-(const RGBS& other) const noexcept;

        RGBS operator*(const RGBS& other) const noexcept;

        RGBS operator*(Float scalar) const noexcept;

        RGBS operator/(const RGBS& other) const noexcept;

        RGBS& operator+=(const RGBS& other) noexcept;

        RGBS& operator-=(const RGBS& other) noexcept;

        RGBS& operator*=(const RGBS& other) noexcept;

        RGBS& operator*=(Float scalar) noexcept;

        RGBS& operator/=(const RGBS& other) noexcept;

        bool operator==(const RGBS& other) const noexcept;

        bool operator!=(const RGBS& other) const noexcept;
    };

    RGBS operator*(Float scalar, const RGBS& rgbs) noexcept;
}
