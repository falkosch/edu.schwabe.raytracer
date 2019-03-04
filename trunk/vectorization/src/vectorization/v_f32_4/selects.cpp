#include "vectorization/v_f32_4/selects.h"

#include "vectorization/selects.h"

namespace vectorization
{

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

}
