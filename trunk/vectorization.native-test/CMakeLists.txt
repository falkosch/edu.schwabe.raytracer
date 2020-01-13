project(vectorization.native-test CXX)

# Source groups
set(HEADER_FILES
    "include/Compare.h"
    "include/Iterate.h"
    "include/MessageFormat.h"
    "include/Mutate.h"
    "include/StandardSample.h"
    "include/ToString_specializations.h"
    "include/vectorization_tests.h"
)
source_group("Header files" FILES ${HEADER_FILES})

set(SOURCE_FILES
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
source_group("Source files" FILES ${SOURCE_FILES})

set(ALL_FILES
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

# Target
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE vectorization.test)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)

# Output directory
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build/$ENV{PlatformShortName}/test-$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/$ENV{PlatformShortName}/test-$<CONFIG>/"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build/$ENV{PlatformShortName}/test-$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/$ENV{PlatformShortName}/test-$<CONFIG>/"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()

# Include directories
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../vectorization/include;"
        "$ENV{VCInstallDir}UnitTest/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../vectorization/include;"
        "$ENV{VCInstallDir}UnitTest/include"
    )
endif()

# Compile definitions
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32_LEAN_AND_MEAN;"
        "NOMINMAX;"
        "X64;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32_LEAN_AND_MEAN;"
        "NOMINMAX;"
        "WIN32;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

# Compile and link options
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                ${DEFAULT_CXX_RUNTIME_LIBRARY};
                /Gy;
                /openmp
            >
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /wd4514;
            /wd4710;
            /wd4711;
            /wd4820;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                ${DEFAULT_CXX_RUNTIME_LIBRARY};
                /Gy;
                /openmp
            >
            /std:c++17;
            /sdl;
            /arch:SSE2;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /wd4514;
            /wd4710;
            /wd4711;
            /wd4820;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG:FULL;
            /SUBSYSTEM:WINDOWS
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG:FULL;
            /SUBSYSTEM:WINDOWS
        )
    endif()
endif()

# Dependencies
add_dependencies(${PROJECT_NAME}
    vectorization
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    vectorization
)

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "$ENV{VCInstallDir}UnitTest/lib"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "$ENV{VCInstallDir}UnitTest/lib"
    )
endif()

