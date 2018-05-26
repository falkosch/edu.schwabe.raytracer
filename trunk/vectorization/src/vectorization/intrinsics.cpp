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
		return vectorization::alloc(size, VectorAlignments::Best);
	}

	void * alloc(const ASizeT size, const std::nothrow_t & nothrow) noexcept
	{
		return vectorization::alloc(size, VectorAlignments::Best, nothrow);
	}

	void * alloc(const ASizeT size, const std::align_val_t alignment) noexcept
	{
		return vectorization::alloc(size, static_cast<ASizeT>(alignment));
	}

	void * alloc(const ASizeT size, const std::align_val_t alignment, const std::nothrow_t & nothrow) noexcept
	{
		return vectorization::alloc(size, static_cast<ASizeT>(alignment), nothrow);
	}

	void * alloc(const ASizeT size, const ASizeT alignment) noexcept
	{
		return vectorization::alloc(size, alignment, std::nothrow_t());
	}

	void * alloc(const ASizeT size, const ASizeT alignment, const std::nothrow_t & nothrow) noexcept
	{
		void * data = _mm_malloc(size, alignment);
		assert(data);
		return data;
	}
}
