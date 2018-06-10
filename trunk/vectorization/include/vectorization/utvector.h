/*
 * Universal template vector type as standard implementation.
 *
 * @file vectorization/utvector.h
 */

#pragma once

#include "functions.h"

#include <iostream>
#include <array>

#define staticCheckMinSize(size) static_assert(Size >= size, "Size is too small")

namespace vectorization
{

#pragma region Forwards
	//{ Forwards

	template <ASizeT Size, typename T>
	struct UTVector;

	template <ASizeT Index, ASizeT Size, typename T>
	inline const T component(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const T x(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const T y(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const T z(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const T w(const UTVector<Size, T> & v) noexcept;

	//}
#pragma endregion

	/// Standard implementation of an universal template vector type
	template <ASizeT Size, typename T>
	struct UTVector
	{
		// Metas
		static const ASizeT SIZE = Size;
		staticCheckMinSize(VectorSizes::X);

		typedef T ValueType;
		typedef typename BoolTypes<T>::Type BoolType;
		typedef UTVector<Size, ValueType> VectorType;
		typedef UTVector<Size, BoolType> VectorBoolType;

		// Compatible type
		typedef VectorType CompatibleVectorType;

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
			staticCheckMinSize(VectorSizes::Y);

			components[VectorIndices::X] = x;
			components[VectorIndices::Y] = y;

			StaticFor<VectorIndices::Z, Size>::apply([&](auto i) {
				components[i] = ValueType();
			});
		}

		explicit UTVector<Size, ValueType>(const ValueType & x, const ValueType & y, const ValueType & z) noexcept
			: components()
		{
			staticCheckMinSize(VectorSizes::Z);

			components[VectorIndices::X] = x;
			components[VectorIndices::Y] = y;
			components[VectorIndices::Z] = z;

			StaticFor<VectorIndices::W, Size>::apply([&](auto i) {
				components[i] = ValueType();
			});
		}

		explicit UTVector<Size, ValueType>(const ValueType & x, const ValueType & y, const ValueType & z, const ValueType & w) noexcept
			: components()
		{
			staticCheckMinSize(VectorSizes::W);

			components[VectorIndices::X] = x;
			components[VectorIndices::Y] = y;
			components[VectorIndices::Z] = z;
			components[VectorIndices::W] = w;

			StaticFor<VectorIndices::AboveW, Size>::apply([&](auto i) {
				components[i] = ValueType();
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
			assert(static_cast<int>(VectorIndices::X) <= index && ASizeT(index) < Size);
			return components[index];
		}

		const ValueType & operator[](const int index) const noexcept
		{
			assert(static_cast<int>(VectorIndices::X) <= index && ASizeT(index) < Size);
			return components[index];
		}

		ValueType & operator[](const ASizeT index) noexcept
		{
			assert(index < Size);
			return components[index];
		}

		const ValueType & operator[](const ASizeT index) const noexcept
		{
			assert(index < Size);
			return components[index];
		}

		void setX(const T & v) noexcept
		{
			components[VectorIndices::X] = v;
		}

		void setY(const T & v) noexcept
		{
			staticCheckMinSize(VectorSizes::Y);
			components[VectorIndices::Y] = v;
		}

		void setZ(const T & v) noexcept
		{
			staticCheckMinSize(VectorSizes::Z);
			components[VectorIndices::Z] = v;
		}

		void setW(const T & v) noexcept
		{
			staticCheckMinSize(VectorSizes::W);
			components[VectorIndices::W] = v;
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

#pragma region Swizzle helpers
	//{ Swizzle helpers

	template <ASizeT X, ASizeT Y, typename T>
	// generic swizzle
	inline const UTVector<VectorSizes::Y, T> swizzle(const UTVector<VectorSizes::Y, T> & v) noexcept
	{
		return UTVector<VectorSizes::Y, T>(component<X>(v), component<Y>(v));
	}

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, typename T>
	// generic swizzle
	inline const UTVector<VectorSizes::W, T> swizzle(const UTVector<VectorSizes::W, T> & v) noexcept
	{
		return UTVector<VectorSizes::W, T>(component<X>(v), component<Y>(v), component<Z>(v), component<W>(v));
	}

	template <bool SelectX, bool SelectY, typename T>
	// generic blend
	inline const UTVector<VectorSizes::Y, T> blend(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept
	{
		return UTVector<VectorSizes::Y, T>(BoolSelector<T, SelectX>(x(b), x(a)), BoolSelector<T, SelectY>(y(b), y(a)));
	}

	template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW, typename T>
	// generic blend
	inline const UTVector<VectorSizes::W, T> blend(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept
	{
		return UTVector<VectorSizes::W, T>(BoolSelector<T, SelectX>(x(b), x(a)), BoolSelector<T, SelectY>(y(b), y(a)), BoolSelector<T, SelectZ>(z(b), z(a)), BoolSelector<T, SelectW>(w(b), w(a)));
	}

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

	template <ASizeT Size, typename T>
	// variadic masked blend
	inline const UTVector<Size, T> blendMasked(const UTVector<Size, T> & a, const UTVector<Size, T> & b, const typename UTVector<Size, T>::VectorBoolType & mask) noexcept
	{
		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			blendMasked(a[i], b[i], mask[i]);
		});
		return t;
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

	//}
#pragma endregion

#pragma region Common swizzle accessors
	//{ Blend swizzle accessors

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

	//}
#pragma endregion

#pragma region Accessors
	//{ Accessors

	template <ASizeT Index, ASizeT Size, typename T>
	inline const T component(const UTVector<Size, T> & v) noexcept
	{
		static_assert(Index < Size, "Index is out of range");
		return v.components[Index];
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
		static_assert(Index < Size, "Size is too small");

		v.components[Index] = s;
	}

	template <ASizeT Index, ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceComponent(const UTVector<Size, T> & v, const T & s) noexcept
	{
		static_assert(Index < Size, "Size is too small");

		UTVector<Size, T> t = v;
		t.components[Index] = s;
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

	//}
#pragma endregion

#pragma region Operators
	//{ Operators

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
			t.components[i] = mod(a.components[i], b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator%(const UTVector<Size, T> & a, const T & b) noexcept
	{
		UTVector<Size, T> t;
		StaticFor<VectorIndices::X, Size>::apply([&](auto i) {
			t.components[i] = mod(a.components[i], b);
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

	//}
#pragma endregion

#pragma region Special math operations
	//{ Special math operations

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

	//}
#pragma endregion

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

#undef staticCheckMinSize
