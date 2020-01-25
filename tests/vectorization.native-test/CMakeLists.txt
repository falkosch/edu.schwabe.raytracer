project("vectorization.native-test"
    LANGUAGES CXX
    VERSION 1.0.0
)

add_library(${PROJECT_NAME}
    SHARED
        "src/vectorization/accessors/component_128d_test.cpp"
        "src/vectorization/accessors/component_128s_test.cpp"
        "src/vectorization/accessors/component_256d_test.cpp"
        "src/vectorization/accessors/component_256s_test.cpp"
        "src/vectorization/accessors/replaceComponent_128d_test.cpp"
        "src/vectorization/accessors/replaceComponent_128s_test.cpp"
        "src/vectorization/accessors/replaceComponent_256d_test.cpp"
        "src/vectorization/accessors/replaceComponent_256s_test.cpp"
        "src/vectorization/architecture/compiler_definitions_test.cpp"
        "src/vectorization/architecture/compiler_intrinsics_test.cpp"
        "src/vectorization/architecture/memory_test.cpp"
        "src/vectorization/architecture/meta_types_test.cpp"
        "src/vectorization/blends/blend_128d_test.cpp"
        "src/vectorization/blends/blend_128s_test.cpp"
        "src/vectorization/blends/blend_256d_test.cpp"
        "src/vectorization/blends/blend_256s_test.cpp"
        "src/vectorization/blends/blendMasked_128d_test.cpp"
        "src/vectorization/blends/blendMasked_128i_test.cpp"
        "src/vectorization/blends/blendMasked_128s_test.cpp"
        "src/vectorization/blends/blendMasked_256d_test.cpp"
        "src/vectorization/blends/blendMasked_256i_test.cpp"
        "src/vectorization/blends/blendMasked_256s_test.cpp"
        "src/vectorization/blends/blendMasked_scalar_test.cpp"
        "src/vectorization/constants/masks/MaskAll_test.cpp"
        "src/vectorization/constants/masks/MaskNone_test.cpp"
        "src/vectorization/constants/masks/MaskW_test.cpp"
        "src/vectorization/constants/masks/MaskX_test.cpp"
        "src/vectorization/constants/masks/MaskXY_test.cpp"
        "src/vectorization/constants/masks/MaskXYZ_test.cpp"
        "src/vectorization/constants/masks/MaskXYZW_test.cpp"
        "src/vectorization/constants/masks/MaskY_test.cpp"
        "src/vectorization/constants/masks/MaskYZ_test.cpp"
        "src/vectorization/constants/masks/MaskYZW_test.cpp"
        "src/vectorization/constants/masks/MaskZ_test.cpp"
        "src/vectorization/constants/masks/MaskZW_test.cpp"
        "src/vectorization/constants/values/DegreeToRadian_test.cpp"
        "src/vectorization/constants/values/Epsilon_test.cpp"
        "src/vectorization/constants/values/Half_test.cpp"
        "src/vectorization/constants/values/Identity_test.cpp"
        "src/vectorization/constants/values/Infinity_test.cpp"
        "src/vectorization/constants/values/NegativeInfinity_test.cpp"
        "src/vectorization/constants/values/NegativeOne_test.cpp"
        "src/vectorization/constants/values/NegativeTwo_test.cpp"
        "src/vectorization/constants/values/NegativeZero_test.cpp"
        "src/vectorization/constants/values/NotANumber_test.cpp"
        "src/vectorization/constants/values/One_test.cpp"
        "src/vectorization/constants/values/OneHalf_test.cpp"
        "src/vectorization/constants/values/OneW_test.cpp"
        "src/vectorization/constants/values/OneX_test.cpp"
        "src/vectorization/constants/values/OneY_test.cpp"
        "src/vectorization/constants/values/OneZ_test.cpp"
        "src/vectorization/constants/values/Pi_test.cpp"
        "src/vectorization/constants/values/RadianToDegree_test.cpp"
        "src/vectorization/constants/values/RadianToUniform_test.cpp"
        "src/vectorization/constants/values/ReciprocalPi_test.cpp"
        "src/vectorization/constants/values/SelfOcclusionEpsilon_test.cpp"
        "src/vectorization/constants/values/Sin45_test.cpp"
        "src/vectorization/constants/values/Two_test.cpp"
        "src/vectorization/constants/values/Zero_test.cpp"
        "src/vectorization/selects/select_128d_test.cpp"
        "src/vectorization/selects/select_128i_test.cpp"
        "src/vectorization/selects/select_128s_test.cpp"
        "src/vectorization/selects/select_256d_test.cpp"
        "src/vectorization/selects/select_256i_test.cpp"
        "src/vectorization/selects/select_256s_test.cpp"
        "src/vectorization/selects/select_scalar_test.cpp"
        "src/vectorization/swizzledBlends/swizzledBlend1x1_128d_test.cpp"
        "src/vectorization/swizzledBlends/swizzledBlend2x2_128s_test.cpp"
        "src/vectorization/swizzledBlends/swizzledBlend2x2_256d_test.cpp"
        "src/vectorization/swizzledBlends/swizzledBlend_128d_test.cpp"
        "src/vectorization/swizzledBlends/swizzledBlend_128s_test.cpp"
        "src/vectorization/swizzledBlends/swizzledBlend_256d_test.cpp"
        "src/vectorization/swizzledBlends/swizzledBlend_256s_test.cpp"
        "src/vectorization/swizzledBlends/swizzledBlendMasked_128d_test.cpp"
        "src/vectorization/swizzles/swizzle_128d_test.cpp"
        "src/vectorization/swizzles/swizzle_128s_test.cpp"
        "src/vectorization/swizzles/swizzle_256d_test.cpp"
        "src/vectorization/swizzles/swizzle_256s_test.cpp"
        "src/vectorization/v_f32_4/accessors_test.cpp"
        "src/vectorization/v_f32_4/blends_test.cpp"
        "src/vectorization/v_f32_4/constants_test.cpp"
        "src/vectorization/v_f32_4/conversions_test.cpp"
        "src/vectorization/v_f32_4/functions_test.cpp"
        "src/vectorization/v_f32_4/functions_trigonometry_test.cpp"
        "src/vectorization/v_f32_4/operators_test.cpp"
        "src/vectorization/v_f32_4/selects_test.cpp"
        "src/vectorization/v_f32_4/swizzledBlends_test.cpp"
        "src/vectorization/v_f32_4/swizzles_test.cpp"
        "src/vectorization/v_f32_4/type_test.cpp"
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    PREFIX "Files"
    REGULAR_EXPRESSION ".+\\.(h|cpp)"
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)

add_dependencies(${PROJECT_NAME}
    vectorization
)

target_link_directories(${PROJECT_NAME}
    PRIVATE
        "$ENV{VCInstallDir}/Auxiliary/VS/UnitTest/lib"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        vectorization
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "$ENV{VCInstallDir}/Auxiliary/VS/UnitTest/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

else()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif()
