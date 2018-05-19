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
	inline const T component(const UTVector<Size, T> & v);

	template <ASizeT Size, typename T>
	inline const T x(const UTVector<Size, T> & v);

	template <ASizeT Size, typename T>
	inline const T y(const UTVector<Size, T> & v);

	template <ASizeT Size, typename T>
	inline const T z(const UTVector<Size, T> & v);

	template <ASizeT Size, typename T>
	inline const T w(const UTVector<Size, T> & v);

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
		typedef std::numeric_limits<T> ValueLimits;

		// Compatible type
		typedef VectorType CompatibleVectorType;

		// the actual data
		ALIGNED(XMM_ALIGNMENT, ValueType components[Size]);

		UTVector<Size, ValueType>()
			: components()
		{ }

		explicit UTVector<Size, ValueType>(const ValueType & v)
		{
			staticFor<VectorIndices::X, Size>([&](auto i) {
				components[i] = v;
			});
		}

		explicit UTVector<Size, ValueType>(const ValueType & x, const ValueType & y)
		{
			staticCheckMinSize(VectorSizes::Y);

			components[VectorIndices::X] = x;
			components[VectorIndices::Y] = y;

			staticFor<VectorIndices::Z, Size>([&](auto i) {
				components[i] = ValueType();
			});
		}

		explicit UTVector<Size, ValueType>(const ValueType & x, const ValueType & y, const ValueType & z)
		{
			staticCheckMinSize(VectorSizes::Z);

			components[VectorIndices::X] = x;
			components[VectorIndices::Y] = y;
			components[VectorIndices::Z] = z;

			staticFor<VectorIndices::W, Size>([&](auto i) {
				components[i] = ValueType();
			});
		}

		explicit UTVector<Size, ValueType>(const ValueType & x, const ValueType & y, const ValueType & z, const ValueType & w)
		{
			staticCheckMinSize(VectorSizes::W);

			components[VectorIndices::X] = x;
			components[VectorIndices::Y] = y;
			components[VectorIndices::Z] = z;
			components[VectorIndices::W] = w;

			staticFor<VectorIndices::AboveW, Size>([&](auto i) {
				components[i] = ValueType();
			});
		}

		explicit UTVector<Size, ValueType>(const std::array<ValueType, Size> & v)
			: components(v)
		{ }

		ALIGNED_ALLOCATORS(__alignof(VectorType));

		operator std::array<ValueType, Size>()
		{
			return components;
		}

		operator const std::array<ValueType, Size>() const
		{
			return components;
		}

		VectorType & operator=(const std::array<ValueType, Size> & v)
		{
			components = v;
			return *this;
		}

		ValueType & operator[](const int index)
		{
			assert(static_cast<int>(VectorIndices::X) <= index && ASizeT(index) < Size);
			return components[index];
		}

		const ValueType & operator[](const int index) const
		{
			assert(static_cast<int>(VectorIndices::X) <= index && ASizeT(index) < Size);
			return components[index];
		}

		ValueType & operator[](const ASizeT index)
		{
			assert(index < Size);
			return components[index];
		}

		const ValueType & operator[](const ASizeT index) const
		{
			assert(index < Size);
			return components[index];
		}

		void setX(const T & v)
		{
			components[VectorIndices::X] = v;
		}

		void setY(const T & v)
		{
			staticCheckMinSize(VectorSizes::Y);
			components[VectorIndices::Y] = v;
		}

		void setZ(const T & v)
		{
			staticCheckMinSize(VectorSizes::Z);
			components[VectorIndices::Z] = v;
		}

		void setW(const T & v)
		{
			staticCheckMinSize(VectorSizes::W);
			components[VectorIndices::W] = v;
		}

		// float reciprocal multiply
		static void operator_div_internal(VectorType & out, const VectorType & in, const ValueType & scalar, std::true_type)
		{
			const ValueType r = reciprocal(scalar);
			staticFor<VectorIndices::X, Size>([&](auto i) {
				out.components[i] = in[i] * r;
			});
		}

		// integer reciprocal multiply
		static void operator_div_internal(VectorType & out, const VectorType & in, const ValueType & scalar, std::false_type)
		{
			staticFor<VectorIndices::X, Size>([&](auto i) {
				out.components[i] = in[i] / scalar;
			});
		}
	};

