#include "../stdafx.h"

#include "common/HDRImage.h"

namespace raytracer
{

    HDRImage::HDRImage()
        :
        resolution(One<ASizeT>()),
        resolutionf(),
        data()
    {
        init();
    }

    HDRImage::HDRImage(const Size2 & resolutionIn)
        :
        resolution(resolutionIn),
        resolutionf(),
        data()
    {
        init();
    }

    HDRImage::HDRImage(const Bitmap & bitmap)
        :
        resolution(bitmap.getResolution()),
        resolutionf(),
        data()
    {
        init();
        const Int MIN = static_cast<Int>(Bitmap::VectorType::ValueLimits::lowest());
        const Int MAX = static_cast<Int>(Bitmap::VectorType::ValueLimits::max());
        const VectorType::ValueType W = static_cast<VectorType::ValueType>(MAX);
        const VectorType::ValueType OFFSET = static_cast<VectorType::ValueType>(-MIN);
        const VectorType::ValueType RSCALE = reciprocal(W + OFFSET);
        const ASizeT width = x(resolution), stride = bitmap.getStride();
        const int heighti = static_cast<int>(y(resolution));

#pragma omp parallel for
        for (int yi = Zero<int>(); yi < heighti; ++yi)
        {
            const ASizeT y = static_cast<ASizeT>(yi);
            const ASizeT scanlineIn = y * stride;
            const ASizeT scanlineOut = y * width;
            for (ASizeT x = Zero<ASizeT>(); x < width; ++x)
            {
                const ASizeT indexIn = scanlineIn + x * Bitmap::VectorType::SIZE;
                const Int4 texel = Int4(
                    bitmap[indexIn + Zero<ASizeT>()],
                    bitmap[indexIn + One<ASizeT>()],
                    bitmap[indexIn + Two<ASizeT>()]);
                data[scanlineOut + x] = (replaceW(convert<Float4>(texel), W) + OFFSET) * RSCALE;
            }
        }
    }

    HDRImage::~HDRImage()
    {
        if (data)
        {
            delete[] data;
            data = nullptr;
        }
    }

    HDRImage::VectorType * const HDRImage::getData()
    {
        return data;
    }

    const HDRImage::VectorType * const HDRImage::getData() const
    {
        return data;
    }

    HDRImage::VectorType & HDRImage::operator[](const ASizeT index)
    {
        return data[index];
    }

    const HDRImage::VectorType & HDRImage::operator[](const ASizeT index) const
    {
        return data[index];
    }

    const Size2 HDRImage::getResolution() const
    {
        return resolution;
    }

    const void HDRImage::minMax(
        VectorType & min,
        VectorType & max,
        HDRImage::SelectorFunction minSelector,
        HDRImage::SelectorFunction maxSelector) const
    {
        const int count = static_cast<int>(x(resolution) * y(resolution));

#pragma omp parallel
        {
            VectorType pmin = min, pmax = max;

#pragma omp for nowait
            for (int i = Zero<int>(); i < count; ++i)
            {
                const VectorType v = data[i];
                pmin = minSelector(pmin, v);
                pmax = maxSelector(pmax, v);
            }
#pragma omp critical
            {
                min = minSelector(min, pmin);
                max = maxSelector(max, pmax);
            }
        }
    }

    void HDRImage::compressChannels()
    {
        struct Selectors {
            static const VectorType min(const VectorType & a, const VectorType & b) {
                return select(length3(a) <= length3(b), a, b);
            }
            static const VectorType max(const VectorType & a, const VectorType & b) {
                return select(length3(a) >= length3(b), a, b);
            }
        };

        VectorType Imin = VectorType(Float_Limits::max());
        VectorType Imax = VectorType(); // zero
        minMax(Imin, Imax, &Selectors::min, &Selectors::max);

        // find scaling of the image from range [0, |Imax|] to [0, 1]
        const VectorType::ValueType epsilon = reciprocal(static_cast<VectorType::ValueType>(256));
        VectorType::ValueType maxLength = length3(Imax);
        const VectorType::ValueType f = reciprocal(maxLength);
        maxLength = max(maxLength, epsilon);

        // find base of logarithmic scaling
        const VectorType::ValueType minLength = max(length3(Imin), epsilon);
        const VectorType base = VectorType(maxLength / minLength);

        // compress the image with a logarithmic model
        const int count = static_cast<int>(x(resolution) * y(resolution));

#pragma omp parallel for
        for (int i = Zero<int>(); i < count; ++i) {
            // apply the logarithmic scaling
            data[i] = pow(base, data[i] * f) * minLength;
        }
    }

    void HDRImage::normalizeEachChannel()
    {
        struct Selectors {
            static const VectorType min(const VectorType & a, const VectorType & b) {
                return vectorization::min(a, b);
            }
            static const VectorType max(const VectorType & a, const VectorType & b) {
                return vectorization::max(a, b);
            }
        };

        VectorType min = VectorType(Float_Limits::max());
        VectorType max = VectorType(Float_Limits::lowest());
        const int count = static_cast<int>(x(resolution) * y(resolution));
        minMax(min, max, &Selectors::min, &Selectors::max);

        // recalculate min and max in order to favor a fma-apply over the image in the last step
        max = reciprocal(max - min);
        min *= -max;

        // the last step, apply a scaling of the image from range [min, max] to [0, 1]
#pragma omp parallel for
        for (int i = Zero<int>(); i < count; ++i)
        {
            const ASizeT ii = static_cast<ASizeT>(i);
            data[ii] = min + max * data[ii];
        }
    }

    void HDRImage::init()
    {
        resolutionf = convert<Float4>(resolution);
        const ASizeT size = x(resolution) * y(resolution);
        data = new VectorType[size];
    }

    const Float4 HDRImage::sampleBilinear(const Float4 & texCoords) const
    {
        // compute bi-linear weighting
        const Float4 uv = (texCoords * resolutionf) % resolutionf;
        const Float4 partialArea = fract(uv);

        // get reference-indices for the texel
        const Size2 uvTL = convert<Size2>(floor(uv)) % resolution;
        const Size2 uvBR = (uvTL + One<Size2>()) % resolution;
        const Size2 row = y_y(uvTL, uvBR) * xx(resolution);
        const Size2 column = x_x(uvTL, uvBR);
        const Size2 TL_TR = xx(row) + column;
        const Size2 BL_BR = yy(row) + column;

        // finally bi-interpolate
        return mix(
            mix(data[x(TL_TR)], data[y(TL_TR)], xxxx(partialArea)),
            mix(data[x(BL_BR)], data[y(BL_BR)], xxxx(partialArea)),
            yyyy(partialArea));
    }
}
