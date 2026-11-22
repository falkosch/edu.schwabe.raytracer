#include "raytracing/common/HDRImage.h"
#include "../../stdafx.h"

#include "raytracing/shading/color/primaries.h"
#include "raytracing/shading/color/view_transform.h"

#ifndef MSVC
#include <memory>
#include <type_traits>
#endif

namespace raytracer
{
    HDRImage::HDRImage() : resolution(One<ASizeT>()), resolutionf(), data()
    {
        init();
    }

    HDRImage::HDRImage(const Size2& resolutionIn) : resolution(resolutionIn), resolutionf(), data()
    {
        init();
    }

    HDRImage::HDRImage(const Bitmap& bitmap, const bool srgbColorConversion)
        : resolution(bitmap.getResolution()), resolutionf(), data()
    {
        typedef Bitmap::VectorType::ValueType BitmapValueType;
        typedef std::numeric_limits<BitmapValueType> BitmapValueLimits;
        typedef VectorType::ValueType ImageValueType;

        init();
        constexpr BitmapValueType MIN = BitmapValueLimits::lowest();
        constexpr BitmapValueType MAX = BitmapValueLimits::max();
        const ImageValueType W = convert<ImageValueType>(MAX);
        const ImageValueType OFFSET = -convert<ImageValueType>(MIN);
        const ImageValueType RSCALE = reciprocal(W + OFFSET);
        const ASizeT width = x(resolution), stride = bitmap.getStride();
        const int heighti = static_cast<int>(y(resolution));

#pragma omp parallel for
        for (int yi = Zero<int>(); yi < heighti; ++yi)
        {
            const auto y = static_cast<ASizeT>(yi);
            const ASizeT scanlineIn = y * stride;
            const ASizeT scanlineOut = y * width;

            for (ASizeT x = Zero<ASizeT>(); x < width; ++x)
            {
                const ASizeT indexIn = scanlineIn + x * Bitmap::VectorType::SIZE;
                const auto texel =
                    Int4(bitmap[indexIn + Zero<ASizeT>()], bitmap[indexIn + One<ASizeT>()],
                         bitmap[indexIn + Two<ASizeT>()]);
                const auto normalized = zeroW(convert<Float4>(texel) + OFFSET) * RSCALE;
                data[scanlineOut + x] = srgbColorConversion
                                            ? color::srgbDecode(normalized)
                                            : normalized;
            }
        }
    }

    HDRImage::~HDRImage() = default;

    HDRImage::VectorType* HDRImage::getData()
    {
        return data.get();
    }

    const HDRImage::VectorType* HDRImage::getData() const
    {
        return data.get();
    }

    HDRImage::VectorType& HDRImage::operator[](const ASizeT index)
    {
        return data[index];
    }

    const HDRImage::VectorType& HDRImage::operator[](const ASizeT index) const
    {
        return data[index];
    }

    const Size2 HDRImage::getResolution() const
    {
        return resolution;
    }

    void
    HDRImage::minMax(VectorType& min, VectorType& max, SelectorFunction minSelector, SelectorFunction maxSelector) const
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
        struct Selectors
        {
            static const VectorType min(const VectorType& a, const VectorType& b)
            {
                return select(length3(a) <= length3(b), a, b);
            }

            static const VectorType max(const VectorType& a, const VectorType& b)
            {
                return select(length3(a) >= length3(b), a, b);
            }
        };

        auto Imin = VectorType(std::numeric_limits<VectorType::ValueType>::max());
        auto Imax = VectorType(); // zero
        minMax(Imin, Imax, &Selectors::min, &Selectors::max);

        // find scaling of the image from range [0, |Imax|] to [0, 1]
        const VectorType::ValueType epsilon = reciprocal(static_cast<VectorType::ValueType>(256));
        VectorType::ValueType maxLength = length3(Imax);
        const VectorType::ValueType f = reciprocal(maxLength);
        maxLength = max(maxLength, epsilon);

        // find base of logarithmic scaling
        const VectorType::ValueType minLength = max(length3(Imin), epsilon);
        const auto base = VectorType(maxLength / minLength);

