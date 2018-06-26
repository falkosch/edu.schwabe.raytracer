#pragma once

#include "type.h"

#include <type_traits>

namespace vectorization
{

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator~(const UTVector<Size, T> & v) noexcept
	{
		static_assert(std::is_integral<T>::value, "T must be integral");

		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = ~v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator!(const UTVector<Size, T> & v) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(!v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator-(const UTVector<Size, T> & v) noexcept
	{
		static_assert(std::is_signed<T>::value, "T must be a signed type");

		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = -v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator+(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = a.components[i] + b.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator+(const UTVector<Size, T> & a, const T & b) noexcept
	{
		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = a.components[i] + b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator-(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = a.components[i] - b.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator-(const UTVector<Size, T> & a, const T & b) noexcept
	{
		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = a.components[i] - b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator*(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = a.components[i] * b.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator*(const UTVector<Size, T> & a, const T & b) noexcept
	{
		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = a.components[i] * b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator%(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = modulo(a.components[i], b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator%(const UTVector<Size, T> & a, const T & b) noexcept
	{
		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = modulo(a.components[i], b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator/(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = a.components[i] / b.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator/(const UTVector<Size, T> & a, const T & b) noexcept
	{
		typedef UTVector<Size, T> V;

		V t;
		V::operator_div_internal(t, a, b, std::is_floating_point<T>());
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator<<(const UTVector<Size, T> & a, const typename UTVector<Size, T>::VectorBoolType & b) noexcept
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsIn = reinterpret_cast<const B * const>(&a.components[0]);
		B * const bitsOut = reinterpret_cast<B * const>(&t.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bitsOut[i] = bitsIn[i] << b.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator<<(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsIn = reinterpret_cast<const B * const>(&a.components[0]);
		B * const bitsOut = reinterpret_cast<B * const>(&t.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bitsOut[i] = bitsIn[i] << b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator>>(const UTVector<Size, T> & a, const typename UTVector<Size, T>::VectorBoolType & b) noexcept
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsIn = reinterpret_cast<const B * const>(&a.components[0]);
		B * const bitsOut = reinterpret_cast<B * const>(&t.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bitsOut[i] = bitsIn[i] >> b.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator>>(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsIn = reinterpret_cast<const B * const>(&a.components[0]);
		B * const bitsOut = reinterpret_cast<B * const>(&t.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bitsOut[i] = bitsIn[i] >> b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator&(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInA = reinterpret_cast<const B * const>(&a.components[0]);
		const B * const bitsInB = reinterpret_cast<const B * const>(&b.components[0]);
		B * const bitsOut = reinterpret_cast<B * const>(&t.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bitsOut[i] = bitsInA[i] & bitsInB[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator&(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInA = reinterpret_cast<const B * const>(&a.components[0]);
		B * const bitsOut = reinterpret_cast<B * const>(&t.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bitsOut[i] = bitsInA[i] & b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator|(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInA = reinterpret_cast<const B * const>(&a.components[0]);
		const B * const bitsInB = reinterpret_cast<const B * const>(&b.components[0]);
		B * const bitsOut = reinterpret_cast<B * const>(&t.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bitsOut[i] = bitsInA[i] | bitsInB[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator|(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInA = reinterpret_cast<const B * const>(&a.components[0]);
		B * const bitsOut = reinterpret_cast<B * const>(&t.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bitsOut[i] = bitsInA[i] | b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator^(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInA = reinterpret_cast<const B * const>(&a.components[0]);
		const B * const bitsInB = reinterpret_cast<const B * const>(&b.components[0]);
		B * const bitsOut = reinterpret_cast<B * const>(&t.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bitsOut[i] = bitsInA[i] ^ bitsInB[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator^(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInA = reinterpret_cast<const B * const>(&a.components[0]);
		B * const bitsOut = reinterpret_cast<B * const>(&t.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bitsOut[i] = bitsInA[i] ^ b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator==(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] == b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator==(const UTVector<Size, T> & a, const T & b) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] == b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator!=(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] != b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator!=(const UTVector<Size, T> & a, const T & b) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] != b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator<(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] < b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator<(const UTVector<Size, T> & a, const T & b) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] < b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator<=(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] <= b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator<=(const UTVector<Size, T> & a, const T & b) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] <= b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator>(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] > b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator>(const UTVector<Size, T> & a, const T & b) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] > b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator>=(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] >= b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator>=(const UTVector<Size, T> & a, const T & b) noexcept
	{
		typename UTVector<Size, T>::VectorBoolType t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] >= b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator+=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			a.components[i] += b.components[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator+=(UTVector<Size, T> & a, const T & b) noexcept
	{
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			a.components[i] += b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator-=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			a.components[i] -= b.components[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator-=(UTVector<Size, T> & a, const T & b) noexcept
	{
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			a.components[i] -= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator*=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			a.components[i] *= b.components[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator*=(UTVector<Size, T> & a, const T & b) noexcept
	{
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			a.components[i] *= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator/=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			a.components[i] /= b.components[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator/=(UTVector<Size, T> & a, const T & b) noexcept
	{
		UTVector<Size, T>::operator_div_internal(a, a, b, std::is_floating_point<T>());
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator%=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			a.components[i] = mod(a.components[i], b.components[i]);
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator%=(UTVector<Size, T> & a, const T & b) noexcept
	{
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			a.components[i] = mod(a.components[i], b);
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator&=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInB = reinterpret_cast<const B * const>(&b.components[0]);
		B * const bits = reinterpret_cast<B * const>(&a.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bits[i] &= bitsInB[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator&=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(&a.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bits[i] &= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator|=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInB = reinterpret_cast<const B * const>(&b.components[0]);
		B * const bits = reinterpret_cast<B * const>(&a.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bits[i] |= bitsInB[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator|=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(&a.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bits[i] |= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator^=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInB = reinterpret_cast<const B * const>(&b.components[0]);
		B * const bits = reinterpret_cast<B * const>(&a.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bits[i] ^= bitsInB[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator^=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(&a.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bits[i] ^= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator<<=(UTVector<Size, T> & a, const typename UTVector<Size, T>::VectorBoolType & b) noexcept
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(&a.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bits[i] <<= b.components[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline  UTVector<Size, T> & operator<<=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(&a.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bits[i] <<= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator>>=(UTVector<Size, T> & a, const typename UTVector<Size, T>::VectorBoolType & b) noexcept
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(&a.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bits[i] >>= b.components[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator>>=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(&a.components[0]);

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			bits[i] >>= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	std::ostream & operator<< (std::ostream & stream, const UTVector<Size, T> & v) noexcept
	{
		stream << "{";
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			if (i > VectorIndices::X) {
				stream << ", ";
			}
			stream << v.components[i];
		});
		return stream << "}";
	}

}
