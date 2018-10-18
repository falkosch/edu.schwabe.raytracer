#pragma once

#include "type.h"
#include "constants.h"

namespace vectorization
{

	template <ASizeT Index, ASizeT Size, typename T>
	inline const T component(const UTVector<Size, T> & v) noexcept
	{
		if constexpr (Index >= 0 && Index < Size) {
			return v.components[Index];
		}
		else {
			return Zero<T>();
		}
	}

	template <ASizeT Size, typename T>
	inline const T x(const UTVector<Size, T> & v) noexcept
	{
		return component<VectorIndices::X>(v);
	}

	template <ASizeT Size, typename T>
	inline const T y(const UTVector<Size, T> & v) noexcept
	{
		return component<VectorIndices::Y>(v);
	}

	template <ASizeT Size, typename T>
	inline const T z(const UTVector<Size, T> & v) noexcept
	{
		return component<VectorIndices::Z>(v);
	}

	template <ASizeT Size, typename T>
	inline const T w(const UTVector<Size, T> & v) noexcept
	{
		return component<VectorIndices::W>(v);
	}

	template <ASizeT Index, ASizeT Size, typename T>
	inline void setComponent(UTVector<Size, T> & v, const T & s) noexcept
	{
		if constexpr (Index >= 0 && Index < Size) {
			v.components[Index] = s;
		}
	}

	template <ASizeT Index, ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceComponent(const UTVector<Size, T> & v, const T & s) noexcept
	{
		UTVector<Size, T> t = v;
		if constexpr (Index >= 0 && Index < Size) {
			t.components[Index] = s;
		}
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceX(const UTVector<Size, T> & v, const T & s) noexcept
	{
		return replaceComponent<VectorIndices::X>(v, s);
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceY(const UTVector<Size, T> & v, const T & s) noexcept
	{
		return replaceComponent<VectorIndices::Y>(v, s);
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceZ(const UTVector<Size, T> & v, const T & s) noexcept
	{
		return replaceComponent<VectorIndices::Z>(v, s);
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceW(const UTVector<Size, T> & v, const T & s) noexcept
	{
		return replaceComponent<VectorIndices::W>(v, s);
	}

}
