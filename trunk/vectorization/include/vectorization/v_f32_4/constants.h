#pragma once

#include "type.h"

#include "../constants.h"

namespace vectorization
{

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
	const v_f32_4 NegativeZero<v_f32_4>() noexcept;

	template <>
	const v_f32_4 NegativeOne<v_f32_4>() noexcept;

	template <>
	const v_f32_4 NegativeTwo<v_f32_4>() noexcept;

	template <>
	const v_f32_4 NotANumber<v_f32_4>() noexcept;

	template <>
	const v_f32_4 Half<v_f32_4>() noexcept;

	template <>
	const v_f32_4 Epsilon<v_f32_4>() noexcept;

	template <>
	const v_f32_4 SelfOcclusionEpsilon<v_f32_4>() noexcept;

}
