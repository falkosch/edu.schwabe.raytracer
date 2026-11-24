#include "raytracing/ATrousDenoiser.h"
#include "../stdafx.h"

#include <limits>

namespace raytracer
{
    namespace
    {
        // SVGF's edge-stopping parameters: the power of the normals' cosine (2^7 = 128), the depth difference per the
        // expected one, the luminance difference per the noise's standard deviation; the albedo difference per this
        constexpr int NormalPowerSquarings = 7;
        constexpr Float DepthSigma = 1.0f;
        constexpr Float LuminanceSigma = 4.0f;
        constexpr Float AlbedoSigma = 0.1f;

        // the passes, and the radius of the window of the variance estimate (7 x 7)
        constexpr int Passes = 5;
        constexpr int VarianceRadius = 3;

        // the B3-spline kernel of the a-trous wavelet transform, and the 3 x 3 Gaussian that smooths the variance
        constexpr Float Kernel[5] = {1.0f / 16.0f, 1.0f / 4.0f, 3.0f / 8.0f, 1.0f / 4.0f, 1.0f / 16.0f};
        constexpr Float Gaussian[3] = {1.0f / 4.0f, 1.0f / 2.0f, 1.0f / 4.0f};

        // the least expected depth difference, relative to the depth
        constexpr Float DepthTolerance = 1e-3f;

        // keeps the luminance weight finite where the variance is 0
        constexpr Float LuminanceEpsilon = 1e-4f;

        // max(0, cosine)^128
        Float normalWeight(const Float cosine)
        {
            auto weight = max(Zero<Float>(), cosine);
            for (auto squaring = 0; squaring < NormalPowerSquarings; ++squaring)
            {
                weight *= weight;
            }
            return weight;
        }

        template <typename Values>
        void computeLuminance(const Values& colors, std::vector<Float>& luminance)
        {
#pragma omp parallel for
            for (int i = 0; i < static_cast<int>(colors.size()); ++i)
            {
                luminance[static_cast<ASizeT>(i)] =
                    ATrousDenoiser::compressedLuminance(colors[static_cast<ASizeT>(i)]);
            }
        }
    }

    ATrousDenoiser::ATrousDenoiser(const HDRImage& guides, const HDRImage& albedos)
        : normalDepth(guides.getData()), albedo(albedos.getData()),
          width(static_cast<int>(x(guides.getResolution()))), height(static_cast<int>(y(guides.getResolution()))),
          gradientX(static_cast<ASizeT>(width) * static_cast<ASizeT>(height)), gradientY(gradientX.size())
    {
#pragma omp parallel for
        for (int py = 0; py < height; ++py)
        {
            for (int px = 0; px < width; ++px)
            {
                const auto index = indexOf(px, py);
                gradientX[index] = oneSidedDifference(px, py, 1, 0);
                gradientY[index] = oneSidedDifference(px, py, 0, 1);
            }
        }
    }

    Float ATrousDenoiser::compressedLuminance(const Float4& rgb) noexcept
    {
        const auto luminance = max(Zero<Float>(), dot3(rgb, Float4(0.2126f, 0.7152f, 0.0722f, 0.0f)));
        return luminance / (One<Float>() + luminance);
    }

    void ATrousDenoiser::denoise(HDRImage& image, const HDRImage* const moments) const
    {
        const auto count = static_cast<ASizeT>(width) * static_cast<ASizeT>(height);
        auto colors = Pixels(image.getData(), image.getData() + count);
        auto filtered = Pixels(count);
        auto luminance = std::vector<Float>(count);

        computeLuminance(colors, luminance);
        if (moments)
        {
            varianceOfMeans(*moments, colors);
        }
        else
        {
            estimateVariance(luminance, colors);
        }
        for (auto pass = 0; pass < Passes; ++pass)
        {
            filterPass(1 << pass, colors, luminance, filtered);
            std::swap(colors, filtered);
            computeLuminance(colors, luminance);
        }

#pragma omp parallel for
        for (int i = 0; i < static_cast<int>(count); ++i)
        {
            const auto index = static_cast<ASizeT>(i);
            image[index] = blend<false, false, false, true>(colors[index], image[index]);
        }
    }

    ASizeT ATrousDenoiser::indexOf(const int px, const int py) const noexcept
    {
        return static_cast<ASizeT>(py) * static_cast<ASizeT>(width) + static_cast<ASizeT>(px);
    }

    bool ATrousDenoiser::inside(const int px, const int py) const noexcept
    {
        return px >= 0 && py >= 0 && px < width && py < height;
    }

    bool ATrousDenoiser::seesSurface(const ASizeT index) const noexcept
    {
        return w(normalDepth[index]) > Zero<Float>();
    }

    Float ATrousDenoiser::oneSidedDifference(const int px, const int py, const int dx, const int dy) const noexcept
    {
        const auto index = indexOf(px, py);
        if (!seesSurface(index))
        {
            return Zero<Float>();
        }
        auto difference = std::numeric_limits<Float>::max();
        for (const auto side : {-1, 1})
        {
            const auto qx = px + side * dx;
            const auto qy = py + side * dy;
            if (inside(qx, qy) && seesSurface(indexOf(qx, qy)))
            {
                const auto neighbourDifference = w(normalDepth[indexOf(qx, qy)]) - w(normalDepth[index]);
                difference = min(difference, vectorization::abs(neighbourDifference));
            }
        }
        return difference < std::numeric_limits<Float>::max() ? difference : Zero<Float>();
    }

