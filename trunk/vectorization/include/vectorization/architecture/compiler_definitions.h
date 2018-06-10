/*
 * Defines the compiler and architecture specifications:
 *      - Compiler identification
 *      - Compiler switches
 *      - Compiler target and architecture identification
 *      - Architecture switches
 */

#pragma once

 //#define VECTORIZATION_APPROXIMATIONS 1
 //#define VECTORIZATION_FINE_APPROXIMATIONS 1

#define VECTORIZATION_SSE 0 // up to SSE 3
#define VECTORIZATION_SSE4 4 // up to SSSE3 & SSE 4.2
#define VECTORIZATION_AVX 5 // AVX1

#define VECTORIZATION_INTRINSICS_LEVEL VECTORIZATION_SSE4


 // Compiler specific id switches for easier handling with different compiler types

#if defined(__MINGW32__) && !defined(__GNUC__)
#define __GNUC__
#endif


// Generalize compiler dependent architecture switches into one switch

#if defined(_M_AMD64) || defined(__amd64__)
#define ARCH_X64
#else
#define ARCH_X86
#endif

#ifndef NDEBUG

#if defined(DEBUG) || defined(_DEBUG)

#ifndef _DEBUG
#define _DEBUG 1
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

#else

#define NDEBUG 1

#endif

#endif


// Define architecture switches which are there for MSVC but left out in GNUC or others

#if !defined(__AVX__) && (defined(_M_IX86_FP) && _M_IX86_FP > 2)
#define __AVX__ 1
#endif

#if !defined(__SSE2__) && (defined(__AVX__) || (defined(_M_IX86_FP) && _M_IX86_FP > 1) || defined(ARCH_X64))
#define __SSE2__ 1
#endif

#if !defined(__SSE__) && (defined(__SSE2__) || defined(__AVX__) || (defined(_M_IX86_FP) && _M_IX86_FP > 0) || defined(ARCH_X64))
#define __SSE__ 1
#endif

// override code path option if compiler enforces different code outputs anyway
#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_AVX && defined(__AVX__)
#undef VECTORIZATION_INTRINSICS_LEVEL
#define VECTORIZATION_INTRINSICS_LEVEL VECTORIZATION_AVX
#endif


// Define alignment parameters

#define X86_ALIGNMENT 4
#define MM_ALIGNMENT 8
#define XMM_ALIGNMENT 16
#define YMM_ALIGNMENT 32
#define ZMM_ALIGNMENT 64

#ifdef ARCH_X64
#define ARCH_ALIGNMENT XMM_ALIGNMENT
#else
#define ARCH_ALIGNMENT X86_ALIGNMENT
#endif

// Default alignment specifications

#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_AVX
#define BEST_ALIGNMENT XMM_ALIGNMENT
#elif VECTORIZATION_INTRINSICS_LEVEL == VECTORIZATION_AVX
#define BEST_ALIGNMENT YMM_ALIGNMENT
#elif VECTORIZATION_INTRINSICS_LEVEL > VECTORIZATION_AVX
#define BEST_ALIGNMENT ZMM_ALIGNMENT
#else 
#define BEST_ALIGNMENT ARCH_ALIGNMENT
#endif


// Macro specs

#if defined(__GNUC__)

#define ARCH_NOINLINE __attribute__((noinline))

#define DEBUG_BREAK raise(SIGTRAP)

#define BEGIN_ALIGNED(alignment)
#define END_ALIGNED(alignment) __attribute__((aligned(alignment)))

#else

#define ARCH_NOINLINE __declspec(noinline)

#define DEBUG_BREAK __debugbreak()

#define BEGIN_ALIGNED(alignment) __declspec(align(alignment))
#define END_ALIGNED(alignment)

#endif


#define ALIGNED_UNPACK(...) __VA_ARGS__
#define ALIGNED(alignment, decleration) BEGIN_ALIGNED(BEST_ALIGNMENT) ALIGNED_UNPACK decleration END_ALIGNED(BEST_ALIGNMENT)

