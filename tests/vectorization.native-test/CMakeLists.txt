project("vectorization.native-test" LANGUAGES CXX VERSION 1.0.0)

set(SOURCES
        src/vectorization/accessors/component_128d_test.cpp
        src/vectorization/accessors/component_128i_test.cpp
        src/vectorization/accessors/component_128s_test.cpp
        src/vectorization/accessors/component_256d_test.cpp
        src/vectorization/accessors/component_256s_test.cpp
        src/vectorization/accessors/replace_component_128d_test.cpp
        src/vectorization/accessors/replace_component_128i_test.cpp
        src/vectorization/accessors/replace_component_128s_test.cpp
        src/vectorization/accessors/replace_component_256d_test.cpp
        src/vectorization/accessors/replace_component_256s_test.cpp
        src/vectorization/architecture/compiler_definitions_test.cpp
        src/vectorization/architecture/compiler_intrinsics_test.cpp
        src/vectorization/architecture/memory_test.cpp
        src/vectorization/architecture/meta_types_test.cpp
        src/vectorization/blends/blend_128d_test.cpp
        src/vectorization/blends/blend_128s_test.cpp
        src/vectorization/blends/blend_256d_test.cpp
        src/vectorization/blends/blend_256s_test.cpp
        src/vectorization/blends/blend_masked_128d_test.cpp
        src/vectorization/blends/blend_masked_128i_test.cpp
        src/vectorization/blends/blend_masked_128s_test.cpp
        src/vectorization/blends/blend_masked_256d_test.cpp
        src/vectorization/blends/blend_masked_256i_test.cpp
        src/vectorization/blends/blend_masked_256s_test.cpp
        src/vectorization/blends/blend_masked_scalar_test.cpp
        src/vectorization/blends/blend_128d_special_values_test.cpp
        src/vectorization/blends/blend_128s_special_values_test.cpp
        src/vectorization/functions/broadcast_test.cpp
        src/vectorization/constants/masks/mask_all_test.cpp
        src/vectorization/constants/masks/mask_none_test.cpp
        src/vectorization/constants/masks/mask_w_test.cpp
        src/vectorization/constants/masks/mask_x_test.cpp
        src/vectorization/constants/masks/mask_xy_test.cpp
        src/vectorization/constants/masks/mask_xyz_test.cpp
        src/vectorization/constants/masks/mask_xyzw_test.cpp
        src/vectorization/constants/masks/mask_y_test.cpp
        src/vectorization/constants/masks/mask_yz_test.cpp
        src/vectorization/constants/masks/mask_yzw_test.cpp
        src/vectorization/constants/masks/mask_z_test.cpp
        src/vectorization/constants/masks/mask_zw_test.cpp
        src/vectorization/constants/values/degree_to_radian_test.cpp
        src/vectorization/constants/values/epsilon_test.cpp
        src/vectorization/constants/values/half_test.cpp
        src/vectorization/constants/values/identity_test.cpp
        src/vectorization/constants/values/infinity_test.cpp
        src/vectorization/constants/values/negative_infinity_test.cpp
        src/vectorization/constants/values/negative_one_test.cpp
        src/vectorization/constants/values/negative_two_test.cpp
        src/vectorization/constants/values/negative_zero_test.cpp
        src/vectorization/constants/values/not_a_number_test.cpp
        src/vectorization/constants/values/one_half_test.cpp
        src/vectorization/constants/values/one_test.cpp
        src/vectorization/constants/values/one_w_test.cpp
        src/vectorization/constants/values/one_x_test.cpp
        src/vectorization/constants/values/one_y_test.cpp
        src/vectorization/constants/values/one_z_test.cpp
        src/vectorization/constants/values/pi_test.cpp
        src/vectorization/constants/values/radian_to_degree_test.cpp
        src/vectorization/constants/values/radian_to_uniform_test.cpp
        src/vectorization/constants/values/reciprocal_pi_test.cpp
        src/vectorization/constants/values/self_occlusion_epsilon_test.cpp
        src/vectorization/constants/values/sin45_test.cpp
        src/vectorization/constants/values/two_test.cpp
        src/vectorization/constants/values/zero_test.cpp
        src/vectorization/m_f32_4x4/m_f32_4x4_test.cpp
        src/vectorization/selects/select_128d_test.cpp
        src/vectorization/selects/select_128i_test.cpp
        src/vectorization/selects/select_128s_test.cpp
        src/vectorization/selects/select_256d_test.cpp
        src/vectorization/selects/select_256i_test.cpp
        src/vectorization/selects/select_256s_test.cpp
        src/vectorization/selects/select_scalar_test.cpp
        src/vectorization/selects/select_128d_special_values_test.cpp
        src/vectorization/selects/select_128s_special_values_test.cpp
        src/vectorization/selects/select_scalar_special_values_test.cpp
        src/vectorization/swizzled_blends/swizzled_blend_128d_test.cpp
        src/vectorization/swizzled_blends/swizzled_blend_128s_test.cpp
        src/vectorization/swizzled_blends/swizzled_blend_1x1_128d_test.cpp
        src/vectorization/swizzled_blends/swizzled_blend_256d_test.cpp
        src/vectorization/swizzled_blends/swizzled_blend_256s_test.cpp
        src/vectorization/swizzled_blends/swizzled_blend_2x2_128s_test.cpp
        src/vectorization/swizzled_blends/swizzled_blend_2x2_256d_test.cpp
        src/vectorization/swizzled_blends/swizzled_blend_masked_128d_test.cpp
        src/vectorization/swizzles/swizzle_128d_test.cpp
        src/vectorization/swizzles/swizzle_128s_test.cpp
        src/vectorization/swizzles/swizzle_256d_test.cpp
        src/vectorization/swizzles/swizzle_256s_test.cpp
        src/vectorization/v_i32_4/accessors_test.cpp
        src/vectorization/v_i32_4/blends_test.cpp
        src/vectorization/v_i32_4/constants_test.cpp
        src/vectorization/v_i32_4/functions_test.cpp
        src/vectorization/v_i32_4/operators_test.cpp
        src/vectorization/v_i32_4/selects_test.cpp
        src/vectorization/v_i32_4/swizzles_test.cpp
        src/vectorization/v_i32_4/type_test.cpp
        src/vectorization/v_ui32_4/accessors_test.cpp
        src/vectorization/v_ui32_4/blends_test.cpp
        src/vectorization/v_ui32_4/constants_test.cpp
        src/vectorization/v_ui32_4/functions_test.cpp
        src/vectorization/v_ui32_4/operators_test.cpp
        src/vectorization/v_ui32_4/selects_test.cpp
        src/vectorization/v_ui32_4/swizzles_test.cpp
        src/vectorization/v_ui32_4/type_test.cpp
        src/vectorization/v_ui64_2/accessors_test.cpp
        src/vectorization/v_ui64_2/blends_test.cpp
        src/vectorization/v_ui64_2/constants_test.cpp
        src/vectorization/v_ui64_2/functions_test.cpp
        src/vectorization/v_ui64_2/operators_test.cpp
        src/vectorization/v_ui64_2/selects_test.cpp
        src/vectorization/v_ui64_2/swizzles_test.cpp
        src/vectorization/v_ui64_2/type_test.cpp
        src/vectorization/v_f32_4/accessors_test.cpp
        src/vectorization/v_f32_4/blends_test.cpp
        src/vectorization/v_f32_4/constants_test.cpp
        src/vectorization/v_f32_4/conversions_test.cpp
        src/vectorization/v_f32_4/functions_test.cpp
        src/vectorization/v_f32_4/functions_trigonometry_test.cpp
        src/vectorization/v_f32_4/operators_test.cpp
        src/vectorization/v_f32_4/selects_test.cpp
        src/vectorization/v_f32_4/swizzled_blends_test.cpp
        src/vectorization/v_f32_4/swizzles_test.cpp
        src/vectorization/v_f32_4/special_values_test.cpp
        src/vectorization/v_f32_4/type_test.cpp
)

set(HEADERS
        include/compare.h
        include/iterate.h
        include/message_format.h
        include/mutate.h
        include/standard_sample.h
        include/to_string_specializations.h
        include/vectorization_tests.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Files" REGULAR_EXPRESSION ".+\\.(h|cpp)")

add_dependencies(${PROJECT_NAME} vectorization)

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{VCInstallDir}/Auxiliary/VS/UnitTest/lib")

target_link_libraries(${PROJECT_NAME} PRIVATE vectorization)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        "$ENV{VCInstallDir}/Auxiliary/VS/UnitTest/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

else ()
    message(FATAL_ERROR "no configuration for compiler ${CMAKE_CXX_COMPILER_ID}")

endif ()
