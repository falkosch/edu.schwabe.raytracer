#include "vectorization/v_i32_4.h"

#include <limits>
#include <cassert>

namespace vectorization
{

#pragma region v_i32_4::
	//{ v_i32_4::

	v_i32_4::v_i32_4() noexcept
		: components(Zero<v_i32_4::PackedType>())
	{ }

	v_i32_4::v_i32_4(const v_i32_4::PackedType & v) noexcept
		: components(v)
	{ }

	v_i32_4::v_i32_4(const v_i32_4::ValueType s) noexcept
		: components(_mm_set1_epi32(s))
	{ }

	v_i32_4::v_i32_4(const v_i32_4::ValueType x, const v_i32_4::ValueType y) noexcept
		: components(_mm_set_epi32(Zero<v_i32_4::ValueType>(), Zero<v_i32_4::ValueType>(), y, x))
	{ }

	v_i32_4::v_i32_4(const v_i32_4::ValueType x, const v_i32_4::ValueType y, const v_i32_4::ValueType z) noexcept
		: components(_mm_set_epi32(Zero<ValueType>(), z, y, x))
	{ }

	v_i32_4::v_i32_4(const v_i32_4::ValueType x, const v_i32_4::ValueType y, const v_i32_4::ValueType z, const v_i32_4::ValueType w) noexcept
		: components(_mm_set_epi32(w, z, y, x))
	{ }

	v_i32_4 & v_i32_4::operator=(const v_i32_4::PackedType & v) noexcept
	{
		components = v;
		return *this;
	}

	v_i32_4::ValueType & v_i32_4::operator[](const int index) noexcept
	{
		assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
		return reinterpret_cast<v_i32_4::ValueType * const>(this)[index];
	}

	const v_i32_4::ValueType & v_i32_4::operator[](const int index) const noexcept
	{
		assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
		return reinterpret_cast<const v_i32_4::ValueType * const>(this)[index];
	}

	v_i32_4::ValueType & v_i32_4::operator[](const ASizeT index) noexcept
	{
		assert(index < SIZE);
		return reinterpret_cast<v_i32_4::ValueType * const>(this)[index];
	}

	const v_i32_4::ValueType & v_i32_4::operator[](const ASizeT index) const noexcept
	{
		assert(index < SIZE);
		return reinterpret_cast<const v_i32_4::ValueType * const>(this)[index];
	}

	void v_i32_4::setX(const v_i32_4::ValueType v) noexcept
	{
		components = _mm_insert_epi32(components, v, VectorIndices::X);
	}

	void v_i32_4::setY(const v_i32_4::ValueType v) noexcept
	{
		components = _mm_insert_epi32(components, v, VectorIndices::Y);
	}

	void v_i32_4::setZ(const v_i32_4::ValueType v) noexcept
	{
		components = _mm_insert_epi32(components, v, VectorIndices::Z);
	}

	void v_i32_4::setW(const v_i32_4::ValueType v) noexcept
	{
		components = _mm_insert_epi32(components, v, VectorIndices::W);
	}

	//}
#pragma endregion

#pragma region Constant generators
	//{Constant generators

	template <>
	const v_i32_4 MaskAll<v_i32_4>() noexcept
	{
		return MaskAll<v_i32_4::PackedType>();
	}

	template <>
	const v_i32_4 MaskX<v_i32_4>() noexcept
	{
		return _mm_castps_si128(MaskX<PackedFloat4_128>());
	}

	template <>
	const v_i32_4 MaskXY<v_i32_4>() noexcept
	{
		return _mm_castps_si128(MaskXY<PackedFloat4_128>());
	}

	template <>
	const v_i32_4 MaskXYZ<v_i32_4>() noexcept
	{
		return _mm_castps_si128(MaskXYZ<PackedFloat4_128>());
	}

	template <>
	const v_i32_4 MaskXYZW<v_i32_4>() noexcept
	{
		return _mm_castps_si128(MaskXYZW<PackedFloat4_128>());
	}