    Float ATrousDenoiser::guidesExponent(
        const ASizeT p, const ASizeT q, const int dx, const int dy, Float& normalFactor
    ) const noexcept
    {
        normalFactor = normalWeight(dot3(normalDepth[p], normalDepth[q]));
        const auto depthP = w(normalDepth[p]);
        const auto expectedDepthDifference =
            DepthSigma * (gradientX[p] * static_cast<Float>(dx < 0 ? -dx : dx)
                + gradientY[p] * static_cast<Float>(dy < 0 ? -dy : dy))
            + DepthTolerance * depthP;
        const auto depthTerm = vectorization::abs(w(normalDepth[q]) - depthP) / expectedDepthDifference;
        const auto albedoTerm = max3(vectorization::abs(albedo[q] - albedo[p])) / AlbedoSigma;
        return depthTerm + albedoTerm;
    }

    void ATrousDenoiser::estimateVariance(const std::vector<Float>& luminance, Pixels& colors) const
    {
#pragma omp parallel for
        for (int py = 0; py < height; ++py)
        {
            for (int px = 0; px < width; ++px)
            {
                const auto p = indexOf(px, py);
                if (!seesSurface(p))
                {
                    continue;
                }
                // weight, weight * l, weight * l^2
                auto moments = Zero<Float4>();
                for (auto dy = -VarianceRadius; dy <= VarianceRadius; ++dy)
                {
                    for (auto dx = -VarianceRadius; dx <= VarianceRadius; ++dx)
                    {
                        if (!inside(px + dx, py + dy) || !seesSurface(indexOf(px + dx, py + dy)))
                        {
                            continue;
                        }
                        const auto q = indexOf(px + dx, py + dy);
                        Float normalFactor;
                        const auto exponent = guidesExponent(p, q, dx, dy, normalFactor);
                        const auto weight = Float4(normalFactor) * vectorization::exp(Float4(-exponent));
                        const auto l = luminance[q];
                        moments = multiplyAdd(weight, Float4(One<Float>(), l, l * l, Zero<Float>()), moments);
                    }
                }
                const auto mean = moments / xxxx(moments);
                const auto variance = max(Zero<Float>(), z(mean) - sqr(y(mean)));
                colors[p] = blend<false, false, false, true>(colors[p], Float4(variance));
            }
        }
    }

    void ATrousDenoiser::varianceOfMeans(const HDRImage& moments, Pixels& colors) const
    {
#pragma omp parallel for
        for (int i = 0; i < static_cast<int>(colors.size()); ++i)
        {
            const auto index = static_cast<ASizeT>(i);
            const auto& m = moments[index];
            const auto count = x(m);
            const auto variance = count > One<Float>()
                                      ? max(Zero<Float>(), (z(m) - sqr(y(m)) / count) / (count - One<Float>()))
                                          / count
                                      : Zero<Float>();
            colors[index] = blend<false, false, false, true>(colors[index], Float4(variance));
        }
    }

    void ATrousDenoiser::filterPass(
        const int step, const Pixels& in, const std::vector<Float>& luminance, Pixels& out
    ) const
    {
#pragma omp parallel for
        for (int py = 0; py < height; ++py)
        {
            for (int px = 0; px < width; ++px)
            {
                const auto p = indexOf(px, py);
                if (!seesSurface(p))
                {
                    out[p] = in[p];
                    continue;
                }
                // the variance at p smoothed by the 3 x 3 Gaussian, for the luminance weight
                auto smoothed = Zero<Float4>();
                for (auto dy = -1; dy <= 1; ++dy)
                {
                    for (auto dx = -1; dx <= 1; ++dx)
                    {
                        if (inside(px + dx, py + dy) && seesSurface(indexOf(px + dx, py + dy)))
                        {
                            const auto weight = Gaussian[dx + 1] * Gaussian[dy + 1];
                            smoothed += Float4(weight, weight * w(in[indexOf(px + dx, py + dy)]), 0.0f, 0.0f);
                        }
                    }
                }
                const auto luminanceScale =
                    LuminanceSigma * vectorization::sqrt(y(smoothed) / x(smoothed)) + LuminanceEpsilon;

                // colour sum (x, y, z), and weight sum (x), squared weights' variance sum (y)
                auto sum = Zero<Float4>();
                auto weights = Zero<Float4>();
                for (auto ky = 0; ky < 5; ++ky)
                {
                    for (auto kx = 0; kx < 5; ++kx)
                    {
                        const auto dx = (kx - 2) * step;
                        const auto dy = (ky - 2) * step;
                        if (!inside(px + dx, py + dy) || !seesSurface(indexOf(px + dx, py + dy)))
                        {
                            continue;
                        }
                        const auto q = indexOf(px + dx, py + dy);
                        Float normalFactor;
                        const auto exponent = guidesExponent(p, q, dx, dy, normalFactor)
                            + vectorization::abs(luminance[q] - luminance[p]) / luminanceScale;
                        const auto weight = Float4(Kernel[kx] * Kernel[ky] * normalFactor)
                            * vectorization::exp(Float4(-exponent));
                        sum = multiplyAdd(weight, in[q], sum);
                        const auto varianceTerm = Float4(One<Float>(), x(weight) * w(in[q]), 0.0f, 0.0f);
                        weights = multiplyAdd(weight, varianceTerm, weights);
                    }
                }
                // p itself weighs 9/64: the weight sum isn't 0
                const auto variance = y(weights) / sqr(x(weights));
                out[p] = blend<false, false, false, true>(sum / xxxx(weights), Float4(variance));
            }
        }
    }
}
