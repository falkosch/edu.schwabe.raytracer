#pragma once

#include "v_i32_4.h"

namespace vectorization
{

	// packed SSE-vector of four float32 scalars
	struct v_f32_4
	{
		// Metas
		static const ASizeT SIZE = VectorSizes::W;
		typedef Float_32 ValueType;
		typedef v_i32_4::ValueType BoolType;
		typedef v_f32_4 VectorType;
		typedef v_i32_4 VectorBoolType;
		typedef PackedTypes<ValueType, SIZE>::Type PackedType;

		// the actual data
		PackedType components;

		v_f32_4() noexcept;

		v_f32_4(const PackedType & v) noexcept;

		explicit v_f32_4(const ValueType v) noexcept;

		explicit v_f32_4(const ValueType x, const ValueType y) noexcept;

		explicit v_f32_4(const ValueType x, const ValueType y, const ValueType z) noexcept;

		explicit v_f32_4(const ValueType x, const ValueType y, const ValueType z, const ValueType w) noexcept;

		explicit v_f32_4(const VectorType * const v) noexcept;

		explicit v_f32_4(const ValueType * const v) noexcept;

		explicit v_f32_4(const PackedType * const v) noexcept;

		ALIGNED_ALLOCATORS(__alignof(PackedType));

		operator const PackedType() noexcept
		{
			return components;
		}

		operator const PackedType() const noexcept
		{
			return components;
		}

		VectorType & operator=(const PackedType & v) noexcept;

		ValueType & operator[](const int index) noexcept;

		const ValueType & operator[](const int index) const noexcept;

		ValueType & operator[](const ASizeT index) noexcept;

		const ValueType & operator[](const ASizeT index) const noexcept;

		void setX(const ValueType s) noexcept;

		void setY(const ValueType s) noexcept;

		void setZ(const ValueType s) noexcept;

		void setW(const ValueType s) noexcept;

	}; // v_f32_4

#pragma region Load/Store
	//{Load/Store

	void store(const v_f32_4 & v, v_f32_4 * const mem) noexcept;

	void store(const v_f32_4 & v, v_f32_4::ValueType * const mem) noexcept;

	//}
#pragma endregion

#pragma region Constant generators
	//{Constant generators

	template <>
	const v_f32_4 MaskAll<v_f32_4>() noexcept;

	template <>
	const v_f32_4 MaskX<v_f32_4>() noexcept;

	template <>
	const v_f32_4 MaskXY<v_f32_4>() noexcept;

	template <>
	const v_f32_4 MaskXYZ<v_f32_4>() noexcept;

	template <>
	const v_f32_4 MaskXYZW<v_f32_4>() noexcept;

	template <>
	const v_f32_4 MaskY<v_f32_4>() noexcept;

	template <>
	const v_f32_4 MaskYZ<v_f32_4>() noexcept;

	template <>
	const v_f32_4 MaskYZW<v_f32_4>() noexcept;

	template <>
	const v_f32_4 MaskZ<v_f32_4>() noexcept;

	template <>
	const v_f32_4 MaskZW<v_f32_4>() noexcept;

	template <>
	const v_f32_4 MaskW<v_f32_4>() noexcept;

	template <>
	const v_f32_4 OneX<v_f32_4>() noexcept;

	template <>
	const v_f32_4 OneY<v_f32_4>() noexcept;

	template <>
	const v_f32_4 OneZ<v_f32_4>() noexcept;

	template <>
	const v_f32_4 OneW<v_f32_4>() noexcept;

	template <>
	const v_f32_4 Zero<v_f32_4>() noexcept;

	template <>
	const v_f32_4 One<v_f32_4>() noexcept;

	template <>
	const v_f32_4 OneHalf<v_f32_4>() noexcept;

	template <>
	const v_f32_4 Two<v_f32_4>() noexcept;

	template <>
	const v_f32_4 NegZero<v_f32_4>() noexcept;

	template <>
	const v_f32_4 NegOne<v_f32_4>() noexcept;