	template <>
	const v_i32_4 MaskY<v_i32_4>() noexcept
	{
		return _mm_castps_si128(MaskY<PackedFloat4_128>());
	}

	template <>
	const v_i32_4 MaskYZ<v_i32_4>() noexcept
	{
		return _mm_castps_si128(MaskYZ<PackedFloat4_128>());
	}

	template <>
	const v_i32_4 MaskYZW<v_i32_4>() noexcept
	{
		return _mm_castps_si128(MaskYZW<PackedFloat4_128>());
	}

	template <>
	const v_i32_4 MaskZ<v_i32_4>() noexcept
	{
		return _mm_castps_si128(MaskZ<PackedFloat4_128>());
	}

	template <>
	const v_i32_4 MaskZW<v_i32_4>() noexcept
	{
		return _mm_castps_si128(MaskZW<PackedFloat4_128>());
	}

	template <>
	const v_i32_4 MaskW<v_i32_4>() noexcept
	{
		return _mm_castps_si128(MaskW<PackedFloat4_128>());
	}

	template <>
	const v_i32_4 OneW<v_i32_4>() noexcept
	{
		return _mm_srli_si128(One<v_i32_4>().components, VectorSizes::Z * sizeof(v_i32_4::ValueType));
	}

	template <>
	const v_i32_4 Zero<v_i32_4>() noexcept
	{
		return Zero<v_i32_4::PackedType>();
	}

	template <>
	const v_i32_4 One<v_i32_4>() noexcept
	{
		return _mm_srli_epi32(MaskAll<v_i32_4::PackedType>(), sizeof(v_i32_4::ValueType) * std::numeric_limits<unsigned char>::digits - 1);
	}

	template <>
	const v_i32_4 Two<v_i32_4>() noexcept
	{
		return _mm_slli_epi32(_mm_srli_epi32(MaskAll<v_i32_4::PackedType>(), sizeof(v_i32_4::ValueType) * std::numeric_limits<unsigned char>::digits - 1), 1);
	}

	template <>
	const v_i32_4 NegOne<v_i32_4>() noexcept
	{
		return MaskAll<v_i32_4::PackedType>();
	}

	template <>
	const v_i32_4 NegTwo<v_i32_4>() noexcept
	{
		return _mm_slli_epi32(MaskAll<v_i32_4::PackedType>(), 1);
	}

	//}
#pragma endregion

#pragma region Commonly used swizzle accessors
	// Commonly used swizzle accessors

	const v_i32_4 xxxx(const v_i32_4 & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(v);
	}

	const v_i32_4 yyyy(const v_i32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(v);
	}

	const v_i32_4 zzzz(const v_i32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
	}

	const v_i32_4 wwww(const v_i32_4 & v) noexcept
	{
		return swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(v);
	}

	const v_i32_4 xxyy(const v_i32_4 & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(v);
	}

	const v_i32_4 xyxy(const v_i32_4 & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v);
	}

	const v_i32_4 yxwz(const v_i32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(v);
	}

	const v_i32_4 zzww(const v_i32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v);
	}

	const v_i32_4 zwzw(const v_i32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v);
	}

	const v_i32_4 xy_xy(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(a, b);
	}

	const v_i32_4 zw_zw(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(a, b);
	}

	//}
#pragma endregion

