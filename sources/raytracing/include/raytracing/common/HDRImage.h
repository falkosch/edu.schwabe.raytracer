#pragma once

#include "Bitmap.h"

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

        Float4 sampleBilinear(const Float4& texCoords) const;
    };
}
