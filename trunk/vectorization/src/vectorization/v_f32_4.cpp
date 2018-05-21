#include "vectorization/v_f32_4.h"

namespace vectorization
{

#pragma region v_f32_4::
	//{ v_f32_4::

	v_f32_4::v_f32_4() noexcept
		: components(Zero<v_f32_4::PackedType>())
	{ }

	v_f32_4::v_f32_4(const v_f32_4::PackedType & v) noexcept
		: components(v)
	{ }

	v_f32_4::v_f32_4(const v_f32_4::ValueType v) noexcept
		: components(_mm_set_ps1(v))
	{ }

	v_f32_4::v_f32_4(const v_f32_4::ValueType x, const v_f32_4::ValueType y) noexcept
		: components(_mm_set_ps(Zero<ValueType>(), Zero<ValueType>(), y, x))
	{ }

	v_f32_4::v_f32_4(const v_f32_4::ValueType x, const v_f32_4::ValueType y, const v_f32_4::ValueType z) noexcept
		: components(_mm_set_ps(Zero<v_f32_4::ValueType>(), z, y, x))
	{ }

	v_f32_4::v_f32_4(const v_f32_4::ValueType x, const v_f32_4::ValueType y, const v_f32_4::ValueType z, const v_f32_4::ValueType w) noexcept
		: components(_mm_set_ps(w, z, y, x))
	{ }

	v_f32_4::v_f32_4(const v_f32_4::VectorType * const v) noexcept
		: v_f32_4(reinterpret_cast<const v_f32_4::ValueType * const>(v))
	{ }

	v_f32_4::v_f32_4(const v_f32_4::ValueType * const v) noexcept
		: components(_mm_load_ps(v))
	{ }

	v_f32_4::v_f32_4(const v_f32_4::PackedType * const v) noexcept
		: v_f32_4(reinterpret_cast<const v_f32_4::ValueType * const>(v))
	{ }

	v_f32_4::VectorType & v_f32_4::operator=(const v_f32_4::PackedType & v) noexcept
	{
		components = v;
		return *this;
	}

	v_f32_4::ValueType & v_f32_4::operator[](const int index) noexcept
	{
		assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
		return reinterpret_cast<v_f32_4::ValueType * const>(this)[index];
	}

	const v_f32_4::ValueType & v_f32_4::operator[](const int index) const noexcept
	{
		assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
		return reinterpret_cast<const v_f32_4::ValueType * const>(this)[index];
	}

	v_f32_4::ValueType & v_f32_4::operator[](const ASizeT index) noexcept
	{
		assert(index < SIZE);
		return reinterpret_cast<v_f32_4::ValueType * const>(this)[index];
	}

	const v_f32_4::ValueType & v_f32_4::operator[](const ASizeT index) const noexcept
	{
		assert(index < SIZE);
		return reinterpret_cast<const v_f32_4::ValueType * const>(this)[index];
	}

	void v_f32_4::setX(const v_f32_4::ValueType s) noexcept
	{
		components = replaceComponent<VectorIndices::X>(*this, s).components;
	}

	void v_f32_4::setY(const v_f32_4::ValueType s) noexcept
	{
		components = replaceComponent<VectorIndices::Y>(*this, s).components;
	}

	void v_f32_4::setZ(const v_f32_4::ValueType s) noexcept
	{
		components = replaceComponent<VectorIndices::Z>(*this, s).components;
	}

	void v_f32_4::setW(const v_f32_4::ValueType s) noexcept
	{
		components = replaceComponent<VectorIndices::W>(*this, s).components;
	}

	//}
#pragma endregion

#pragma region Load/Store
	//{Load/Store

	void store(const v_f32_4 & v, v_f32_4 * const mem) noexcept
	{
		store(v, reinterpret_cast<v_f32_4::ValueType * const>(mem));
	}

	void store(const v_f32_4 & v, v_f32_4::ValueType * const mem) noexcept
	{
		_mm_store_ps(mem, v.components);
	}

	//}
#pragma endregion

#pragma region Constant generators
	//{Constant generators

