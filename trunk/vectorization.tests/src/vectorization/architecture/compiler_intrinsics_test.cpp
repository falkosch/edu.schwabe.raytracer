#include "vectorization/vectorization_tests.h"

#include <array>
#include <algorithm>

namespace vectorization
{
    namespace test
    {

        SCENARIO("4 packed unsigned 32 bit ints can be converted to 4 packed 32 bit floats") {

            GIVEN("multiple ints, converted to scalar floats, and their broadcast representation") {
                auto specimen = std::array<v_ui32_4::ValueType, 5>({ 0, 1, 2, 3, 4 });
                auto packedSpecimen = std::array<v_ui32_4, 5>();
                auto specimenFloats = std::array<v_f32_4::ValueType, 5>();
                auto packedSpecimenFloats = std::array<v_f32_4, 5>();
                staticFor<0, 5>([&](auto i) {
                    auto value = specimen.at(i);
                    packedSpecimen.at(i) == v_ui32_4(value);
                    auto valueFloat = convert<v_f32_4::ValueType>(value);
                    specimenFloats.at(i) == valueFloat;
                    packedSpecimenFloats.at(i) == v_f32_4(valueFloat);
                });

                WHEN("each broadcasted ints is converted to packed floats") {
                    auto packedConverted = std::array<v_f32_4, 5>();
                    std::transform(packedSpecimen.cbegin(), packedSpecimen.cend(), packedConverted.begin(), [](auto v) {
                        return _mm_cvtepu32_ps(v);
                    });

                    THEN("each converted packed floats are component-wise equal to the scalar floats") {
                        for (auto i = 0; i < packedConverted.size(); ++i) {
                            REQUIRE(allTrue(packedSpecimenFloats.at(i) == packedConverted.at(i)));
                        }
                    }
                }
            }
        }

        TEST_CASE("compiler intrinsics")
        {

            SECTION("generates masks for Float4_32")
            {
                REQUIRE(0 + 0 + 0 + 0 == BLEND_MASK_FLOAT4_32(0, 0, 0, 0));
                REQUIRE(1 + 0 + 4 + 0 == BLEND_MASK_FLOAT4_32(1, 0, 1, 0));
                REQUIRE(0 + 2 + 0 + 8 == BLEND_MASK_FLOAT4_32(0, 1, 0, 1));
                REQUIRE(1 + 2 + 4 + 8 == BLEND_MASK_FLOAT4_32(1, 1, 1, 1));
            }

            SECTION("generates masks for Int4_32")
            {
                REQUIRE(0 + 0 + 0 + 0 + 0 + 0 + 0 + 0 == BLEND_MASK_INT4_32(0, 0, 0, 0));
                REQUIRE(1 + 2 + 0 + 0 + 16 + 32 + 0 + 0 == BLEND_MASK_INT4_32(1, 0, 1, 0));
                REQUIRE(0 + 0 + 4 + 8 + 0 + 0 + 64 + 128 == BLEND_MASK_INT4_32(0, 1, 0, 1));
                REQUIRE(1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 == BLEND_MASK_INT4_32(1, 1, 1, 1));
            }

        };

    }
}