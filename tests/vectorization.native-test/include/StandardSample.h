#pragma once

#include <vectorization.h>

#include <array>
#include <limits>

namespace vectorization
{
    namespace test
    {
        struct StandardSample
        {
            static const ASizeT ImportantFloatConstantsSize = 39;

            template <typename T>
            static const std::array<T, ImportantFloatConstantsSize> importantFloatConstants() {
                return {
                    Zero<T>(),
                    NegativeZero<T>(),
                    Half<T>(),
                    -Half<T>(),
                    One<T>(),
                    NegativeOne<T>(),
                    OneHalf<T>(),
                    -OneHalf<T>(),

                    Two<T>(),
                    NegativeTwo<T>(),
                    Pi<T>(),
                    -Pi<T>(),
                    ReciprocalPi<T>(),
                    -ReciprocalPi<T>(),
                    DegreeToRadian<T>(),
                    -DegreeToRadian<T>(),

                    RadianToDegree<T>(),
                    -RadianToDegree<T>(),
                    RadianToUniform<T>(),
                    -RadianToUniform<T>(),
                    Sin45<T>(),
                    -Sin45<T>(),
                    Infinity<T>(),
                    NegativeInfinity<T>(),

                    Epsilon<T>(),
                    -Epsilon<T>(),
                    SelfOcclusionEpsilon<T>(),
                    -SelfOcclusionEpsilon<T>(),
                    std::numeric_limits<T>::max(),
                    std::numeric_limits<T>::lowest(),
                    std::numeric_limits<T>::min(),
                    -std::numeric_limits<T>::min(),

                    std::numeric_limits<T>::denorm_min(),
                    -std::numeric_limits<T>::denorm_min(),
                    std::numeric_limits<T>::epsilon(),
                    -std::numeric_limits<T>::epsilon(),
                    std::numeric_limits<T>::round_error(),
                    -std::numeric_limits<T>::round_error(),
                    NotANumber<T>()
                };
            }

            template <typename T, ASizeT Size>
            static const std::array<T, Size> ofArrayType();

            template <>
            static const std::array<Float_32, VectorSizes::W> ofArrayType() {
                return { 0.0f, 1.0f, 2.0f, Infinity<Float_32>() };
            }

            template <>
            static const std::array<Float_32, VectorSizes::X8> ofArrayType() {
                return {
                    0.0f, 1.0f, 2.0f, Infinity<Float_32>(),
                    -0.0f, -1.0f, -2.0f, NegativeInfinity<Float_32>(),
                };
            }

            template <>
            static const std::array<Float_64, VectorSizes::Y> ofArrayType() {
                return { 2.0f, Infinity<Float_64>() };
            }

            template <>
            static const std::array<Float_64, VectorSizes::W> ofArrayType() {
                return {
                    0.0, 1.0, 2.0, Infinity<Float_64>(),
                };
            }

            template <typename T>
            static const T ofVectorTypeFromArray(std::array<typename T::ValueType, T::SIZE> sampleComponents);

            template <>
            static const v_f32_4 ofVectorTypeFromArray(std::array<v_f32_4::ValueType, v_f32_4::SIZE> sampleComponents) {
                return v_f32_4(
                    sampleComponents.at(VectorIndices::X),
                    sampleComponents.at(VectorIndices::Y),
                    sampleComponents.at(VectorIndices::Z),
                    sampleComponents.at(VectorIndices::W)
                );
            }

            template <typename T>
            static const T ofVectorType();

            template <>
            static const v_f32_4 ofVectorType() {
                auto sampleComponents = StandardSample::ofArrayType<v_f32_4::ValueType, v_f32_4::SIZE>();
                return StandardSample::ofVectorTypeFromArray<v_f32_4>(sampleComponents);
            }
        };
    }
}
