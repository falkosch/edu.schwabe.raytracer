project(vectorization
    LANGUAGES CXX
    VERSION 1.0.0
)

add_library(${PROJECT_NAME}
    STATIC
        "src/primitives/AxisAlignedBoundingBox.cpp"
        "src/primitives/BoundingSphere.cpp"
        "src/primitives/Facet.cpp"
        "src/primitives/FacetEdges.cpp"
        "src/primitives/Ray.cpp"
        "src/primitives/Raycast.cpp"
        "src/primitives/SplittingPlane.cpp"
        "src/vectorization/accessors/component_128d.cpp"
        "src/vectorization/accessors/component_128s.cpp"
        "src/vectorization/accessors/component_256d.cpp"
        "src/vectorization/accessors/component_256s.cpp"
        "src/vectorization/accessors/replaceComponent_128d.cpp"
        "src/vectorization/accessors/replaceComponent_128s.cpp"
        "src/vectorization/accessors/replaceComponent_256d.cpp"
        "src/vectorization/accessors/replaceComponent_256s.cpp"
        "src/vectorization/architecture/compiler_intrinsics.cpp"
        "src/vectorization/architecture/memory.cpp"
        "src/vectorization/blends/blend_128d.cpp"
        "src/vectorization/blends/blend_128s.cpp"
        "src/vectorization/blends/blend_256d.cpp"
        "src/vectorization/blends/blend_256s.cpp"
        "src/vectorization/blends/blendMasked_128d.cpp"
        "src/vectorization/blends/blendMasked_128i.cpp"
        "src/vectorization/blends/blendMasked_128s.cpp"
        "src/vectorization/blends/blendMasked_256d.cpp"
        "src/vectorization/blends/blendMasked_256i.cpp"
        "src/vectorization/blends/blendMasked_256s.cpp"
        "src/vectorization/blends/blendMasked_scalar.cpp"
        "src/vectorization/constants/masks/MaskAll.cpp"
        "src/vectorization/constants/masks/MaskNone.cpp"
        "src/vectorization/constants/masks/MaskW.cpp"
        "src/vectorization/constants/masks/MaskX.cpp"
        "src/vectorization/constants/masks/MaskXY.cpp"
        "src/vectorization/constants/masks/MaskXYZ.cpp"
        "src/vectorization/constants/masks/MaskXYZW.cpp"
        "src/vectorization/constants/masks/MaskY.cpp"
        "src/vectorization/constants/masks/MaskYZ.cpp"
        "src/vectorization/constants/masks/MaskYZW.cpp"
        "src/vectorization/constants/masks/MaskZ.cpp"
        "src/vectorization/constants/masks/MaskZW.cpp"
        "src/vectorization/constants/values/DegreeToRadian.cpp"
        "src/vectorization/constants/values/Epsilon.cpp"
        "src/vectorization/constants/values/Half.cpp"
        "src/vectorization/constants/values/Identity.cpp"
        "src/vectorization/constants/values/Infinity.cpp"
        "src/vectorization/constants/values/NegativeInfinity.cpp"
        "src/vectorization/constants/values/NegativeOne.cpp"
        "src/vectorization/constants/values/NegativeTwo.cpp"
        "src/vectorization/constants/values/NegativeZero.cpp"
        "src/vectorization/constants/values/NotANumber.cpp"
        "src/vectorization/constants/values/One.cpp"
        "src/vectorization/constants/values/OneHalf.cpp"
        "src/vectorization/constants/values/OneW.cpp"
        "src/vectorization/constants/values/OneX.cpp"
        "src/vectorization/constants/values/OneY.cpp"
        "src/vectorization/constants/values/OneZ.cpp"
        "src/vectorization/constants/values/Pi.cpp"
        "src/vectorization/constants/values/RadianToDegree.cpp"
        "src/vectorization/constants/values/RadianToUniform.cpp"
        "src/vectorization/constants/values/ReciprocalPi.cpp"
        "src/vectorization/constants/values/SelfOcclusionEpsilon.cpp"
        "src/vectorization/constants/values/Sin45.cpp"
        "src/vectorization/constants/values/Two.cpp"
        "src/vectorization/constants/values/Zero.cpp"
        "src/vectorization/conversions/complex.cpp"
        "src/vectorization/conversions/native.cpp"
        "src/vectorization/functions/abs.cpp"
        "src/vectorization/functions/acos.cpp"
        "src/vectorization/functions/atan2.cpp"
        "src/vectorization/functions/bitcount.cpp"
        "src/vectorization/functions/bitpad.cpp"
        "src/vectorization/functions/ceil.cpp"
        "src/vectorization/functions/clamp.cpp"
        "src/vectorization/functions/copysign.cpp"
        "src/vectorization/functions/cos.cpp"
        "src/vectorization/functions/divide.cpp"
        "src/vectorization/functions/exp.cpp"
        "src/vectorization/functions/floor.cpp"
        "src/vectorization/functions/fract.cpp"
        "src/vectorization/functions/isNaN.cpp"
        "src/vectorization/functions/log.cpp"
        "src/vectorization/functions/logN.cpp"
        "src/vectorization/functions/max.cpp"
        "src/vectorization/functions/min.cpp"
        "src/vectorization/functions/mix.cpp"
        "src/vectorization/functions/modpad.cpp"
        "src/vectorization/functions/modulo.cpp"
        "src/vectorization/functions/pow.cpp"
        "src/vectorization/functions/reciprocal.cpp"
        "src/vectorization/functions/round.cpp"
        "src/vectorization/functions/roundDown.cpp"
        "src/vectorization/functions/rsqrt.cpp"
        "src/vectorization/functions/sign.cpp"
        "src/vectorization/functions/sin.cpp"
        "src/vectorization/functions/sqr.cpp"
        "src/vectorization/functions/sqrt.cpp"
        "src/vectorization/functions/tan.cpp"
        "src/vectorization/m_f32_4x4.cpp"
        "src/vectorization/selects/selects_128d.cpp"
        "src/vectorization/selects/selects_128i.cpp"
        "src/vectorization/selects/selects_128s.cpp"
        "src/vectorization/selects/selects_256d.cpp"
        "src/vectorization/selects/selects_256i.cpp"
        "src/vectorization/selects/selects_256s.cpp"
        "src/vectorization/selects/selects_scalar.cpp"
        "src/vectorization/swizzle_blends/swizzleBlend1x1_128d.cpp"
        "src/vectorization/swizzle_blends/swizzleBlend2x2_128s.cpp"
        "src/vectorization/swizzle_blends/swizzleBlend2x2_256d.cpp"
        "src/vectorization/swizzle_blends/swizzleBlend_128d.cpp"
        "src/vectorization/swizzle_blends/swizzleBlend_128s.cpp"
        "src/vectorization/swizzles/swizzle_128d.cpp"
        "src/vectorization/swizzles/swizzle_128s.cpp"
        "src/vectorization/swizzles/swizzle_256d.cpp"
        "src/vectorization/swizzles/swizzle_256s.cpp"
        "src/vectorization/v_f32_4/accessors.cpp"
        "src/vectorization/v_f32_4/api.cpp"
        "src/vectorization/v_f32_4/blends.cpp"
        "src/vectorization/v_f32_4/constants.cpp"
        "src/vectorization/v_f32_4/conversions.cpp"
        "src/vectorization/v_f32_4/functions.cpp"
        "src/vectorization/v_f32_4/functions_trigonometry.cpp"
        "src/vectorization/v_f32_4/operators.cpp"
        "src/vectorization/v_f32_4/selects.cpp"
        "src/vectorization/v_f32_4/swizzledBlends.cpp"
        "src/vectorization/v_f32_4/swizzles.cpp"
        "src/vectorization/v_f32_4/type.cpp"
        "src/vectorization/v_f32_4x4.cpp"
        "src/vectorization/v_i32_4.cpp"
        "src/vectorization/v_ui32_4.cpp"
        "src/vectorization/v_ui64_2.cpp"
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    PREFIX "Files"
    REGULAR_EXPRESSION ".+\\.(h|cpp)"
)

add_dependencies(${PROJECT_NAME}
    sources-build-aggregate
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        sources-build-aggregate
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty"
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        _LIB
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

else()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif()
