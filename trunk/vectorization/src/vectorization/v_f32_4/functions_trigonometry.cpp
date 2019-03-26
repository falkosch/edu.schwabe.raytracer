#include "vectorization/v_f32_4/functions_trigonometry.h"

#include "vectorization/v_f32_4/accessors.h"
#include "vectorization/v_f32_4/blends.h"

#define USE_THIRD_PARTY 1

#if defined(USE_THIRD_PARTY)

#define USE_SSE2 1
#include <sse_mathfun.h>
#undef USE_SSE2

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
//#include <avx_mathfun.h>
#endif

#endif

namespace vectorization
{

	const v_f32_4 log(const v_f32_4 & v) noexcept
	{
#ifdef USE_THIRD_PARTY

		return log_ps(v);

#else

		return v_f32_4(
			log(x(v)),
			log(y(v)),
			log(z(v)),
			log(w(v))
		);

#endif
	}

	const v_f32_4 log3(const v_f32_4 & v) noexcept
	{
#ifdef USE_THIRD_PARTY

		return blend<false, false, false, true>(log_ps(v), v);

#else

		return v_f32_4(
			log(x(v)),
			log(y(v)),
			log(z(v)),
			w(v)
		);

#endif
	}

	const v_f32_4 exp(const v_f32_4 & v) noexcept
	{

#ifdef USE_THIRD_PARTY

		return exp_ps(v);

#else

		return v_f32_4(
			exp(x(v)),
			exp(y(v)),
			exp(z(v)),
			exp(w(v))
		);

#endif
	}

	const v_f32_4 exp3(const v_f32_4 & v) noexcept
	{
#ifdef USE_THIRD_PARTY

		return blend<false, false, false, true>(exp_ps(v), v);

#else

		return v_f32_4(
			exp(x(v)),
			exp(y(v)),
			exp(z(v)),
			w(v)
		);

#endif
	}

	const v_f32_4 pow(const v_f32_4 & v, const v_f32_4 & exponent) noexcept
	{
		return v_f32_4(
			pow(x(v), x(exponent)),
			pow(y(v), y(exponent)),
			pow(z(v), z(exponent)),
			pow(w(v), w(exponent))
		);
	}

	const v_f32_4 pow(const v_f32_4 & v, const v_f32_4::ValueType exponent) noexcept
	{
		return v_f32_4(
			pow(x(v), exponent),
			pow(y(v), exponent),
			pow(z(v), exponent),
			pow(w(v), exponent)
		);
	}

	const v_f32_4 pow3(const v_f32_4 & v, const v_f32_4 & exponent) noexcept
	{
		return v_f32_4(
			pow(x(v), x(exponent)),
			pow(y(v), y(exponent)),
			pow(z(v), z(exponent)),
			w(v)
		);
	}

	const v_f32_4 pow3(const v_f32_4 & v, const v_f32_4::ValueType exponent) noexcept
	{
		return v_f32_4(
			pow(x(v), exponent),
			pow(y(v), exponent),
			pow(z(v), exponent),
			w(v)
		);
	}

}
