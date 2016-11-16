/*
 * Defines numerical constants.
 *
 * @file vectorization/constants.h
 */

#pragma once

#include "intrinsics.h"

namespace vectorization
{

#pragma region Mask...()
    //{ Mask...()

    template <typename V>
    const V MaskAll();

    template <typename V>
    const V MaskX();

    template <typename V>
    const V MaskXY();

    template <typename V>
    const V MaskXYZ();

    template <typename V>
    const V MaskXYZW();

    template <typename V>
    const V MaskY();

    template <typename V>
    const V MaskYZ();

    template <typename V>
    const V MaskYZW();

    template <typename V>
    const V MaskZ();

    template <typename V>
    const V MaskZW();

    template <typename V>
    const V MaskW();

    template <>
    const PackedFloat4_128 MaskW<PackedFloat4_128>();

    //}
#pragma endregion

#pragma region Constant generators
    //{ Constant generators

    template <typename T>
    const T Zero();

    template <typename T>
    const T Half();

    template <typename T>
    const T One();

    template <typename V>
    const V OneX();

    template <typename V>
    const V OneY();

    template <typename V>
    const V OneZ();

    template <typename V>
    const V OneW();

    template <typename T>
    const T OneHalf();

    template <typename T>
    const T Two();

    template <typename T>
    const T NegZero();

    template <typename T>
    const T NegOne();

    template <typename T>
    const T NegTwo();

    template <typename T>
    const T Infinity();

    template <typename T>
    const T NegInfinity();

    template <typename T>
    const T Epsilon();

    template <typename T>
    const T SelfOcclusionEpsilon();

    template <typename T>
    const T Pi();

    template <typename T>
    const T ReciprocalPi();

    template <typename T>
    const T Sin45();

    template <typename T>
    const T RadianToUniform();

    template <typename T>
    const T RadianToDegree();

    template <typename T>
    const T DegreeToRadian();

    template <typename M>
    const M Identity();

    //}
#pragma endregion

}
