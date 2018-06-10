#include "vectorization/functions.h"

#include <cmath>

namespace vectorization
{

#pragma region min()
	//{ min()

	const bool min(const bool a, const bool b) noexcept
	{
		return a & b;
	}

	const long min(const long a, const long b) noexcept
	{
		return select(a < b, a, b);
	}

	const unsigned long min(const unsigned long a, const unsigned long b) noexcept
	{
		return select(a < b, a, b);
	}

	const Int_8 min(const Int_8 a, const Int_8 b) noexcept
	{
		return select(a < b, a, b);
	}

	const UInt_8 min(const UInt_8 a, const UInt_8 b) noexcept
	{
		return select(a < b, a, b);
	}

	const Int_16 min(const Int_16 a, const Int_16 b) noexcept
	{
		return select(a < b, a, b);
	}

	const UInt_16 min(const UInt_16 a, const UInt_16 b) noexcept
	{
		return select(a < b, a, b);
	}

	const Int_32 min(const Int_32 a, const Int_32 b) noexcept
	{
		return select(a < b, a, b);
	}

	const UInt_32 min(const UInt_32 a, const UInt_32 b) noexcept
	{
		return select(a < b, a, b);
	}

	const Int_64 min(const Int_64 a, const Int_64 b) noexcept
	{
		return select(a < b, a, b);
	}

	const UInt_64 min(const UInt_64 a, const UInt_64 b) noexcept
	{
		return select(a < b, a, b);
	}

	const Float_32 min(const Float_32 a, const Float_32 b) noexcept
	{
		return _mm_cvtss_f32(_mm_min_ss(_mm_set_ss(a), _mm_set_ss(b)));
	}

	const Float_64 min(const Float_64 a, const Float_64 b) noexcept
	{
		return _mm_cvtsd_f64(_mm_min_sd(_mm_set_sd(a), _mm_set_sd(b)));
	}

