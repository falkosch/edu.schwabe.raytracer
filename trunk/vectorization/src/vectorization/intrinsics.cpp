#include "vectorization/intrinsics.h"
#include "../stdafx.h"

namespace vectorization
{

    const ASizeT VectorAlignments::ARCH;
    const ASizeT VectorAlignments::MM;
    const ASizeT VectorAlignments::XMM;
    const ASizeT VectorAlignments::YMM;
    const ASizeT VectorAlignments::ZMM;
    const ASizeT VectorAlignments::Best;

    void free(void * & data)
    {
        assert(data);
        _mm_free(data);
        data = nullptr;
    }

    void * alloc(const ASizeT size)
    {
        return alloc(size, VectorAlignments::Best);
    }

    void * alloc(const ASizeT size, const ASizeT alignment)
    {
        void * data = _mm_malloc(size, alignment);
        assert(data);
        return data;
    }

}
