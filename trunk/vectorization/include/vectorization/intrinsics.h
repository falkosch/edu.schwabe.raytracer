/*
* Includes the required packed datatypes and defines some useful constants and
* base functions.
*
* @file vectorization/intrinsics.h
*/

#pragma once

#include "datatypes.h"

#include <cassert>
#include <new>

#if defined(__GNUC__)

#include <x86intrin.h>

#define BEGIN_ALIGNED(alignment)
#define END_ALIGNED(alignment) __attribute__((aligned(alignment)))

#else

#include <intrin.h>

#define BEGIN_ALIGNED(alignment) __declspec(align(alignment))
#define END_ALIGNED(alignment)

#endif

#define MM_ALIGNMENT 8
#define XMM_ALIGNMENT 16
#define YMM_ALIGNMENT 32
#define ZMM_ALIGNMENT 64

#if VECTORIZATION_INTRINSICS_LEVEL < VECTORIZATION_AVX
#define BEST_ALIGNMENT XMM_ALIGNMENT
#elif VECTORIZATION_INTRINSICS_LEVEL == VECTORIZATION_AVX
#define BEST_ALIGNMENT YMM_ALIGNMENT
#elif VECTORIZATION_INTRINSICS_LEVEL > VECTORIZATION_AVX
#define BEST_ALIGNMENT ZMM_ALIGNMENT
#else 
#define BEST_ALIGNMENT ARCH_ALIGNMENT
#endif

#define ALIGNED_UNPACK(...) __VA_ARGS__
#define ALIGNED(alignment, decleration) BEGIN_ALIGNED(BEST_ALIGNMENT) ALIGNED_UNPACK decleration END_ALIGNED(BEST_ALIGNMENT)

#define ALIGNED_ALLOCATORS(alignment) \
    static void * operator new(std::size_t size) { return vectorization::alloc(size, alignment); } \
	static void * operator new(std::size_t size, const std::nothrow_t & nothrow) noexcept { return vectorization::alloc(size, alignment, nothrow); } \
	static void * operator new(std::size_t size, std::align_val_t requestedAlignment) { return vectorization::alloc(size, requestedAlignment); } \
	static void * operator new(std::size_t size, std::align_val_t requestedAlignment, const std::nothrow_t & nothrow) noexcept { return vectorization::alloc(size, requestedAlignment, nothrow); } \
	\
	static void operator delete(void * ptr) noexcept { vectorization::free(ptr); } \
	static void operator delete(void * ptr, const std::nothrow_t & nothrow) noexcept { vectorization::free(ptr); } \
	static void operator delete(void * ptr, std::align_val_t) noexcept { vectorization::free(ptr); } \
	static void operator delete(void * ptr, std::align_val_t, const std::nothrow_t & nothrow) noexcept { vectorization::free(ptr); } \
	static void operator delete(void * ptr, std::size_t) noexcept { vectorization::free(ptr); } \
	static void operator delete(void * ptr, std::size_t, const std::nothrow_t & nothrow) noexcept { vectorization::free(ptr); } \
	static void operator delete(void * ptr, std::size_t, std::align_val_t) noexcept { vectorization::free(ptr); } \
	static void operator delete(void * ptr, std::size_t, std::align_val_t, const std::nothrow_t & nothrow) noexcept { vectorization::free(ptr); } \
	\
    static void * operator new[](std::size_t size) { return vectorization::alloc(size, alignment); } \
	static void * operator new[](std::size_t size, const std::nothrow_t & nothrow) noexcept { return vectorization::alloc(size, alignment, nothrow); } \
	static void * operator new[](std::size_t size, std::align_val_t requestedAlignment) { return vectorization::alloc(size, requestedAlignment); } \
	static void * operator new[](std::size_t size, std::align_val_t requestedAlignment, const std::nothrow_t & nothrow) noexcept { return vectorization::alloc(size, requestedAlignment, nothrow); } \
	\
	static void operator delete[](void * ptr) noexcept { vectorization::free(ptr); } \
	static void operator delete[](void * ptr, const std::nothrow_t & nothrow) noexcept { vectorization::free(ptr); } \
	static void operator delete[](void * ptr, std::align_val_t) noexcept { vectorization::free(ptr); } \
	static void operator delete[](void * ptr, std::align_val_t, const std::nothrow_t & nothrow) noexcept { vectorization::free(ptr); } \
	static void operator delete[](void * ptr, std::size_t) noexcept { vectorization::free(ptr); } \
	static void operator delete[](void * ptr, std::size_t, const std::nothrow_t & nothrow) noexcept { vectorization::free(ptr); } \
	static void operator delete[](void * ptr, std::size_t, std::align_val_t) noexcept { vectorization::free(ptr); } \
	static void operator delete[](void * ptr, std::size_t, std::align_val_t, const std::nothrow_t & nothrow) noexcept { vectorization::free(ptr); }


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

