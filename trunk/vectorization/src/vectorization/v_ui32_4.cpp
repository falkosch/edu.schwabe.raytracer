#include "vectorization/v_ui32_4.h"

#include <limits.h>

namespace vectorization
{

#pragma region v_ui32_4::
	//{ v_ui32_4::

	v_ui32_4::v_ui32_4()
		:
		components(Zero<v_ui32_4::PackedType>())
	{ }

	v_ui32_4::v_ui32_4(const v_ui32_4::PackedType & v)
		:
		components(v)
	{ }

	v_ui32_4::v_ui32_4(const v_ui32_4::ValueType s)
		:
		components(_mm_set1_epi32(static_cast<int>(s)))
	{ }

	v_ui32_4::v_ui32_4(const v_ui32_4::ValueType x, const v_ui32_4::ValueType y)
		:
		components(_mm_set_epi32(Zero<int>(), Zero<int>(), static_cast<int>(y), static_cast<int>(x)))
	{ }

	v_ui32_4::v_ui32_4(const v_ui32_4::ValueType x, const v_ui32_4::ValueType y, const v_ui32_4::ValueType z)
		:
		components(_mm_set_epi32(Zero<int>(), static_cast<int>(z), static_cast<int>(y), static_cast<int>(x)))
	{ }

	v_ui32_4::v_ui32_4(const v_ui32_4::ValueType x, const v_ui32_4::ValueType y, const v_ui32_4::ValueType z, const v_ui32_4::ValueType w)
		:
		components(_mm_set_epi32(static_cast<int>(w), static_cast<int>(z), static_cast<int>(y), static_cast<int>(x)))
	{ }

	v_ui32_4 & v_ui32_4::operator=(const v_ui32_4::PackedType & v)
	{
		components = v;
		return *this;
	}

	v_ui32_4::ValueType & v_ui32_4::operator[](const int index)
	{
		assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
		return reinterpret_cast<v_ui32_4::ValueType * const>(this)[index];
	}

	const v_ui32_4::ValueType & v_ui32_4::operator[](const int index) const
	{
		assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
		return reinterpret_cast<const v_ui32_4::ValueType * const>(this)[index];
	}

	v_ui32_4::ValueType & v_ui32_4::operator[](const ASizeT index)
	{
		assert(index < SIZE);
		return reinterpret_cast<v_ui32_4::ValueType * const>(this)[index];
	}

	const v_ui32_4::ValueType & v_ui32_4::operator[](const ASizeT index) const
	{
		assert(index < SIZE);
		return reinterpret_cast<const v_ui32_4::ValueType * const>(this)[index];
	}

	void v_ui32_4::setX(const v_ui32_4::ValueType v)
	{
		components = _mm_insert_epi32(components, static_cast<int>(v), VectorIndices::X);
	}

	void v_ui32_4::setY(const v_ui32_4::ValueType v)
	{
		components = _mm_insert_epi32(components, static_cast<int>(v), VectorIndices::Y);
	}

	void v_ui32_4::setZ(const v_ui32_4::ValueType v)
	{
		components = _mm_insert_epi32(components, static_cast<int>(v), VectorIndices::Z);
	}

	void v_ui32_4::setW(const v_ui32_4::ValueType v)
	{
		components = _mm_insert_epi32(components, static_cast<int>(v), VectorIndices::W);
	}

	//}
#pragma endregion

#pragma region Constant generators
	//{ Constant generators

	template <>
	const v_ui32_4 MaskAll<v_ui32_4>()
	{
		return MaskAll<v_ui32_4::PackedType>();
	}

	template <>
	const v_ui32_4 MaskX<v_ui32_4>()
	{
		return _mm_castps_si128(MaskX<PackedFloat4_128>());
	}

	template <>
	const v_ui32_4 MaskXY<v_ui32_4>()
	{
		return _mm_castps_si128(MaskXY<PackedFloat4_128>());
	}

	template <>
	const v_ui32_4 MaskXYZ<v_ui32_4>()
	{
		return _mm_castps_si128(MaskXYZ<PackedFloat4_128>());
	}

	template <>
	const v_ui32_4 MaskXYZW<v_ui32_4>()
	{
		return _mm_castps_si128(MaskXYZW<PackedFloat4_128>());
	}

	template <>
	const v_ui32_4 MaskY<v_ui32_4>()
	{
		return _mm_castps_si128(MaskY<PackedFloat4_128>());
	}

	template <>
	const v_ui32_4 MaskYZ<v_ui32_4>()
	{
		return _mm_castps_si128(MaskYZ<PackedFloat4_128>());
	}

