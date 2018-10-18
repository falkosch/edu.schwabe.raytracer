#pragma once

#include "api.h"

#include "../functions.h"

#include <array>
#include <cassert>
#include <type_traits>

namespace vectorization
{

	/// Standard implementation of a universal template vector type
	template <ASizeT Size, typename T>
	struct UTVector
	{
		// Metas
		static const ASizeT SIZE = Size;
		typedef T ValueType;
		typedef typename BoolTypes<T>::Type BoolType;
		typedef UTVector<Size, ValueType> VectorType;
		typedef UTVector<Size, BoolType> VectorBoolType;

		// the actual data
		ALIGNED(ARCH_ALIGNMENT, (std::array<ValueType, Size> components));

		UTVector<Size, ValueType>() noexcept
			: components()
		{ }

		explicit UTVector<Size, ValueType>(const ValueType & v) noexcept
			: components()
		{
			StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
				components[i] = v;
			});
		}

		explicit UTVector<Size, ValueType>(const ValueType & x, const ValueType & y) noexcept
			: components()
		{
			if constexpr (VectorIndices::X < Size) {
				components[VectorIndices::X] = x;
			}
			if constexpr (VectorIndices::Y < Size) {
				components[VectorIndices::Y] = y;
			}

			StaticFor<VectorIndices::Z, Size>::apply([&](auto i) {
				components[i] = Zero<ValueType>();
			});
		}

		explicit UTVector<Size, ValueType>(const ValueType & x, const ValueType & y, const ValueType & z) noexcept
			: components()
		{
			if constexpr (VectorIndices::X < Size) {
				components[VectorIndices::X] = x;
			}
			if constexpr (VectorIndices::Y < Size) {
				components[VectorIndices::Y] = y;
			}
			if constexpr (VectorIndices::Z < Size) {
				components[VectorIndices::Z] = z;
			}

			StaticFor<VectorIndices::W, Size>::apply([&](auto i) {
				components[i] = Zero<ValueType>();
			});
		}

		explicit UTVector<Size, ValueType>(const ValueType & x, const ValueType & y, const ValueType & z, const ValueType & w) noexcept
			: components()
		{
			if constexpr (VectorIndices::X < Size) {
				components[VectorIndices::X] = x;
			}
			if constexpr (VectorIndices::Y < Size) {
				components[VectorIndices::Y] = y;
			}
			if constexpr (VectorIndices::Z < Size) {
				components[VectorIndices::Z] = z;
			}
			if constexpr (VectorIndices::W < Size) {
				components[VectorIndices::W] = w;
			}

			StaticFor<VectorIndices::AboveW, Size>::apply([&](auto i) {
				components[i] = Zero<ValueType>();
			});
		}

		explicit UTVector<Size, ValueType>(const std::array<ValueType, Size> & v) noexcept
			: components(v)
		{ }

		ALIGNED_ALLOCATORS(__alignof(VectorType));

		operator std::array<ValueType, Size>() noexcept
		{
			return components;
		}

		operator const std::array<ValueType, Size>() const noexcept
		{
			return components;
		}

		VectorType & operator=(const std::array<ValueType, Size> & v) noexcept
		{
			components = v;
			return *this;
		}

		ValueType & operator[](const int index) noexcept
		{
			assert(static_cast<int>(VectorIndices::X) <= index && index < static_cast<int>(Size));
			return components[index];
		}

		const ValueType & operator[](const int index) const noexcept
		{
			assert(static_cast<int>(VectorIndices::X) <= index && index < static_cast<int>(Size));
			return components[index];
		}

		ValueType & operator[](const ASizeT index) noexcept
		{
			assert(VectorIndices::X <= index && index < Size);
			return components[index];
		}

		const ValueType & operator[](const ASizeT index) const noexcept
		{
			assert(VectorIndices::X <= index && index < Size);
			return components[index];
		}

		// float reciprocal multiply
		static void operator_div_internal(VectorType & out, const VectorType & in, const ValueType & scalar, std::true_type) noexcept
		{
			const ValueType r = reciprocal(scalar);
			StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
				out.components[i] = in[i] * r;
			});
		}

		// integer reciprocal multiply
		static void operator_div_internal(VectorType & out, const VectorType & in, const ValueType & scalar, std::false_type) noexcept
		{
			StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
				out.components[i] = in[i] / scalar;
			});
		}
	};

}
