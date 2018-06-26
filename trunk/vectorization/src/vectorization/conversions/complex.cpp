#include "vectorization/conversions/complex.h"

namespace vectorization
{

	template <>
	const v_i32_4 convert<v_i32_4>(const bool v) noexcept
	{
		return v_i32_4(boolMask<Int_32>(v));
	}

	template <>
	const v_ui32_4 convert<v_ui32_4>(const bool v) noexcept
	{
		return v_ui32_4(convert<v_i32_4>(v).components);
	}



	template <>
	const v_i32_4 convert<v_i32_4>(const bool a, const bool b) noexcept
	{
		return v_i32_4(boolMask<Int_32>(a), boolMask<Int_32>(b));
	}

	template <>
	const v_ui32_4 convert<v_ui32_4>(const bool a, const bool b) noexcept
	{
		return v_ui32_4(convert<v_i32_4>(a, b).components);
	}



	template <>
	const v_ui32_4 convert<v_ui32_4>(const UTVector<VectorSizes::X, UInt_32> & v) noexcept
	{
		auto vPtr = reinterpret_cast<const v_ui32_4::PackedType * const>(&v.components[0]);
		return blend<false, true, true, true>(_mm_load_si128(vPtr), Zero<v_ui32_4>());
	}

	template <>
	const v_i32_4 convert<v_i32_4>(const UTVector<VectorSizes::X, Int_32> & v) noexcept
	{
		auto vPtr = reinterpret_cast<const v_i32_4::PackedType * const>(&v.components[0]);
		return blend<false, true, true, true>(_mm_load_si128(vPtr), Zero<v_i32_4>());
	}