	template <>
	const v_f32_4 MaskAll<v_f32_4>() noexcept
	{
		return MaskAll<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 MaskX<v_f32_4>() noexcept
	{
		return MaskX<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 MaskXY<v_f32_4>() noexcept
	{
		return MaskXY<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 MaskXYZ<v_f32_4>() noexcept
	{
		return MaskXYZ<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 MaskXYZW<v_f32_4>() noexcept
	{
		return MaskXYZW<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 MaskY<v_f32_4>() noexcept
	{
		return MaskY<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 MaskYZ<v_f32_4>() noexcept
	{
		return MaskYZ<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 MaskYZW<v_f32_4>() noexcept
	{
		return MaskYZW<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 MaskZ<v_f32_4>() noexcept
	{
		return MaskZ<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 MaskZW<v_f32_4>() noexcept
	{
		return MaskZW<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 MaskW<v_f32_4>() noexcept
	{
		return MaskW<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 OneX<v_f32_4>() noexcept
	{
		return OneX<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 OneY<v_f32_4>() noexcept
	{
		return OneY<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 OneZ<v_f32_4>() noexcept
	{
		return OneZ<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 OneW<v_f32_4>() noexcept
	{
		return OneW<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 Zero<v_f32_4>() noexcept
	{
		return Zero<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 Half<v_f32_4>() noexcept
	{
		return Half<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 One<v_f32_4>() noexcept
	{
		return One<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 OneHalf<v_f32_4>() noexcept
	{
		return OneHalf<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 Two<v_f32_4>() noexcept
	{
		return Two<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 NegZero<v_f32_4>() noexcept
	{
		return NegZero<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 NegOne<v_f32_4>() noexcept
	{
		return NegOne<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 NegTwo<v_f32_4>() noexcept
	{
		return NegTwo<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 Infinity<v_f32_4>() noexcept
	{
		return Infinity<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 NegInfinity<v_f32_4>() noexcept
	{
		return NegInfinity<v_f32_4::PackedType>();
	}

	const v_f32_4 Epsilon_v_f32_4 = v_f32_4(Epsilon<v_f32_4::ValueType>());

	template <>
	const v_f32_4 Epsilon<v_f32_4>() noexcept
	{
		return Epsilon_v_f32_4;
	}

	const v_f32_4 SelfOcclusionEpsilon_v_f32_4 = v_f32_4(SelfOcclusionEpsilon<v_f32_4::ValueType>());

	template <>
	const v_f32_4 SelfOcclusionEpsilon<v_f32_4>() noexcept
	{
		return SelfOcclusionEpsilon_v_f32_4;
	}

	//}
#pragma endregion

#pragma region Commonly used swizzle accessors
	// Commonly used swizzle accessors

	const v_f32_4 xxxx(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(v);
	}

	const v_f32_4 yyyy(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(v);
	}

	const v_f32_4 zzzz(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
	}

	const v_f32_4 wwww(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(v);
	}

	const v_f32_4 xxxz(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::Z>(v);
	}

	const v_f32_4 xxyy(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(v);
	}

	const v_f32_4 xxzz(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(v);
	}

	const v_f32_4 xyxy(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(v);
	}

	const v_f32_4 xzzz(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(v);
	}

	const v_f32_4 yxxy(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(v);
	}

	const v_f32_4 yxwz(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z>(v);
	}

	const v_f32_4 yyww(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(v);
	}

	const v_f32_4 yzxw(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::X, VectorIndices::W>(v);
	}

	const v_f32_4 yzwx(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::X>(v);
	}

	const v_f32_4 zxyw(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::X, VectorIndices::Y, VectorIndices::W>(v);
	}

	const v_f32_4 zzyy(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(v);
	}

	const v_f32_4 zzww(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(v);
	}

	const v_f32_4 zwxy(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::X, VectorIndices::Y>(v);
	}

	const v_f32_4 zwzw(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(v);
	}

	const v_f32_4 wxyz(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::W, VectorIndices::X, VectorIndices::Y, VectorIndices::Z>(v);
	}

	const v_f32_4 wzyx(const v_f32_4 & v) noexcept
	{
		return swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(v);
	}

	const v_f32_4 x_yzw(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return blend<false, true, true, true>(a, b);
	}

	const v_f32_4 xx_xx(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(a, b);
	}

	const v_f32_4 xx_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(a, b);
	}

	const v_f32_4 xy_xy(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(a, b);
	}

	const v_f32_4 xy_zw(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(a, b);
	}

	const v_f32_4 xz_xz(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::X, VectorIndices::Z, VectorIndices::X, VectorIndices::Z>(a, b);
	}

	const v_f32_4 yx_xy(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::Y, VectorIndices::X, VectorIndices::X, VectorIndices::Y>(a, b);
	}

	const v_f32_4 yy_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(a, b);
	}

	const v_f32_4 yw_yw(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::Y, VectorIndices::W, VectorIndices::Y, VectorIndices::W>(a, b);
	}

	const v_f32_4 zz_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Y, VectorIndices::Y>(a, b);
	}

	const v_f32_4 zz_zz(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(a, b);
	}

	const v_f32_4 zw_zw(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(a, b);
	}

	const v_f32_4 ww_ww(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return swizzledBlend_2x2<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(a, b);
	}

	//}
#pragma endregion

#pragma region Operators
	//{ Operators

	// http://fastcpp.blogspot.de/2011/03/changing-sign-of-float-values-using-sse.html
	const v_f32_4 operator-(const v_f32_4 & v) noexcept
	{
		return _mm_xor_ps(v.components, NegZero<v_f32_4::PackedType>());
	}

	const v_f32_4::VectorBoolType operator!(const v_f32_4 & v) noexcept
	{
		return _mm_cmpeq_epi32(Zero<v_f32_4::VectorBoolType::PackedType>(), _mm_castps_si128(v.components));
	}

	const v_f32_4 operator~(const v_f32_4 & v) noexcept
	{
		return _mm_andnot_ps(v.components, MaskAll<v_f32_4::PackedType>());
	}

	const v_f32_4 operator+(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_add_ps(a.components, b.components);
	}

	const v_f32_4 operator+(const v_f32_4 & a, const v_f32_4::ValueType & b) noexcept
	{
		return _mm_add_ps(a.components, _mm_set_ps1(b));
	}

	const v_f32_4 operator+(const v_f32_4::ValueType & a, const v_f32_4 & b) noexcept
	{
		return _mm_add_ps(_mm_set_ps1(a), b.components);
	}

	const v_f32_4 operator-(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_sub_ps(a.components, b.components);
	}

	const v_f32_4 operator-(const v_f32_4 & a, const v_f32_4::ValueType & b) noexcept
	{
		return _mm_sub_ps(a.components, _mm_set_ps1(b));
	}

	const v_f32_4 operator-(const v_f32_4::ValueType & a, const v_f32_4 & b) noexcept
	{
		return _mm_sub_ps(_mm_set_ps1(a), b.components);
	}

	const v_f32_4 operator*(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_mul_ps(a.components, b.components);
	}

	const v_f32_4 operator*(const v_f32_4 & a, const v_f32_4::ValueType & b) noexcept
	{
		return _mm_mul_ps(a.components, _mm_set_ps1(b));
	}

	const v_f32_4 operator*(const v_f32_4::ValueType & a, const v_f32_4 & b) noexcept
	{
		return _mm_mul_ps(_mm_set_ps1(a), b.components);
	}

	const v_f32_4 operator/(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return divide(a.components, b.components);
	}

	const v_f32_4 operator/(const v_f32_4 & a, const v_f32_4::ValueType & b) noexcept
	{
		return a / v_f32_4(b);
	}

	const v_f32_4 operator/(const v_f32_4::ValueType & a, const v_f32_4 & b) noexcept
	{
		return v_f32_4(a) / b;
	}

	const v_f32_4 operator%(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return modulo(a.components, b.components);
	}

	const v_f32_4 operator&(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_and_ps(a.components, b.components);
	}

	const v_f32_4 operator|(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_or_ps(a.components, b.components);
	}

	const v_f32_4 operator^(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_xor_ps(a.components, b.components);
	}

	const v_f32_4 operator<<(const v_f32_4 & a, const v_f32_4::VectorBoolType & b) noexcept
	{
		return _mm_castsi128_ps(_mm_sll_epi32(_mm_castps_si128(a.components), b.components));
	}

	const v_f32_4 operator>>(const v_f32_4 & a, const v_f32_4::VectorBoolType & b) noexcept
	{
		return _mm_castsi128_ps(_mm_srl_epi32(_mm_castps_si128(a.components), b.components));
	}

	const v_f32_4::VectorBoolType operator<(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_castps_si128(_mm_cmplt_ps(a.components, b.components));
	}

	const v_f32_4::VectorBoolType operator>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_castps_si128(_mm_cmpgt_ps(a.components, b.components));
	}

	const v_f32_4::VectorBoolType operator<=(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_castps_si128(_mm_cmple_ps(a.components, b.components));
	}

	const v_f32_4::VectorBoolType operator>=(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_castps_si128(_mm_cmpge_ps(a.components, b.components));
	}

	const v_f32_4::VectorBoolType operator==(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_castps_si128(_mm_cmpeq_ps(a.components, b.components));
	}

	const v_f32_4::VectorBoolType operator!=(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_castps_si128(_mm_cmpneq_ps(a.components, b.components));
	}

	v_f32_4 & operator+=(v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return a = a + b;
	}

	v_f32_4 & operator-=(v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return a = a - b;
	}

	v_f32_4 & operator*=(v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return a = a * b;
	}

	v_f32_4 & operator/=(v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return a = a / b;
	}

	v_f32_4 & operator%=(v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return a = a % b;
	}

	v_f32_4 & operator&=(v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return a = a & b;
	}

	v_f32_4 & operator|=(v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return a = a | b;
	}

	v_f32_4 & operator^=(v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return a = a ^ b;
	}

	v_f32_4 & operator<<=(v_f32_4 & a, const v_f32_4::VectorBoolType & b) noexcept
	{
		return a = a << b;
	}

	v_f32_4 & operator>>=(v_f32_4 & a, const v_f32_4::VectorBoolType & b) noexcept
	{
		return a = a >> b;
	}

	//}
#pragma endregion

#pragma region swizzle()
	//{ swizzle()

	template <>
	const v_f32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & v) noexcept
	{
		return v;
	}

	template <>
	const v_f32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const v_f32_4 & v) noexcept
	{
		return _mm_unpacklo_ps(v.components, v.components);
	}

	template <>
	const v_f32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(const v_f32_4 & v) noexcept
	{
		return _mm_moveldup_ps(v.components);
	}

	template <>
	const v_f32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_f32_4 & v) noexcept
	{
		return _mm_movelh_ps(v.components, v.components);
	}

	template <>
	const v_f32_4 swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const v_f32_4 & v) noexcept
	{
		return _mm_movehdup_ps(v.components);
	}

	template <>
	const v_f32_4 swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const v_f32_4 & v) noexcept
	{
		return _mm_unpackhi_ps(v.components, v.components);
	}

	template <>
	const v_f32_4 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & v) noexcept
	{
		return _mm_movehl_ps(v.components, v.components);
	}

	//}
#pragma endregion

#pragma region blend()
	//{ blend()

	template <>
	const v_f32_4 blend<false, false, false, false>(const v_f32_4 & a, const v_f32_4 &) noexcept
	{
		return a;
	}

	template <>
	const v_f32_4 blend<true, true, true, true>(const v_f32_4 &, const v_f32_4 & b) noexcept
	{
		return b;
	}

	template <>
	const v_f32_4 blend<false, false, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_shuffle_ps(a.components, b.components, _MM_SHUFFLE(VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X));
	}

	template <>
	const v_f32_4 blend<false, true, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		// move x(a) into b
		return _mm_move_ss(b.components, a.components);
	}

	template <>
	const v_f32_4 blend<true, false, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		// move x(b) into a
		return _mm_move_ss(a.components, b.components);
	}

	//}
#pragma endregion

#pragma region swizzledBlend()
	//{ swizzledBlend()

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, false, false, false, false>(const v_f32_4 & a, const v_f32_4 &) noexcept
	{
		return a;
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, true, true, true, true>(const v_f32_4 &, const v_f32_4 & b) noexcept
	{
		return b;
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, true, false, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		// move x(b) into a
		return _mm_move_ss(a.components, b.components);
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, false, true, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		// move x(a) into b
		return _mm_move_ss(b.components, a.components);
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false, true, false, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_unpacklo_ps(a.components, b.components);
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true, false, true, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_unpacklo_ps(b.components, a.components);
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false, false, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_movelh_ps(a.components, b.components);
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true, true, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_movelh_ps(b.components, a.components);
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false, true, false, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_unpackhi_ps(a.components, b.components);
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true, false, true, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_unpackhi_ps(b.components, a.components);
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true, true, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_movehl_ps(a.components, b.components);
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false, false, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_movehl_ps(b.components, a.components);
	}

	template <>
	const v_f32_4 swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_movelh_ps(a.components, b.components);
	}

	template <>
	const v_f32_4 swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_movehl_ps(b.components, a.components);
	}

	//}
#pragma endregion

#pragma region blendMasked()
	//{ blendMasked()

	const v_f32_4 blendMasked(const v_f32_4 & onBitNotSet, const v_f32_4 & onBitSet, const v_f32_4 & mask) noexcept
	{
		return blendMasked(onBitNotSet.components, onBitSet.components, mask.components);
	}

	const v_f32_4 blendMasked(const v_f32_4 & onBitNotSet, const v_f32_4 & onBitSet, const v_f32_4::VectorBoolType & mask) noexcept
	{
		return blendMasked(onBitNotSet.components, onBitSet.components, _mm_castsi128_ps(mask.components));
	}

	//}
#pragma endregion

#pragma region select()
	//{ select()

	const v_f32_4 select(const bool mask, const v_f32_4 & onTrue, const v_f32_4 & onFalse) noexcept
	{
		return select(mask, onTrue.components, onFalse.components);
	}

	const v_f32_4 select(const v_f32_4::VectorBoolType & mask, const v_f32_4 & onTrue, const v_f32_4 & onFalse) noexcept
	{
		return select(_mm_castsi128_ps(mask.components), onTrue.components, onFalse.components);
	}

	const v_f32_4 select(const v_f32_4 & mask, const v_f32_4 & onTrue, const v_f32_4 & onFalse) noexcept
	{
		return select(mask.components, onTrue.components, onFalse.components);
	}

	//}
#pragma endregion

#pragma region swizzledBlendMasked()
	//{ swizzledBlendMasked()

	template <>
	const v_f32_4 swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4 & mask) noexcept
	{
		return blendMasked(a, b, mask);
	}

	template <>
	const v_f32_4 swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4::VectorBoolType & mask) noexcept
	{
		return blendMasked(a, b, mask);
	}

	//}
#pragma endregion

#pragma region setComponent()
	//{ setComponent()

	template <>
	void setComponent<VectorIndices::X>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept
	{
		v.components = replaceComponent<VectorIndices::X>(v, s).components;
	}

	template <>
	void setComponent<VectorIndices::Y>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept
	{
		v.components = replaceComponent<VectorIndices::Y>(v, s).components;
	}

	template <>
	void setComponent<VectorIndices::Z>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept
	{
		v.components = replaceComponent<VectorIndices::Z>(v, s).components;
	}

	template <>
	void setComponent<VectorIndices::W>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept
	{
		v.components = replaceComponent<VectorIndices::W>(v, s).components;
	}

	//}
#pragma endregion

#pragma region replaceComponent()
	//{ replaceComponent()

	template <>
	const v_f32_4 replaceComponent<VectorIndices::X>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return _mm_move_ss(v.components, _mm_set_ss(s));
#else
		return _mm_insert_ps(v.components, _mm_set_ss(s), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::X, VectorBits::None));
#endif
	}

	template <>
	const v_f32_4 replaceComponent<VectorIndices::Y>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return blendMasked(v.components, _mm_set_ps1(s), MaskY<v_f32_4::PackedType>());
#else
		return _mm_insert_ps(v.components, _mm_set_ss(s), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::Y, VectorBits::None));
#endif
	}

	template <>
	const v_f32_4 replaceComponent<VectorIndices::Z>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return blendMasked(v.components, _mm_set_ps1(s), MaskZ<v_f32_4::PackedType>());
#else
		return _mm_insert_ps(v.components, _mm_set_ss(s), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::Z, VectorBits::None));
#endif
	}

	template <>
	const v_f32_4 replaceComponent<VectorIndices::W>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return blendMasked(v.components, _mm_set_ps1(s), MaskW<v_f32_4::PackedType>());
#else
		return _mm_insert_ps(v.components, _mm_set_ss(s), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::W, VectorBits::None));
#endif
	}