	template <>
	const v_ui32_4 MaskYZW<v_ui32_4>()
	{
		return _mm_castps_si128(MaskYZW<PackedFloat4_128>());
	}

	template <>
	const v_ui32_4 MaskZ<v_ui32_4>()
	{
		return _mm_castps_si128(MaskZ<PackedFloat4_128>());
	}

	template <>
	const v_ui32_4 MaskZW<v_ui32_4>()
	{
		return _mm_castps_si128(MaskZW<PackedFloat4_128>());
	}

	template <>
	const v_ui32_4 MaskW<v_ui32_4>()
	{
		return _mm_castps_si128(MaskW<PackedFloat4_128>());
	}

	template <>
	const v_ui32_4 OneW<v_ui32_4>()
	{
        return _mm_srli_si128(One<v_ui32_4>().components, VectorSizes::Z * sizeof(v_ui32_4::ValueType));
	}

	template <>
	const v_ui32_4 Zero<v_ui32_4>()
	{
		return Zero<v_ui32_4::PackedType>();
	}

	template <>
	const v_ui32_4 One<v_ui32_4>()
	{
		return _mm_srli_epi32(MaskAll<v_ui32_4::PackedType>(), sizeof(v_ui32_4::ValueType) * CHAR_BIT - 1);
	}

	template <>
	const v_ui32_4 Two<v_ui32_4>()
	{
		return _mm_slli_epi32(_mm_srli_epi32(MaskAll<v_ui32_4::PackedType>(), sizeof(v_ui32_4::ValueType) * CHAR_BIT - 1), 1);
	}

	//}
#pragma endregion

#pragma region Commonly used swizzle accessors
	// Commonly used swizzle accessors

	const v_ui32_4 xxxx(const v_ui32_4 & v)
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(v);
	}

	const v_ui32_4 yyyy(const v_ui32_4 & v)
	{
		return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(v);
	}

	const v_ui32_4 zzzz(const v_ui32_4 & v)
	{
		return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
	}

	const v_ui32_4 wwww(const v_ui32_4 & v)
	{
		return swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(v);
	}

	const v_ui32_4 xxyy(const v_ui32_4 & v)
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(v);
	}

	const v_ui32_4 xyxy(const v_ui32_4 & v)
	{
		return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v);
	}

	const v_ui32_4 yxwz(const v_ui32_4 & v)
	{
		return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(v);
	}

	const v_ui32_4 zzww(const v_ui32_4 & v)
	{
		return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v);
	}

	const v_ui32_4 zwzw(const v_ui32_4 & v)
	{
		return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v);
	}

	const v_ui32_4 xy_xy(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(a, b);
	}

	const v_ui32_4 zw_zw(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(a, b);
	}

	//}
#pragma endregion

