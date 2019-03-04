#pragma once

#include "Bitmap.h"

namespace raytracer
{
	using namespace vectorization;

	class HDRImage : public Image < Float4 >
	{
	public:
		typedef Float4 VectorType;
		typedef const VectorType(*const SelectorFunction)(const VectorType &, const VectorType &);

	private:

		Size2 resolution;

		Float4 resolutionf;

		VectorType *data;

		void init();

	public:

		ALIGNED_ALLOCATORS(__alignof(HDRImage));

		HDRImage();

		HDRImage(const Size2 & resolution);

		HDRImage(const Bitmap & bitmap);

		virtual ~HDRImage();

		VectorType * const getData();

		const VectorType * const getData() const;

		VectorType & operator[](const ASizeT index);

		const VectorType & operator[](const ASizeT index) const;

		const Size2 getResolution() const;

		const void minMax(
			VectorType & min,
			VectorType & max,
			SelectorFunction minSelector,
			SelectorFunction maxSelector
		) const;

		void normalizeEachChannel();

		void compressChannels();

		const Float4 sampleBilinear(const Float4 & texCoords) const;
	};

}
