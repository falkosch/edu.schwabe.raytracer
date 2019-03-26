#include "vectorization/constants/values/Epsilon.h"

#include "vectorization/constants/values/Half.h"
#include "vectorization/constants/values/One.h"

#include <iostream>

namespace vectorization
{

	template <typename T>
	inline const T benchmarkMachineEpsilon() noexcept
	{
		T f = One<T>();
		T e = f;
		while (f + One<T>() != One<T>()) {
			e = f;
			f *= Half<T>();
		}
		std::cout << "Machine epsilon for " << typeid(T).name() << " is " << e << std::endl;
		return e;
	}

	const PackedFloat4_128 Epsilon_PackedFloat4_128 = _mm_set_ps1(benchmarkMachineEpsilon<Float_32>());

	const PackedFloat2_128 Epsilon_PackedFloat2_128 = _mm_set1_pd(benchmarkMachineEpsilon<Float_64>());

	template <>
	const PackedFloat4_128 Epsilon<PackedFloat4_128>() noexcept
	{
		return Epsilon_PackedFloat4_128;
	}

	template <>
	const PackedFloat2_128 Epsilon<PackedFloat2_128>() noexcept
	{
		return Epsilon_PackedFloat2_128;
	}

	template <>
	const Float_32 Epsilon<Float_32>() noexcept
	{
		return _mm_cvtss_f32(Epsilon<PackedFloat4_128>());
	}

	template <>
	const Float_64 Epsilon<Float_64>() noexcept
	{
		return _mm_cvtsd_f64(Epsilon<PackedFloat2_128>());
	}

}