	template <>
	const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::X, Float_32> & v) noexcept
	{
		return _mm_load_ss(&v.components[0]);
	}

	template <>
	const v_ui64_2 convert<v_ui64_2>(const UTVector<VectorSizes::X, UInt_64> & v) noexcept
	{
		auto vPtr = reinterpret_cast<const v_ui64_2::PackedType * const>(&v.components[0]);
		return blend<false, true>(_mm_load_si128(vPtr), Zero<v_ui64_2>());
	}



	template <>
	const v_ui32_4 convert<v_ui32_4>(const UTVector<VectorSizes::Y, UInt_32> & v) noexcept
	{
		auto vPtr = reinterpret_cast<const v_ui32_4::PackedType * const>(&v.components[0]);
		return blend<false, false, true, true>(_mm_load_si128(vPtr), Zero<v_ui32_4>());
	}

	template <>
	const v_i32_4 convert<v_i32_4>(const UTVector<VectorSizes::Y, Int_32> & v) noexcept
	{
		auto vPtr = reinterpret_cast<const v_i32_4::PackedType * const>(&v.components[0]);
		return blend<false, false, true, true>(_mm_load_si128(vPtr), Zero<v_i32_4>());
	}

	template <>
	const v_ui64_2 convert<v_ui64_2>(const UTVector<VectorSizes::Y, Int_32> & v) noexcept
	{
		return v_ui64_2(convert<UInt_64>(x(v)), convert<UInt_64>(y(v)));
	}

	template <>
	const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::Y, Float_32> & v) noexcept
	{
		return xy_zw(_mm_load_ps(&v.components[0]), Zero<v_f32_4>());
	}

	template <>
	const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::Y, UInt_32> & v) noexcept
	{
		return convert<v_f32_4>(convert<v_ui32_4, UInt_32>(v));
	}

	template <>
	const v_ui64_2 convert<v_ui64_2>(const UTVector<VectorSizes::Y, UInt_64> & v) noexcept
	{
		auto vPtr = reinterpret_cast<const v_ui64_2::PackedType * const>(&v.components[0]);
		return v_ui64_2(_mm_load_si128(vPtr));
	}



	template <>
	const v_ui32_4 convert<v_ui32_4>(const UTVector<VectorSizes::Z, UInt_32> & v) noexcept
	{
		auto vPtr = reinterpret_cast<const v_ui32_4::PackedType * const>(&v.components[0]);
		return zeroW(v_ui32_4(_mm_load_si128(vPtr)));
	}

	template <>
	const v_i32_4 convert<v_i32_4>(const UTVector<VectorSizes::Z, Int_32> & v) noexcept
	{
		auto vPtr = reinterpret_cast<const v_i32_4::PackedType * const>(&v.components[0]);
		return zeroW(v_i32_4(_mm_load_si128(vPtr)));
	}

	template <>
	const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::Z, Float_32> & v) noexcept
	{
		return zeroW(_mm_load_ps(&v.components[0]));
	}



	template <>
	const v_ui32_4 convert<v_ui32_4>(const UTVector<VectorSizes::W, UInt_32> & v) noexcept
	{
		auto vPtr = reinterpret_cast<const v_ui32_4::PackedType * const>(&v.components[0]);
		return _mm_load_si128(vPtr);
	}

	template <>
	const v_i32_4 convert<v_i32_4>(const UTVector<VectorSizes::W, Int_32> & v) noexcept
	{
		auto vPtr = reinterpret_cast<const v_i32_4::PackedType * const>(&v.components[0]);
		return _mm_load_si128(vPtr);
	}

	template <>
	const v_f32_4 convert<v_f32_4>(const UTVector<VectorSizes::W, Float_32> & v) noexcept
	{
		return _mm_load_ps(&v.components[0]);
	}



	template <>
	const UTVector<VectorSizes::X, UInt_32> convert<UTVector<VectorSizes::X, UInt_32>>(const v_ui32_4 & v) noexcept
	{
		UTVector<VectorSizes::X, UInt_32> t;
		auto vPtr = reinterpret_cast<v_ui32_4::PackedType * const>(&t.components[0]);
		_mm_store_si128(vPtr, v.components);
		return t;
	}

	template <>
	const UTVector<VectorSizes::Y, UInt_32> convert<UTVector<VectorSizes::Y, UInt_32>>(const v_ui32_4 & v) noexcept
	{
		UTVector<VectorSizes::Y, UInt_32> t;
		auto vPtr = reinterpret_cast<v_ui32_4::PackedType * const>(&t.components[0]);
		_mm_store_si128(vPtr, v.components);
		return t;
	}

	template <>
	const UTVector<VectorSizes::Z, UInt_32> convert<UTVector<VectorSizes::Z, UInt_32>>(const v_ui32_4 & v) noexcept
	{
		UTVector<VectorSizes::Z, UInt_32> t;
		auto vPtr = reinterpret_cast<v_ui32_4::PackedType * const>(&t.components[0]);
		_mm_store_si128(vPtr, v.components);
		return t;
	}

	template <>
	const UTVector<VectorSizes::W, UInt_32> convert<UTVector<VectorSizes::W, UInt_32>>(const v_ui32_4 & v) noexcept
	{
		UTVector<VectorSizes::W, UInt_32> t;
		auto vPtr = reinterpret_cast<v_ui32_4::PackedType * const>(&t.components[0]);
		_mm_store_si128(vPtr, v.components);
		return t;
	}

	template <>
	const v_f32_4 convert<v_f32_4>(const v_ui32_4 & v) noexcept
	{
		return _mm_cvtepu32_ps(v.components);
	}



	template <>
	const UTVector<VectorSizes::X, Int_32> convert<UTVector<VectorSizes::X, Int_32>>(const v_i32_4 & v) noexcept
	{
		UTVector<VectorSizes::X, Int_32> t;
		auto vPtr = reinterpret_cast<v_i32_4::PackedType * const>(&t.components[0]);
		_mm_store_si128(vPtr, v.components);
		return t;
	}

	template <>
	const UTVector<VectorSizes::Y, Int_32> convert<UTVector<VectorSizes::Y, Int_32>>(const v_i32_4 & v) noexcept
	{
		UTVector<VectorSizes::Y, Int_32> t;
		auto vPtr = reinterpret_cast<v_i32_4::PackedType * const>(&t.components[0]);
		_mm_store_si128(vPtr, v.components);
		return t;
	}

	template <>
	const UTVector<VectorSizes::Y, UInt_32> convert<UTVector<VectorSizes::Y, UInt_32>>(const v_i32_4 & v) noexcept
	{
		UTVector<VectorSizes::Y, UInt_32> t;
		auto vPtr = reinterpret_cast<v_i32_4::PackedType * const>(&t.components[0]);
		_mm_store_si128(vPtr, v.components);
		return t;
	}

	template <>
	const UTVector<VectorSizes::Z, Int_32> convert<UTVector<VectorSizes::Z, Int_32>>(const v_i32_4 & v) noexcept
	{
		UTVector<VectorSizes::Z, Int_32> t;
		auto vPtr = reinterpret_cast<v_i32_4::PackedType * const>(&t.components[0]);
		_mm_store_si128(vPtr, v.components);
		return t;
	}

	template <>
	const UTVector<VectorSizes::W, Int_32> convert<UTVector<VectorSizes::W, Int_32>>(const v_i32_4 & v) noexcept
	{
		UTVector<VectorSizes::W, Int_32> t;
		auto vPtr = reinterpret_cast<v_i32_4::PackedType * const>(&t.components[0]);
		_mm_store_si128(vPtr, v.components);
		return t;
	}

	template <>
	const v_f32_4 convert<v_f32_4>(const v_i32_4 & v) noexcept
	{
		return _mm_cvtepi32_ps(v.components);
	}



	template <>
	const UTVector<VectorSizes::X, Float_32> convert<UTVector<VectorSizes::X, Float_32>>(const v_f32_4 & v) noexcept
	{
		UTVector<VectorSizes::X, Float_32> t;
		_mm_store_ps(&t.components[0], v.components);
		return t;
	}

	template <>
	const UTVector<VectorSizes::Y, Float_32> convert<UTVector<VectorSizes::Y, Float_32>>(const v_f32_4 & v) noexcept
	{
		UTVector<VectorSizes::Y, Float_32> t;
		_mm_store_ps(&t.components[0], v.components);
		return t;
	}

	template <>
	const UTVector<VectorSizes::Y, UInt_32> convert<UTVector<VectorSizes::Y, UInt_32>>(const v_f32_4 & v) noexcept
	{
		return convert<UTVector<VectorSizes::Y, UInt_32>>(convert<v_i32_4>(v));
	}

	template <>
	const UTVector<VectorSizes::Z, Float_32> convert<UTVector<VectorSizes::Z, Float_32>>(const v_f32_4 & v) noexcept
	{
		UTVector<VectorSizes::Z, Float_32> t;
		_mm_store_ps(&t.components[0], v.components);
		return t;
	}

	template <>
	const UTVector<VectorSizes::W, Float_32> convert<UTVector<VectorSizes::W, Float_32>>(const v_f32_4 & v) noexcept
	{
		UTVector<VectorSizes::W, Float_32> t;
		_mm_store_ps(&t.components[0], v.components);
		return t;
	}

	template <>
	const v_i32_4 convert<v_i32_4>(const v_f32_4 & v) noexcept
	{
		return _mm_cvtps_epi32(v.components);
	}

	template <>
	const v_f32_4 convert<v_f32_4>(const v_f32_4 & v) noexcept
	{
		return v;
	}

	template <>
	const v_ui64_2 convert<v_ui64_2>(const v_f32_4 & v) noexcept
	{
#ifdef ARCH_X64
		return v_ui64_2(
			convert<UInt_64>(_mm_cvtss_si64(v.components)),
			convert<UInt_64>(_mm_cvtss_si64(yyww(v).components))
		);
#else
		return v_ui64_2(convert<UInt_64>(x(v)), convert<UInt_64>(y(v)));
#endif
	}



	template <>
	const UTVector<VectorSizes::X, UInt_64> convert<UTVector<VectorSizes::X, UInt_64>>(const v_ui64_2 & v) noexcept
	{
		UTVector<VectorSizes::X, UInt_64> t;
		auto vPtr = reinterpret_cast<v_ui64_2::PackedType * const>(&t.components[0]);
		_mm_store_si128(vPtr, v.components);
		return t;
	}

	template <>
	const UTVector<VectorSizes::Y, UInt_64> convert<UTVector<VectorSizes::Y, UInt_64>>(const v_ui64_2 & v) noexcept
	{
		UTVector<VectorSizes::Y, UInt_64> t;
		auto vPtr = reinterpret_cast<v_ui64_2::PackedType * const>(&t.components[0]);
		_mm_store_si128(vPtr, v.components);
		return t;
	}

	template <>
	const UTVector<VectorSizes::Y, Int_32> convert<UTVector<VectorSizes::Y, Int_32>>(const v_ui64_2 & v) noexcept
	{
		return UTVector<VectorSizes::Y, Int_32>(convert<Int_32>(x(v)), convert<Int_32>(y(v)));
	}

	template <>
	const UTVector<VectorSizes::Y, Float_32> convert<UTVector<VectorSizes::Y, Float_32>>(const v_ui64_2 & v) noexcept
	{
		return UTVector<VectorSizes::Y, Float_32>(convert<Float_32>(x(v)), convert<Float_32>(y(v)));
	}

	template <>
	const UTVector<VectorSizes::Z, UInt_64> convert<UTVector<VectorSizes::Z, UInt_64>>(const v_ui64_2 & v) noexcept
	{
		return UTVector<VectorSizes::Z, UInt_64>(x(v), y(v), Zero<UInt_64>());
	}

	template <>
	const UTVector<VectorSizes::W, UInt_64> convert<UTVector<VectorSizes::W, UInt_64>>(const v_ui64_2 & v) noexcept
	{
		return UTVector<VectorSizes::W, UInt_64>(x(v), y(v), Zero<UInt_64>(), Zero<UInt_64>());
	}

	template <>
	const v_f32_4 convert<v_f32_4>(const v_ui64_2 & v) noexcept
	{
		return v_f32_4(convert<Float_32>(x(v)), convert<Float_32>(y(v)));
	}

}
