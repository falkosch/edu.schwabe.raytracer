#pragma once

#include "common/HDRImage.h"

#include <vector>

namespace raytracer
{
    using namespace vectorization;

    // Denoises a rendered image (sRGB-linear): the edge-avoiding a-trous wavelet filter (Dammertz et al. 2010), 5
    // passes of a 5 x 5 B3-spline kernel spread by 1, 2, 4, 8 and 16 pixels, with the edge-stopping weights of SVGF
    // (Schied et al. 2017). The guides of each pixel (PixelFeatures, averaged over its sub-samples): the normal
    // (x, y, z) and depth (w, 0 where the pixel sees no surface) in guides, the diffuse albedo (x, y, z) in albedos.
    // A neighbour weighs less the more its normal, depth or albedo differ, and the more its luminance differs against
    // the noise's spread there, filtered along with the colour. Pixels that see no surface stay as they are.
    class ATrousDenoiser
    {
    public:
        ATrousDenoiser(const HDRImage& guides, const HDRImage& albedos);

        // Replaces the colour (x, y, z) of the image's pixels by the filtered one; w stays. The noise's spread is the
        // variance of each pixel's mean from its sub-samples where moments has them (the count, sum and sum of
        // squares of their compressed luminance in x, y and z; at least 2), and otherwise the variance of the
        // luminance around the pixel, among the pixels of the same surface (a single sub-sample)
        void denoise(HDRImage& image, const HDRImage* moments = nullptr) const;

        // The luminance the weights compare: compressed like a filmic view transform's shoulder (y / (1 + y)), so
        // that bright pixels don't dominate the differences
        static Float compressedLuminance(const Float4& rgb) noexcept;

    private:
        using Pixels = std::vector<Float4, AlignedAllocator<Float4>>;

        const Float4* normalDepth;
        const Float4* albedo;
        int width;
        int height;

        // The depth gradient: the smaller one-sided depth difference to a neighbour in the same row (x) and column
        // (y) that sees a surface, for the depth difference expected along an offset
        std::vector<Float> gradientX;
        std::vector<Float> gradientY;

        ASizeT indexOf(int px, int py) const noexcept;
        bool inside(int px, int py) const noexcept;
        bool seesSurface(ASizeT index) const noexcept;
        Float oneSidedDifference(int px, int py, int dx, int dy) const noexcept;

        // The exponent of the guides' weight of the neighbour q at the offset (dx, dy) in pixels from p (depth and
        // albedo), and the normals' factor
        Float guidesExponent(ASizeT p, ASizeT q, int dx, int dy, Float& normalFactor) const noexcept;

        // The variance of the compressed luminance around each pixel (w of colors), among its neighbours (7 x 7) of
        // the same surface by the guides' weights
        void estimateVariance(const std::vector<Float>& luminance, Pixels& colors) const;

        // The variance of each pixel's mean (w of colors) from the moments of its sub-samples
        void varianceOfMeans(const HDRImage& moments, Pixels& colors) const;

        // One pass of the filter at the step (the kernel's spread): colours (x, y, z) and their variance (w)
        void filterPass(int step, const Pixels& in, const std::vector<Float>& luminance, Pixels& out) const;
    };
}