#pragma region Operators
	//{Operators

	const v_i32_4 operator-(const v_i32_4 & v) noexcept
	{
		return _mm_sub_epi32(Zero<v_i32_4::PackedType>(), v.components);
	}

	const v_i32_4 operator!(const v_i32_4 & v) noexcept
	{
		return _mm_cmpeq_epi32(Zero<v_i32_4::PackedType>(), v.components);
	}

	const v_i32_4 operator~(const v_i32_4 & v) noexcept
	{
		return _mm_andnot_si128(v.components, MaskAll<v_i32_4::PackedType>());
	}

	const v_i32_4 operator+(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_add_epi32(a.components, b.components);
	}

	const v_i32_4 operator-(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_sub_epi32(a.components, b.components);
	}

	const v_i32_4 operator*(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return v_i32_4(x(a) * x(b), y(a) * y(b), z(a) * z(b), w(a) * w(b));
#else
		return _mm_mullo_epi32(a.components, b.components);
#endif
	}

	const v_i32_4 operator/(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return v_i32_4(x(a) / x(b), y(a) / y(b), z(a) / z(b), w(a) / w(b));
	}

	const v_i32_4 operator%(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return v_i32_4(x(a) % x(b), y(a) % y(b), z(a) % z(b), w(a) % w(b));
	}

	const v_i32_4 operator&(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_and_si128(a.components, b.components);
	}

	const v_i32_4 operator|(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_or_si128(a.components, b.components);
	}

	const v_i32_4 operator^(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_xor_si128(a.components, b.components);
	}

	const v_i32_4 operator<<(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_sll_epi32(a.components, b.components);
	}

	const v_i32_4 operator>>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_srl_epi32(a.components, b.components);
	}

	const v_i32_4 operator<(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_cmplt_epi32(a.components, b.components);
	}

	const v_i32_4 operator>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_cmpgt_epi32(a.components, b.components);
	}

	const v_i32_4 operator<=(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_cmpeq_epi32(a.components, _mm_min_epi32(a.components, b.components));
	}

	const v_i32_4 operator>=(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_cmpeq_epi32(a.components, _mm_max_epi32(a.components, b.components));
	}

	const v_i32_4 operator==(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_cmpeq_epi32(a.components, b.components);
	}

	const v_i32_4 operator!=(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return !(a == b);
	}

	v_i32_4 & operator+=(v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return a = a + b;
	}

	v_i32_4 & operator-=(v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return a = a - b;
	}

	v_i32_4 & operator*=(v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return a = a * b;
	}

	v_i32_4 & operator/=(v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return a = a / b;
	}

	v_i32_4 & operator%=(v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return a = a % b;
	}

	v_i32_4 & operator&=(v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return a = a & b;
	}

	v_i32_4 & operator|=(v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return a = a | b;
	}

	v_i32_4 & operator^=(v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return a = a ^ b;
	}

	v_i32_4 & operator<<=(v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return a = a << b;
	}

	v_i32_4 & operator>>=(v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return a = a >> b;
	}

	//}
#pragma endregion

#pragma region swizzle()
	//{ swizzle()

	template <>
	const v_i32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_i32_4 & v) noexcept
	{
		return v;
	}

	template <>
	const v_i32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const v_i32_4 & v) noexcept
	{
		return _mm_unpacklo_epi32(v.components, v.components);
	}

	template <>
	const v_i32_4 swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const v_i32_4 & v) noexcept
	{
		return _mm_unpackhi_epi32(v.components, v.components);
	}

	template <>
	const v_i32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_i32_4 & v) noexcept
	{
		return _mm_unpacklo_epi64(v.components, v.components);
	}

	template <>
	const v_i32_4 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_i32_4 & v) noexcept
	{
		return _mm_unpackhi_epi64(v.components, v.components);
	}

	//}
#pragma endregion

#pragma region blend()
	//{ blend()

	template <>
	const v_i32_4 blend<false, false, false, false>(const v_i32_4 & a, const v_i32_4 &) noexcept
	{
		return a;
	}

	template <>
	const v_i32_4 blend<true, true, true, true>(const v_i32_4 &, const v_i32_4 & b) noexcept
	{
		return b;
	}

	//}
#pragma endregion