	//}
#pragma endregion

#pragma region component()
	//{ component()

	template <>
	const v_f32_4::ValueType component<VectorIndices::X>(const v_f32_4 & v) noexcept
	{
		return _mm_cvtss_f32(v.components);
	}

	template <>
	const v_f32_4::ValueType component<VectorIndices::Y>(const v_f32_4 & v) noexcept
	{
		return _mm_cvtss_f32(yyww(v).components);
	}

	template <>
	const v_f32_4::ValueType component<VectorIndices::Z>(const v_f32_4 & v) noexcept
	{
		return _mm_cvtss_f32(zwzw(v).components);
	}

	template <>
	const v_f32_4::ValueType component<VectorIndices::W>(const v_f32_4 & v) noexcept
	{
		return _mm_cvtss_f32(wwww(v).components);
	}

	//}
#pragma endregion

#pragma region _()
	//{ _()

	const v_f32_4::ValueType x(const v_f32_4 & v) noexcept
	{
		return component<VectorIndices::X>(v);
	}

	const v_f32_4::ValueType y(const v_f32_4 & v) noexcept
	{
		return component<VectorIndices::Y>(v);
	}

	const v_f32_4::ValueType z(const v_f32_4 & v) noexcept
	{
		return component<VectorIndices::Z>(v);
	}

