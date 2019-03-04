#include "vectorization/v_ui64_2.h"

#include <limits>
#include <cassert>

namespace vectorization
{

#pragma region v_ui64_2::
	//{ v_ui64_2::

	v_ui64_2::v_ui64_2() noexcept
		: components(Zero<v_ui64_2::PackedType>())
	{ }

	v_ui64_2::v_ui64_2(const v_ui64_2::PackedType & v) noexcept
		: components(v)
	{ }

	v_ui64_2::v_ui64_2(const v_ui64_2::ValueType s) noexcept
		:
#ifdef ARCH_X64
		components(_mm_set1_epi64x(static_cast<long long>(s)))
	{ }
#else
		components()
	{
		setComponent<VectorIndices::X>(*this, s);
		setComponent<VectorIndices::Y>(*this, s);
	}
#endif

	v_ui64_2::v_ui64_2(const v_ui64_2::ValueType x, const v_ui64_2::ValueType y) noexcept
		:
#ifdef ARCH_X64
		components(_mm_set_epi64x(static_cast<long long>(y), static_cast<long long>(x)))
	{ }
#else
		components()
	{
		setComponent<VectorIndices::X>(*this, x);
		setComponent<VectorIndices::Y>(*this, y);
	}
#endif

	v_ui64_2 & v_ui64_2::operator=(const v_ui64_2::PackedType & v) noexcept
	{
		components = v;
		return *this;
	}

	v_ui64_2::ValueType & v_ui64_2::operator[](const int index) noexcept
	{
		assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
		return reinterpret_cast<v_ui64_2::ValueType * const>(this)[index];
	}

	const v_ui64_2::ValueType & v_ui64_2::operator[](const int index) const noexcept
	{
		assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
		return reinterpret_cast<const v_ui64_2::ValueType * const>(this)[index];
	}

	v_ui64_2::ValueType & v_ui64_2::operator[](const ASizeT index) noexcept
	{
		assert(index < SIZE);
		return reinterpret_cast<v_ui64_2::ValueType * const>(this)[index];
	}

	const v_ui64_2::ValueType & v_ui64_2::operator[](const ASizeT index) const noexcept
	{
		assert(index < SIZE);
		return reinterpret_cast<const v_ui64_2::ValueType * const>(this)[index];
	}

	//}
#pragma endregion

#pragma region Constant generators
	//{Constant generators

	template <>
	const v_ui64_2 MaskAll<v_ui64_2>() noexcept
	{
		return MaskAll<v_ui64_2::PackedType>();
	}

	template <>
	const v_ui64_2 Zero<v_ui64_2>() noexcept
	{
		return Zero<v_ui64_2::PackedType>();
	}

	template <>
	const v_ui64_2 One<v_ui64_2>() noexcept
	{
		return _mm_srli_epi64(MaskAll<v_ui64_2::PackedType>(), sizeof(v_ui64_2::ValueType) * std::numeric_limits<unsigned char>::digits - 1);
	}

	template <>
	const v_ui64_2 Two<v_ui64_2>() noexcept
	{
		return _mm_slli_epi64(_mm_srli_epi32(MaskAll<v_ui64_2::PackedType>(), sizeof(v_ui64_2::ValueType) * std::numeric_limits<unsigned char>::digits - 1), 1);
	}

	//}
#pragma endregion

#pragma region Commonly used swizzle accessors
	// Commonly used swizzle accessors

	const v_ui64_2 xx(const v_ui64_2 & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::X>(v);
	}

	const v_ui64_2 xy(const v_ui64_2 & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::Y>(v);
	}

	const v_ui64_2 yx(const v_ui64_2 & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::X>(v);
	}

	const v_ui64_2 yy(const v_ui64_2 & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::Y>(v);
	}

	const v_ui64_2 x_x(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return swizzledBlend_1x1<VectorIndices::X, VectorIndices::X>(a, b);
	}

	const v_ui64_2 x_y(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return swizzledBlend_1x1<VectorIndices::X, VectorIndices::Y>(a, b);
	}

	const v_ui64_2 y_x(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return swizzledBlend_1x1<VectorIndices::Y, VectorIndices::X>(a, b);
	}

	const v_ui64_2 y_y(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return swizzledBlend_1x1<VectorIndices::Y, VectorIndices::Y>(a, b);
	}

	//}
#pragma endregion

#pragma region Operators
	//{ Operators