	const PackedFloat4_128 min(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept
	{
		return _mm_min_ps(a, b);
	}

	const PackedFloat2_128 min(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept
	{
		return _mm_min_pd(a, b);
	}

	//}
#pragma endregion

#pragma region max()
	//{ max()

	const bool max(const bool a, const bool b) noexcept
	{
		return a | b;
	}

	const long max(const long a, const long b) noexcept
	{
		return select(a > b, a, b);
	}

	const unsigned long max(const unsigned long a, const unsigned long b) noexcept
	{
		return select(a > b, a, b);
	}

	const Int_8 max(const Int_8 a, const Int_8 b) noexcept
	{
		return select(a > b, a, b);
	}

	const UInt_8 max(const UInt_8 a, const UInt_8 b) noexcept
	{
		return select(a > b, a, b);
	}

	const Int_16 max(const Int_16 a, const Int_16 b) noexcept
	{
		return select(a > b, a, b);
	}

	const UInt_16 max(const UInt_16 a, const UInt_16 b) noexcept
	{
		return select(a > b, a, b);
	}

	const Int_32 max(const Int_32 a, const Int_32 b) noexcept
	{
		return select(a > b, a, b);
	}

	const UInt_32 max(const UInt_32 a, const UInt_32 b) noexcept
	{
		return select(a > b, a, b);
	}

	const Int_64 max(const Int_64 a, const Int_64 b) noexcept
	{
		return select(a > b, a, b);
	}

	const UInt_64 max(const UInt_64 a, const UInt_64 b) noexcept
	{
		return select(a > b, a, b);
	}

	const Float_32 max(const Float_32 a, const Float_32 b) noexcept
	{
		return _mm_cvtss_f32(_mm_max_ss(_mm_set_ss(a), _mm_set_ss(b)));
	}

	const Float_64 max(const Float_64 a, const Float_64 b) noexcept
	{
		return _mm_cvtsd_f64(_mm_max_sd(_mm_set_sd(a), _mm_set_sd(b)));
	}

	const PackedFloat4_128 max(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept
	{
		return _mm_max_ps(a, b);
	}

	const PackedFloat2_128 max(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept
	{
		return _mm_max_pd(a, b);
	}

	//}
#pragma endregion

#pragma region clamp()
	//{ clamp()

	const bool clamp(const bool v, const bool lower, const bool upper) noexcept
	{
		return max(lower, min(v, upper));
	}

	const long clamp(const long v, const long lower, const long upper) noexcept
	{
		return max(lower, min(v, upper));
	}

	const unsigned long clamp(const unsigned long v, const unsigned long lower, const unsigned long upper) noexcept
	{
		return max(lower, min(v, upper));
	}

	const Int_8 clamp(const Int_8 v, const Int_8 lower, const Int_8 upper) noexcept
	{
		return max(lower, min(v, upper));
	}

	const UInt_8 clamp(const UInt_8 v, const UInt_8 lower, const UInt_8 upper) noexcept
	{
		return max(lower, min(v, upper));
	}

	const Int_16 clamp(const Int_16 v, const Int_16 lower, const Int_16 upper) noexcept
	{
		return max(lower, min(v, upper));
	}

	const UInt_16 clamp(const UInt_16 v, const UInt_16 lower, const UInt_16 upper) noexcept
	{
		return max(lower, min(v, upper));
	}

	const Int_32 clamp(const Int_32 v, const Int_32 lower, const Int_32 upper) noexcept
	{
		return max(lower, min(v, upper));
	}

	const UInt_32 clamp(const UInt_32 v, const UInt_32 lower, const UInt_32 upper) noexcept
	{
		return max(lower, min(v, upper));
	}

	const Int_64 clamp(const Int_64 v, const Int_64 lower, const Int_64 upper) noexcept
	{
		return max(lower, min(v, upper));
	}

	const UInt_64 clamp(const UInt_64 v, const UInt_64 lower, const UInt_64 upper) noexcept
	{
		return max(lower, min(v, upper));
	}

	const Float_32 clamp(const Float_32 v, const Float_32 lower, const Float_32 upper) noexcept
	{
		return _mm_cvtss_f32(_mm_max_ss(_mm_set_ss(lower), _mm_min_ss(_mm_set_ss(v), _mm_set_ss(upper))));
	}

	const Float_64 clamp(const Float_64 v, const Float_64 lower, const Float_64 upper) noexcept
	{
		return _mm_cvtsd_f64(_mm_max_sd(_mm_set_sd(lower), _mm_min_sd(_mm_set_sd(v), _mm_set_sd(upper))));
	}

	const PackedFloat4_128 clamp(const PackedFloat4_128 & v, const PackedFloat4_128 & lower, const PackedFloat4_128 & upper) noexcept
	{
		return _mm_max_ps(lower, _mm_min_ps(v, upper));
	}

	const PackedFloat2_128 clamp(const PackedFloat2_128 & v, const PackedFloat2_128 & lower, const PackedFloat2_128 & upper) noexcept
	{
		return _mm_max_pd(lower, _mm_min_pd(v, upper));
	}

	//}
#pragma endregion

#pragma region sign()
	//{ sign()

	const long sign(const long v) noexcept
	{
		return static_cast<long>((v > Zero<long>()) - (v < Zero<long>()));
	}

	const Int_8 sign(const Int_8 v) noexcept
	{
		return static_cast<Int_8>((v > Zero<Int_8>()) - (v < Zero<Int_8>()));
	}

	const Int_16 sign(const Int_16 v) noexcept
	{
		return static_cast<Int_8>((v > Zero<Int_16>()) - (v < Zero<Int_16>()));
	}

	const Int_32 sign(const Int_32 v) noexcept
	{
		return static_cast<Int_32>((v > Zero<Int_32>()) - (v < Zero<Int_32>()));
	}

	const Int_64 sign(const Int_64 v) noexcept
	{
		return static_cast<Int_64>((v > Zero<Int_64>()) - (v < Zero<Int_64>()));
	}

	const PackedFloat4_128 sign(const PackedFloat4_128 & v) noexcept
	{
		return blendMasked(
			copysign(v),
			Zero<PackedFloat4_128>(),
			_mm_cmpeq_ps(
				Zero<PackedFloat4_128>(),
				_mm_andnot_ps(NegZero<PackedFloat4_128>(), v)
			)
		);
	}

	const PackedFloat2_128 sign(const PackedFloat2_128 & v) noexcept
	{
		return blendMasked(
			copysign(v),
			Zero<PackedFloat2_128>(),
			_mm_cmpeq_pd(
				Zero<PackedFloat2_128>(),
				_mm_andnot_pd(NegZero<PackedFloat2_128>(), v)
			)
		);
	}

	const Float_32 sign(const Float_32 v) noexcept
	{
		const PackedFloat4_128 vv = _mm_set_ss(v);
		return _mm_cvtss_f32(
			blendMasked(
				copysign(vv),
				Zero<PackedFloat4_128>(),
				_mm_cmpeq_ss(Zero<PackedFloat4_128>(),
					_mm_andnot_ps(NegZero<PackedFloat4_128>(), vv)
				)
			)
		);
	}

	const Float_64 sign(const Float_64 v) noexcept
	{
		const PackedFloat2_128 vv = _mm_set_sd(v);
		return _mm_cvtsd_f64(
			blendMasked(
				copysign(vv),
				Zero<PackedFloat2_128>(),
				_mm_cmpeq_sd(
					Zero<PackedFloat2_128>(),
					_mm_andnot_pd(NegZero<PackedFloat2_128>(), vv)
				)
			)
		);
	}

	//}
#pragma endregion

#pragma region copysign()
	//{ copysign()

	const PackedFloat4_128 copysign(const PackedFloat4_128 & v) noexcept
	{
		return _mm_or_ps(One<PackedFloat4_128>(), _mm_and_ps(NegZero<PackedFloat4_128>(), v));
	}

	const PackedFloat2_128 copysign(const PackedFloat2_128 & v) noexcept
	{
		return _mm_or_pd(One<PackedFloat2_128>(), _mm_and_pd(NegZero<PackedFloat2_128>(), v));
	}

	const Float_32 copysign(const Float_32 v) noexcept
	{
		return _mm_cvtss_f32(copysign(_mm_set_ss(v)));
	}

	const Float_64 copysign(const Float_64 v) noexcept
	{
		return _mm_cvtsd_f64(copysign(_mm_set_sd(v)));
	}

	//}
#pragma endregion

#pragma region abs()
	//{ abs()

	const long abs(const long v) noexcept
	{
		return std::labs(v);
	}

	const Int_8 abs(const Int_8 v) noexcept
	{
		return select(v < Zero<Int_8>(), static_cast<Int_8>(-v), v);
	}

	const Int_16 abs(const Int_16 v) noexcept
	{
		return select(v < Zero<Int_16>(), static_cast<Int_16>(-v), v);
	}

	const Int_32 abs(const Int_32 v) noexcept
	{
		return std::abs(v);
	}

	const Int_64 abs(const Int_64 v) noexcept
	{
		return std::llabs(v);
	}

	const Float_32 abs(const Float_32 v) noexcept
	{
		return _mm_cvtss_f32(abs(_mm_set_ss(v)));
	}

	const Float_64 abs(const Float_64 v) noexcept
	{
		return _mm_cvtsd_f64(abs(_mm_set_sd(v)));
	}

	const PackedFloat4_128 abs(const PackedFloat4_128 & v) noexcept
	{
		return _mm_andnot_ps(NegZero<PackedFloat4_128>(), v);
	}

	const PackedFloat2_128 abs(const PackedFloat2_128 & v) noexcept
	{
		return _mm_andnot_pd(NegZero<PackedFloat2_128>(), v);
	}

	//}
#pragma endregion

#pragma region isNaN()
	//{ isNaN()

	const bool isNaN(const Float_32 v) noexcept
	{
		const PackedFloat4_128 t = _mm_set_ss(v);
		return !!_mm_cvtsi128_si32(_mm_castps_si128(_mm_cmpunord_ss(t, t)));
	}

	const bool isNaN(const Float_32 a, const Float_32 b) noexcept
	{
		return !!_mm_cvtsi128_si32(
			_mm_castps_si128(
				_mm_cmpunord_ss(_mm_set_ss(a), _mm_set_ss(b))
			)
		);
	}

	const bool isNaN(const Float_64 v) noexcept
	{
		const PackedFloat2_128 t = _mm_set_sd(v);
		return !!_mm_cvtsi128_si32(_mm_castpd_si128(_mm_cmpunord_sd(t, t)));
	}

	const bool isNaN(const Float_64 a, const Float_64 b) noexcept
	{
		return !!_mm_cvtsi128_si32(
			_mm_castpd_si128(
				_mm_cmpunord_sd(_mm_set_sd(a), _mm_set_sd(b))
			)
		);
	}

	const PackedFloat4_128 isNaN(const PackedFloat4_128 & v) noexcept
	{
		return _mm_cmpunord_ps(v, v);
	}

	const PackedFloat4_128 isNaN(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept
	{
		return _mm_cmpunord_ps(a, b);
	}

	const PackedFloat2_128 isNaN(const PackedFloat2_128 & v) noexcept
	{
		return _mm_cmpunord_pd(v, v);
	}

	const PackedFloat2_128 isNaN(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept
	{
		return _mm_cmpunord_pd(a, b);
	}

	//}
#pragma endregion

#pragma region mix()
	//{ mix()

	const PackedFloat4_128 mix(const PackedFloat4_128 & x, const PackedFloat4_128 & y, const Float_32 factor) noexcept
	{
		return mix(x, y, _mm_set_ps1(factor));
	}

	const PackedFloat4_128 mix(const PackedFloat4_128 & x, const PackedFloat4_128 & y, const PackedFloat4_128 & factor) noexcept
	{
		return _mm_add_ps(x, _mm_mul_ps(_mm_sub_ps(y, x), factor));
	}

	const PackedFloat2_128 mix(const PackedFloat2_128 & x, const PackedFloat2_128 & y, const Float_64 factor) noexcept
	{
		return mix(x, y, _mm_set1_pd(factor));
	}

	const PackedFloat2_128 mix(const PackedFloat2_128 & x, const PackedFloat2_128 & y, const PackedFloat2_128 & factor) noexcept
	{
		return _mm_add_pd(x, _mm_mul_pd(_mm_sub_pd(y, x), factor));
	}

	const Float_32 mix(const Float_32 x, const Float_32 y, const Float_32 factor) noexcept
	{
		return x + (y - x) * factor;
	}

	const Float_64 mix(const Float_64 x, const Float_64 y, const Float_64 factor) noexcept
	{
		return x + (y - x) * factor;
	}

	//}
#pragma endregion

#pragma region sqr()
	//{ sqr()

	const Float_32 sqr(const Float_32 v) noexcept
	{
		return v * v;
	}

	const Float_64 sqr(const Float_64 v) noexcept
	{
		return v * v;
	}

	const PackedFloat4_128 sqr(const PackedFloat4_128 & v) noexcept
	{
		return _mm_mul_ps(v, v);
	}

	const PackedFloat2_128 sqr(const PackedFloat2_128 & v) noexcept
	{
		return _mm_mul_pd(v, v);
	}

	//}
#pragma endregion

#pragma region reciprocal()
	//{ reciprocal()

	const Float_32 reciprocal(const Float_32 v) noexcept
	{
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
		// Do two Newton-Raphson steps for y = 1/x
		// y_n+1 = y_n(2 - x * y_n)
		// y_0 = rcp(x)
		// y_1 = y_0(2 - x * y_0)
		// y_2 = y_1(2 - x * y_1)
		const PackedFloat4_128 two = Two<PackedFloat4_128>();
		const PackedFloat4_128 N = _mm_set_ss(v);
		const PackedFloat4_128 x0 = _mm_rcp_ss(N);
		const PackedFloat4_128 x1 = _mm_mul_ss(_mm_sub_ss(two, _mm_mul_ss(N, x0)), x0);
		return _mm_cvtss_f32(_mm_mul_ss(_mm_sub_ss(two, _mm_mul_ss(N, x1)), x1));
#else
		return _mm_cvtss_f32(_mm_rcp_ss(_mm_set_ss(v)));
#endif
#else
		return _mm_cvtss_f32(_mm_div_ss(One<PackedFloat4_128>(), _mm_set_ss(v)));
#endif
	}

	const Float_64 reciprocal(const Float_64 v) noexcept
	{
		// no approximative variants available in SSE or AVX
		return _mm_cvtsd_f64(_mm_div_sd(One<PackedFloat2_128>(), _mm_set_sd(v)));
	}

	const PackedFloat4_128 reciprocal(const PackedFloat4_128 & v) noexcept
	{
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
		// Do two Newton-Raphson steps for y = 1/x
		// y_n+1 = y_n(2 - x * y_n)
		// y_0 = rcp(x)
		// y_1 = y_0(2 - x * y_0)
		// y_2 = y_1(2 - x * y_1)
		const PackedFloat4_128 two = Two<PackedFloat4_128>();
		const PackedFloat4_128 x0 = _mm_rcp_ps(v);
		const PackedFloat4_128 x1 = _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(v, x0)), x0);
		return _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(v, x1)), x1);
#else
		return _mm_rcp_ps(v);
#endif
#else
		return _mm_div_ps(One<PackedFloat4_128>(), v);
#endif
	}

	const PackedFloat2_128 reciprocal(const PackedFloat2_128 & v) noexcept
	{
		// no approximative variants available in SSE or AVX
		return _mm_div_pd(One<PackedFloat2_128>(), v);
	}

	//}
#pragma endregion

#pragma region sqrt()
	//{ sqrt()

	const Float_32 sqrt(const Float_32 v) noexcept
	{
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(v)));
	}

