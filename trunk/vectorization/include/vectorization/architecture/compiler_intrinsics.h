/*
* Includes the required header for the intrinsics defines some useful macros not always readily available for all compilers.
*/

#pragma once

#include "compiler_definitions.h"

#if defined(__GNUC__)
#include <x86intrin.h>
#else
#include <intrin.h>
#endif


#define BLEND_MASK(X, Y, Z, W) ( \
		((X & 1) << 0) \
		| ((Y & 1) << 1) \
		| ((Z & 1) << 2) \
		| ((W & 1) << 3) \
		)

#define BLEND_MASK_i4(X, Y, Z, W) ( \
        ((X & 1) << 0) \
		| ((X & 1) << 1) \
		| ((Y & 1) << 2) \
		| ((Y & 1) << 3) \
        | ((Z & 1) << 4) \
		| ((Z & 1) << 5) \
		| ((W & 1) << 6) \
		| ((W & 1) << 7) \
		)