#pragma region Operators
	//{Operators

	const v_ui32_4 operator!(const v_ui32_4 & v)
	{
		return _mm_cmpeq_epi32(Zero<v_ui32_4::PackedType>(), v.components);
	}

	const v_ui32_4 operator~(const v_ui32_4 & v)
	{
		return _mm_andnot_si128(v.components, MaskAll<v_ui32_4::PackedType>());
	}

	const v_ui32_4 operator+(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_add_epi32(a.components, b.components);
	}

	const v_ui32_4 operator-(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_sub_epi32(a.components, b.components);
	}

	const v_ui32_4 operator*(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_mullo_epi32(a.components, b.components);
		//return swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z, false, true, false, true>(
		//    _mm_mul_epu32(a.components, b.components),
		//    _mm_mul_epu32(yxwz(a).components, yxwz(b).components)
		//);
	}

	const v_ui32_4 operator/(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return v_ui32_4(x(a) / x(b), y(a) / y(b), z(a) / z(b), w(a) / w(b));
	}

	const v_ui32_4 operator%(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return v_ui32_4(x(a) % x(b), y(a) % y(b), z(a) % z(b), w(a) % w(b));
	}

	const v_ui32_4 operator&(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_and_si128(a.components, b.components);
	}

	const v_ui32_4 operator|(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_or_si128(a.components, b.components);
	}

	const v_ui32_4 operator^(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_xor_si128(a.components, b.components);
	}

	const v_ui32_4 operator<<(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_sll_epi32(a.components, b.components);
	}

	const v_ui32_4 operator>>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_srl_epi32(a.components, b.components);
	}

	const v_ui32_4 operator<(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_cmplt_epi32(a.components, b.components);
	}

	const v_ui32_4 operator>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_cmpgt_epi32(a.components, b.components);
	}

	const v_ui32_4 operator<=(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_cmpeq_epi32(a.components, _mm_min_epu32(a.components, b.components));
	}

	const v_ui32_4 operator>=(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_cmpeq_epi32(a.components, _mm_max_epu32(a.components, b.components));
	}

	const v_ui32_4 operator==(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_cmpeq_epi32(a.components, b.components);
	}

	const v_ui32_4 operator!=(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return !(a == b);
	}

	v_ui32_4 & operator+=(v_ui32_4 & a, const v_ui32_4 & b)
	{
		return a = a + b;
	}

	v_ui32_4 & operator-=(v_ui32_4 & a, const v_ui32_4 & b)
	{
		return a = a - b;
	}

	v_ui32_4 & operator*=(v_ui32_4 & a, const v_ui32_4 & b)
	{
		return a = a * b;
	}

	v_ui32_4 & operator/=(v_ui32_4 & a, const v_ui32_4 & b)
	{
		return a = a / b;
	}

	v_ui32_4 & operator%=(v_ui32_4 & a, const v_ui32_4 & b)
	{
		return a = a % b;
	}

	v_ui32_4 & operator&=(v_ui32_4 & a, const v_ui32_4 & b)
	{
		return a = a & b;
	}

	v_ui32_4 & operator|=(v_ui32_4 & a, const v_ui32_4 & b)
	{
		return a = a | b;
	}

	v_ui32_4 & operator^=(v_ui32_4 & a, const v_ui32_4 & b)
	{
		return a = a ^ b;
	}

	v_ui32_4 & operator<<=(v_ui32_4 & a, const v_ui32_4 & b)
	{
		return a = a << b;
	}

	v_ui32_4 & operator>>=(v_ui32_4 & a, const v_ui32_4 & b)
	{
		return a = a >> b;
	}

	//}
#pragma endregion

#pragma region swizzle()
	//{ swizzle()

	template <>
	const v_ui32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_ui32_4 & v)
	{
		return v;
	}

	template <>
	const v_ui32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const v_ui32_4 & v)
	{
		return _mm_unpacklo_epi32(v.components, v.components);
	}

	template <>
	const v_ui32_4 swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const v_ui32_4 & v)
	{
		return _mm_unpackhi_epi32(v.components, v.components);
	}

	template <>
	const v_ui32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_ui32_4 & v)
	{
		return _mm_unpacklo_epi64(v.components, v.components);
	}

	template <>
	const v_ui32_4 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_ui32_4 & v)
	{
		return _mm_unpackhi_epi64(v.components, v.components);
	}

	//}
#pragma endregion

#pragma region blend()
	//{ blend()

	template <>
	const v_ui32_4 blend<false, false, false, false>(const v_ui32_4 & a, const v_ui32_4 &)
	{
		return a;
	}

	template <>
	const v_ui32_4 blend<true, true, true, true>(const v_ui32_4 &, const v_ui32_4 & b)
	{
		return b;
	}

	//}
#pragma endregion

#pragma region swizzledBlend()
	//{ swizzledBlend()

	template <>
	const v_ui32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false, true, false, true>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_unpacklo_epi32(a.components, b.components);
	}

	template <>
	const v_ui32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true, false, true, false>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		// argpos of a and b swapped here
		return _mm_unpacklo_epi32(b.components, a.components);
	}

	template <>
	const v_ui32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false, true, false, true>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_unpackhi_epi32(a.components, b.components);
	}

	template <>
	const v_ui32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true, false, true, false>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_unpackhi_epi32(b.components, a.components);
	}

	template <>
	const v_ui32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false, false, true, true>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_unpacklo_epi64(a.components, b.components);
	}

	template <>
	const v_ui32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true, true, false, false>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_unpacklo_epi64(b.components, a.components);
	}

	template <>
	const v_ui32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false, false, true, true>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_unpackhi_epi64(a.components, b.components);
	}

	template <>
	const v_ui32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true, true, false, false>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_unpackhi_epi64(b.components, a.components);
	}

	template <>
	const v_ui32_4 swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return blend<false, false, true, true>(a, b);
	}

	template <>
	const v_ui32_4 swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_unpacklo_epi64(a.components, b.components);
	}

	template <>
	const v_ui32_4 swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_unpackhi_epi64(a.components, b.components);
	}

	//}
#pragma endregion

