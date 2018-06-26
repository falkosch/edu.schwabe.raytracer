#pragma once

#include "../architecture/meta_types.h"
#include "../constants/values/Zero.h"

#include <ostream>

namespace vectorization
{

	// Type

	template <ASizeT Size, typename T>
	struct UTVector;



	// Blends

	template <bool SelectX, bool SelectY, typename T>
	// generic blend
	inline const UTVector<VectorSizes::Y, T> blend(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept;

	template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW, typename T>
	// generic blend
	inline const UTVector<VectorSizes::W, T> blend(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;

	template <ASizeT Size, typename T>
	// variadic masked blend
	inline const UTVector<Size, T> blendMasked(const UTVector<Size, T> & a, const UTVector<Size, T> & b, const typename UTVector<Size, T>::VectorBoolType & mask) noexcept;



	// Constants



	// Conversions



	// Operators

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator~(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator!(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator-(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator+(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator+(const UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator-(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator-(const UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator*(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator*(const UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator%(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator%(const UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator/(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator/(const UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator<<(const UTVector<Size, T> & a, const typename UTVector<Size, T>::VectorBoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator<<(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator>>(const UTVector<Size, T> & a, const typename UTVector<Size, T>::VectorBoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator>>(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator&(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator&(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator|(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator|(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator^(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> operator^(const UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator==(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator==(const UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator!=(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator!=(const UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator<(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator<(const UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator<=(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator<=(const UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator>(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator>(const UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator>=(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::VectorBoolType operator>=(const UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator+=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator+=(UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator-=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator-=(UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator*=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator*=(UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator/=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator/=(UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator%=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator%=(UTVector<Size, T> & a, const T & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator&=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator&=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator|=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator|=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator^=(UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator^=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator<<=(UTVector<Size, T> & a, const typename UTVector<Size, T>::VectorBoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline  UTVector<Size, T> & operator<<=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator>>=(UTVector<Size, T> & a, const typename UTVector<Size, T>::VectorBoolType & b) noexcept;

	template <ASizeT Size, typename T>
	inline UTVector<Size, T> & operator>>=(UTVector<Size, T> & a, const typename UTVector<Size, T>::BoolType & b) noexcept;

	template <ASizeT Size, typename T>
	std::ostream & operator<< (std::ostream & stream, const UTVector<Size, T> & v) noexcept;



	// Swizzles

	template <ASizeT X, ASizeT Y, typename T>
	// generic swizzle
	inline const UTVector<VectorSizes::Y, T> swizzle(const UTVector<VectorSizes::Y, T> & v) noexcept;

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, typename T>
	// generic swizzle
	inline const UTVector<VectorSizes::W, T> swizzle(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> xx(const UTVector<VectorSizes::Y, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> yy(const UTVector<VectorSizes::Y, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xxxx(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yyyy(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zzzz(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> wwww(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xxxz(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xxyy(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xxzz(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xyxy(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xzzz(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yxxy(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yxwz(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yzxw(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yzwx(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zxyw(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zzyy(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zzww(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zwxy(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zwzw(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> wxyz(const UTVector<VectorSizes::W, T> & v) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> wzyx(const UTVector<VectorSizes::W, T> & v) noexcept;



	// Swizzled blends

	template <ASizeT X, ASizeT Y, bool SelectX, bool SelectY, typename T>
	// generic swizzled blend
	inline const UTVector<VectorSizes::Y, T> swizzledBlend(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept;

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, bool SelectX, bool SelectY, bool SelectZ, bool SelectW, typename T>
	// generic swizzled blend
	inline const UTVector<VectorSizes::W, T> swizzledBlend(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;

	template <ASizeT X, ASizeT Y, typename T>
	// Special case swizzled blend of two vectors, in which only the first
	// component of this vector and the last component of the other vector
	// is taken for blending after swizzling.
	inline const UTVector<VectorSizes::Y, T> swizzledBlend_1x1(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept;

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, typename T>
	// Special case swizzled blend of two vectors, in which only the first
	// two components of this vector and the last two components of the
	// other vector are taken for blending after swizzling.
	inline const UTVector<VectorSizes::W, T> swizzledBlend_2x2(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;

	template <ASizeT X, ASizeT Y, typename T>
	inline const UTVector<VectorSizes::Y, T> swizzledBlendMasked(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::Y, T> & b, const typename UTVector<VectorSizes::Y, T>::VectorBoolType & mask) noexcept;

	template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, typename T>
	inline const UTVector<VectorSizes::W, T> swizzledBlendMasked(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b, const typename UTVector<VectorSizes::W, T>::VectorBoolType & mask) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> x_x(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> x_y(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> y_x(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::Y, T> y_y(const UTVector<VectorSizes::Y, T> & a, const UTVector<VectorSizes::Y, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xx_xx(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xx_yy(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xy_xy(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xy_zw(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> xz_xz(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yy_yy(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> yw_yw(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zz_zz(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> zw_zw(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;

	template <typename T>
	inline const UTVector<VectorSizes::W, T> ww_ww(const UTVector<VectorSizes::W, T> & a, const UTVector<VectorSizes::W, T> & b) noexcept;



	// Accessors

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

	template <ASizeT Index, ASizeT Size, typename T>
	inline void setComponent(UTVector<Size, T> & v, const T & s) noexcept;

	template <ASizeT Index, ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceComponent(const UTVector<Size, T> & v, const T & s) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceX(const UTVector<Size, T> & v, const T & s) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceY(const UTVector<Size, T> & v, const T & s) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceZ(const UTVector<Size, T> & v, const T & s) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> replaceW(const UTVector<Size, T> & v, const T & s) noexcept;



	// Functions

	template <ASizeT Size, typename T>
	inline const typename UTVector<Size, T>::BoolType isNegative(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const T dot(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> dotv(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const T dot3(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> dot3v(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const T length(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> lengthv(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const T length3(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> length3v(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const T distance(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const T distance3(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> min(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const T min(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const T min3(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> max(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const T max(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const T max3(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const T horizontalSum(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const T horizontalSum3(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> floor(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> fract(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> exp(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> exp3(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> log(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> log3(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> clamp(const UTVector<Size, T> & v, const UTVector<Size, T> & l, const UTVector<Size, T> & m) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> clamp(const UTVector<Size, T> & v, const T & l, const T & m) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> abs(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> mix(const UTVector<Size, T> & a, const UTVector<Size, T> & b, const UTVector<Size, T> & factor) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> mix(const UTVector<Size, T> & a, const UTVector<Size, T> & b, const T & factor) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> cross3(const UTVector<Size, T> & a, const UTVector<Size, T> & b) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> normalize(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> normalize3(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> reciprocal(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> pow(const UTVector<Size, T> & v, const UTVector<Size, T> & exponent) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> pow(const UTVector<Size, T> & v, const T & exponent) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> pow3(const UTVector<Size, T> & v, const UTVector<Size, T> & exponent) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> pow3(const UTVector<Size, T> & v, const T & exponent) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> zeroW(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const UTVector<Size, T> oneW(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const bool anyTrue(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const bool anyTrue3(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const bool anyFalse(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const bool anyFalse3(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const bool allTrue(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const bool allTrue3(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const bool allFalse(const UTVector<Size, T> & v) noexcept;

	template <ASizeT Size, typename T>
	inline const bool allFalse3(const UTVector<Size, T> & v) noexcept;

}
