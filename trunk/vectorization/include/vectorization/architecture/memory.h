/*
* Includes the required packed datatypes and defines some useful constants and
* base functions.
*/

#pragma once

#include "compiler_intrinsics.h"

#include <new>

namespace vectorization
{
    struct Alignments
    {
        static const std::size_t ARCH = ARCH_ALIGNMENT;
        static const std::size_t X86 = X86_ALIGNMENT;
        static const std::size_t MM = MM_ALIGNMENT;
        static const std::size_t XMM = XMM_ALIGNMENT;
        static const std::size_t YMM = YMM_ALIGNMENT;
        static const std::size_t ZMM = ZMM_ALIGNMENT;
        static const std::size_t Best = BEST_ALIGNMENT;
    };

    void free(void *& data) noexcept;

    void free(void *& data, const std::nothrow_t & nothrow) noexcept;

    void * alloc(const std::size_t size) noexcept;

    void * alloc(const std::size_t size, const std::nothrow_t & nothrow) noexcept;

    void * alloc(const std::size_t size, const std::align_val_t alignment) noexcept;

    void * alloc(const std::size_t size, const std::align_val_t alignment, const std::nothrow_t & nothrow) noexcept;

    void * alloc(const std::size_t size, const std::size_t alignment) noexcept;

    void * alloc(const std::size_t size, const std::size_t alignment, const std::nothrow_t & nothrow) noexcept;

    template <class T>
    struct AlignedAllocator
    {
        // typedefs required for xmemory
        typedef T value_type;
        typedef T * pointer;
        typedef std::size_t size_type;
        typedef std::true_type is_always_equal;

        AlignedAllocator<T>() noexcept { }

        template<class U>
        AlignedAllocator<T>(const AlignedAllocator<U> &) noexcept { }

        template<class U>
        const bool operator==(const AlignedAllocator<U> &) const {
            return true;
        }

        template<class U>
        const bool operator!=(const AlignedAllocator<U> &) const {
            return false;
        }

        pointer const allocate(const size_type n) const {
            if (n == 0) {
                return nullptr;
            }
            if (n > static_cast<size_type>(-1) / sizeof(T)) {
                throw std::bad_array_new_length();
            }

            void * const pv = vectorization::alloc(sizeof(T) * n, __alignof(T));

            if (!pv) {
                throw std::bad_alloc();
            }

            return static_cast<pointer const>(pv);
        }

        void deallocate(pointer const p, const size_type) const {
            if (p) {
                void * vp = reinterpret_cast<void *>(p);
                vectorization::free(vp);
            }
        }

        // templated alloc
        static pointer const allocElements(const size_type n, const size_type alignment) {
            return static_cast<pointer const>(vectorization::alloc(sizeof(T) * n, alignment));
        }

        static pointer const allocElements(const size_type n) {
            return allocElements(n, __alignof(T));
        }
    };
}