	const v_ui64_2 operator!(const v_ui64_2 & v) noexcept
	{
		return _mm_cmpeq_epi64(Zero<v_ui64_2::PackedType>(), v.components);
	}

	const v_ui64_2 operator~(const v_ui64_2 & v) noexcept
	{
		return _mm_andnot_si128(v.components, MaskAll<v_ui64_2::PackedType>());
	}

	const v_ui64_2 operator+(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_add_epi64(a.components, b.components);
	}

	const v_ui64_2 operator-(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_sub_epi64(a.components, b.components);
	}

	const v_ui64_2 operator*(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return v_ui64_2(x(a) * x(b), y(a) * y(b));
	}

	const v_ui64_2 operator/(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return v_ui64_2(x(a) / x(b), y(a) / y(b));
	}

	const v_ui64_2 operator%(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return v_ui64_2(x(a) % x(b), y(a) % y(b));
	}

	const v_ui64_2 operator&(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_and_si128(a.components, b.components);
	}

	const v_ui64_2 operator|(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_or_si128(a.components, b.components);
	}

	const v_ui64_2 operator^(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_xor_si128(a.components, b.components);
	}

	const v_ui64_2 operator<<(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_sll_epi64(a.components, b.components);
	}

	const v_ui64_2 operator>>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_srl_epi64(a.components, b.components);
	}

	const v_ui64_2 operator<(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return v_ui64_2(
			select(x(a) < x(b), MaskAll<v_ui64_2::ValueType>(), Zero<v_ui64_2::ValueType>()),
			select(y(a) < y(b), MaskAll<v_ui64_2::ValueType>(), Zero<v_ui64_2::ValueType>())
		);
#else
		// requires sse4.2
		return !(a >= b);
#endif
	}

	const v_ui64_2 operator>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return v_ui64_2(
			select(x(a) > x(b), MaskAll<v_ui64_2::ValueType>(), Zero<v_ui64_2::ValueType>()),
			select(y(a) > y(b), MaskAll<v_ui64_2::ValueType>(), Zero<v_ui64_2::ValueType>())
		);
#else
		// requires sse4.2
		return _mm_cmpgt_epi64(a.components, b.components);
#endif
	}

	const v_ui64_2 operator<=(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return v_ui64_2(
			select(x(a) <= x(b), MaskAll<v_ui64_2::ValueType>(), Zero<v_ui64_2::ValueType>()),
			select(y(a) <= y(b), MaskAll<v_ui64_2::ValueType>(), Zero<v_ui64_2::ValueType>())
		);
#else
		// requires sse4.2
		return !(a > b);
#endif
	}

	const v_ui64_2 operator>=(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return v_ui64_2(
			select(x(a) >= x(b), MaskAll<v_ui64_2::ValueType>(), Zero<v_ui64_2::ValueType>()),
			select(y(a) >= y(b), MaskAll<v_ui64_2::ValueType>(), Zero<v_ui64_2::ValueType>())
		);
#else
		// requires sse4.2
		return (a > b) | (a == b);
#endif
	}

	const v_ui64_2 operator==(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_cmpeq_epi64(a.components, b.components);
	}

	const v_ui64_2 operator!=(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return !(a == b);
	}

	v_ui64_2 & operator+=(v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return a = a + b;
	}

	v_ui64_2 & operator-=(v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return a = a - b;
	}

	v_ui64_2 & operator*=(v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return a = a * b;
	}

	v_ui64_2 & operator/=(v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return a = a / b;
	}

	v_ui64_2 & operator%=(v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return a = a % b;
	}

	v_ui64_2 & operator&=(v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return a = a & b;
	}

	v_ui64_2 & operator|=(v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return a = a | b;
	}

	v_ui64_2 & operator^=(v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return a = a ^ b;
	}

	v_ui64_2 & operator<<=(v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return a = a << b;
	}

	v_ui64_2 & operator>>=(v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return a = a >> b;
	}

	//}
#pragma endregion

#pragma region swizzle()
	//{ swizzle()

	template <>
	const v_ui64_2 swizzle<VectorIndices::X, VectorIndices::X>(const v_ui64_2 & v) noexcept
	{
		return _mm_unpacklo_epi64(v.components, v.components);
	}

	template <>
	const v_ui64_2 swizzle<VectorIndices::X, VectorIndices::Y>(const v_ui64_2 & v) noexcept
	{
		return v;
	}

	template <>
	const v_ui64_2 swizzle<VectorIndices::Y, VectorIndices::X>(const v_ui64_2 & v) noexcept
	{
		return _mm_unpacklo_epi64(_mm_unpackhi_epi64(v.components, v.components), v.components);
	}

	template <>
	const v_ui64_2 swizzle<VectorIndices::Y, VectorIndices::Y>(const v_ui64_2 & v) noexcept
	{
		return _mm_unpackhi_epi64(v.components, v.components);
	}

	//}