#pragma region swizzledBlend()
	//{ swizzledBlend()

	template <>
	const v_i32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false, true, false, true>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_unpacklo_epi32(a.components, b.components);
	}

	template <>
	const v_i32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true, false, true, false>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		// argpos of a and b swapped here
		return _mm_unpacklo_epi32(b.components, a.components);
	}

	template <>
	const v_i32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false, true, false, true>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_unpackhi_epi32(a.components, b.components);
	}

	template <>
	const v_i32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true, false, true, false>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_unpackhi_epi32(b.components, a.components);
	}

	template <>
	const v_i32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false, false, true, true>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_unpacklo_epi64(a.components, b.components);
	}

	template <>
	const v_i32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true, true, false, false>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_unpacklo_epi64(b.components, a.components);
	}

	template <>
	const v_i32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false, false, true, true>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_unpackhi_epi64(a.components, b.components);
	}

	template <>
	const v_i32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true, true, false, false>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_unpackhi_epi64(b.components, a.components);
	}

	template <>
	const v_i32_4 swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return blend<false, false, true, true>(a, b);
	}

	template <>
	const v_i32_4 swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_unpacklo_epi64(a.components, b.components);
	}

	template <>
	const v_i32_4 swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_unpackhi_epi64(a.components, b.components);
	}

	//}
#pragma endregion

#pragma region blendMasked()
	//{ blendMasked()

	const v_i32_4 blendMasked(const v_i32_4 & a, const v_i32_4 & b, const v_i32_4 & mask) noexcept
	{
		return _mm_blendv_epi8(a.components, b.components, mask.components);
	}

	//}
#pragma endregion

#pragma region setComponent()
	//{ setComponent()

	template <>
	void setComponent<VectorIndices::X>(v_i32_4 & v, const v_i32_4::ValueType s) noexcept
	{
		v.components = _mm_insert_epi32(v.components, s, VectorIndices::X);
	}

	template <>
	void setComponent<VectorIndices::Y>(v_i32_4 & v, const v_i32_4::ValueType s) noexcept
	{
		v.components = _mm_insert_epi32(v.components, s, VectorIndices::Y);
	}

	template <>
	void setComponent<VectorIndices::Z>(v_i32_4 & v, const v_i32_4::ValueType s) noexcept
	{
		v.components = _mm_insert_epi32(v.components, s, VectorIndices::Z);
	}

	template <>
	void setComponent<VectorIndices::W>(v_i32_4 & v, const v_i32_4::ValueType s) noexcept
	{
		v.components = _mm_insert_epi32(v.components, s, VectorIndices::W);
	}

	//}
#pragma endregion

#pragma region replaceComponent()
	//{ replaceComponent()

	template <>
	const v_i32_4 replaceComponent<VectorIndices::X>(const v_i32_4 & v, const v_i32_4::ValueType s) noexcept
	{
		return _mm_insert_epi32(v.components, s, VectorIndices::X);
	}

	template <>
	const v_i32_4 replaceComponent<VectorIndices::Y>(const v_i32_4 & v, const v_i32_4::ValueType s) noexcept
	{
		return _mm_insert_epi32(v.components, s, VectorIndices::Y);
	}

	template <>
	const v_i32_4 replaceComponent<VectorIndices::Z>(const v_i32_4 & v, const v_i32_4::ValueType s) noexcept
	{
		return _mm_insert_epi32(v.components, s, VectorIndices::Z);
	}

	template <>
	const v_i32_4 replaceComponent<VectorIndices::W>(const v_i32_4 & v, const v_i32_4::ValueType s) noexcept
	{
		return _mm_insert_epi32(v.components, s, VectorIndices::W);
	}

	//}
#pragma endregion