	const Float_64 sqrt(const Float_64 v) noexcept
	{
		return _mm_cvtsd_f64(_mm_sqrt_sd(_mm_set_sd(v), Zero<PackedFloat2_128>()));
	}

	const PackedFloat4_128 sqrt(const PackedFloat4_128 & v) noexcept
	{
		return _mm_sqrt_ps(v);
	}

	const PackedFloat2_128 sqrt(const PackedFloat2_128 & v) noexcept
	{
		return _mm_sqrt_pd(v);
	}

	//}
#pragma endregion

#pragma region rsqrt()
	//{ rsqrt()

	const Float_32 rsqrt(const Float_32 v) noexcept
	{
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
		// Do two Newton-Raphson steps for y = 1/sqrt(x)
		// y_n+1 = y_n*(3/2 - x*y_n*y_n/2)
		// y_0 = rsqrt(x)
		// y_1 = y_0(3/2 - x * y_0 * y_0 / 2)
		// y_2 = y_1(3/2 - x * y_1 * y_1 / 2)
		const PackedFloat4_128 half = Half<PackedFloat4_128>();
		const PackedFloat4_128 oneHalf = OneHalf<PackedFloat4_128>();
		const PackedFloat4_128 N = _mm_set_ss(v);
		const PackedFloat4_128 x0 = _mm_rsqrt_ss(N);
		const PackedFloat4_128 x1 = _mm_mul_ss(_mm_sub_ss(oneHalf, _mm_mul_ss(half, _mm_mul_ss(N, _mm_mul_ss(x0, x0)))), x0);
		return _mm_cvtss_f32(_mm_mul_ss(_mm_sub_ss(oneHalf, _mm_mul_ss(half, _mm_mul_ss(N, _mm_mul_ss(x1, x1)))), x1));
#else
		return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(v)));
