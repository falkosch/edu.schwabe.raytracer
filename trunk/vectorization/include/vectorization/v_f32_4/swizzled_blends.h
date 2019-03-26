#pragma once

#include "type.h"
#include "blends.h"

namespace vectorization
{

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
	// generic swizzled blend
	inline const v_f32_4 swizzledBlend(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return blend<SelectX, SelectY, SelectZ, SelectW>(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b));
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, false, false, false, false>(const v_f32_4 & a, const v_f32_4 &) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, true, true, true, true>(const v_f32_4 & a, const v_f32_4 &) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, true, false, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, false, true, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false, true, false, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true, false, true, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false, false, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true, true, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false, true, false, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true, false, true, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true, true, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false, false, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;


	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
	// Special case swizzled blend of two vectors, in which only the first
	// two components of this vector and the last two components of the
	// other vector are taken for blending after swizzling.
	inline const v_f32_4 swizzledBlend_2x2(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		static_assert(X < v_f32_4::SIZE, "Index is out of range");
		static_assert(Y < v_f32_4::SIZE, "Index is out of range");
		static_assert(Z < v_f32_4::SIZE, "Index is out of range");
		static_assert(W < v_f32_4::SIZE, "Index is out of range");
		return _mm_shuffle_ps(a.components, b.components, _MM_SHUFFLE(W, Z, Y, X));
	}

	template <>
	const v_f32_4 swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & a, const v_f32_4 & b) noexcept;


	const v_f32_4 xx_xx(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 xx_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 xy_xy(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 xy_zw(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 xz_xz(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 yx_xy(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 yy_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 yw_yw(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 zz_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 zz_zz(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 zw_zw(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 ww_ww(const v_f32_4 & a, const v_f32_4 & b) noexcept;


	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
	inline const v_f32_4 swizzledBlendMasked(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4 & mask) noexcept
	{
		return blendMasked(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b), mask);
	}

	template <>
	const v_f32_4 swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4 & mask) noexcept;


	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
	inline const v_f32_4 swizzledBlendMasked(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4::VectorBoolType & mask) noexcept
	{
		return swizzledBlendMasked<X, Y, Z, W>(a, b, _mm_castsi128_ps(mask));
	}

	template <>
	const v_f32_4 swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4::VectorBoolType & mask) noexcept;

}