namespace vectorization
{

#pragma region Packed types
	//{Packed types

	typedef __m128 PackedFloat4_128;
	typedef __m128d PackedFloat2_128;
	typedef __m128i PackedInts_128;

	template <typename TValueType, ASizeT Size>
	struct PackedTypes
	{
		static const ASizeT SIZE = Size;
		typedef TValueType ValueType;
	};

	template<>
	struct PackedTypes < Float_32, 4 >
	{
		typedef PackedFloat4_128 Type;
	};

	template<>
	struct PackedTypes < Float_64, 2 >
	{
		typedef PackedFloat2_128 Type;
	};

	template<>
	struct PackedTypes < Int_8, 16 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < UInt_8, 16 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < Int_16, 8 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < UInt_16, 8 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < Int_32, 4 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < UInt_32, 4 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < Int_64, 2 >
	{
		typedef PackedInts_128 Type;
	};

	template<>
	struct PackedTypes < UInt_64, 2 >
	{
		typedef PackedInts_128 Type;
	};


#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX

	typedef __m256 PackedFloat8_256;
	typedef __m256d PackedFloat4_256;
	typedef __m256i PackedInts_256;

	template<>
	struct PackedTypes < Float_32, 8 >
	{
		typedef PackedFloat8_256 Type;
	};

	template<>
	struct PackedTypes < Float_64, 4 >
	{
		typedef PackedFloat4_256 Type;
	};

	template<>
	struct PackedTypes < Int_8, 32 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < UInt_8, 32 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < Int_16, 16 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < UInt_16, 16 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < Int_32, 8 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < UInt_32, 8 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < Int_64, 4 >
	{
		typedef PackedInts_256 Type;
	};

	template<>
	struct PackedTypes < UInt_64, 4 >
	{
		typedef PackedInts_256 Type;
	};

#endif

	//}
#pragma endregion

#pragma region Alignments
	//{Alignments

	void free(void * & data) noexcept;

	void * alloc(const ASizeT size) noexcept;

	void * alloc(const ASizeT size, const std::nothrow_t & nothrow) noexcept;

	void * alloc(const ASizeT size, const std::align_val_t alignment) noexcept;

	void * alloc(const ASizeT size, const std::align_val_t alignment, const std::nothrow_t & nothrow) noexcept;

	void * alloc(const ASizeT size, const ASizeT alignment) noexcept;

	void * alloc(const ASizeT size, const ASizeT alignment, const std::nothrow_t & nothrow) noexcept;

	struct VectorAlignments
	{
		static const ASizeT ARCH = ARCH_ALIGNMENT;
		static const ASizeT MM = MM_ALIGNMENT;
		static const ASizeT XMM = XMM_ALIGNMENT;
		static const ASizeT YMM = YMM_ALIGNMENT;
		static const ASizeT ZMM = ZMM_ALIGNMENT;
		static const ASizeT Best = BEST_ALIGNMENT;
	};

	template <class T>
	struct AlignedAllocator
	{
		// typedefs required for xmemory
		typedef T value_type;
		typedef T * pointer;
		typedef ASizeT size_type;

		AlignedAllocator() noexcept
		{
		}

		template<class U>
		AlignedAllocator(const AlignedAllocator<U> &) noexcept
		{
		}

		template<class U>
		const bool operator==(const AlignedAllocator<U> &) const
		{
			return true;
		}

		template<class U>
		const bool operator!=(const AlignedAllocator<U> &) const
		{
			return false;
		}

		pointer const allocate(const size_type n) const
		{
			if (n == 0) {
				return nullptr;
			}
			if (n > static_cast<size_type>(-1) / sizeof(T)) {
				throw std::bad_array_new_length();
			}

			void* const pv = alloc(sizeof(T) * n, __alignof(T));

			if (!pv) {
				throw std::bad_alloc();
			}

			return static_cast<pointer const>(pv);
		}

		void deallocate(pointer const p, const size_type) const
		{
			if (p) {
				void * vp = reinterpret_cast<void *>(p);
				free(vp);
			}
		}

		// templated alloc
		static pointer const allocElements(const size_type n, const size_type alignment)
		{
			return static_cast<pointer const>(alloc(sizeof(T) * n, alignment));
		}
		static pointer const allocElements(const size_type n)
		{
			return allocElements(n, __alignof(T));
		}
	};

	//}
#pragma endregion

}