#pragma region component()
	//{ component()

	template <>
	const v_i32_4::ValueType component<VectorIndices::X>(const v_i32_4 & v) noexcept
	{
		return _mm_cvtsi128_si32(v.components);
	}

	template <>
	const v_i32_4::ValueType component<VectorIndices::Y>(const v_i32_4 & v) noexcept
	{
		return _mm_cvtsi128_si32(_mm_srli_si128(v.components, VectorSizes::X * sizeof(v_i32_4::ValueType)));
	}

	template <>
	const v_i32_4::ValueType component<VectorIndices::Z>(const v_i32_4 & v) noexcept
	{
		return _mm_cvtsi128_si32(_mm_srli_si128(v.components, VectorSizes::Y * sizeof(v_i32_4::ValueType)));
	}

	template <>
	const v_i32_4::ValueType component<VectorIndices::W>(const v_i32_4 & v) noexcept
	{
		return _mm_cvtsi128_si32(_mm_srli_si128(v.components, VectorSizes::Z * sizeof(v_i32_4::ValueType)));
	}

	//}
#pragma endregion

#pragma region _()
	//{ _()

	const v_i32_4::ValueType x(const v_i32_4 & v) noexcept
	{
		return component<VectorIndices::X>(v);
	}

	const v_i32_4::ValueType y(const v_i32_4 & v) noexcept
	{
		return component<VectorIndices::Y>(v);
	}

	const v_i32_4::ValueType z(const v_i32_4 & v) noexcept
	{
		return component<VectorIndices::Z>(v);
	}

	const v_i32_4::ValueType w(const v_i32_4 & v) noexcept
	{
		return component<VectorIndices::W>(v);
	}

	//}
#pragma endregion

#pragma region replace_()
	//{ replace_()

	const v_i32_4 replaceX(const v_i32_4 & v, const v_i32_4::ValueType s) noexcept
	{
		return replaceComponent<VectorIndices::X>(v, s);
	}

	const v_i32_4 replaceY(const v_i32_4 & v, const v_i32_4::ValueType s) noexcept
	{
		return replaceComponent<VectorIndices::Y>(v, s);
	}

	const v_i32_4 replaceZ(const v_i32_4 & v, const v_i32_4::ValueType s) noexcept
	{
		return replaceComponent<VectorIndices::Z>(v, s);
	}

	const v_i32_4 replaceW(const v_i32_4 & v, const v_i32_4::ValueType s) noexcept
	{
		return replaceComponent<VectorIndices::W>(v, s);
	}

	//}
#pragma endregion

#pragma region length()
	//{ length()

	const v_i32_4 lengthv(const v_i32_4 & v) noexcept
	{
		const v_i32_4 t = abs(v);
		return xxxx(t) + yyyy(t) + zzzz(t) + wwww(t);
	}

	const v_i32_4 length3v(const v_i32_4 & v) noexcept
	{
		const v_i32_4 t = abs(v);
		return xxxx(t) + yyyy(t) + zzzz(t);
	}

	const v_i32_4::ValueType length(const v_i32_4 & v) noexcept
	{
		return x(lengthv(v));
	}

	const v_i32_4::ValueType length3(const v_i32_4 & v) noexcept
	{
		return x(length3v(v));
	}

	//}
#pragma endregion

#pragma region distance()
	//{ distance()

	const v_i32_4::ValueType distance(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return length(b - a);
	}

	const v_i32_4::ValueType distance3(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return length3(b - a);
	}

	//}
#pragma endregion

#pragma region min()
	//{ min()

	const v_i32_4 min(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return v_i32_4(min(x(a), x(b)), min(y(a), y(b)), min(z(a), z(b)), min(w(a), w(b)));
#else
		return _mm_min_epi32(a.components, b.components);
#endif
	}

	const v_i32_4::ValueType min(const v_i32_4 & v) noexcept
	{
		// gets the min of this (x, y, z, w) and shuffled (z, w, z, w) and then
		// gets the min of XZ_YW (x', y', z', w') and its shuffled (y', x', w', z')
		const v_i32_4 XZ_YW = min(v, zwzw(v));
		return x(min(XZ_YW, yxwz(XZ_YW)));
	}

	const v_i32_4::ValueType min3(const v_i32_4 & v) noexcept
	{
		// gets the min of (x, y, z, w), (z, w, z, w) and then of (y, x, w, z)
		return x(min(yxwz(v), min(v, zwzw(v))));
	}

	//}
