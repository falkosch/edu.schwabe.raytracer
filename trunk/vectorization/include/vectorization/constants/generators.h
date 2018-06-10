#pragma once

#include "../architecture.h"

namespace vectorization
{

    template <typename T>
    const T Zero() noexcept;

    template <typename T>
    const T Half() noexcept;

    template <typename T>
    const T One() noexcept;

    template <typename V>
    const V OneX() noexcept;

    template <typename V>
    const V OneY() noexcept;

    template <typename V>
    const V OneZ() noexcept;

    template <typename V>
    const V OneW() noexcept;

    template <typename T>
    const T OneHalf() noexcept;

    template <typename T>
    const T Two() noexcept;

    template <typename T>
    const T NegZero() noexcept;

    template <typename T>
    const T NegOne() noexcept;

    template <typename T>
    const T NegTwo() noexcept;

    template <typename T>
    const T Infinity() noexcept;

    template <typename T>
    const T NegInfinity() noexcept;

    template <typename T>
    const T Epsilon() noexcept;

    template <typename T>
    const T SelfOcclusionEpsilon() noexcept;

    template <typename T>
    const T Pi() noexcept;

    template <typename T>
    const T ReciprocalPi() noexcept;

    template <typename T>
    const T Sin45() noexcept;

    template <typename T>
    const T RadianToUniform() noexcept;

    template <typename T>
    const T RadianToDegree() noexcept;

    template <typename T>
    const T DegreeToRadian() noexcept;

    template <typename M>
    const M Identity() noexcept;

}
