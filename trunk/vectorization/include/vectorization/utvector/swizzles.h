#pragma once

#include "type.h"

namespace vectorization
{

	template <ASizeT X, ASizeT Y, typename T>
	inline const UTVector<VectorSizes::Y, T> swizzle(const UTVector<VectorSizes::Y, T> & v) noexcept
	{
		return UTVector<VectorSizes::Y, T>(component<X>(v), component<Y>(v));
	}

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, typename T>
	inline const UTVector<VectorSizes::W, T> swizzle(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return UTVector<VectorSizes::W, T>(component<X>(v), component<Y>(v), component<Z>(v), component<W>(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> xx(const UTVector<VectorSizes::Y, T> & v) noexcept
	{
		return UTVector<VectorSizes::Y, T>(x(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> yy(const UTVector<VectorSizes::Y, T> & v) noexcept
	{
		return UTVector<VectorSizes::Y, T>(y(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xxxx(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return UTVector<VectorSizes::W, T>(x(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yyyy(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return UTVector<VectorSizes::W, T>(y(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zzzz(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return UTVector<VectorSizes::W, T>(z(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> wwww(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return UTVector<VectorSizes::W, T>(w(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xxxz(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::Z>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xxyy(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xxzz(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xyxy(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xzzz(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yxxy(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yxwz(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yzxw(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::X, VectorIndices::W>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yzwx(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::X>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zxyw(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::X, VectorIndices::Y, VectorIndices::W>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zzyy(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zzww(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zwxy(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zwzw(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> wxyz(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::W, VectorIndices::X, VectorIndices::Y, VectorIndices::Z>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> wzyx(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(v);
	}

}