	const v_f32_4::ValueType w(const v_f32_4 & v) noexcept
	{
		return component<VectorIndices::W>(v);
	}

	//}
#pragma endregion

#pragma region replace_()
	//{ replace_()

	const v_f32_4 replaceX(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept
	{
		return replaceComponent<VectorIndices::X>(v, s);
	}

	const v_f32_4 replaceY(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept
	{
		return replaceComponent<VectorIndices::Y>(v, s);
	}

	const v_f32_4 replaceZ(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept
	{
		return replaceComponent<VectorIndices::Z>(v, s);
	}

	const v_f32_4 replaceW(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept
	{
		return replaceComponent<VectorIndices::W>(v, s);
	}

	//}
#pragma endregion

#pragma region min()
	//{ min()

	const v_f32_4 min(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return min(a.components, b.components);
	}

	const v_f32_4 minv(const v_f32_4 & v) noexcept
	{
		const v_f32_4 XZ_YW = min(v, zwzw(v));
		return min(XZ_YW, yyww(XZ_YW));
	}

	const v_f32_4::ValueType min(const v_f32_4 & v) noexcept
	{
		return x(minv(v));
	}

	const v_f32_4 min3v(const v_f32_4 & v) noexcept
	{
		return xxxx(min(min(v, yyww(v)), zwzw(v)));
	}

	const v_f32_4::ValueType min3(const v_f32_4 & v) noexcept
	{
		return x(min3v(v));
	}

	//}
#pragma endregion

#pragma region max()
	//{ max()

	const v_f32_4 max(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return max(a.components, b.components);
	}

	const v_f32_4 maxv(const v_f32_4 & v) noexcept
	{
		const v_f32_4 XZ_YW = max(v, zwzw(v));
		return max(XZ_YW, yyww(XZ_YW));
	}

	const v_f32_4::ValueType max(const v_f32_4 & v) noexcept
	{
		return x(maxv(v));
	}

	const v_f32_4 max3v(const v_f32_4 & v) noexcept
	{
		return xxxx(max(max(v, yyww(v)), zwzw(v)));
	}

	const v_f32_4::ValueType max3(const v_f32_4 & v) noexcept
	{
		return x(max3v(v));
	}

	//}
#pragma endregion

#pragma region horizontalSum()
	//{ horizontalSum()

	const v_f32_4 horizontalSumv(const v_f32_4 & v) noexcept
	{
		const v_f32_4::PackedType t = _mm_hadd_ps(v.components, v.components);
		return _mm_hadd_ps(t, t);
	}

	const v_f32_4::ValueType horizontalSum(const v_f32_4 & v) noexcept
	{
		return x(horizontalSumv(v));
	}

	const v_f32_4 horizontalSum3v(const v_f32_4 & v) noexcept
	{
		return horizontalSumv(zeroW(v));
	}

	const v_f32_4::ValueType horizontalSum3(const v_f32_4 & v) noexcept
	{
		return x(horizontalSum3v(v));
	}

	//}
#pragma endregion

#pragma region dot()
	//{ dot()

	const v_f32_4 dotv(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return horizontalSumv(a * b);
#else
		return _mm_dp_ps(a.components, b.components, 0xFF);
#endif
	}

	const v_f32_4::ValueType dot(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return x(dotv(a, b));
	}

	const v_f32_4 dot3v(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return horizontalSumv(zeroW(a * b));
#else
		return _mm_dp_ps(a.components, b.components, 0x7F);
#endif
	}

	const v_f32_4::ValueType dot3(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return x(dot3v(a, b));
	}

	//}
#pragma endregion

#pragma region length()
	//{ length()

	const v_f32_4 lengthv(const v_f32_4 & v) noexcept
	{
		return sqrt(dotv(v, v));
	}

	const v_f32_4::ValueType length(const v_f32_4 & v) noexcept
	{
		return x(lengthv(v));
	}

	const v_f32_4 rlengthv(const v_f32_4 & v) noexcept
	{
		return rsqrt(dotv(v, v));
	}

	const v_f32_4::ValueType rlength(const v_f32_4 & v) noexcept
	{
		return x(rlengthv(v));
	}

	const v_f32_4 length3v(const v_f32_4 & v) noexcept
	{
		return sqrt(dot3v(v, v));
	}

	const v_f32_4::ValueType length3(const v_f32_4 & v) noexcept
	{
		return x(length3v(v));
	}

	const v_f32_4 rlength3v(const v_f32_4 & v) noexcept
	{
		return rsqrt(dot3v(v, v));
	}

	const v_f32_4::ValueType rlength3(const v_f32_4 & v) noexcept
	{
		return x(rlength3v(v));
	}

	//}
#pragma endregion

#pragma region normalize()
	//{ normalize()

	const v_f32_4 normalize(const v_f32_4 & v) noexcept
	{
#ifdef VECTORIZATION_APPROXIMATIONS
		return v * rlengthv(v);
#else
		return v / lengthv(v);
#endif
	}

	const v_f32_4 normalize3(const v_f32_4 & v) noexcept
	{
#ifdef VECTORIZATION_APPROXIMATIONS
		return zeroW(v * rlength3v(v));
#else
		return zeroW(v / length3v(v));
#endif
	}

	//}
#pragma endregion

#pragma region distance()
	//{ distance()

	const v_f32_4 distancev(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return lengthv(b - a);
	}

	const v_f32_4::ValueType distance(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return x(distancev(a, b));
	}

	const v_f32_4 distance3v(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return length3v(b - a);
	}

	const v_f32_4::ValueType distance3(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return x(distance3v(a, b));
	}

	//}
#pragma endregion

#pragma region reflect()
	//{ reflect()

	const v_f32_4 reflect(const v_f32_4 & incident, const v_f32_4 & normal) noexcept
	{
		return reflect(incident, normal, dotv(normal, incident));
	}

	const v_f32_4 reflect(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & NdotI) noexcept
	{
		const v_f32_4 t = normal * NdotI;
		return incident - (t + t);
	}

	//}
#pragma endregion

#pragma region refract()
	//{ refract()

	const v_f32_4 refractEta(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & NdotI, const v_f32_4 & eta) noexcept
	{
		// By snells law:
		// sin²(phi_t) = (eta_i/eta_t)² * sin²(phi_i)
		// = (eta_i/eta_t)² * (1 - cos²(phi_i)) = (eta_i/eta_t)² * (1 - (-I.N)²)
		// = (eta_i/eta_t)² - ((eta_i/eta_t) * (I.N))²
		// = 1 - cos²(phi_t)
		const v_f32_4 etaNdotI = eta * NdotI;
		const v_f32_4 sinSqrPhiT = eta * eta - etaNdotI * etaNdotI;
		// cos²(phi_t) = 1 - sin²(phi_t)
		const v_f32_4 cosSqrPhiT = One<v_f32_4>() - sinSqrPhiT;
		// TIR if 0 < cosSqrPhiT
		if (isNegative(cosSqrPhiT)) return Zero<v_f32_4>();
		// (eta_i/eta_t) * I + ((eta_i/eta_t) * cos(phi_i) - sqrt(cos²(phi_t))) * N
		// = (eta_i/eta_t) * I + ((eta_i/eta_t) * (-I.N) - sqrt(cos²(phi_t))) * N
		// = (eta_i/eta_t) * I - ((eta_i/eta_t) * (I.N) + sqrt(cos²(phi_t))) * N
		return eta * incident - (etaNdotI + sqrt(cosSqrPhiT)) * normal;
	}

	const v_f32_4 refractEta(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & eta) noexcept
	{
		return refractEta(incident, normal, dotv(normal, incident), eta);
	}

	const v_f32_4 refract(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & eta) noexcept
	{
		return refract(incident, normal, dotv(normal, incident), eta);
	}

	const v_f32_4 refract(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & NdotI, const v_f32_4 & eta) noexcept
	{
		if (isNegative(NdotI))
		{
			return refractEta(incident, normal, NdotI, xxxx(eta));
		}
		return refractEta(incident, -normal, -NdotI, yyyy(eta));
	}

	const v_f32_4 refract(const v_f32_4::VectorBoolType & cullingOrientation, const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & eta) noexcept
	{
		return refract(cullingOrientation, incident, normal, dotv(normal, incident), eta);
	}

	const v_f32_4 refract(const v_f32_4::VectorBoolType & cullingOrientation, const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & NdotI, const v_f32_4 & eta) noexcept
	{
		if (x(cullingOrientation)) // => NdotI < 0
		{
			return refractEta(incident, normal, NdotI, xxxx(eta));
		}
		if (y(cullingOrientation)) // => NdotI > 0
		{
			return refractEta(incident, -normal, -NdotI, yyyy(eta));
		}
		// cullingOrientation is zero => "no culling" set => no implicit ray-orientation set
		return refract(incident, normal, NdotI, eta);
	}

	//}
#pragma endregion

#pragma region log()
	//{ log()

	const v_f32_4 log(const v_f32_4 & v) noexcept
	{
		return v_f32_4(log(x(v)), log(y(v)), log(z(v)), log(w(v)));
	}

	const v_f32_4 log3(const v_f32_4 & v) noexcept
	{
		return v_f32_4(log(x(v)), log(y(v)), log(z(v)), w(v));
	}

	//}
#pragma endregion

#pragma region exp()
	//{ exp()

	const v_f32_4 exp(const v_f32_4 & v) noexcept
	{
		return v_f32_4(exp(x(v)), exp(y(v)), exp(z(v)), exp(w(v)));
	}

	const v_f32_4 exp3(const v_f32_4 & v) noexcept
	{
		return v_f32_4(exp(x(v)), exp(y(v)), exp(z(v)), w(v));
	}

	//}
#pragma endregion

#pragma region pow()
	//{ pow()

	const v_f32_4 pow(const v_f32_4 & v, const v_f32_4 & exponent) noexcept
	{
		return v_f32_4(pow(x(v), x(exponent)), pow(y(v), y(exponent)), pow(z(v), z(exponent)), pow(w(v), w(exponent)));
	}

	const v_f32_4 pow(const v_f32_4 & v, const v_f32_4::ValueType exponent) noexcept
	{
		return v_f32_4(pow(x(v), exponent), pow(y(v), exponent), pow(z(v), exponent), pow(w(v), exponent));
	}

	const v_f32_4 pow3(const v_f32_4 & v, const v_f32_4 & exponent) noexcept
	{
		return v_f32_4(pow(x(v), x(exponent)), pow(y(v), y(exponent)), pow(z(v), z(exponent)), w(v));
	}

	const v_f32_4 pow3(const v_f32_4 & v, const v_f32_4::ValueType exponent) noexcept
	{
		return v_f32_4(pow(x(v), exponent), pow(y(v), exponent), pow(z(v), exponent), w(v));
	}

	//}
#pragma endregion

#pragma region mix()
	//{ mix()

	const v_f32_4 mix(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4 & factor) noexcept
	{
		return mix(a.components, b.components, factor.components);
	}

	const v_f32_4 mix(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4::ValueType factor) noexcept
	{
		return mix(a.components, b.components, _mm_set_ps1(factor));
	}

	//}
#pragma endregion

#pragma region Special vector operations
	//{ Special vector operations

	const v_f32_4 zeroW(const v_f32_4 & v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return _mm_and_ps(v.components, MaskXYZ<v_f32_4::PackedType>());
#else
		return _mm_insert_ps(v.components, v.components, _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::X, VectorBits::W));
#endif
	}

	const v_f32_4 oneW(const v_f32_4 & v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return blendMasked(v.components, One<v_f32_4::PackedType>(), MaskW<v_f32_4::PackedType>());
#else
		return _mm_insert_ps(v.components, One<v_f32_4::PackedType>(), _MM_MK_INSERTPS_NDX(VectorIndices::W, VectorIndices::W, VectorBits::None));
#endif
	}

	const v_f32_4::BoolType isNegative(const v_f32_4 & v) noexcept
	{
		return _mm_movemask_ps(v.components);
	}

	const v_f32_4::VectorBoolType isNaN(const v_f32_4 & v) noexcept
	{
		return _mm_castps_si128(isNaN(v.components));
	}

	const v_f32_4::VectorBoolType isNaN(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_castps_si128(isNaN(a.components, b.components));
	}

	const v_f32_4 subadd(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return _mm_addsub_ps(a.components, b.components);
	}

	const v_f32_4 sqrt(const v_f32_4 & v) noexcept
	{
		return sqrt(v.components);
	}

	const v_f32_4 reciprocal(const v_f32_4 & v) noexcept
	{
		return reciprocal(v.components);
	}

	const v_f32_4 rsqrt(const v_f32_4 & v) noexcept
	{
		return rsqrt(v.components);
	}

	const v_f32_4 ceil(const v_f32_4 & v) noexcept
	{
		return ceil(v.components);
	}

	const v_f32_4 floor(const v_f32_4 & v) noexcept
	{
		return floor(v.components);
	}

	const v_f32_4 round(const v_f32_4 & v) noexcept
	{
		return round(v.components);
	}

	const v_f32_4 fract(const v_f32_4 & v) noexcept
	{
		return fract(v.components);
	}

	const v_f32_4 modulo(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return modulo(a.components, b.components);
	}

	const v_f32_4 abs(const v_f32_4 & v) noexcept
	{
		return abs(v.components);
	}

	const v_f32_4 cross3(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		// http://fastcpp.blogspot.de/2011/04/vector-cross-product-using-sse-code.html
		return yzxw(a) * zxyw(b) - zxyw(a) * yzxw(b);
	}

	const v_f32_4 clamp(const v_f32_4 & v, const v_f32_4 & lower, const v_f32_4 & upper) noexcept
	{
		return clamp(v.components, lower.components, upper.components);
	}

	const v_f32_4 mapOrthogonal(const v_f32_4 & v, const v_f32_4 & sPlane, const v_f32_4 & tPlane) noexcept
	{
		return x_yzw(
			dotv(v, sPlane),
			zw_zw(dotv(v, tPlane), OneW<v_f32_4>())
		);
	}

	const v_f32_4 mapSpherical(const v_f32_4 & v) noexcept
	{
		return replaceX(
			replaceY(OneW<v_f32_4>(), acos(-y(v)) * ReciprocalPi<v_f32_4::ValueType>()),
			atan2(z(v), x(v)) * RadianToUniform<v_f32_4::ValueType>() + Half<v_f32_4::ValueType>()
		);
	}

	const ASizeT argmin3(const v_f32_4 & v) noexcept
	{
		typedef v_f32_4::VectorBoolType VBT;
		typedef v_f32_4::BoolType BT;
		const VBT mask = xyxy(v) < zzyy(v);
		const VBT argXY = blendMasked(
			VBT(static_cast<BT>(VectorIndices::Z)),
			VBT(static_cast<BT>(VectorIndices::X), static_cast<BT>(VectorIndices::Y)),
			xyxy(mask) /* x(v) < z(v), y(v) < z(v) */);
		return static_cast<ASizeT>(x(blendMasked(yyyy(argXY), argXY, zzzz(mask) /* x(v) < y(v) */)));
	}

	const ASizeT argmax3(const v_f32_4 & v) noexcept
	{
		typedef v_f32_4::VectorBoolType VBT;
		typedef v_f32_4::BoolType BT;
		const VBT mask = xyxy(v) > zzyy(v);
		const VBT argXY = blendMasked(
			VBT(static_cast<BT>(VectorIndices::Z)),
			VBT(static_cast<BT>(VectorIndices::X), static_cast<BT>(VectorIndices::Y)),
			xyxy(mask) /* x(v) > z(v), y(v) > z(v) */);
		return static_cast<ASizeT>(x(blendMasked(yyyy(argXY), argXY, zzzz(mask) /* x(v) > y(v) */)));
	}

	const v_f32_4 axisAlignedNormal3(const v_f32_4 & normal) noexcept
	{
		const ASizeT maxAxis = argmax3(abs(normal));
		const v_f32_4 normalSigns = copysign(normal.components);
		v_f32_4 alignedNormal = Zero<v_f32_4>();
		alignedNormal[maxAxis] = normalSigns[maxAxis];
		return alignedNormal;
	}

	//}
#pragma endregion


	std::ostream & operator<< (std::ostream & stream, const v_f32_4 & v) noexcept
	{
		return stream << "{" << x(v) << ", " << y(v) << ", " << z(v) << ", " << w(v) << "}";
	}

}