#endif
#else
		return reciprocal(sqrt(v));
#endif
	}

	const Float_64 rsqrt(const Float_64 v) noexcept
	{
		return reciprocal(sqrt(v));
	}

	const PackedFloat4_128 rsqrt(const PackedFloat4_128 & v) noexcept
	{
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
		const PackedFloat4_128 half = Half<PackedFloat4_128>();
		const PackedFloat4_128 oneHalf = OneHalf<PackedFloat4_128>();
		const PackedFloat4_128 x0 = _mm_rsqrt_ps(v);
		const PackedFloat4_128 x1 = _mm_mul_ps(_mm_sub_ps(oneHalf, _mm_mul_ps(half, _mm_mul_ps(v, _mm_mul_ps(x0, x0)))), x0);
		return _mm_mul_ps(_mm_sub_ps(oneHalf, _mm_mul_ps(half, _mm_mul_ps(v, _mm_mul_ps(x1, x1)))), x1);
#else
		return _mm_rsqrt_ps(v);
#endif
#else
		return reciprocal(sqrt(v));
#endif
	}

	const PackedFloat2_128 rsqrt(const PackedFloat2_128 & v) noexcept
	{
		return reciprocal(sqrt(v));
	}

	//}
#pragma endregion

#pragma region ceil()
	//{ ceil()

	const Float_32 ceil(const Float_32 v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		// ceil(v) == -floor(-v)
		return -floor(-v);
#else
		return _mm_cvtss_f32(_mm_round_ss(Zero<PackedFloat4_128>(), _mm_set_ss(v), _MM_FROUND_TO_POS_INF));
#endif
	}

	const Float_64 ceil(const Float_64 v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		// ceil(v) == -floor(-v)
		return -floor(-v);
#else
		return _mm_cvtsd_f64(_mm_round_sd(Zero<PackedFloat2_128>(), _mm_set_sd(v), _MM_FROUND_TO_POS_INF));
#endif
	}

	const PackedFloat4_128 ceil(const PackedFloat4_128 & v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		// ceil(v) == -floor(-v)
		const PackedFloat4_128 negMask = NegZero<PackedFloat4_128>();
		return _mm_xor_ps(floor(_mm_xor_ps(v, negMask)), negMask);
#else
		return _mm_round_ps(v, _MM_FROUND_TO_POS_INF);
#endif
	}

	const PackedFloat2_128 ceil(const PackedFloat2_128 & v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		// ceil(v) == -floor(-v)
		const PackedFloat2_128 negMask = NegZero<PackedFloat2_128>();
		return _mm_xor_pd(floor(_mm_xor_pd(v, negMask)), negMask);
#else
		return _mm_round_pd(v, _MM_FROUND_TO_POS_INF);
#endif
	}

	//}