#pragma region Swizzle helpers
	//{ Swizzle helpers

	template <ASizeT X, ASizeT Y, typename T>
	// generic swizzle
	inline const UTVector<VectorSizes::Y, T> swizzle(const UTVector<VectorSizes::Y, T> & v)
	{
		return UTVector<VectorSizes::Y, T>(component<X>(v), component<Y>(v));
	}

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, typename T>
	// generic swizzle
	inline const UTVector<VectorSizes::W, T> swizzle(const UTVector<VectorSizes::W, T> & v)
	{
		return UTVector<VectorSizes::W, T>(component<X>(v), component<Y>(v), component<Z>(v), component<W>(v));
	}

	template <bool SelectX, bool SelectY, typename T>
	// generic blend
	inline const UTVector<VectorSizes::Y, T> blend(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b)
	{
		return UTVector<VectorSizes::Y, T>(BoolSelector<T, SelectX>(x(b), x(a)), BoolSelector<T, SelectY>(y(b), y(a)));
	}

	template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW, typename T>
	// generic blend
	inline const UTVector<VectorSizes::W, T> blend(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return UTVector<VectorSizes::W, T>(BoolSelector<T, SelectX>(x(b), x(a)), BoolSelector<T, SelectY>(y(b), y(a)), BoolSelector<T, SelectZ>(z(b), z(a)), BoolSelector<T, SelectW>(w(b), w(a)));
	}

	template <ASizeT X, ASizeT Y, bool SelectX, bool SelectY, typename T>
	// generic swizzled blend
	inline const UTVector<VectorSizes::Y, T> swizzledBlend(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b)
	{
		return blend<SelectX, SelectY, T>(swizzle<X, Y, T>(a), swizzle<X, Y, T>(b));
	}

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, bool SelectX, bool SelectY, bool SelectZ, bool SelectW, typename T>
	// generic swizzled blend
	inline const UTVector<VectorSizes::W, T> swizzledBlend(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return blend<SelectX, SelectY, SelectZ, SelectW, T>(swizzle<X, Y, Z, W, T>(a), swizzle<X, Y, Z, W, T>(b));
	}

	template <ASizeT X, ASizeT Y, typename T>
	// Special case swizzled blend of two vectors, in which only the first
	// component of this vector and the last component of the other vector
	// is taken for blending after swizzling.
	inline const UTVector<VectorSizes::Y, T> swizzledBlend_1x1(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b)
	{
		return UTVector<VectorSizes::Y, T>(component<X>(a), component<Y>(b));
	}

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, typename T>
	// Special case swizzled blend of two vectors, in which only the first
	// two components of this vector and the last two components of the
	// other vector are taken for blending after swizzling.
	inline const UTVector<VectorSizes::W, T> swizzledBlend_2x2(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return UTVector<VectorSizes::W, T>(component<X>(a), component<Y>(a), component<Z>(b), component<W>(b));
	}

	template <ASizeT Size, typename T>
	// variadic masked blend
	inline const UTVector<Size, T> blendMasked(const UTVector<Size, T> & a, const UTVector<Size, T> & b, const typename UTVector<Size, T>::VectorBoolType & mask)
	{
		UTVector<Size, T> t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			blendMasked(a[i], b[i], mask[i]);
		});
		return t;
	}

	template <ASizeT X, ASizeT Y, typename T>
	inline const UTVector<VectorSizes::Y, T> swizzledBlendMasked(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::Y, T> & b, const typename UTVector<VectorSizes::Y, T>::VectorBoolType & mask)
	{
		return blendMasked<T>(swizzle<X, Y, T>(a), swizzle<X, Y, T>(b), mask);
	}

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, typename T>
	inline const UTVector<VectorSizes::W, T> swizzledBlendMasked(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b, const typename UTVector<VectorSizes::W, T>::VectorBoolType & mask)
	{
		return blendMasked<T>(swizzle<X, Y, Z, W, T>(a), swizzle<X, Y, Z, W, T>(b), mask);
	}

	//}
#pragma endregion

