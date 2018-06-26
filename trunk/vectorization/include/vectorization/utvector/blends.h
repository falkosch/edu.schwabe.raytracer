#pragma once

#include "type.h"

#include "../functions.h"

namespace vectorization
{

	template <bool SelectX, bool SelectY, typename T>
	inline const UTVector<VectorSizes::Y, T> blend(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept
	{
		return UTVector<VectorSizes::Y, T>(BoolSelector<T, SelectX>(x(b), x(a)), BoolSelector<T, SelectY>(y(b), y(a)));
	}

	template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW, typename T>
	inline const UTVector<VectorSizes::W, T> blend(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return UTVector<VectorSizes::W, T>(BoolSelector<T, SelectX>(x(b), x(a)), BoolSelector<T, SelectY>(y(b), y(a)), BoolSelector<T, SelectZ>(z(b), z(a)), BoolSelector<T, SelectW>(w(b), w(a)));
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> blendMasked(const UTVector<Size, T> & a, const UTVector<Size, T> & b, const typename UTVector<Size, T>::VectorBoolType & mask) noexcept
	{
		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			blendMasked(a[i], b[i], mask[i]);
		});
		return t;
	}

}