#pragma endregion

#pragma region floor()
	//{ floor()

	const Float_32 floor(const Float_32 v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		// _mm_cvttss_epi32 truncates with round to zero, so for negative v we subtract the sign bit from
		// converted int value to floor it.
		const Int_32 vint = _mm_cvtt_ss2si(_mm_set_ss(v));
		return _mm_cvtss_f32(
			_mm_cvt_si2ss(
				Zero<PackedFloat4_128>(),
				vint - (vint >> (sizeof(Int_32) * CHAR_BIT - 1))
			)
		);
#else
		return _mm_cvtss_f32(_mm_round_ss(Zero<PackedFloat4_128>(), _mm_set_ss(v), _MM_FROUND_TO_NEG_INF));
#endif
	}

	const Float_64 floor(const Float_64 v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		const Int_32 vint = _mm_cvttsd_si32(_mm_set_sd(v));
		return _mm_cvtsd_f64(
			_mm_cvtsi32_sd(
				Zero<PackedFloat2_128>(),
				vint - (vint >> (sizeof(Int_32) * CHAR_BIT - 1))
			)
		);
#else
		return _mm_cvtsd_f64(_mm_round_sd(Zero<PackedFloat2_128>(), _mm_set_sd(v), _MM_FROUND_TO_NEG_INF));
#endif
	}

	const PackedFloat4_128 floor(const PackedFloat4_128 & v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		// _mm_cvttps_epi32 truncates with round to zero, so for negative v we subtract the sign bit from
		// converted int value to floor it.
		return _mm_cvtepi32_ps(
			_mm_sub_epi32(
				_mm_cvttps_epi32(v),
				_mm_srli_epi32(_mm_castps_si128(v), sizeof(Float_32) * CHAR_BIT - 1)
			)
		);
#else
		return _mm_round_ps(v, _MM_FROUND_TO_NEG_INF);
#endif
	}

	const PackedFloat2_128 floor(const PackedFloat2_128 & v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		// TODO : epi32 may be not suitable
		// _mm_cvttpd_epi32 truncates with round to zero, so for negative v we subtract the sign bit from
		// converted int value to floor it.
		// x: sign bit of v.x, y: sign bit of v.y
		const PackedInts_128 signBitShifted = _mm_shuffle_epi32(
			_mm_srli_epi64(_mm_castpd_si128(v), sizeof(Float_64) * CHAR_BIT - 1),
			_MM_SHUFFLE(VectorIndices::Z, VectorIndices::X, VectorIndices::Z, VectorIndices::X)
		);
		return _mm_cvtepi32_pd(_mm_sub_epi32(_mm_cvttpd_epi32(v), signBitShifted));
#else
		return _mm_round_pd(v, _MM_FROUND_TO_NEG_INF);
#endif
	}

	//}
#pragma endregion

