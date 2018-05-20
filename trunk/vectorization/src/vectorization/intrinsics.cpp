#include "vectorization/intrinsics.h"

namespace vectorization
{

    const ASizeT VectorAlignments::ARCH;
    const ASizeT VectorAlignments::MM;
    const ASizeT VectorAlignments::XMM;
    const ASizeT VectorAlignments::YMM;
    const ASizeT VectorAlignments::ZMM;
    const ASizeT VectorAlignments::Best;

    void free(void * & data) noexcept
    {
        assert(data);
        _mm_free(data);
        data = nullptr;
    }

    void * alloc(const ASizeT size) noexcept
    {
        return alloc(size, VectorAlignments::Best);
    }

    void * alloc(const ASizeT size, const ASizeT alignment) noexcept
    {
        void * data = _mm_malloc(size, alignment);
        assert(data);
        return data;
    }

	void * alloc(const ASizeT size, const std::align_val_t alignment) noexcept
	{
		return alloc(size, ASizeT(alignment));
	}

}
