#include "vectorization/architecture/memory.h"

#include <cassert>

namespace vectorization
{
    const std::size_t Alignments::ARCH;
    const std::size_t Alignments::X86;
    const std::size_t Alignments::MM;
    const std::size_t Alignments::XMM;
    const std::size_t Alignments::YMM;
    const std::size_t Alignments::ZMM;
    const std::size_t Alignments::Best;

    void free(void *& data) noexcept {
        assert(data);
        vectorization::free(data, std::nothrow_t());
    }

    void free(void *& data, const std::nothrow_t & /* nothrow */) noexcept {
        assert(data);
        _mm_free(data);
        data = nullptr;
    }

    void * alloc(const std::size_t size) noexcept {
        return vectorization::alloc(size, Alignments::Best, std::nothrow_t());
    }

    void * alloc(const std::size_t size, const std::nothrow_t & nothrow) noexcept {
        return vectorization::alloc(size, Alignments::Best, nothrow);
    }

    void * alloc(const std::size_t size, const std::align_val_t alignment) noexcept {
        return vectorization::alloc(size, static_cast<std::size_t>(alignment), std::nothrow_t());
    }

    void * alloc(const std::size_t size, const std::align_val_t alignment, const std::nothrow_t & nothrow) noexcept {
        return vectorization::alloc(size, static_cast<std::size_t>(alignment), nothrow);
    }

    void * alloc(const std::size_t size, const std::size_t alignment) noexcept {
        return vectorization::alloc(size, alignment, std::nothrow_t());
    }

    void * alloc(const std::size_t size, const std::size_t alignment, const std::nothrow_t & /* nothrow */) noexcept {
        void * data = _mm_malloc(size, alignment);
        assert(data);
        return data;
    }
}