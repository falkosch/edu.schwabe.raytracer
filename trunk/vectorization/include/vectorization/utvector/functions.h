#pragma once

#include "type.h"
#include "constants.h"

#include "../functions.h"

namespace vectorization
{

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::BoolType isNegative(const UTVector<Size, T> & v) noexcept
	{
		typedef typename UTVector<Size, T>::BoolType B;

		B t = Zero<B>();
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t = (static_cast<B>(v.components[i] < Zero<T>()) & One<B>()) | (t << One<ASizeT>());
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const T dot(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		T accumulator = Zero<T>();
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			accumulator += a.components[i] * b.components[i];
		});
		return accumulator;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> dotv(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		return UTVector<Size, T>(dot(a, b));
	}

	template <ASizeT Size, typename T>
	inline const T dot3(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		return x(a) * x(b) + y(a) * y(b) + z(a) * z(b);
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> dot3v(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		return UTVector<Size, T>(dot3(a, b));
	}

	template <ASizeT Size, typename T>
	inline const T length(const UTVector<Size, T> & v) noexcept
	{
		return sqrt(dot(v, v));
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> lengthv(const UTVector<Size, T> & v) noexcept
	{
		return UTVector<Size, T>(length(v));
	}

	template <ASizeT Size, typename T>
	inline const T length3(const UTVector<Size, T> & v) noexcept
	{
		return sqrt(dot3(v, v));
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> length3v(const UTVector<Size, T> & v) noexcept
	{
		return UTVector<Size, T>(length3(v));
	}

	template <ASizeT Size, typename T>
	inline const T distance(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		return length(a - b);
	}

	template <ASizeT Size, typename T>
	inline const T distance3(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		auto sqr = [](T v) {
			return v * v;
		};
		return sqrt(sqr(x(a) - x(b)) + sqr(y(a) - y(b)) + sqr(z(a) - z(b)));
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> min(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = min(a.components[i], b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const T min(const UTVector<Size, T> & v) noexcept
	{
		T accumulator = v.x();

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			accumulator = min(v.components[i], accumulator);
		});
		return accumulator;
	}

	template <ASizeT Size, typename T>
	inline const T min3(const UTVector<Size, T> & v) noexcept
	{
		return min(v.x(), min(v.y(), v.z()));
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> max(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = max(a.components[i], b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const T max(const UTVector<Size, T> & v) noexcept
	{
		T accumulator = v.x();

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			accumulator = max(v.components[i], accumulator);
		});
		return accumulator;
	}

	template <ASizeT Size, typename T>
	inline const T max3(const UTVector<Size, T> & v) noexcept
	{
		return max(x(v), max(y(v), z(v)));
	}

	template <ASizeT Size, typename T>
	inline const T horizontalSum(const UTVector<Size, T> & v) noexcept
	{
		T accumulator = Zero<T>();

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			accumulator += v.components[i];
		});
		return accumulator;
	}

	template <ASizeT Size, typename T>
	inline const T horizontalSum3(const UTVector<Size, T> & v) noexcept
	{
		return x(v) + y(v) + z(v);
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> floor(const UTVector<Size, T> & v) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = floor(v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> fract(const UTVector<Size, T> & v) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = fract(v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> exp(const UTVector<Size, T> & v) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = exp(v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> exp3(const UTVector<Size, T> & v) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, VectorIndices::W>::apply([&](auto i) {
			t.components[i] = exp(v.components[i]);
		});

		StaticFor<VectorIndices::W, Size>::apply([&](auto i) {
			t.components[i] = v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> log(const UTVector<Size, T> & v) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = log(v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> log3(const UTVector<Size, T> & v) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, VectorIndices::W>::apply([&](auto i) {
			t.components[i] = log(v.components[i]);
		});

		StaticFor<VectorIndices::W, Size>::apply([&](auto i) {
			t.components[i] = v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> clamp(const UTVector<Size, T> & v, const UTVector<Size, T> & l, const UTVector<Size, T> & m) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = clamp(v.components[i], l.components[i], m.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> clamp(const UTVector<Size, T> & v, const T & l, const T & m) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = clamp(v.components[i], l, m);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> abs(const UTVector<Size, T> & v) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = abs(v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> mix(const UTVector<Size, T> & a, const UTVector<Size, T> & b, const UTVector<Size, T> & factor) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = mix(a.components[i], b.components[i], factor.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> mix(const UTVector<Size, T> & a, const UTVector<Size, T> & b, const T & factor) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = mix(a.components[i], b.components[i], factor);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> cross3(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept
	{
		UTVector<Size, T> t;
		x(t, y(a) * z(b) - y(b) * z(a));
		y(t, z(a) * x(b) - z(b) * x(a));
		z(t, x(a) * y(b) - x(b) * y(a));

		StaticFor<VectorIndices::W, Size>::apply([&](auto i) {
			t.components[i] = Zero<T>();
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> normalize(const UTVector<Size, T> & v) noexcept
	{
		return v * reciprocal(length(v));
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> normalize3(const UTVector<Size, T> & v) noexcept
	{
		const T rl = reciprocal(length3(v));
		UTVector<Size, T> t;
		x(t, x(v) * rl);
		y(t, y(v) * rl);
		z(t, z(v) * rl);

		StaticFor<VectorIndices::W, Size>::apply([&](auto i) {
			t.components[i] = Zero<T>();
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> reciprocal(const UTVector<Size, T> & v) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = reciprocal(v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> pow(const UTVector<Size, T> & v, const UTVector<Size, T> & exponent) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = pow(v.components[i], exponent.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> pow(const UTVector<Size, T> & v, const T & exponent) noexcept
	{
		UTVector<Size, T> t;

		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = pow(v.components[i], exponent);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> pow3(const UTVector<Size, T> & v, const UTVector<Size, T> & exponent) noexcept
	{
		UTVector<Size, T> t;
		x(t, pow(x(v), x(exponent)));
		y(t, pow(y(v), y(exponent)));
		z(t, pow(z(v), z(exponent)));

		StaticFor<VectorIndices::W, Size>::apply([&](auto i) {
			t.components[i] = v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> pow3(const UTVector<Size, T> & v, const T & exponent) noexcept
	{
		UTVector<Size, T> t;
		x(t, pow(x(v), exponent));
		y(t, pow(y(v), exponent));
		z(t, pow(z(v), exponent));

		StaticFor<VectorIndices::W, Size>::apply([&](auto i) {
			t.components[i] = v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> zeroW(const UTVector<Size, T> & v) noexcept
	{
		UTVector<Size, T> t;
		x(t, x(v));
		y(t, y(v));
		z(t, z(v));
		w(t, Zero<T>());

		StaticFor<VectorIndices::AboveW, Size>::apply([&](auto i) {
			t.components[i] = v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> oneW(const UTVector<Size, T> & v) noexcept
	{
		UTVector<Size, T> t;
		x(t, x(v));
		y(t, y(v));
		z(t, z(v));
		w(t, One<T>());

		StaticFor<VectorIndices::AboveW, Size>::apply([&](auto i) {
			t.components[i] = v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const bool anyTrue(const UTVector<Size, T> & v) noexcept
	{
		for (ASizeT i = VectorIndices::X; i < Size; ++i)
			if (v.components[i] != Zero<T>())
				return true;
		return false;
	}

	template <ASizeT Size, typename T>
	inline const bool anyTrue3(const UTVector<Size, T> & v) noexcept
	{
		return !!x(v) | !!y(v) | !!z(v);
	}

	template <ASizeT Size, typename T>
	inline const bool anyFalse(const UTVector<Size, T> & v) noexcept
	{
		for (ASizeT i = VectorIndices::X; i < Size; ++i)
			if (v.components[i] == Zero<T>())
				return true;
		return false;
	}

	template <ASizeT Size, typename T>
	inline const bool anyFalse3(const UTVector<Size, T> & v) noexcept
	{
		return !x(v) | !y(v) | !z(v);
	}

	template <ASizeT Size, typename T>
	inline const bool allTrue(const UTVector<Size, T> & v) noexcept
	{
		return !anyFalse(v);
	}

	template <ASizeT Size, typename T>
	inline const bool allTrue3(const UTVector<Size, T> & v) noexcept
	{
		return !anyFalse3(v);
	}

	template <ASizeT Size, typename T>
	inline const bool allFalse(const UTVector<Size, T> & v) noexcept
	{
		return !anyTrue(v);
	}

	template <ASizeT Size, typename T>
	inline const bool allFalse3(const UTVector<Size, T> & v) noexcept
	{
		return !anyTrue3(v);
	}

}