	template <>
	const v_f32_4 NegTwo<v_f32_4>() noexcept;

	template <>
	const v_f32_4 Half<v_f32_4>() noexcept;

	template <>
	const v_f32_4 Epsilon<v_f32_4>() noexcept;

	template <>
	const v_f32_4 SelfOcclusionEpsilon<v_f32_4>() noexcept;

	//}
#pragma endregion

#pragma region Commonly used swizzle accessors
	// Commonly used swizzle accessors

	const v_f32_4 xxxx(const v_f32_4 & v) noexcept;

	const v_f32_4 yyyy(const v_f32_4 & v) noexcept;

	const v_f32_4 zzzz(const v_f32_4 & v) noexcept;

	const v_f32_4 wwww(const v_f32_4 & v) noexcept;

	const v_f32_4 xxxz(const v_f32_4 & v) noexcept;

	const v_f32_4 xxyy(const v_f32_4 & v) noexcept;

	const v_f32_4 xxzz(const v_f32_4 & v) noexcept;

	const v_f32_4 xyxy(const v_f32_4 & v) noexcept;

	const v_f32_4 xzzz(const v_f32_4 & v) noexcept;

	const v_f32_4 yxxy(const v_f32_4 & v) noexcept;

	const v_f32_4 yxwz(const v_f32_4 & v) noexcept;

	const v_f32_4 yyww(const v_f32_4 & v) noexcept;

	const v_f32_4 yzxw(const v_f32_4 & v) noexcept;

	const v_f32_4 yzwx(const v_f32_4 & v) noexcept;

	const v_f32_4 zxyw(const v_f32_4 & v) noexcept;

	const v_f32_4 zzyy(const v_f32_4 & v) noexcept;

	const v_f32_4 zzww(const v_f32_4 & v) noexcept;

	const v_f32_4 zwxy(const v_f32_4 & v) noexcept;

	const v_f32_4 zwzw(const v_f32_4 & v) noexcept;

	const v_f32_4 wxyz(const v_f32_4 & v) noexcept;

	const v_f32_4 wzyx(const v_f32_4 & v) noexcept;

