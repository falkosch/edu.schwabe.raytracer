#include "raytracing/common/HDRImage.h"
#include "../../stdafx.h"

#include "raytracing/shading/color/primaries.h"
#include "raytracing/shading/color/view_transform.h"

#include <bit>
#include <cstdint>
#include <fstream>
#include <logging.h>
#include <vector>

#ifndef MSVC
#include <memory>
#include <type_traits>
#endif

static const auto Log = logging::scope("HDRImage");

namespace raytracer
{
    namespace
    {
        // PFM stores IEEE float32; the writer and the little-endian detection rely on the native float format
        using PFMValueType = float;
        static_assert(sizeof(PFMValueType) == sizeof(std::uint32_t));
        static_assert(std::endian::native == std::endian::little);

        constexpr ASizeT PFMColorChannels = 3;
    }

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
                // the bitmap stores b,g,r
                const ASizeT indexIn = scanlineIn + x * Bitmap::VectorType::SIZE;
                const auto texel =
                    Int4(bitmap[indexIn + Two<ASizeT>()], bitmap[indexIn + One<ASizeT>()],
                         bitmap[indexIn + Zero<ASizeT>()]);
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

    bool HDRImage::saveAsPFM(const std::string& filename) const
    {
        std::ofstream file(filename.c_str(), std::ios::binary | std::ios::trunc);
        if (!file.is_open())
        {
            Log.error([fn = filename] { return "opening file " + fn + " failed"; });
            return false;
        }

        // a negative scale marks little-endian data
        const auto width = x(resolution);
        const auto height = y(resolution);
        file << "PF\n" << width << ' ' << height << "\n-1.0\n";

        std::vector<PFMValueType> scanline(PFMColorChannels * width);
        for (auto row = Zero<ASizeT>(); row < height; ++row)
        {
            for (auto column = Zero<ASizeT>(); column < width; ++column)
            {
                const auto& texel = data[row * width + column];
                auto* const values = &scanline[PFMColorChannels * column];
                values[0] = static_cast<PFMValueType>(x(texel));
                values[1] = static_cast<PFMValueType>(y(texel));
                values[2] = static_cast<PFMValueType>(z(texel));
            }
            file.write(
                reinterpret_cast<const char*>(scanline.data()),
                static_cast<std::streamsize>(scanline.size() * sizeof(PFMValueType))
            );
        }

        file.flush();
        file.close();
        return file.good();
    }

    std::unique_ptr<HDRImage> HDRImage::loadPFM(const std::string& filename)
    {
        std::ifstream file(filename.c_str(), std::ios::binary);
        if (!file.is_open())
        {
            Log.error([fn = filename] { return "opening file " + fn + " failed"; });
            return nullptr;
        }

        // the header: magic, width, height and scale separated by whitespace, then a single whitespace character
        std::string magic;
        auto width = Zero<ASizeT>();
        auto height = Zero<ASizeT>();
        auto scale = Zero<double>();
        file >> magic >> width >> height >> scale;
        file.get();
        if (!file || (magic != "PF" && magic != "Pf") || width == Zero<ASizeT>() || height == Zero<ASizeT>()
            || scale == Zero<double>())
        {
            Log.error([fn = filename] { return "File " + fn + " is not a PFM file"; });
            return nullptr;
        }

        const auto channels = magic == "PF" ? PFMColorChannels : One<ASizeT>();
        // a positive scale marks big-endian data
        const auto byteSwapped = scale > Zero<double>();
        auto loaded = std::make_unique<HDRImage>(Size2(width, height));

        std::vector<std::uint32_t> scanline(channels * width);
        const auto value = [&scanline, byteSwapped](const ASizeT index)
        {
            const auto bits = byteSwapped ? std::byteswap(scanline[index]) : scanline[index];
            return static_cast<Float>(std::bit_cast<PFMValueType>(bits));
        };
        for (auto row = Zero<ASizeT>(); row < height; ++row)
        {
            file.read(
                reinterpret_cast<char*>(scanline.data()),
                static_cast<std::streamsize>(scanline.size() * sizeof(std::uint32_t))
            );
            if (!file)
            {
                Log.error([fn = filename] { return "File " + fn + " is truncated"; });
                return nullptr;
            }
            for (auto column = Zero<ASizeT>(); column < width; ++column)
            {
                const auto index = channels * column;
                loaded->data[row * width + column] = channels == PFMColorChannels
                    ? Float4(value(index), value(index + One<ASizeT>()), value(index + Two<ASizeT>()), Zero<Float>())
                    : zeroW(Float4(value(index)));
            }
        }

        Log.info([fn = filename, width, height]
        {
            return "Image " + fn + "; " + std::to_string(width) + "x" + std::to_string(height) + " loaded.";
        });
        return loaded;
    }
}