#pragma region round()
	//{ round()

	const Float_32 round(const Float_32 v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return floor(v + Half<Float_32>());
#else
		return _mm_cvtss_f32(_mm_round_ss(Zero<PackedFloat4_128>(), _mm_set_ss(v), _MM_FROUND_TO_NEAREST_INT));
#endif
	}

	const Float_64 round(const Float_64 v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return floor(v + Half<Float_64>());
#else
		return _mm_cvtsd_f64(_mm_round_sd(Zero<PackedFloat2_128>(), _mm_set_sd(v), _MM_FROUND_TO_NEAREST_INT));
#endif
	}

	const PackedFloat4_128 round(const PackedFloat4_128 & v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return floor(_mm_add_ps(v, Half<PackedFloat4_128>()));
#else
		return _mm_round_ps(v, _MM_FROUND_TO_NEAREST_INT);
#endif
	}

	const PackedFloat2_128 round(const PackedFloat2_128 & v) noexcept
	{
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_SSE4
		return floor(_mm_add_pd(v, Half<PackedFloat2_128>()));
#else
		return _mm_round_pd(v, _MM_FROUND_TO_NEAREST_INT);
#endif
	}

	//}
#pragma endregion

#pragma region fract()
	//{ fract()

	const Float_32 fract(const Float_32 v) noexcept
	{
		return v - floor(v);
	}

	const Float_64 fract(const Float_64 v) noexcept
	{
		return v - floor(v);
	}

	const PackedFloat4_128 fract(const PackedFloat4_128 & v) noexcept
	{
		return _mm_sub_ps(v, floor(v));
	}

	const PackedFloat2_128 fract(const PackedFloat2_128 & v) noexcept
	{
		return _mm_sub_pd(v, floor(v));
	}

	//}
#pragma endregion

#pragma region divide()
	//{ divide()

	const long divide(const long a, const long b) noexcept
	{
		return a / b;
	}

	const unsigned long divide(const unsigned long a, const unsigned long b) noexcept
	{
		return a / b;
	}

	const Int_8 divide(const Int_8 a, const Int_8 b) noexcept
	{
		return static_cast<Int_8>(a / b);
	}

	const UInt_8 divide(const UInt_8 a, const UInt_8 b) noexcept
	{
		return static_cast<UInt_8>(a / b);
	}

	const Int_16 divide(const Int_16 a, const Int_16 b) noexcept
	{
		return static_cast<Int_16>(a / b);
	}

	const UInt_16 divide(const UInt_16 a, const UInt_16 b) noexcept
	{
		return static_cast<UInt_16>(a / b);
	}

	const Int_32 divide(const Int_32 a, const Int_32 b) noexcept
	{
		return a / b;
	}

	const UInt_32 divide(const UInt_32 a, const UInt_32 b) noexcept
	{
		return a / b;
	}

	const Int_64 divide(const Int_64 a, const Int_64 b) noexcept
	{
		return a / b;
	}

	const UInt_64 divide(const UInt_64 a, const UInt_64 b) noexcept
	{
		return a / b;
	}

	const Float_32 divide(const Float_32 a, const Float_32 b) noexcept
	{
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
		const PackedFloat4_128 two = Two<PackedFloat4_128>();
		const PackedFloat4_128 bs = _mm_set_ss(b);
		const PackedFloat4_128 x0 = _mm_rcp_ss(bs);
		const PackedFloat4_128 x1 = _mm_mul_ss(_mm_sub_ss(two, _mm_mul_ss(bs, x0)), x0);
		return _mm_cvtss_f32(_mm_mul_ss(_mm_set_ss(a), _mm_mul_ss(_mm_sub_ss(two, _mm_mul_ss(bs, x1)), x1)));
#else
		return _mm_cvtss_f32(_mm_mul_ss(_mm_set_ss(a), _mm_rcp_ss(_mm_set_ss(b))));
#endif
#else
		return _mm_cvtss_f32(_mm_div_ss(_mm_set_ss(a), _mm_set_ss(b)));
#endif
	}

	const Float_64 divide(const Float_64 a, const Float_64 b) noexcept
	{
		return a / b;
	}

	const PackedFloat4_128 divide(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept
	{
#ifdef VECTORIZATION_APPROXIMATIONS
#ifdef VECTORIZATION_FINE_APPROXIMATIONS
		const PackedFloat4_128 two = Two<PackedFloat4_128>();
		const PackedFloat4_128 x0 = _mm_rcp_ps(b);
		const PackedFloat4_128 x1 = _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(b, x0)), x0);
		return _mm_mul_ps(a, _mm_mul_ps(_mm_sub_ps(two, _mm_mul_ps(b, x1)), x1));
#else
		return _mm_mul_ps(a, _mm_rcp_ps(b));
#endif
#else
		return _mm_div_ps(a, b);
