#include "raytracing/shading/color/view_transform.h"

namespace raytracer::color
{
    static constexpr Float LOG2_E = 1.44269504088896340736f;

    Float4 acesFilmic(const Float4& linearRGB) noexcept
    {
        const auto a = Float4{2.51f};
        const auto b = Float4{0.03f};
        const auto c = Float4{2.43f};
        const auto d = Float4{0.59f};
        const auto e = Float4{0.14f};

        const auto clamped = max(linearRGB, Zero<Float4>());
        const auto numerator = clamped * (a * clamped + b);
        const auto denominator = clamped * (c * clamped + d) + e;

        return clamp(numerator / denominator, Zero<Float4>(), One<Float4>());
    }

    static const MFloat4x4 g_AgXInsetMatrix{
        Float4{0.8424790622530940f, 0.0784335999585498f, 0.0792237451477643f, 0.0f},
        Float4{0.0423282422610123f, 0.8784686057793130f, 0.0791661274605434f, 0.0f},
        Float4{0.0423756549057051f, 0.0784336050245193f, 0.8791429738932040f, 0.0f},
        Float4{0.0f, 0.0f, 0.0f, 1.0f}
    };

    static const MFloat4x4 g_AgXOutsetMatrix{
        Float4{1.1968790286181420f, -0.0980208811401368f, -0.0990581477979856f, 0.0f},
        Float4{-0.0528968517574562f, 1.1519031031927860f, -0.0989611768173100f, 0.0f},
        Float4{-0.0529716355144438f, -0.0980434728690828f, 1.1510347523642640f, 0.0f},
        Float4{0.0f, 0.0f, 0.0f, 1.0f}
    };

    Float4 agx(const Float4& linearRGB) noexcept
    {
        const auto clamped = max(linearRGB, Float4{1e-10f});
        const auto inset = g_AgXInsetMatrix * clamped;

        // Vectorized log2 encoding via ln(x) * log2(e), clamped to [0,1]
        const auto logMin = Float4{-12.47393f};
        const auto logMax = Float4{4.02607f};
        const auto logRange = logMax - logMin;
        const auto logged = (log(max(inset, Float4{1e-10f})) * LOG2_E - logMin) / logRange;
        const auto logClamped = clamp(logged, Zero<Float4>(), One<Float4>());

        // 6th-degree polynomial sigmoid (Blender AgX-Base default contrast)
        const auto x2 = sqr(logClamped);
        const auto x4 = sqr(x2);
        const auto sigmoided = x4 * x2 * 15.5f - x4 * logClamped * 40.14f + x4 * 31.96f
            - x2 * logClamped * 6.868f + x2 * 0.4298f
            + logClamped * 0.1191f - Float4{0.00232f};

        const auto result = g_AgXOutsetMatrix * sigmoided;
        return clamp(result, Zero<Float4>(), One<Float4>());
    }

    // IEC 61966-2-1 sRGB OETF: linear -> sRGB (piecewise, vectorized)
    Float4 srgbEncode(const Float4& linear) noexcept
    {
        const auto threshold = Float4{0.0031308f};
        const auto lowBranch = linear * 12.92f;
        const auto highBranch = pow(linear, reciprocal(2.4f)) * 1.055f - Float4{0.055f};

        const auto mask = linear <= threshold;
        const auto result = select(mask, lowBranch, highBranch);
        return zeroW(result) + OneW<Float4>() * linear;
    }

    // IEC 61966-2-1 sRGB EOTF: sRGB -> linear (piecewise, vectorized)
    Float4 srgbDecode(const Float4& encoded) noexcept
    {
        const auto threshold = Float4{0.04045f};
        const auto lowBranch = encoded / 12.92f;
        const auto highBranch = pow((encoded + Float4{0.055f}) / 1.055f, 2.4f);

        const auto mask = encoded <= threshold;
        const auto result = select(mask, lowBranch, highBranch);
        return zeroW(result) + OneW<Float4>() * encoded;
    }
}
