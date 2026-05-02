#include "raytracing/common/RGBS.h"
#include "../../stdafx.h"

namespace raytracer
{
    RGBS::RGBS() noexcept : value()
    {
    }

    RGBS::RGBS(const Float4& v) noexcept : value(v)
    {
    }

    RGBS::RGBS(const Float r, const Float g, const Float b, const Float s) noexcept : value(r, g, b, s)
    {
    }

    RGBS RGBS::black() noexcept
    {
        return {Zero<Float4>()};
    }

    RGBS RGBS::white() noexcept
    {
        return {One<Float4>()};
    }

    Float4 RGBS::rgb() const noexcept
    {
        return zeroW(value);
    }

    Float RGBS::scale() const noexcept
    {
        return w(value);
    }

    RGBS RGBS::scalev() const noexcept
    {
        return {wwww(value)};
    }

    RGBS RGBS::operator+(const RGBS& other) const noexcept
    {
        return {value + other.value};
    }

    RGBS RGBS::operator-(const RGBS& other) const noexcept
    {
        return {value - other.value};
    }

    RGBS RGBS::operator*(const RGBS& other) const noexcept
    {
        return {value * other.value};
    }

    RGBS RGBS::operator*(const Float scalar) const noexcept
    {
        return {value * scalar};
    }

    RGBS RGBS::operator/(const RGBS& other) const noexcept
    {
        return {value / other.value};
    }

    RGBS& RGBS::operator+=(const RGBS& other) noexcept
    {
        value = value + other.value;
        return *this;
    }

    RGBS& RGBS::operator-=(const RGBS& other) noexcept
    {
        value = value - other.value;
        return *this;
    }

    RGBS& RGBS::operator*=(const RGBS& other) noexcept
    {
        value = value * other.value;
        return *this;
    }

    RGBS& RGBS::operator*=(const Float scalar) noexcept
    {
        value = value * scalar;
        return *this;
    }

    RGBS& RGBS::operator/=(const RGBS& other) noexcept
    {
        value = value / other.value;
        return *this;
    }

    bool RGBS::operator==(const RGBS& other) const noexcept
    {
        return allTrue(value == other.value);
    }

    bool RGBS::operator!=(const RGBS& other) const noexcept
    {
        return !allTrue(value == other.value);
    }

    RGBS operator*(const Float scalar, const RGBS& rgbs) noexcept
    {
        return {scalar * rgbs.value};
    }
}
