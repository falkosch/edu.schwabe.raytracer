#pragma once

#include "type.h"

namespace vectorization
{

	template <ASizeT X, ASizeT Y, bool SelectX, bool SelectY, typename T>
	// generic swizzled blend
	inline const UTVector<VectorSizes::Y, T> swizzledBlend(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept
	{
		return blend<SelectX, SelectY, T>(swizzle<X, Y, T>(a), swizzle<X, Y, T>(b));
	}

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, bool SelectX, bool SelectY, bool SelectZ, bool SelectW, typename T>
	// generic swizzled blend
	inline const UTVector<VectorSizes::W, T> swizzledBlend(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return blend<SelectX, SelectY, SelectZ, SelectW, T>(swizzle<X, Y, Z, W, T>(a), swizzle<X, Y, Z, W, T>(b));
	}

	template <ASizeT X, ASizeT Y, typename T>
	// Special case swizzled blend of two vectors, in which only the first
	// component of this vector and the last component of the other vector
	// is taken for blending after swizzling.
	inline const UTVector<VectorSizes::Y, T> swizzledBlend_1x1(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept
	{
		return UTVector<VectorSizes::Y, T>(component<X>(a), component<Y>(b));
	}

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, typename T>
	// Special case swizzled blend of two vectors, in which only the first
	// two components of this vector and the last two components of the
	// other vector are taken for blending after swizzling.
	inline const UTVector<VectorSizes::W, T> swizzledBlend_2x2(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return UTVector<VectorSizes::W, T>(component<X>(a), component<Y>(a), component<Z>(b), component<W>(b));
	}

	template <ASizeT X, ASizeT Y, typename T>
	inline const UTVector<VectorSizes::Y, T> swizzledBlendMasked(
		const UTVector<VectorSizes::W, T> & a,
		const UTVector<VectorSizes::Y, T> & b,
		const typename UTVector<VectorSizes::Y, T>::VectorBoolType & mask
	) noexcept
	{
		return blendMasked<T>(swizzle<X, Y, T>(a), swizzle<X, Y, T>(b), mask);
	}

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, typename T>
	inline const UTVector<VectorSizes::W, T> swizzledBlendMasked(
		const UTVector<VectorSizes::W, T> & a,
		const UTVector<VectorSizes::W, T> & b,
		const typename UTVector<VectorSizes::W, T>::VectorBoolType & mask
	) noexcept
	{
		return blendMasked<T>(swizzle<X, Y, Z, W, T>(a), swizzle<X, Y, Z, W, T>(b), mask);
	}

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> x_x(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept
	{
		return UTVector<VectorSizes::Y, T>(x(a), x(b));
	}

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> x_y(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept
	{
		return UTVector<VectorSizes::Y, T>(x(a), y(b));
	}

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> y_x(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept
	{
		return UTVector<VectorSizes::Y, T>(y(a), x(b));
	}

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> y_y(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept
	{
		return UTVector<VectorSizes::Y, T>(y(a), y(b));
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xx_xx(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xx_yy(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xy_xy(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xy_zw(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xz_xz(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Z, VectorIndices::X, VectorIndices::Z>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yy_yy(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yw_yw(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::Y, VectorIndices::W, VectorIndices::Y, VectorIndices::W>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zz_zz(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zw_zw(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> ww_ww(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(a, b);
	}

}
