#include "vectorization/v_f32_4/constants.h"

#include <cassert>

namespace vectorization
{

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
	const v_f32_4 NegativeZero<v_f32_4>() noexcept
	{
		return NegativeZero<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 NegativeOne<v_f32_4>() noexcept
	{
		return NegativeOne<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 NegativeTwo<v_f32_4>() noexcept
	{
		return NegativeTwo<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 Infinity<v_f32_4>() noexcept
	{
		return Infinity<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 NegativeInfinity<v_f32_4>() noexcept
	{
		return NegativeInfinity<v_f32_4::PackedType>();
	}

	template <>
	const v_f32_4 NotANumber<v_f32_4>() noexcept
	{
		return NotANumber<v_f32_4::PackedType>();
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

}