#endif
	}

	const PackedFloat2_128 divide(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept
	{
		return _mm_div_pd(a, b);
	}

	//}
#pragma endregion

#pragma region modulo()
	//{ modulo()

	const long modulo(const long a, const long b) noexcept
	{
		return a % b;
	}

	const unsigned long modulo(const unsigned long a, const unsigned long b) noexcept
	{
		return a % b;
	}

	const Int_8 modulo(const Int_8 a, const Int_8 b) noexcept
	{
		return static_cast<Int_8>(a % b);
	}

	const UInt_8 modulo(const UInt_8 a, const UInt_8 b) noexcept
	{
		return static_cast<UInt_8>(a % b);
	}

	const Int_16 modulo(const Int_16 a, const Int_16 b) noexcept
	{
		return static_cast<Int_16>(a % b);
	}

	const UInt_16 modulo(const UInt_16 a, const UInt_16 b) noexcept
	{
		return static_cast<UInt_16>(a % b);
	}

	const Int_32 modulo(const Int_32 a, const Int_32 b) noexcept
	{
		return a % b;
	}

	const UInt_32 modulo(const UInt_32 a, const UInt_32 b) noexcept
	{
		return a % b;
	}

	const Int_64 modulo(const Int_64 a, const Int_64 b) noexcept
	{
		return a % b;
	}

	const UInt_64 modulo(const UInt_64 a, const UInt_64 b) noexcept
	{
		return a % b;
	}

	const Float_32 modulo(const Float_32 a, const Float_32 b) noexcept
	{
		return a - b * floor(divide(a, b));
	}

	const Float_64 modulo(const Float_64 a, const Float_64 b) noexcept
	{
		return a - b * floor(divide(a, b));
	}

	const PackedFloat4_128 modulo(const PackedFloat4_128 & a, const PackedFloat4_128 & b) noexcept
	{
		return _mm_sub_ps(a, _mm_mul_ps(b, floor(divide(a, b))));
	}

	const PackedFloat2_128 modulo(const PackedFloat2_128 & a, const PackedFloat2_128 & b) noexcept
	{
		return _mm_sub_pd(a, _mm_mul_pd(b, floor(divide(a, b))));
	}

	//}
#pragma endregion

#pragma region roundDown() for integers
	//{ roundDown() for integers

	const Int_8 roundDown(const Int_8 value, const Int_8 stepSize) noexcept
	{
		return static_cast<Int_8>(value & ~(stepSize - One<Int_8>()));
	}

	const UInt_8 roundDown(const UInt_8 value, const UInt_8 stepSize) noexcept
	{
		return static_cast<UInt_8>(value & ~(stepSize - One<UInt_8>()));
	}

	const Int_16 roundDown(const Int_16 value, const Int_16 stepSize) noexcept
	{
		return static_cast<Int_16>(value & ~(stepSize - One<Int_16>()));
	}

	const UInt_16 roundDown(const UInt_16 value, const UInt_16 stepSize) noexcept
	{
		return static_cast<UInt_16>(value & ~(stepSize - One<UInt_16>()));
	}

	const Int_32 roundDown(const Int_32 value, const Int_32 stepSize) noexcept
	{
		return value & ~(stepSize - One<Int_32>());
	}

	const UInt_32 roundDown(const UInt_32 value, const UInt_32 stepSize) noexcept
	{
		return value & ~(stepSize - One<UInt_32>());
	}

	const Int_64 roundDown(const Int_64 value, const Int_64 stepSize) noexcept
	{
		return value & ~(stepSize - One<Int_64>());
	}

	const UInt_64 roundDown(const UInt_64 value, const UInt_64 stepSize) noexcept
	{
		return value & ~(stepSize - One<UInt_64>());
	}

	//}
#pragma endregion

#pragma region bitpad()
	//{ bitpad()

	const Int_8 bitpad(const Int_8 unpadded, const Int_8 bitPadding) noexcept
	{
		return static_cast<Int_8>((bitPadding + unpadded) & ~bitPadding);
	}

	const UInt_8 bitpad(const UInt_8 unpadded, const UInt_8 bitPadding) noexcept
	{
		return static_cast<UInt_8>((bitPadding + unpadded) & ~bitPadding);
	}

	const Int_16 bitpad(const Int_16 unpadded, const Int_16 bitPadding) noexcept
	{
		return static_cast<Int_16>((bitPadding + unpadded) & ~bitPadding);
	}

	const UInt_16 bitpad(const UInt_16 unpadded, const UInt_16 bitPadding) noexcept
	{
		return static_cast<UInt_16>((bitPadding + unpadded) & ~bitPadding);
	}

	const Int_32 bitpad(const Int_32 unpadded, const Int_32 bitPadding) noexcept
	{
		return (bitPadding + unpadded) & ~bitPadding;
	}

	const UInt_32 bitpad(const UInt_32 unpadded, const UInt_32 bitPadding) noexcept
	{
		return (bitPadding + unpadded) & ~bitPadding;
	}

	const Int_64 bitpad(const Int_64 unpadded, const Int_64 bitPadding) noexcept
	{
		return (bitPadding + unpadded) & ~bitPadding;
	}

	const UInt_64 bitpad(const UInt_64 unpadded, const UInt_64 bitPadding) noexcept
	{
		return (bitPadding + unpadded) & ~bitPadding;
	}

	//}
#pragma endregion

#pragma region modpad()
	//{ modpad()

	const Int_8 modpad(const Int_8 unpadded, const Int_8 modPadding) noexcept
	{
		const Int_32 overPadded = unpadded + modPadding - One<Int_32>();
		return static_cast<Int_8>(overPadded - modulo(
			overPadded,
			static_cast<Int_32>(modPadding)
		));
	}

	const UInt_8 modpad(const UInt_8 unpadded, const UInt_8 modPadding) noexcept
	{
		const Int_32 overPadded = unpadded + modPadding - One<Int_32>();
		return static_cast<UInt_8>(overPadded - modulo(
			overPadded,
			static_cast<Int_32>(modPadding)
		));
	}

	const Int_16 modpad(const Int_16 unpadded, const Int_16 modPadding) noexcept
	{
		const Int_32 overPadded = unpadded + modPadding - One<Int_32>();
		return static_cast<Int_16>(overPadded - modulo(
			overPadded,
			static_cast<Int_32>(modPadding)
		));
	}

	const UInt_16 modpad(const UInt_16 unpadded, const UInt_16 modPadding) noexcept
	{
		const Int_32 overPadded = unpadded + modPadding - One<Int_32>();
		return static_cast<UInt_16>(overPadded - modulo(
			overPadded,
			static_cast<Int_32>(modPadding)
		));
	}

	const Int_32 modpad(const Int_32 unpadded, const Int_32 modPadding) noexcept
	{
		const Int_32 overPadded = unpadded + modPadding - One<Int_32>();
		return overPadded - modulo(overPadded, modPadding);
	}

	const UInt_32 modpad(const UInt_32 unpadded, const UInt_32 modPadding) noexcept
	{
		const UInt_32 overPadded = unpadded + modPadding - One<UInt_32>();
		return overPadded - modulo(overPadded, modPadding);
	}

	const Int_64 modpad(const Int_64 unpadded, const Int_64 modPadding) noexcept
	{
		const Int_64 overPadded = unpadded + modPadding - One<Int_64>();
		return overPadded - modulo(overPadded, modPadding);
	}

	const UInt_64 modpad(const UInt_64 unpadded, const UInt_64 modPadding) noexcept
	{
		const UInt_64 overPadded = unpadded + modPadding - One<UInt_64>();
		return overPadded - modulo(overPadded, modPadding);
	}

	//}
#pragma endregion

#pragma region log()
	//{ log()

	const Float_32 log(const Float_32 v) noexcept
	{
		return std::log(v);
	}

	const Float_64 log(const Float_64 v) noexcept
	{
		return std::log(v);
	}

	//}
#pragma endregion

#pragma region logN()
	//{ logN()

	const Float_32 logN(const Float_32 v, const Float_32 logBase) noexcept
	{
		return log(v) / log(logBase);
	}

	const Float_64 logN(const Float_64 v, const Float_64 logBase) noexcept
	{
		return log(v) / log(logBase);
	}

	//}
#pragma endregion

#pragma region pow()
	//{ pow()

	const Float_32 pow(const Float_32 base, const Float_32 exponent) noexcept
	{
		return std::pow(base, exponent);
	}

	const Float_64 pow(const Float_64 base, const Float_64 exponent) noexcept
	{
		return std::pow(base, exponent);
	}

	//}
#pragma endregion

#pragma region exp()
	//{ exp()

	// Fixes error in std::exp for MSVC on X64
	const Float_32 exp(const Float_32 v) noexcept
	{
#if defined(ARCH_X64) && _MSC_VER < 1900
		return _mm_cvtss_f32(
			blendMasked(
				_mm_set_ss(std::exp(v)),
				Zero<PackedFloat4_128>(),
				_mm_cmpeq_ss(_mm_set_ss(v), NegInfinity<PackedFloat4_128>())
			)
		);
#else
		return std::exp(v);
#endif
	}

	const Float_64 exp(const Float_64 v) noexcept
	{
		return std::exp(v);
	}

	//}
#pragma endregion

#pragma region cos()
	//{ cos()

	const Float_32 cos(const Float_32 v) noexcept
	{
		return std::cos(v);
	}

	const Float_64 cos(const Float_64 v) noexcept
	{
		return std::cos(v);
	}

	//}
#pragma endregion

#pragma region sin()
	//{ sin()

	const Float_32 sin(const Float_32 v) noexcept
	{
		return std::sin(v);
	}

	const Float_64 sin(const Float_64 v) noexcept
	{
		return std::sin(v);
	}

	//}
#pragma endregion

#pragma region atan2()
	//{ atan2()

	const Float_32 atan2(const Float_32 x, const Float_32 y) noexcept
	{
		return std::atan2(y, x);
	}

	const Float_64 atan2(const Float_64 x, const Float_64 y) noexcept
	{
		return std::atan2(y, x);
	}

	//}
#pragma endregion

#pragma region acos()
	//{ acos()

	const Float_32 acos(const Float_32 v) noexcept
	{
		return std::acos(v);
	}

	const Float_64 acos(const Float_64 v) noexcept
	{
		return std::acos(v);
	}

	//}
#pragma endregion

#pragma region tan()
	//{ tan()

	const Float_32 tan(const Float_32 v) noexcept
	{
		return std::tan(v);
	}

	const Float_64 tan(const Float_64 v) noexcept
	{
		return std::tan(v);
	}

	//}
#pragma endregion

}