        // compress the image with a logarithmic model
        const int count = static_cast<int>(x(resolution) * y(resolution));

#pragma omp parallel for
        for (int i = Zero<int>(); i < count; ++i)
        {
            // apply the logarithmic scaling
            data[i] = pow(base, data[i] * f) * minLength;
        }
    }

    void HDRImage::normalizeEachChannel()
    {
        struct Selectors
        {
            static const VectorType min(const VectorType& a, const VectorType& b)
            {
                return vectorization::min(a, b);
            }

            static const VectorType max(const VectorType& a, const VectorType& b)
            {
                return vectorization::max(a, b);
            }
        };

        auto min = VectorType(std::numeric_limits<VectorType::ValueType>::max());
        auto max = VectorType(std::numeric_limits<VectorType::ValueType>::lowest());
        const int count = static_cast<int>(x(resolution) * y(resolution));
        minMax(min, max, &Selectors::min, &Selectors::max);

        // recalculate min and max in order to favor a fma-apply over the image in the last step
        max = reciprocal(max - min);
        min *= -max;

        // the last step, apply a scaling of the image from range [min, max] to [0, 1]
#pragma omp parallel for
        for (int i = Zero<int>(); i < count; ++i)
        {
            const auto ii = static_cast<ASizeT>(i);
            data[ii] = multiplyAdd(max, data[ii], min);
        }
    }

    void HDRImage::init()
    {
        resolutionf = convert<Float4>(resolution);
        lastTexelf = resolutionf - One<Float4>();
        resolutioni = xyxy(convert<Int4>(resolutionf));
        lastTexeli = resolutioni - One<Int4>();
        const auto size = x(resolution) * y(resolution);
        data = std::make_unique<VectorType[]>(size);
    }

    Float4 HDRImage::sampleBilinear(const Float4& texCoords, const TextureWrap wrap) const
    {
        using Lane = Int4::ValueType;

        // the lanes (u, v, u, v) that are clamped instead of wrapped around, from TextureWrap's bits (1: u, 2: v)
        const Int4 axisBits(One<Lane>(), Two<Lane>(), One<Lane>(), Two<Lane>());
        const Int4 clampedLanes = (Int4(static_cast<Lane>(wrap)) & axisBits) != Zero<Int4>();

        // bi-linear weights between the texel centers; clamped lanes stay between the centers of the edge texels
        // (wrapping would blend v = 1 with the bottom row)
        const Float4 texels = texCoords * resolutionf - Half<Float4>();
        const Float4 uv = select(clampedLanes, clamp(texels, Zero<Float4>(), lastTexelf), texels % resolutionf);
        const Float4 cell = floor(uv);
        const Float4 partialArea = uv - cell;

        // column and row of the texel and of its successor. A wrapped lane at the resolution (the successor of the
        // last texel, or rounding in the modulo) goes back to 0. The clamp keeps a clamped lane's successor at the
        // last texel and every index within the image, also for NaN coordinates (converted to the lowest integer).
        const Int4 corners =
            xyxy(convert<Int4>(cell)) + Int4(Zero<Lane>(), Zero<Lane>(), One<Lane>(), One<Lane>());
        const Int4 wraps = (corners >= resolutioni) & ~clampedLanes & resolutioni;
        const Int4 texel = clamp(corners - wraps, Zero<Int4>(), lastTexeli);

        // the indices of (x0, y0), (x1, y0), (x0, y1) and (x1, y1)
        const Int4 rows = swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(texel);
        const Int4 columns = swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::X, VectorIndices::Z>(texel);
        const Int4 indices = rows * xxxx(resolutioni) + columns;

        return mix(
            mix(data[static_cast<ASizeT>(x(indices))], data[static_cast<ASizeT>(y(indices))], xxxx(partialArea)),
            mix(data[static_cast<ASizeT>(z(indices))], data[static_cast<ASizeT>(w(indices))], xxxx(partialArea)),
            yyyy(partialArea)
        );
    }
}