#pragma endregion

#pragma region blend()
	//{ blend()

	template <>
	const v_ui64_2 blend<false, false>(const v_ui64_2 & a, const v_ui64_2 &) noexcept
	{
		return a;
	}

	template <>
	const v_ui64_2 blend<false, true>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		// 'x' of a and 'y' of b
		return _mm_unpacklo_epi64(a.components, _mm_unpackhi_epi64(b.components, b.components));
	}

	template <>
	const v_ui64_2 blend<true, false>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		// 'x' of b and 'y' of a
		return _mm_unpacklo_epi64(b.components, _mm_unpackhi_epi64(a.components, a.components));
	}

	template <>
	const v_ui64_2 blend<true, true>(const v_ui64_2 &, const v_ui64_2 & b) noexcept
	{
		return b;
	}

	//}
#pragma endregion

#pragma region swizzledBlend()
	//{ swizzledBlend()

	template <>
	const v_ui64_2 swizzledBlend<VectorIndices::X, VectorIndices::X, false, true>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_unpacklo_epi64(a.components, b.components);
	}

	template <>
	const v_ui64_2 swizzledBlend<VectorIndices::X, VectorIndices::X, true, false>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_unpacklo_epi64(b.components, a.components);
	}

	template <>
	const v_ui64_2 swizzledBlend<VectorIndices::Y, VectorIndices::Y, false, true>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_unpackhi_epi64(a.components, b.components);
	}

	template <>
	const v_ui64_2 swizzledBlend<VectorIndices::Y, VectorIndices::Y, true, false>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_unpackhi_epi64(b.components, a.components);
	}

	template <>
	const v_ui64_2 swizzledBlend_1x1<VectorIndices::X, VectorIndices::X>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_unpacklo_epi64(a.components, b.components);
	}

	template <>
	const v_ui64_2 swizzledBlend_1x1<VectorIndices::X, VectorIndices::Y>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_unpacklo_epi64(a.components, _mm_unpackhi_epi64(b.components, b.components));
	}

	template <>
	const v_ui64_2 swizzledBlend_1x1<VectorIndices::Y, VectorIndices::X>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_unpackhi_epi64(a.components, _mm_unpacklo_epi64(b.components, b.components));
	}

	template <>
	const v_ui64_2 swizzledBlend_1x1<VectorIndices::Y, VectorIndices::Y>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return _mm_unpackhi_epi64(a.components, b.components);
	}

	//}
#pragma endregion

#pragma region blendMasked()
	//{ blendMasked()

	const v_ui64_2 blendMasked(const v_ui64_2 & a, const v_ui64_2 & b, const v_ui64_2 & mask) noexcept
	{
		return _mm_blendv_epi8(a.components, b.components, mask.components);
	}

	//}
#pragma endregion

#pragma region swizzledBlendMasked()
	//{ swizzledBlendMasked()

	template <>
	const v_ui64_2 swizzledBlendMasked<VectorIndices::X, VectorIndices::Y>(const v_ui64_2 & a, const v_ui64_2 & b, const v_ui64_2 & mask) noexcept
	{
		return blendMasked(a, b, mask);
	}

	//}
#pragma endregion