	const v_f32_4 x_yzw(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 xx_xx(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 xx_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 xy_xy(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 xy_zw(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 xz_xz(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 yx_xy(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 yy_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 yw_yw(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 zz_yy(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 zz_zz(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 zw_zw(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 ww_ww(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	//}
#pragma endregion

#pragma region Operators
	//{ Operators

	const v_f32_4 operator-(const v_f32_4 & v) noexcept;

	const v_f32_4::VectorBoolType operator!(const v_f32_4 & v) noexcept;

	const v_f32_4 operator~(const v_f32_4 & v) noexcept;

	const v_f32_4 operator+(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 operator+(const v_f32_4 & a, const v_f32_4::ValueType & b) noexcept;

	const v_f32_4 operator+(const v_f32_4::ValueType & a, const v_f32_4 & b) noexcept;

	const v_f32_4 operator-(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 operator-(const v_f32_4 & a, const v_f32_4::ValueType & b) noexcept;

	const v_f32_4 operator-(const v_f32_4::ValueType & a, const v_f32_4 & b) noexcept;

	const v_f32_4 operator*(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 operator*(const v_f32_4 & a, const v_f32_4::ValueType & b) noexcept;

	const v_f32_4 operator*(const v_f32_4::ValueType & a, const v_f32_4 & b) noexcept;

	const v_f32_4 operator/(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 operator/(const v_f32_4 & a, const v_f32_4::ValueType & b) noexcept;

	const v_f32_4 operator/(const v_f32_4::ValueType & a, const v_f32_4 & b) noexcept;

	const v_f32_4 operator%(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 operator&(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 operator|(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 operator^(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 operator<<(const v_f32_4 & a, const v_f32_4::VectorBoolType & b) noexcept;

	const v_f32_4 operator>>(const v_f32_4 & a, const v_f32_4::VectorBoolType & b) noexcept;

	const v_f32_4::VectorBoolType operator<(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::VectorBoolType operator>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::VectorBoolType operator<=(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::VectorBoolType operator>=(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::VectorBoolType operator==(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::VectorBoolType operator!=(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	v_f32_4 & operator+=(v_f32_4 & a, const v_f32_4 & b) noexcept;

	v_f32_4 & operator-=(v_f32_4 & a, const v_f32_4 & b) noexcept;

	v_f32_4 & operator*=(v_f32_4 & a, const v_f32_4 & b) noexcept;

	v_f32_4 & operator/=(v_f32_4 & a, const v_f32_4 & b) noexcept;

	v_f32_4 & operator%=(v_f32_4 & a, const v_f32_4 & b) noexcept;

	v_f32_4 & operator&=(v_f32_4 & a, const v_f32_4 & b) noexcept;

	v_f32_4 & operator|=(v_f32_4 & a, const v_f32_4 & b) noexcept;

	v_f32_4 & operator^=(v_f32_4 & a, const v_f32_4 & b) noexcept;

	v_f32_4 & operator<<=(v_f32_4 & a, const v_f32_4::VectorBoolType & b) noexcept;

	v_f32_4 & operator>>=(v_f32_4 & a, const v_f32_4::VectorBoolType & b) noexcept;

	//}
#pragma endregion

#pragma region swizzle()
	//{ swizzle()

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
	// generic swizzle
	inline const v_f32_4 swizzle(const v_f32_4 & v) noexcept
	{
		static_assert(X < v_f32_4::SIZE, "Index is out of range");
		static_assert(Y < v_f32_4::SIZE, "Index is out of range");
		static_assert(Z < v_f32_4::SIZE, "Index is out of range");
		static_assert(W < v_f32_4::SIZE, "Index is out of range");
#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
		// seems to create a vex instructions in SSE gen mode
		return _mm_permute_ps(v.components, _MM_SHUFFLE(W, Z, Y, X));
#else
		return _mm_shuffle_ps(v.components, v.components, _MM_SHUFFLE(W, Z, Y, X));
#endif
	}

	template <>
	// passthrough swizzle
	const v_f32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & v) noexcept;

	template <>
	// special unpack low-values swizzle
	const v_f32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const v_f32_4 & v) noexcept;

	template <>
	// special unpack XZ-values swizzle
	const v_f32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(const v_f32_4 & v) noexcept;

	template <>
	// special move low-values swizzle
	const v_f32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_f32_4 & v) noexcept;

	template <>
	// special unpack YW-values swizzle
	const v_f32_4 swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(const v_f32_4 & v) noexcept;

	template <>
	// special unpack high-values swizzle
	const v_f32_4 swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const v_f32_4 & v) noexcept;

	template <>
	// special move high-values swizzle
	const v_f32_4 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & v) noexcept;

	//}
#pragma endregion

#pragma region blend()
	//{ blend()

	template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
	// generic blend of two vectors
	inline const v_f32_4 blend(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return _mm_shuffle_ps(
			// a.x, a.y, b.x, b.y
			_mm_movelh_ps(a.components, b.components),
			// a.z, a.w, b.z, b.w
			_mm_movehl_ps(b.components, a.components),
			_MM_SHUFFLE(
				SelectW ? VectorIndices::W : VectorIndices::Y, // SelectW ? b.w : a.w
				SelectZ ? VectorIndices::Z : VectorIndices::X, // SelectZ ? b.z : a.z
				SelectY ? VectorIndices::W : VectorIndices::Y, // SelectY ? b.y : a.y
				SelectX ? VectorIndices::Z : VectorIndices::X // SelectX ? b.x : a.x
			)
		);
#else
		// blend is a more general form of shuffle for interleaving same-index components of two vectors
		// and has a better throughput!
		return _mm_blend_ps(a.components, b.components, BLEND_MASK(SelectX, SelectY, SelectZ, SelectW));
#endif
	}

	template <>
	// passtrough blend
	const v_f32_4 blend<false, false, false, false>(const v_f32_4 & a, const v_f32_4 &) noexcept;

	template <>
	// passthrough blend other
	const v_f32_4 blend<true, true, true, true>(const v_f32_4 &, const v_f32_4 & b) noexcept;

	template <>
	// blend using shuffle
	const v_f32_4 blend<false, false, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	// move x(a) into b
	const v_f32_4 blend<false, true, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	// move x(b) into a
	const v_f32_4 blend<true, false, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	//}
#pragma endregion

#pragma region swizzledBlend()
	//{ swizzledBlend()

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
	// generic swizzled blend
	inline const v_f32_4 swizzledBlend(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		return blend<SelectX, SelectY, SelectZ, SelectW>(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b));
	}

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, false, false, false, false>(const v_f32_4 & a, const v_f32_4 &) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, true, true, true, true>(const v_f32_4 & a, const v_f32_4 &) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, true, false, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W, false, true, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false, true, false, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true, false, true, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false, false, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true, true, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false, true, false, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true, false, true, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true, true, false, false>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false, false, true, true>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
	// Special case swizzled blend of two vectors, in which only the first
	// two components of this vector and the last two components of the
	// other vector are taken for blending after swizzling.
	inline const v_f32_4 swizzledBlend_2x2(const v_f32_4 & a, const v_f32_4 & b) noexcept
	{
		static_assert(X < v_f32_4::SIZE, "Index is out of range");
		static_assert(Y < v_f32_4::SIZE, "Index is out of range");
		static_assert(Z < v_f32_4::SIZE, "Index is out of range");
		static_assert(W < v_f32_4::SIZE, "Index is out of range");
		return _mm_shuffle_ps(a.components, b.components, _MM_SHUFFLE(W, Z, Y, X));
	}

	template <>
	const v_f32_4 swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	template <>
	const v_f32_4 swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	//}
#pragma endregion

#pragma region blendMasked()
	//{ blendMasked()

	const v_f32_4 blendMasked(const v_f32_4 & onBitNotSet, const v_f32_4 & onBitSet, const v_f32_4 & mask) noexcept;

	const v_f32_4 blendMasked(const v_f32_4 & onBitNotSet, const v_f32_4 & onBitSet, const v_f32_4::VectorBoolType & mask) noexcept;

	//}
#pragma endregion

#pragma region select()
	//{ select()

	const v_f32_4 select(const bool mask, const v_f32_4 & onTrue, const v_f32_4 & onFalse) noexcept;

	const v_f32_4 select(const v_f32_4::VectorBoolType & mask, const v_f32_4 & onTrue, const v_f32_4 & onFalse) noexcept;

	const v_f32_4 select(const v_f32_4 & mask, const v_f32_4 & onTrue, const v_f32_4 & onFalse) noexcept;

	//}
#pragma endregion

#pragma region swizzledBlendMasked()
	//{ swizzledBlendMasked()

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
	inline const v_f32_4 swizzledBlendMasked(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4 & mask) noexcept
	{
		return blendMasked(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b), mask);
	}

	template <>
	const v_f32_4 swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4 & mask) noexcept;

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
	inline const v_f32_4 swizzledBlendMasked(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4::VectorBoolType & mask) noexcept
	{
		return swizzledBlendMasked<X, Y, Z, W>(a, b, _mm_castsi128_ps(mask));
	}

	template <>
	const v_f32_4 swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4::VectorBoolType & mask) noexcept;

	//}
#pragma endregion

#pragma region setComponent()
	//{ setComponent()

	template <ASizeT Index>
	void setComponent(v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	template <>
	void setComponent<VectorIndices::X>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	template <>
	void setComponent<VectorIndices::Y>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	template <>
	void setComponent<VectorIndices::Z>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	template <>
	void setComponent<VectorIndices::W>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	//}
#pragma endregion

#pragma region replaceComponent()
	//{ replaceComponent()

	template <ASizeT Index>
	const v_f32_4 replaceComponent(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	template <>
	const v_f32_4 replaceComponent<VectorIndices::X>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	template <>
	const v_f32_4 replaceComponent<VectorIndices::Y>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	template <>
	const v_f32_4 replaceComponent<VectorIndices::Z>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	template <>
	const v_f32_4 replaceComponent<VectorIndices::W>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	//}
#pragma endregion

#pragma region component()
	//{ component()

	template <ASizeT Index>
	const v_f32_4::ValueType component(const v_f32_4 & v) noexcept;

	template <>
	const v_f32_4::ValueType component<VectorIndices::X>(const v_f32_4 & v) noexcept;

	template <>
	const v_f32_4::ValueType component<VectorIndices::Y>(const v_f32_4 & v) noexcept;

	template <>
	const v_f32_4::ValueType component<VectorIndices::Z>(const v_f32_4 & v) noexcept;

	template <>
	const v_f32_4::ValueType component<VectorIndices::W>(const v_f32_4 & v) noexcept;

	//}
#pragma endregion

#pragma region _()
	//{ _()

	const v_f32_4::ValueType x(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType y(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType z(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType w(const v_f32_4 & v) noexcept;

	//}
#pragma endregion

#pragma region replace_()
	//{ replace_()

	const v_f32_4 replaceX(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	const v_f32_4 replaceY(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	const v_f32_4 replaceZ(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	const v_f32_4 replaceW(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

	//}
#pragma endregion

#pragma region min()
	//{ min()

	const v_f32_4 min(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 minv(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType min(const v_f32_4 & v) noexcept;

	const v_f32_4 min3v(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType min3(const v_f32_4 & v) noexcept;

	//}
#pragma endregion

#pragma region max()
	//{ min()

	const v_f32_4 max(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 maxv(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType max(const v_f32_4 & v) noexcept;

	const v_f32_4 max3v(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType max3(const v_f32_4 & v) noexcept;

	//}
#pragma endregion

#pragma region horizontalSum()
	//{ horizontalSum()

	const v_f32_4 horizontalSumv(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType horizontalSum(const v_f32_4 & v) noexcept;

	const v_f32_4 horizontalSum3v(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType horizontalSum3(const v_f32_4 & v) noexcept;

	//}
#pragma endregion

#pragma region dot()
	//{ dot()

	const v_f32_4 dotv(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::ValueType dot(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 dot3v(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::ValueType dot3(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	//}
#pragma endregion

#pragma region length()
	//{ length()

	const v_f32_4 lengthv(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType length(const v_f32_4 & v) noexcept;

	const v_f32_4 rlengthv(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType rlength(const v_f32_4 & v) noexcept;

	const v_f32_4 length3v(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType length3(const v_f32_4 & v) noexcept;

	const v_f32_4 rlength3v(const v_f32_4 & v) noexcept;

	const v_f32_4::ValueType rlength3(const v_f32_4 & v) noexcept;

	//}
#pragma endregion

#pragma region normalize()
	//{ normalize()

	const v_f32_4 normalize(const v_f32_4 & v) noexcept;

	const v_f32_4 normalize3(const v_f32_4 & v) noexcept;

	//}
#pragma endregion

#pragma region distance()
	//{ distance()

	const v_f32_4 distancev(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::ValueType distance(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 distance3v(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::ValueType distance3(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	//}
#pragma endregion

#pragma region reflect()
	//{ reflect()

	const v_f32_4 reflect(const v_f32_4 & incident, const v_f32_4 & normal) noexcept;

	const v_f32_4 reflect(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & NdotI) noexcept;

	//}
#pragma endregion

#pragma region refract()
	//{ refract()

	const v_f32_4 refractEta(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & eta) noexcept;

	const v_f32_4 refractEta(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & NdotI, const v_f32_4 & eta) noexcept;

	const v_f32_4 refract(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & eta) noexcept;

	const v_f32_4 refract(const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & NdotI, const v_f32_4 & eta) noexcept;

	const v_f32_4 refract(const v_f32_4::VectorBoolType & cullingOrientation, const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & eta) noexcept;

	const v_f32_4 refract(const v_f32_4::VectorBoolType & cullingOrientation, const v_f32_4 & incident, const v_f32_4 & normal, const v_f32_4 & NdotI, const v_f32_4 & eta) noexcept;

	//}
#pragma endregion

#pragma region log()
	//{ log()

	const v_f32_4 log(const v_f32_4 & v) noexcept;

	const v_f32_4 log3(const v_f32_4 & v) noexcept;

	//}
#pragma endregion

#pragma region exp()
	//{ exp()

	const v_f32_4 exp(const v_f32_4 & v) noexcept;

	const v_f32_4 exp3(const v_f32_4 & v) noexcept;

	//}
#pragma endregion

#pragma region pow()
	//{ pow()

	const v_f32_4 pow(const v_f32_4 & v, const v_f32_4 & exponent) noexcept;

	const v_f32_4 pow(const v_f32_4 & v, const v_f32_4::ValueType exponent) noexcept;

	const v_f32_4 pow3(const v_f32_4 & v, const v_f32_4 & exponent) noexcept;

	const v_f32_4 pow3(const v_f32_4 & v, const v_f32_4::ValueType exponent) noexcept;

	//}
#pragma endregion

#pragma region mix()
	//{ mix()

	const v_f32_4 mix(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4 & factor) noexcept;

	const v_f32_4 mix(const v_f32_4 & a, const v_f32_4 & b, const v_f32_4::ValueType factor) noexcept;

	//}
#pragma endregion

#pragma region Special vector operations
	//{ Special vector operations

	const v_f32_4 zeroW(const v_f32_4 & v) noexcept;

	const v_f32_4 oneW(const v_f32_4 & v) noexcept;

	// Returns a bitmask of VectorBits indicating components with negative values. A set bit indicates a value
	// less than "0.0".
	const v_f32_4::BoolType isNegative(const v_f32_4 & v) noexcept;

	const v_f32_4::VectorBoolType isNaN(const v_f32_4 & v) noexcept;

	// Checks whether a has Nan or b has NaN values
	const v_f32_4::VectorBoolType isNaN(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	// Alternatively subtracts and adds the components, i.e.:
	// x(out) = x(a) - x(b);
	// y(out) = y(a) + y(b);
	// z(out) = z(a) - z(b);
	// w(out) = w(a) + w(b);
	const v_f32_4 subadd(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 sqrt(const v_f32_4 & v) noexcept;

	const v_f32_4 reciprocal(const v_f32_4 & v) noexcept;

	const v_f32_4 rsqrt(const v_f32_4 & v) noexcept;

	const v_f32_4 ceil(const v_f32_4 & v) noexcept;

	const v_f32_4 floor(const v_f32_4 & v) noexcept;

	const v_f32_4 round(const v_f32_4 & v) noexcept;

	const v_f32_4 fract(const v_f32_4 & v) noexcept;

	const v_f32_4 modulo(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 abs(const v_f32_4 & v) noexcept;

	const v_f32_4 cross3(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4 clamp(const v_f32_4 & v, const v_f32_4 & lower, const v_f32_4 & upper) noexcept;

	const v_f32_4 mapOrthogonal(const v_f32_4 & v, const v_f32_4 & sPlane, const v_f32_4 & tPlane) noexcept;

	const v_f32_4 mapSpherical(const v_f32_4 & v) noexcept;

	const ASizeT argmin3(const v_f32_4 & v) noexcept;

	const ASizeT argmax3(const v_f32_4 & v) noexcept;

	const v_f32_4 axisAlignedNormal3(const v_f32_4 & normal) noexcept;

	//}
#pragma endregion

	std::ostream & operator<< (std::ostream & stream, const v_f32_4 & v) noexcept;

}
