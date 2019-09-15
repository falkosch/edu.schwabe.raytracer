#pragma once

#include <vectorization.h>

#include <array>
#include <limits>

namespace vectorization
{
    namespace test
    {
        struct StandardSample {

            static const ASizeT ImportantFloatConstantsSize = 39;

            static const std::array<v_f32_4::ValueType, ImportantFloatConstantsSize> importantFloatConstants() {
                return {
                    Zero<v_f32_4::ValueType>(),
                    NegativeZero<v_f32_4::ValueType>(),
                    Half<v_f32_4::ValueType>(),
                    -Half<v_f32_4::ValueType>(),
                    One<v_f32_4::ValueType>(),
                    NegativeOne<v_f32_4::ValueType>(),
                    OneHalf<v_f32_4::ValueType>(),
                    -OneHalf<v_f32_4::ValueType>(),

                    Two<v_f32_4::ValueType>(),
                    NegativeTwo<v_f32_4::ValueType>(),
                    Pi<v_f32_4::ValueType>(),
                    -Pi<v_f32_4::ValueType>(),
                    ReciprocalPi<v_f32_4::ValueType>(),
                    -ReciprocalPi<v_f32_4::ValueType>(),
                    DegreeToRadian<v_f32_4::ValueType>(),
                    -DegreeToRadian<v_f32_4::ValueType>(),

                    RadianToDegree<v_f32_4::ValueType>(),
                    -RadianToDegree<v_f32_4::ValueType>(),
                    RadianToUniform<v_f32_4::ValueType>(),
                    -RadianToUniform<v_f32_4::ValueType>(),
                    Sin45<v_f32_4::ValueType>(),
                    -Sin45<v_f32_4::ValueType>(),
                    Infinity<v_f32_4::ValueType>(),
                    NegativeInfinity<v_f32_4::ValueType>(),

                    Epsilon<v_f32_4::ValueType>(),
                    -Epsilon<v_f32_4::ValueType>(),
                    SelfOcclusionEpsilon<v_f32_4::ValueType>(),
                    -SelfOcclusionEpsilon<v_f32_4::ValueType>(),
                    std::numeric_limits<v_f32_4::ValueType>::max(),
                    std::numeric_limits<v_f32_4::ValueType>::lowest(),
                    std::numeric_limits<v_f32_4::ValueType>::min(),
                    -std::numeric_limits<v_f32_4::ValueType>::min(),

                    std::numeric_limits<v_f32_4::ValueType>::denorm_min(),
                    -std::numeric_limits<v_f32_4::ValueType>::denorm_min(),
                    std::numeric_limits<v_f32_4::ValueType>::epsilon(),
                    -std::numeric_limits<v_f32_4::ValueType>::epsilon(),
                    std::numeric_limits<v_f32_4::ValueType>::round_error(),
                    -std::numeric_limits<v_f32_4::ValueType>::round_error(),
                    NotANumber<v_f32_4::ValueType>()
                };
            }


            template <typename T, ASizeT Size>
            static const std::array<T, Size> ofArrayType();

            template <>
            static const std::array<v_f32_4::ValueType, v_f32_4::SIZE> ofArrayType() {
                return {
                    One<v_f32_4::ValueType>(),
                    Two<v_f32_4::ValueType>(),
                    OneHalf<v_f32_4::ValueType>(),
                    Infinity<v_f32_4::ValueType>()
                };
            }


            template <typename T>
            static const T ofVectorType();

            template <>
            static const v_f32_4 ofVectorType() {
                auto sampleComponents = StandardSample::ofArrayType<v_f32_4::ValueType, v_f32_4::SIZE>();
                return v_f32_4(
                    sampleComponents.at(VectorIndices::X),
                    sampleComponents.at(VectorIndices::Y),
                    sampleComponents.at(VectorIndices::Z),
                    sampleComponents.at(VectorIndices::W)
                );
            }

        };
    }
}