#pragma region setComponent()
	//{ setComponent()

	template <>
	void setComponent<VectorIndices::X>(v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept
	{
#ifdef ARCH_X64
		v.components = _mm_insert_epi64(v.components, static_cast<long long>(s), VectorIndices::X);
#else
		reinterpret_cast<v_ui64_2::ValueType * const>(&v)[VectorIndices::X] = s;
#endif
	}

	template <>
	void setComponent<VectorIndices::Y>(v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept
	{
#ifdef ARCH_X64
		v.components = _mm_insert_epi64(v.components, static_cast<long long>(s), VectorIndices::Y);
#else
		reinterpret_cast<v_ui64_2::ValueType * const>(&v)[VectorIndices::Y] = s;
#endif
	}

	//}
#pragma endregion

#pragma region replaceComponent()
	//{ replaceComponent()

	template <>
	const v_ui64_2 replaceComponent<VectorIndices::X>(const v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept
	{
#ifdef ARCH_X64
		return _mm_insert_epi64(v.components, static_cast<long long>(s), VectorIndices::X);
#else
		return v_ui64_2(s, component<VectorIndices::Y>(v));
#endif
	}

	template <>
	const v_ui64_2 replaceComponent<VectorIndices::Y>(const v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept
	{
#ifdef ARCH_X64
		return _mm_insert_epi64(v.components, static_cast<long long>(s), VectorIndices::Y);
#else
		return v_ui64_2(component<VectorIndices::X>(v), s);
#endif
	}

	//}
#pragma endregion

#pragma region component()
	//{ component()

	template <>
	const v_ui64_2::ValueType component<VectorIndices::X>(const v_ui64_2 & v) noexcept
	{
#ifdef ARCH_X64
		return static_cast<v_ui64_2::ValueType>(_mm_cvtsi128_si64x(v.components));
#else
		return reinterpret_cast<const v_ui64_2::ValueType * const>(&v)[VectorIndices::X];
#endif
	}

	template <>
	const v_ui64_2::ValueType component<VectorIndices::Y>(const v_ui64_2 & v) noexcept
	{
#ifdef ARCH_X64
		return static_cast<v_ui64_2::ValueType>(_mm_cvtsi128_si64x(_mm_unpackhi_epi64(v.components, v.components)));
#else
		return reinterpret_cast<const v_ui64_2::ValueType * const>(&v)[VectorIndices::Y];
#endif
	}

	//}
#pragma endregion

#pragma region _()
	//{ _()

	const v_ui64_2::ValueType x(const v_ui64_2 & v) noexcept
	{
		return component<VectorIndices::X>(v);
	}

	const v_ui64_2::ValueType y(const v_ui64_2 & v) noexcept
	{
		return component<VectorIndices::Y>(v);
	}

	//}
#pragma endregion

#pragma region replace_()
	//{ replace_()

	const v_ui64_2 replaceX(const v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept
	{
		return replaceComponent<VectorIndices::X>(v, s);
	}

	const v_ui64_2 replaceY(const v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept
	{
		return replaceComponent<VectorIndices::Y>(v, s);
	}

	//}
#pragma endregion

#pragma region min()
	//{ min()

	const v_ui64_2 min(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return blendMasked(a.components, b.components, (a > b).components);
	}

	const v_ui64_2 minv(const v_ui64_2 & v) noexcept
	{
		return min(v, yy(v));
	}

	const v_ui64_2::ValueType min(const v_ui64_2 & v) noexcept
	{
		return x(minv(v));
	}

	//}
#pragma endregion

#pragma region max()
	//{ max()

	const v_ui64_2 max(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return blendMasked(b.components, a.components, (a > b).components);
	}

	const v_ui64_2 maxv(const v_ui64_2 & v) noexcept
	{
		return max(v, yy(v));
	}

	const v_ui64_2::ValueType max(const v_ui64_2 & v) noexcept
	{
		return x(maxv(v));
	}

	//}
#pragma endregion

#pragma region Special vector operations
	//{ Special vector operations

	const v_ui64_2 lengthv(const v_ui64_2 & v) noexcept
	{
		return xx(v) + yy(v);
	}

	const v_ui64_2::ValueType length(const v_ui64_2 & v) noexcept
	{
		return x(lengthv(v));
	}

	const v_ui64_2::ValueType distance(const v_ui64_2 & a, const v_ui64_2 & b) noexcept
	{
		return length(a - b);
	}

	const v_ui64_2 clamp(const v_ui64_2 & v, const v_ui64_2 & l, const v_ui64_2 & m) noexcept
	{
		return max(l, min(v, m));
	}

	const bool anyTrue(const v_ui64_2 & v) noexcept
	{
		return !_mm_testz_si128(v.components, MaskAll<v_ui64_2::PackedType>());
	}

	const bool anyFalse(const v_ui64_2 & v) noexcept
	{
		return !_mm_testc_si128(v.components, MaskAll<v_ui64_2::PackedType>());
	}

	const bool allTrue(const v_ui64_2 & v) noexcept
	{
		return !!_mm_testc_si128(v.components, MaskAll<v_ui64_2::PackedType>());
	}

	const bool allFalse(const v_ui64_2 & v) noexcept
	{
		return !!_mm_testz_si128(v.components, MaskAll<v_ui64_2::PackedType>());
	}

	//}
#pragma endregion

}
