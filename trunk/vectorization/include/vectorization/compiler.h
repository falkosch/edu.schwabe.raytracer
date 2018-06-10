/*
 * Defines the compiler and architecture specifications:
 *      - Compiler identification
 *      - Compiler switches
 *      - Compiler target and architecture identification
 *      - Architecture switches
 *
 * @file vectorization/compiler.h
 */

#pragma once

#define VECTORIZATION_SSE 0 // up to SSE 3
#define VECTORIZATION_SSE4 4 // up to SSSE3 & SSE 4.2
#define VECTORIZATION_AVX 5 // AVX1

#define VECTORIZATION_INTRINSICS_LEVEL VECTORIZATION_SSE4

 //#define VECTORIZATION_APPROXIMATIONS 1
 //#define VECTORIZATION_FINE_APPROXIMATIONS 1


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

#undef NDEBUG

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


// Default alignment specifications

#ifdef ARCH_X64
#define ARCH_ALIGNMENT 16
#else
#define ARCH_ALIGNMENT 4
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


// Function specs

#if defined(__GNUC__)
#define ARCH_NOINLINE __attribute__((noinline))
#define DEBUG_BREAK raise(SIGTRAP)
#else
#define ARCH_NOINLINE __declspec(noinline)
#define DEBUG_BREAK __debugbreak()
#endif


#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_AVX && defined(__AVX__)
#undef VECTORIZATION_INTRINSICS_LEVEL
#define VECTORIZATION_INTRINSICS_LEVEL VECTORIZATION_AVX
#endif