#pragma region blendMasked()
	//{ blendMasked()

	const v_ui32_4 blendMasked(const v_ui32_4 & a, const v_ui32_4 & b, const v_ui32_4 & mask)
	{
		return _mm_blendv_epi8(a.components, b.components, mask.components);
	}

	//}
#pragma endregion

#pragma region setComponent()
	//{ setComponent()

	template <>
	void setComponent<VectorIndices::X>(v_ui32_4 & v, const v_ui32_4::ValueType s)
	{
		v.components = _mm_insert_epi32(v.components, static_cast<int>(s), VectorIndices::X);
	}

	template <>
	void setComponent<VectorIndices::Y>(v_ui32_4 & v, const v_ui32_4::ValueType s)
	{
		v.components = _mm_insert_epi32(v.components, static_cast<int>(s), VectorIndices::Y);
	}

	template <>
	void setComponent<VectorIndices::Z>(v_ui32_4 & v, const v_ui32_4::ValueType s)
	{
		v.components = _mm_insert_epi32(v.components, static_cast<int>(s), VectorIndices::Z);
	}

	template <>
	void setComponent<VectorIndices::W>(v_ui32_4 & v, const v_ui32_4::ValueType s)
	{
		v.components = _mm_insert_epi32(v.components, static_cast<int>(s), VectorIndices::W);
	}

	//}
#pragma endregion

#pragma region replaceComponent()
	//{ replaceComponent()

	template <>
	const v_ui32_4 replaceComponent<VectorIndices::X>(const v_ui32_4 & v, const v_ui32_4::ValueType s)
	{
		return _mm_insert_epi32(v.components, static_cast<int>(s), VectorIndices::X);
	}

	template <>
	const v_ui32_4 replaceComponent<VectorIndices::Y>(const v_ui32_4 & v, const v_ui32_4::ValueType s)
	{
		return _mm_insert_epi32(v.components, static_cast<int>(s), VectorIndices::Y);
	}

	template <>
	const v_ui32_4 replaceComponent<VectorIndices::Z>(const v_ui32_4 & v, const v_ui32_4::ValueType s)
	{
		return _mm_insert_epi32(v.components, static_cast<int>(s), VectorIndices::Z);
	}

	template <>
	const v_ui32_4 replaceComponent<VectorIndices::W>(const v_ui32_4 & v, const v_ui32_4::ValueType s)
	{
		return _mm_insert_epi32(v.components, static_cast<int>(s), VectorIndices::W);
	}

	//}
#pragma endregion

#pragma region component()
	//{ component()

	template <>
	const v_ui32_4::ValueType component<VectorIndices::X>(const v_ui32_4 & v)
	{
		return static_cast<v_ui32_4::ValueType>(_mm_cvtsi128_si32(v.components));
	}

	template <>
	const v_ui32_4::ValueType component<VectorIndices::Y>(const v_ui32_4 & v)
	{
		return static_cast<v_ui32_4::ValueType>(_mm_cvtsi128_si32(
			_mm_srli_si128(v.components, VectorSizes::X * sizeof(v_ui32_4::ValueType))));
	}

	template <>
	const v_ui32_4::ValueType component<VectorIndices::Z>(const v_ui32_4 & v)
	{
		return static_cast<v_ui32_4::ValueType>(_mm_cvtsi128_si32(_mm_unpackhi_epi32(v.components, v.components)));
	}

	template <>
	const v_ui32_4::ValueType component<VectorIndices::W>(const v_ui32_4 & v)
	{
		return static_cast<v_ui32_4::ValueType>(_mm_cvtsi128_si32(
			_mm_srli_si128(v.components, VectorSizes::Z * sizeof(v_ui32_4::ValueType))
		));
	}

	//}
#pragma endregion

#pragma region _()
	//{ _()

	const v_ui32_4::ValueType x(const v_ui32_4 & v)
	{
		return component<VectorIndices::X>(v);
	}

	const v_ui32_4::ValueType y(const v_ui32_4 & v)
	{
		return component<VectorIndices::Y>(v);
	}

	const v_ui32_4::ValueType z(const v_ui32_4 & v)
	{
		return component<VectorIndices::Z>(v);
	}

	const v_ui32_4::ValueType w(const v_ui32_4 & v)
	{
		return component<VectorIndices::W>(v);
	}

	//}
#pragma endregion

