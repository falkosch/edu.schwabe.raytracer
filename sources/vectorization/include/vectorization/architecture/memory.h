/*
 * Includes the required types, and defines useful constants and base functions.
 */

#pragma once

#include "compiler_intrinsics.h"

#include <new>
#include <type_traits>

namespace vectorization {
  struct Alignments {
    static constexpr std::size_t ARCH = ARCH_ALIGNMENT;
    static constexpr std::size_t X86 = X86_ALIGNMENT;
    static constexpr std::size_t MM = MM_ALIGNMENT;
    static constexpr std::size_t XMM = XMM_ALIGNMENT;
    static constexpr std::size_t YMM = YMM_ALIGNMENT;
    static constexpr std::size_t ZMM = ZMM_ALIGNMENT;
    static constexpr std::size_t Best = BEST_ALIGNMENT;
  };

  void free(void *&data) noexcept;

  void free(void *&data, const std::nothrow_t &nothrow) noexcept;

  void *alloc(std::size_t size) noexcept;

  void *alloc(std::size_t size, const std::nothrow_t &nothrow) noexcept;

  void *alloc(std::size_t size, std::align_val_t alignment) noexcept;

  void *alloc(std::size_t size, std::align_val_t alignment, const std::nothrow_t &nothrow) noexcept;

  void *alloc(std::size_t size, std::size_t alignment) noexcept;

  void *alloc(std::size_t size, std::size_t alignment, const std::nothrow_t &nothrow) noexcept;

  template <class T>
  struct AlignedAllocator {
    // typedefs required by xmemory
    using value_type = T;
    using pointer = T *;
    using size_type = std::size_t;
    using is_always_equal = std::true_type;

    AlignedAllocator<T>() noexcept = default;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
    template <class U>
    AlignedAllocator<T>(const AlignedAllocator<U> & /*unused*/) noexcept {
    }
#pragma clang diagnostic pop

    template <class U>
    bool operator==(const AlignedAllocator<U> & /*unused*/) const {
      return true;
    }

    template <class U>
    bool operator!=(const AlignedAllocator<U> & /*unused*/) const {
      return false;
    }

    pointer allocate(const size_type arrayLength) const {
      if (arrayLength == 0) {
        return nullptr;
      }
      constexpr auto maxLength = static_cast<size_type>(-1) / sizeof(T);
      if (arrayLength > maxLength) {
        throw std::bad_array_new_length();
      }

      void *allocated = vectorization::alloc(sizeof(T) * arrayLength, __alignof(T));

      if (allocated == nullptr) {
        throw std::bad_alloc();
      }

      return static_cast<pointer>(allocated);
    }

    void deallocate(pointer allocated, const size_type /*unused*/) const {
      if (allocated != nullptr) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-reinterpret-cast"
        void *vp = reinterpret_cast<void *>(allocated);
        vectorization::free(vp);
#pragma clang diagnostic pop
      }
    }

    // templated alloc
    static pointer allocElements(const size_type arrayLength, const size_type alignment) {
      return static_cast<pointer const>(vectorization::alloc(sizeof(T) * arrayLength, alignment));
    }

    static pointer allocElements(const size_type arrayLength) {
      return allocElements(arrayLength, __alignof(T));
    }
  };
}