#pragma region Common swizzle accessors
	//{ Blend swizzle accessors

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> xx(const UTVector<VectorSizes::Y, T> & v)
	{
		return UTVector<VectorSizes::Y, T>(x(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> yy(const UTVector<VectorSizes::Y, T> & v)
	{
		return UTVector<VectorSizes::Y, T>(y(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> x_x(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b)
	{
		return UTVector<VectorSizes::Y, T>(x(a), x(b));
	}

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> x_y(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b)
	{
		return UTVector<VectorSizes::Y, T>(x(a), y(b));
	}

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> y_x(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b)
	{
		return UTVector<VectorSizes::Y, T>(y(a), x(b));
	}

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> y_y(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b)
	{
		return UTVector<VectorSizes::Y, T>(y(a), y(b));
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xxxx(const UTVector<VectorSizes::W, T> & v)
	{
		return UTVector<VectorSizes::W, T>(x(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yyyy(const UTVector<VectorSizes::W, T> & v)
	{
		return UTVector<VectorSizes::W, T>(y(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zzzz(const UTVector<VectorSizes::W, T> & v)
	{
		return UTVector<VectorSizes::W, T>(z(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> wwww(const UTVector<VectorSizes::W, T> & v)
	{
		return UTVector<VectorSizes::W, T>(w(v));
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xxxz(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::Z>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xxyy(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xxzz(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xyxy(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xzzz(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yxxy(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yxwz(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yzxw(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::X, VectorIndices::W>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yzwx(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::X>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zxyw(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::Z, VectorIndices::X, VectorIndices::Y, VectorIndices::W>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zzyy(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zzww(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zwxy(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zwzw(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> wxyz(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::W, VectorIndices::X, VectorIndices::Y, VectorIndices::Z>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> wzyx(const UTVector<VectorSizes::W, T> & v)
	{
		return swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(v);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xx_xx(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xx_yy(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xy_xy(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xy_zw(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xz_xz(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Z, VectorIndices::X, VectorIndices::Z>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yy_yy(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return swizzledBlend_2x2<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yw_yw(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return swizzledBlend_2x2<VectorIndices::Y, VectorIndices::W, VectorIndices::Y, VectorIndices::W>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zz_zz(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return swizzledBlend_2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zw_zw(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(a, b);
	}

	template <typename T>
	inline const UTVector<VectorSizes::W, T> ww_ww(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b)
	{
		return swizzledBlend_2x2<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(a, b);
	}

	//}
#pragma endregion

#pragma region Accessors
	//{ Accessors

	template <ASizeT Index, ASizeT Size, typename T>
	inline const T component(const UTVector<Size, T> & v)
	{
		static_assert(Index < Size, "Index is out of range");
		return v.components[Index];
	}

	template <ASizeT Size, typename T>
	inline const T x(const UTVector<Size, T> & v)
	{
		return component<VectorIndices::X>(v);
	}

	template <ASizeT Size, typename T>
	inline const T y(const UTVector<Size, T> & v)
	{
		return component<VectorIndices::Y>(v);
	}

	template <ASizeT Size, typename T>
	inline const T z(const UTVector<Size, T> & v)
	{
		return component<VectorIndices::Z>(v);
	}

	template <ASizeT Size, typename T>
	inline const T w(const UTVector<Size, T> & v)
	{
		return component<VectorIndices::W>(v);
	}

	template <ASizeT Index, ASizeT Size, typename T>
	inline void setComponent(UTVector<Size, T> & v, const T & s)
	{
		static_assert(Index < Size, "Size is too small");

		v.components[Index] = s;
	}

	template <ASizeT Index, ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceComponent(const UTVector<Size, T> & v, const T & s)
	{
		static_assert(Index < Size, "Size is too small");

		UTVector<Size, T> t = v;
		t.components[Index] = s;
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceX(const UTVector<Size, T> & v, const T & s)
	{
		return replaceComponent<VectorIndices::X>(v, s);
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceY(const UTVector<Size, T> & v, const T & s)
	{
		return replaceComponent<VectorIndices::Y>(v, s);
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceZ(const UTVector<Size, T> & v, const T & s)
	{
		return replaceComponent<VectorIndices::Z>(v, s);
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceW(const UTVector<Size, T> & v, const T & s)
	{
		return replaceComponent<VectorIndices::W>(v, s);
	}

	//}
#pragma endregion

#pragma region Operators
	//{ Operators

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator~(const UTVector<Size, T> & v)
	{
		static_assert(std::is_integral<T>::value, "T must be integral");

		UTVector<Size, T> t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = ~v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator!(const UTVector<Size, T> & v)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(!v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator-(const UTVector<Size, T> & v)
	{
		static_assert(std::is_signed<T>::value, "T must be a signed type");

		UTVector<Size, T> t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = -v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator+(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		UTVector<Size, T> t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = a.components[i] + b.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator+(const UTVector<Size, T> & a, const T & b)
	{
		UTVector<Size, T> t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = a.components[i] + b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator-(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		UTVector<Size, T> t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = a.components[i] - b.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator-(const UTVector<Size, T> & a, const T & b)
	{
		UTVector<Size, T> t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = a.components[i] - b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator*(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		UTVector<Size, T> t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = a.components[i] * b.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator*(const UTVector<Size, T> & a, const T & b)
	{
		UTVector<Size, T> t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = a.components[i] * b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator%(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		UTVector<Size, T> t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = mod(a.components[i], b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator%(const UTVector<Size, T> & a, const T & b)
	{
		UTVector<Size, T> t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = mod(a.components[i], b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator/(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		UTVector<Size, T> t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = a.components[i] / b.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator/(const UTVector<Size, T> & a, const T & b)
	{
		typedef UTVector<Size, T> V;

		V t;
		V::operator_div_internal(t, a, b, std::is_floating_point<T>());
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator<<(const UTVector<Size, T> & a, const typename UTVector<Size, T>::VectorBoolType & b)
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsIn = reinterpret_cast<const B * const>(a.components);
		B * const bitsOut = reinterpret_cast<B * const>(t.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bitsOut[i] = bitsIn[i] << b.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator<<(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b)
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsIn = reinterpret_cast<const B * const>(a.components);
		B * const bitsOut = reinterpret_cast<B * const>(t.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bitsOut[i] = bitsIn[i] << b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator>>(const UTVector<Size, T> & a, const typename UTVector<Size, T>::VectorBoolType & b)
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsIn = reinterpret_cast<const B * const>(a.components);
		B * const bitsOut = reinterpret_cast<B * const>(t.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bitsOut[i] = bitsIn[i] >> b.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator>>(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b)
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsIn = reinterpret_cast<const B * const>(a.components);
		B * const bitsOut = reinterpret_cast<B * const>(t.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bitsOut[i] = bitsIn[i] >> b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator&(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInA = reinterpret_cast<const B * const>(a.components);
		const B * const bitsInB = reinterpret_cast<const B * const>(b.components);
		B * const bitsOut = reinterpret_cast<B * const>(t.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bitsOut[i] = bitsInA[i] & bitsInB[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator&(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b)
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInA = reinterpret_cast<const B * const>(a.components);
		B * const bitsOut = reinterpret_cast<B * const>(t.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bitsOut[i] = bitsInA[i] & b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator|(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInA = reinterpret_cast<const B * const>(a.components);
		const B * const bitsInB = reinterpret_cast<const B * const>(b.components);
		B * const bitsOut = reinterpret_cast<B * const>(t.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bitsOut[i] = bitsInA[i] | bitsInB[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator|(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b)
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInA = reinterpret_cast<const B * const>(a.components);
		B * const bitsOut = reinterpret_cast<B * const>(t.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bitsOut[i] = bitsInA[i] | b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator^(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInA = reinterpret_cast<const B * const>(a.components);
		const B * const bitsInB = reinterpret_cast<const B * const>(b.components);
		B * const bitsOut = reinterpret_cast<B * const>(t.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bitsOut[i] = bitsInA[i] ^ bitsInB[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator^(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b)
	{
		UTVector<Size, T> t;

		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInA = reinterpret_cast<const B * const>(a.components);
		B * const bitsOut = reinterpret_cast<B * const>(t.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bitsOut[i] = bitsInA[i] ^ b;
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator==(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] == b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator==(const UTVector<Size, T> & a, const T & b)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] == b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator!=(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] != b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator!=(const UTVector<Size, T> & a, const T & b)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] != b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator<(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] < b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator<(const UTVector<Size, T> & a, const T & b)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] < b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator<=(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] <= b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator<=(const UTVector<Size, T> & a, const T & b)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] <= b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator>(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] > b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator>(const UTVector<Size, T> & a, const T & b)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] > b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator>=(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] >= b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator>=(const UTVector<Size, T> & a, const T & b)
	{
		typename UTVector<Size, T>::VectorBoolType t;
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = static_cast<typename UTVector<Size, T>::BoolType>(a.components[i] >= b);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator+=(UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		staticFor<VectorIndices::X, Size>([&](auto i) {
			a.components[i] += b.components[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator+=(UTVector<Size, T> & a, const T & b)
	{
		staticFor<VectorIndices::X, Size>([&](auto i) {
			a.components[i] += b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator-=(UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		staticFor<VectorIndices::X, Size>([&](auto i) {
			a.components[i] -= b.components[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator-=(UTVector<Size, T> & a, const T & b)
	{
		staticFor<VectorIndices::X, Size>([&](auto i) {
			a.components[i] -= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator*=(UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		staticFor<VectorIndices::X, Size>([&](auto i) {
			a.components[i] *= b.components[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator*=(UTVector<Size, T> & a, const T & b)
	{
		staticFor<VectorIndices::X, Size>([&](auto i) {
			a.components[i] *= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator/=(UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		staticFor<VectorIndices::X, Size>([&](auto i) {
			a.components[i] /= b.components[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator/=(UTVector<Size, T> & a, const T & b)
	{
		UTVector<Size, T>::operator_div_internal(a, a, b, std::is_floating_point<T>());
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator%=(UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		staticFor<VectorIndices::X, Size>([&](auto i) {
			a.components[i] = mod(a.components[i], b.components[i]);
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator%=(UTVector<Size, T> & a, const T & b)
	{
		staticFor<VectorIndices::X, Size>([&](auto i) {
			a.components[i] = mod(a.components[i], b);
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator&=(UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInB = reinterpret_cast<const B * const>(b.components);
		B * const bits = reinterpret_cast<B * const>(a.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bits[i] &= bitsInB[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator&=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b)
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(a.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bits[i] &= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator|=(UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInB = reinterpret_cast<const B * const>(b.components);
		B * const bits = reinterpret_cast<B * const>(a.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bits[i] |= bitsInB[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator|=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b)
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(a.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bits[i] |= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator^=(UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		typedef typename UTVector<Size, T>::BoolType B;
		const B * const bitsInB = reinterpret_cast<const B * const>(b.components);
		B * const bits = reinterpret_cast<B * const>(a.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bits[i] ^= bitsInB[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator^=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b)
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(a.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bits[i] ^= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator<<=(UTVector<Size, T> & a, const typename UTVector<Size, T>::VectorBoolType & b)
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(a.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bits[i] <<= b.components[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline  UTVector<Size, T> & operator<<=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b)
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(a.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bits[i] <<= b;
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator>>=(UTVector<Size, T> & a, const typename UTVector<Size, T>::VectorBoolType & b)
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(a.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bits[i] >>= b.components[i];
		});
		return a;
	}

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator>>=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b)
	{
		typedef typename UTVector<Size, T>::BoolType B;
		B * const bits = reinterpret_cast<B * const>(a.components);

		staticFor<VectorIndices::X, Size>([&](auto i) {
			bits[i] >>= b;
		});
		return a;
	}

	//}
#pragma endregion

#pragma region Special math operations
	//{ Special math operations

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::BoolType isNegative(const UTVector<Size, T> & v)
	{
		typedef typename UTVector<Size, T>::BoolType B;

		B t = Zero<B>();
		staticFor<VectorIndices::X, Size>([&](auto i) {
			t = (static_cast<B>(v.components[i] < Zero<T>()) & One<B>()) | (t << One<ASizeT>());
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const T dot(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		T accumulator = Zero<T>();
		staticFor<VectorIndices::X, Size>([&](auto i) {
			accumulator += a.components[i] * b.components[i];
		});
		return accumulator;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> dotv(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		return UTVector<Size, T>(dot(a, b));
	}

	template <ASizeT Size, typename T>
	inline const T dot3(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		return x(a) * x(b) + y(a) * y(b) + z(a) * z(b);
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> dot3v(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		return UTVector<Size, T>(dot3(a, b));
	}

	template <ASizeT Size, typename T>
	inline const T length(const UTVector<Size, T> & v)
	{
		return sqrt(dot(v, v));
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> lengthv(const UTVector<Size, T> & v)
	{
		return UTVector<Size, T>(length(v));
	}

	template <ASizeT Size, typename T>
	inline const T length3(const UTVector<Size, T> & v)
	{
		return sqrt(dot3(v, v));
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> length3v(const UTVector<Size, T> & v)
	{
		return UTVector<Size, T>(length3(v));
	}

	template <ASizeT Size, typename T>
	inline const T distance(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		return length(a - b);
	}

	template <ASizeT Size, typename T>
	inline const T distance3(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		auto sqr = [](T v) {
			return v * v;
		};
		return sqrt(sqr(x(a) - x(b)) + sqr(y(a) - y(b)) + sqr(z(a) - z(b)));
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> min(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = min(a.components[i], b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const T min(const UTVector<Size, T> & v)
	{
		T accumulator = v.x();

		staticFor<VectorIndices::X, Size>([&](auto i) {
			accumulator = min(v.components[i], accumulator);
		});
		return accumulator;
	}

	template <ASizeT Size, typename T>
	inline const T min3(const UTVector<Size, T> & v)
	{
		return min(v.x(), min(v.y(), v.z()));
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> max(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = max(a.components[i], b.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const T max(const UTVector<Size, T> & v)
	{
		T accumulator = v.x();

		staticFor<VectorIndices::X, Size>([&](auto i) {
			accumulator = max(v.components[i], accumulator);
		});
		return accumulator;
	}

	template <ASizeT Size, typename T>
	inline const T max3(const UTVector<Size, T> & v)
	{
		return max(x(v), max(y(v), z(v)));
	}

	template <ASizeT Size, typename T>
	inline const T horizontalSum(const UTVector<Size, T> & v)
	{
		T accumulator = Zero<T>();

		staticFor<VectorIndices::X, Size>([&](auto i) {
			accumulator += v.components[i];
		});
		return accumulator;
	}

	template <ASizeT Size, typename T>
	inline const T horizontalSum3(const UTVector<Size, T> & v)
	{
		return x(v) + y(v) + z(v);
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> floor(const UTVector<Size, T> & v)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = floor(v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> fract(const UTVector<Size, T> & v)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = fract(v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> exp(const UTVector<Size, T> & v)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = exp(v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> exp3(const UTVector<Size, T> & v)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, VectorIndices::W>([&](auto i) {
			t.components[i] = exp(v.components[i]);
		});

		staticFor<VectorIndices::W, Size>([&](auto i) {
			t.components[i] = v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> log(const UTVector<Size, T> & v)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = log(v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> log3(const UTVector<Size, T> & v)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, VectorIndices::W>([&](auto i) {
			t.components[i] = log(v.components[i]);
		});

		staticFor<VectorIndices::W, Size>([&](auto i) {
			t.components[i] = v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> clamp(const UTVector<Size, T> & v, const UTVector<Size, T> & l, const UTVector<Size, T> & m)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = clamp(v.components[i], l.components[i], m.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> clamp(const UTVector<Size, T> & v, const T & l, const T & m)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = clamp(v.components[i], l, m);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> abs(const UTVector<Size, T> & v)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = abs(v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> mix(const UTVector<Size, T> & a, const UTVector<Size, T> & b, const UTVector<Size, T> & factor)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = mix(a.components[i], b.components[i], factor.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> mix(const UTVector<Size, T> & a, const UTVector<Size, T> & b, const T & factor)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = mix(a.components[i], b.components[i], factor);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> cross3(const UTVector<Size, T> & a, const UTVector<Size, T> & b)
	{
		UTVector<Size, T> t;
		x(t, y(a) * z(b) - y(b) * z(a));
		y(t, z(a) * x(b) - z(b) * x(a));
		z(t, x(a) * y(b) - x(b) * y(a));

		staticFor<VectorIndices::W, Size>([&](auto i) {
			t.components[i] = Zero<T>();
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> normalize(const UTVector<Size, T> & v)
	{
		return v * reciprocal(length(v));
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> normalize3(const UTVector<Size, T> & v)
	{
		const T rl = reciprocal(length3(v));
		UTVector<Size, T> t;
		x(t, x(v) * rl);
		y(t, y(v) * rl);
		z(t, z(v) * rl);

		staticFor<VectorIndices::W, Size>([&](auto i) {
			t.components[i] = Zero<T>();
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> reciprocal(const UTVector<Size, T> & v)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = reciprocal(v.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> pow(const UTVector<Size, T> & v, const UTVector<Size, T> & exponent)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = pow(v.components[i], exponent.components[i]);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> pow(const UTVector<Size, T> & v, const T & exponent)
	{
		UTVector<Size, T> t;

		staticFor<VectorIndices::X, Size>([&](auto i) {
			t.components[i] = pow(v.components[i], exponent);
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> pow3(const UTVector<Size, T> & v, const UTVector<Size, T> & exponent)
	{
		UTVector<Size, T> t;
		x(t, pow(x(v), x(exponent)));
		y(t, pow(y(v), y(exponent)));
		z(t, pow(z(v), z(exponent)));

		staticFor<VectorIndices::W, Size>([&](auto i) {
			t.components[i] = v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> pow3(const UTVector<Size, T> & v, const T & exponent)
	{
		UTVector<Size, T> t;
		x(t, pow(x(v), exponent));
		y(t, pow(y(v), exponent));
		z(t, pow(z(v), exponent));

		staticFor<VectorIndices::W, Size>([&](auto i) {
			t.components[i] = v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> zeroW(const UTVector<Size, T> & v)
	{
		UTVector<Size, T> t;
		x(t, x(v));
		y(t, y(v));
		z(t, z(v));
		w(t, Zero<T>());

		staticFor<VectorIndices::AboveW, Size>([&](auto i) {
			t.components[i] = v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> oneW(const UTVector<Size, T> & v)
	{
		UTVector<Size, T> t;
		x(t, x(v));
		y(t, y(v));
		z(t, z(v));
		w(t, One<T>());

		staticFor<VectorIndices::AboveW, Size>([&](auto i) {
			t.components[i] = v.components[i];
		});
		return t;
	}

	template <ASizeT Size, typename T>
	inline const bool anyTrue(const UTVector<Size, T> & v)
	{
		for (ASizeT i = VectorIndices::X; i < Size; ++i)
			if (v.components[i] != Zero<T>())
				return true;
		return false;
	}

	template <ASizeT Size, typename T>
	inline const bool anyTrue3(const UTVector<Size, T> & v)
	{
		return !!x(v) | !!y(v) | !!z(v);
	}

	template <ASizeT Size, typename T>
	inline const bool anyFalse(const UTVector<Size, T> & v)
	{
		for (ASizeT i = VectorIndices::X; i < Size; ++i)
			if (v.components[i] == Zero<T>())
				return true;
		return false;
	}

	template <ASizeT Size, typename T>
	inline const bool anyFalse3(const UTVector<Size, T> & v)
	{
		return !x(v) | !y(v) | !z(v);
	}

	template <ASizeT Size, typename T>
	inline const bool allTrue(const UTVector<Size, T> & v)
	{
		return !anyFalse(v);
	}

	template <ASizeT Size, typename T>
	inline const bool allTrue3(const UTVector<Size, T> & v)
	{
		return !anyFalse3(v);
	}

	template <ASizeT Size, typename T>
	inline const bool allFalse(const UTVector<Size, T> & v)
	{
		return !anyTrue(v);
	}

	template <ASizeT Size, typename T>
	inline const bool allFalse3(const UTVector<Size, T> & v)
	{
		return !anyTrue3(v);
	}

	//}
#pragma endregion

	template <ASizeT Size, typename T>
	std::ostream & operator<< (std::ostream & stream, const UTVector<Size, T> & v)
	{
		stream << "{";
		staticFor<VectorIndices::X, Size>([&](auto i) {
			if (i > VectorIndices::X) {
				stream << ", ";
			}
			stream << v.components[i];
		});
		return stream << "}";
	}

}

#undef staticCheckMinSize