#pragma region replace_()
	//{ replace_()

	const v_ui32_4 replaceX(const v_ui32_4 & v, const v_ui32_4::ValueType s)
	{
		return replaceComponent<VectorIndices::X>(v, s);
	}

	const v_ui32_4 replaceY(const v_ui32_4 & v, const v_ui32_4::ValueType s)
	{
		return replaceComponent<VectorIndices::Y>(v, s);
	}

	const v_ui32_4 replaceZ(const v_ui32_4 & v, const v_ui32_4::ValueType s)
	{
		return replaceComponent<VectorIndices::Z>(v, s);
	}

	const v_ui32_4 replaceW(const v_ui32_4 & v, const v_ui32_4::ValueType s)
	{
		return replaceComponent<VectorIndices::W>(v, s);
	}

	//}
#pragma endregion

#pragma region Special vector operations
	//{

	const v_ui32_4 zeroW(const v_ui32_4 & v)
	{
		// Shift by four bytes left and then back right to erase w-component
		return _mm_srli_si128(
			_mm_slli_si128(v.components, sizeof(v_ui32_4::ValueType)),
			sizeof(v_ui32_4::ValueType)
		);
	}

	const v_ui32_4 lengthv(const v_ui32_4 & v)
	{
		return xxxx(v) + yyyy(v) + zzzz(v) + wwww(v);
	}

	const v_ui32_4 length3v(const v_ui32_4 & v)
	{
		return xxxx(v) + yyyy(v) + zzzz(v);
	}

	const v_ui32_4::ValueType length(const v_ui32_4 & v)
	{
		return x(lengthv(v));
	}

	const v_ui32_4::ValueType length3(const v_ui32_4 & v)
	{
		return x(length3v(v));
	}

	const v_ui32_4::ValueType distance(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return length(a - b);
	}

	const v_ui32_4::ValueType distance3(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return length3(a - b);
	}

	const v_ui32_4 min(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_min_epu32(a.components, b.components);
	}

	const v_ui32_4::ValueType min(const v_ui32_4 & v)
	{
		// gets the min of this (x, y, z, w) and shuffled (z, w, z, w) and then
		// gets the min of XZ_YW (x', y', z', w') and its shuffled (y', x', w', z')
		const v_ui32_4 XZ_YW = min(v, zwzw(v));
		return x(min(XZ_YW, yxwz(XZ_YW)));
	}

	const v_ui32_4::ValueType min3(const v_ui32_4 & v)
	{
		// gets the min of (x, y, z, w), (z, w, z, w) and then of (y, x, w, z)
		return x(min(yxwz(v), min(v, zwzw(v))));
	}

	const v_ui32_4 max(const v_ui32_4 & a, const v_ui32_4 & b)
	{
		return _mm_max_epu32(a.components, b.components);
	}

	const v_ui32_4::ValueType max(const v_ui32_4 & v)
	{
		const v_ui32_4 XZ_YW = max(v, zwzw(v));
		return x(max(XZ_YW, yxwz(XZ_YW)));
	}

	const v_ui32_4::ValueType max3(const v_ui32_4 & v)
	{
		return x(max(yxwz(v), max(v, zwzw(v))));
	}

	const v_ui32_4 clamp(const v_ui32_4 & v, const v_ui32_4 & l, const v_ui32_4 & m)
	{
		return max(l, min(v, m));
	}

	const bool anyTrue(const v_ui32_4 & v)
	{
		return !_mm_testz_si128(v.components, MaskAll<v_ui32_4::PackedType>());
	}

	const bool anyTrue3(const v_ui32_4 & v)
	{
		return !_mm_testz_si128(v.components, MaskXYZ<v_ui32_4>().components);
	}

	const bool anyFalse(const v_ui32_4 & v)
	{
		return !_mm_testc_si128(v.components, MaskAll<v_ui32_4::PackedType>());
	}

	const bool anyFalse3(const v_ui32_4 & v)
	{
		return !_mm_testc_si128(v.components, MaskXYZ<v_ui32_4>().components);
	}

	const bool allTrue(const v_ui32_4 & v)
	{
		return !!_mm_testc_si128(v.components, MaskAll<v_ui32_4::PackedType>());
	}

	const bool allTrue3(const v_ui32_4 & v)
	{
		return !!_mm_testc_si128(v.components, MaskXYZ<v_ui32_4>().components);
	}

	const bool allFalse(const v_ui32_4 & v)
	{
		return !!_mm_testz_si128(v.components, MaskAll<v_ui32_4::PackedType>());
	}

	const bool allFalse3(const v_ui32_4 & v)
	{
		return !!_mm_testz_si128(v.components, MaskXYZ<v_ui32_4>().components);
	}

	//}
#pragma endregion

}