#pragma endregion

#pragma region max()
	//{ max()

	const v_i32_4 max(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return v_i32_4(max(x(a), x(b)), max(y(a), y(b)), max(z(a), z(b)), max(w(a), w(b)));
#else
		return _mm_max_epi32(a.components, b.components);
#endif
	}

	const v_i32_4::ValueType max(const v_i32_4 & v) noexcept
	{
		const v_i32_4 XZ_YW = max(v, zwzw(v));
		return x(max(XZ_YW, yxwz(XZ_YW)));
	}

	const v_i32_4::ValueType max3(const v_i32_4 & v) noexcept
	{
		return x(max(yxwz(v), max(v, zwzw(v))));
	}

	//}
#pragma endregion

#pragma region Special vector operations
	//{

	const v_i32_4 zeroW(const v_i32_4 & v) noexcept
	{
		// Shift by four bytes left and then back right to erase w-component
		return _mm_srli_si128(_mm_slli_si128(v.components, sizeof(v_i32_4::ValueType)), sizeof(v_i32_4::ValueType));
	}

	const v_i32_4 andnot(const v_i32_4 & a, const v_i32_4 & b) noexcept
	{
		return _mm_andnot_si128(a, b);
	}

	const v_i32_4::BoolType isNegative(const v_i32_4 & v) noexcept
	{
		const v_i32_4::PackedType t = _mm_packs_epi32(v.components, v.components);
		return _mm_movemask_epi8(_mm_packs_epi16(t, t));
	}

	const v_i32_4 abs(const v_i32_4 & v) noexcept
	{
		return _mm_abs_epi32(v.components);
	}

	const v_i32_4 clamp(const v_i32_4 & v, const v_i32_4 & l, const v_i32_4 & m) noexcept
	{
		return max(l, min(v, m));
	}

	inline const bool testAllZero(const v_i32_4 & v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		const v_i32_4 t = v | zwzw(v);
		return !x(yyyy(t) | t);
#else
		return !!_mm_testz_si128(v.components, MaskAll<v_i32_4::PackedType>());
#endif
	}

	inline const bool testAllZero3(const v_i32_4 & v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		const v_i32_4 t = v | zzww(v);
		return !x(yyyy(t) | t);
#else
		return !!_mm_testz_si128(v.components, MaskXYZ<v_i32_4>().components);
#endif
	}

	inline const bool testAllOnes(const v_i32_4 & v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		const v_i32_4 t = v & zwzw(v);
		return !!x(yyyy(t) & t);
#else
		return !!_mm_testc_si128(v.components, MaskAll<v_i32_4::PackedType>());
#endif
	}

	inline const bool testAllOnes3(const v_i32_4 & v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		const v_i32_4 t = v & zzww(v);
		return !!x(yyyy(t) & t);
#else
		return !!_mm_testc_si128(v.components, MaskXYZ<v_i32_4>().components);
#endif
	}

	const bool anyTrue(const v_i32_4 & v) noexcept
	{
		return !testAllZero(v);
	}

	const bool anyTrue3(const v_i32_4 & v) noexcept
	{
		return !testAllZero3(v);
	}

	const bool anyFalse(const v_i32_4 & v) noexcept
	{
		return !testAllOnes(v);
	}

	const bool anyFalse3(const v_i32_4 & v) noexcept
	{
		return !testAllOnes3(v);
	}

	const bool allTrue(const v_i32_4 & v) noexcept
	{
		return testAllOnes(v);
	}

	const bool allTrue3(const v_i32_4 & v) noexcept
	{
		return testAllOnes3(v);
	}

	const bool allFalse(const v_i32_4 & v) noexcept
	{
		return testAllZero(v);
	}

	const bool allFalse3(const v_i32_4 & v) noexcept
	{
		return testAllZero3(v);
	}

	//}
#pragma endregion

}
