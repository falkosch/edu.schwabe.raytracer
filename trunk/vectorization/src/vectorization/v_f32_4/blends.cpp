#include "vectorization/v_f32_4/blends.h"

#include "vectorization/selects/blends.h"

namespace vectorization
{

	template <>
	const v_f32_4 blend<false, false, false, false>(const v_f32_4 & a, const v_f32_4 &) noexcept
	{
		return a;
	}

	template <>
	const v_f32_4 blend<true, true, true, true>(const v_f32_4 &, const v_f32_4 & b) noexcept
	{
		return b;
	}

	template <>
	const v_f32_4 blend<false, false, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_shuffle_ps(a.components, b.components, _MM_SHUFFLE(VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X));
	}

	template <>
	const v_f32_4 blend<false, true, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		// move x(a) into b
		return _mm_move_ss(b.components, a.components);
	}

	template <>
	const v_f32_4 blend<true, false, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		// move x(b) into a
		return _mm_move_ss(a.components, b.components);
	}


	const v_f32_4 x_yzw(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return blend<false, true, true, true>(a, b);
	}


	const v_f32_4 blendMasked(const v_f32_4 & onBitNotSet, const v_f32_4 & onBitSet, const v_f32_4 & mask) noexcept
	{
		return blendMasked(onBitNotSet.components, onBitSet.components, mask.components);
	}

	const v_f32_4 blendMasked(const v_f32_4 & onBitNotSet, const v_f32_4 & onBitSet, const v_f32_4::VectorBoolType & mask) noexcept
	{
		return blendMasked(onBitNotSet.components, onBitSet.components, _mm_castsi128_ps(mask.components));
	}

}
