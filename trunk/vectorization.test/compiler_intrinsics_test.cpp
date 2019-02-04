#include "pch.h"

#include "vectorization/architecture/compiler_intrinsics.h"

#include <array>

TEST(CompilerIntrinsics, GeneratesMasksForFloat4_32) {
	EXPECT_EQ(0 + 0 + 0 + 0, BLEND_MASK_FLOAT4_32(0, 0, 0, 0));
	EXPECT_EQ(1 + 0 + 4 + 0, BLEND_MASK_FLOAT4_32(1, 0, 1, 0));
	EXPECT_EQ(0 + 2 + 0 + 8, BLEND_MASK_FLOAT4_32(0, 1, 0, 1));
	EXPECT_EQ(1 + 2 + 4 + 8, BLEND_MASK_FLOAT4_32(1, 1, 1, 1));
}

TEST(CompilerIntrinsics, GeneratesMasksForInt4_32) {
	EXPECT_EQ(0 + 0 + 0 + 0 + 0 + 0 + 0 + 0, BLEND_MASK_INT4_32(0, 0, 0, 0));
	EXPECT_EQ(1 + 2 + 0 + 0 + 16 + 32 + 0 + 0, BLEND_MASK_INT4_32(1, 0, 1, 0));
	EXPECT_EQ(0 + 0 + 4 + 8 + 0 + 0 + 64 + 128, BLEND_MASK_INT4_32(0, 1, 0, 1));
	EXPECT_EQ(1 + 2 + 4 + 8 + 16 + 32 + 64 + 128, BLEND_MASK_INT4_32(1, 1, 1, 1));
}

TEST(CompilerIntrinsics, ConvertsUnsignedIntegersToFloats) {

	std::array<vectorization::UInt_32, 5> specimen = { 0, 1, 2, 3, 4 };

	for (const auto & v : specimen) {
		auto test = _mm_set1_epi32(v);
		auto actual = vectorization::_mm_cvtepu32_ps(test);
		auto expected = _mm_set1_ps(static_cast<vectorization::Float_32>(v));
		EXPECT_EQ(1, _mm_comieq_ss(expected, actual));
	}
}
