#pragma once

#include "type.h"

#include <ostream>

namespace vectorization
{

	const v_f32_4 operator-(const v_f32_4 & v) noexcept;

	const v_f32_4 operator~(const v_f32_4 & v) noexcept;

	const v_f32_4::VectorBoolType operator!(const v_f32_4 & v) noexcept;


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


	const v_f32_4::VectorBoolType operator<(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::VectorBoolType operator>(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::VectorBoolType operator<=(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::VectorBoolType operator>=(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::VectorBoolType operator==(const v_f32_4 & a, const v_f32_4 & b) noexcept;

	const v_f32_4::VectorBoolType operator!=(const v_f32_4 & a, const v_f32_4 & b) noexcept;


	std::ostream & operator<<(std::ostream & stream, const v_f32_4 & v);

}
