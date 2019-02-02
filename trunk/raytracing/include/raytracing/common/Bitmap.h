#pragma once

#include "Image.h"

#include <Windows.h>

namespace raytracer
{
	using namespace vectorization;

	// Bitmap with 8-bit-width red, green and blue color-channels but no alpha-channel.
	// The width of each scanline is padded to be a multiple of 4 bytes.
	class Bitmap
	{
	public:
		typedef struct {
			static const ASizeT SIZE = VectorSizes::Z;
			typedef UInt_8 ValueType;
			typedef UInt_8 BoolType;
		} VectorType;

	private:

		Size2 resolution;

		ASizeT stride;

		VectorType::ValueType * data;

		void init();

	public:

		ALIGNED_ALLOCATORS(__alignof(Bitmap));

		Bitmap();

		Bitmap(const Size2 & resolution);

		template <typename ImageVectorType>
		Bitmap(const Image<ImageVectorType> & image)
			: resolution(image.getResolution()), stride(), data()
		{
			typedef VectorType::ValueType BitmapValueType;
			typedef std::numeric_limits<BitmapValueType> BitmapValueLimits;
			typedef typename ImageVectorType::ValueType ImageValueType;

			init();
			const Int4 BMIN = Int4(convert<Int4::ValueType>(BitmapValueLimits::lowest()));
			const Int4 BMAX = Int4(convert<Int4::ValueType>(BitmapValueLimits::max()));
			const ImageVectorType VMIN = convert<ImageVectorType>(BMIN);
			const ImageVectorType VMAX = convert<ImageVectorType>(BMAX);
			const int heighti = convert<int>(y(resolution));

#pragma omp parallel for
			for (int yi = Zero<int>(); yi < heighti; ++yi) {
				const ASizeT sy = convert<ASizeT>(yi);
				const ASizeT rx = x(resolution);
				const ASizeT scanlineIn = sy * rx;
				const ASizeT scanlineOut = sy * stride;

				for (ASizeT sx = Zero<ASizeT>(); sx < rx; ++sx) {
					const Int4 scaled = clamp(
						convert<Int4>(image[scanlineIn + sx] * VMAX + VMIN),
						BMIN,
						BMAX
					);
					BitmapValueType * dataOut = &data[scanlineOut + sx * VectorType::SIZE];
					// need to swap r,g,b to b,g,r
					*(dataOut++) = convert<BitmapValueType>(z(scaled)); // blue
					*(dataOut++) = convert<BitmapValueType>(y(scaled)); // green
					*dataOut = convert<BitmapValueType>(x(scaled)); // red
				}
			}
		}

		virtual ~Bitmap();

		VectorType::ValueType & operator[](const ASizeT index);

		const VectorType::ValueType & operator[](const ASizeT index) const;

		const Size2 getResolution() const;

		const ASizeT getStride() const;

		const BITMAPINFO getBITMAPINFO() const;

		const BITMAP getBITMAP() const;

		void saveAsBMP(const std::string & filename) const;

		static Bitmap * const loadPPM(const std::string & filename);
	};

}
