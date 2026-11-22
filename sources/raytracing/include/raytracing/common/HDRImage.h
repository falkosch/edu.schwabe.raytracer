#pragma once

#include "Bitmap.h"
#include "TextureWrap.h"

#include <memory>

namespace raytracer
{
    using namespace vectorization;

    class HDRImage : public Image<Float4>
    {
    public:
        typedef Float4 VectorType;
        typedef const VectorType (*const SelectorFunction)(const VectorType&, const VectorType&);

    private:
        Size2 resolution;

        Float4 resolutionf;

        // The coordinates of the last column and row, (x, y) of resolutionf minus 1
        Float4 lastTexelf;

        // Width, height, width, height: the lanes of the texel corners in sampleBilinear
        Int4 resolutioni;

        // The last column and row in those lanes
        Int4 lastTexeli;

        std::unique_ptr<VectorType[]> data;

        void init();

    public:
        HDRImage();

        explicit HDRImage(const Size2& resolution);

        explicit HDRImage(const Bitmap& bitmap, bool srgbColorConversion = true);

        ~HDRImage() override;

        VectorType* getData();

        const VectorType* getData() const;

        VectorType& operator[](ASizeT index) override;

        const VectorType& operator[](ASizeT index) const override;

        const Size2 getResolution() const override;

        void minMax(VectorType& min, VectorType& max, SelectorFunction minSelector, SelectorFunction maxSelector) const;

        void normalizeEachChannel();

        void compressChannels();

        // Texel i's center lies at (i + 0.5) / resolution, row 0 at the bottom (v = 0)
        Float4 sampleBilinear(const Float4& texCoords, TextureWrap wrap = TextureWrap::Repeat) const;
    };
}
