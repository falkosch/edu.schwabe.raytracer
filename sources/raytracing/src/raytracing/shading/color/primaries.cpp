#include "raytracing/shading/color/primaries.h"

namespace raytracer::color
{
    // ITU-R BT.709 / IEC 61966-2-1 (sRGB primaries, D65 white)
    static const MFloat4x4 g_sRGBLinearToXYZ{
        Float4{0.4124564f, 0.3575761f, 0.1804375f, 0.0f},
        Float4{0.2126729f, 0.7151522f, 0.0721750f, 0.0f},
        Float4{0.0193339f, 0.1191920f, 0.9503041f, 0.0f},
        OneW<Float4>()
    };

    static const MFloat4x4 g_XYZToSRGBLinear{
        Float4{3.2404542f, -1.5371385f, -0.4985314f, 0.0f},
        Float4{-0.9692660f, 1.8760108f, 0.0415560f, 0.0f},
        Float4{0.0556434f, -0.2040259f, 1.0572252f, 0.0f},
        OneW<Float4>()
    };

    // ACES TB-2014-004 (ACEScg primaries, ACES ~D60 white point)
    static const MFloat4x4 g_ACEScgToXYZ{
        Float4{0.6624542f, 0.1340042f, 0.1561877f, 0.0f},
        Float4{0.2722287f, 0.6740818f, 0.0536895f, 0.0f},
        Float4{-0.0055746f, 0.0040607f, 1.0103391f, 0.0f},
        OneW<Float4>()
    };

    static const MFloat4x4 g_XYZToACEScg{
        Float4{1.6410234f, -0.3248033f, -0.2364247f, 0.0f},
        Float4{-0.6636629f, 1.6153316f, 0.0167563f, 0.0f},
        Float4{0.0117219f, -0.0082844f, 0.9883948f, 0.0f},
        OneW<Float4>()
    };

    // ITU-R BT.2020 (Rec.2020 primaries, D65 white)
    static const MFloat4x4 g_Rec2020LinearToXYZ{
        Float4{0.6369580f, 0.1446169f, 0.1688810f, 0.0f},
        Float4{0.2627002f, 0.6779981f, 0.0593017f, 0.0f},
        Float4{0.0000000f, 0.0280727f, 1.0609851f, 0.0f},
        OneW<Float4>()
    };

    static const MFloat4x4 g_XYZToRec2020Linear{
        Float4{1.7166512f, -0.3556708f, -0.2533663f, 0.0f},
        Float4{-0.6666844f, 1.6164812f, 0.0157685f, 0.0f},
        Float4{0.0176399f, -0.0427706f, 0.9421031f, 0.0f},
        OneW<Float4>()
    };

    const MFloat4x4& sRGBLinearToXYZ() noexcept
    {
        return g_sRGBLinearToXYZ;
    }

    const MFloat4x4& XYZToSRGBLinear() noexcept
    {
        return g_XYZToSRGBLinear;
    }

    const MFloat4x4& ACEScgToXYZ() noexcept
    {
        return g_ACEScgToXYZ;
    }

    const MFloat4x4& XYZToACEScg() noexcept
    {
        return g_XYZToACEScg;
    }

    const MFloat4x4& Rec2020LinearToXYZ() noexcept
    {
        return g_Rec2020LinearToXYZ;
    }

    const MFloat4x4& XYZToRec2020Linear() noexcept
    {
        return g_XYZToRec2020Linear;
    }

    // Bradford cone response matrix
    static const MFloat4x4 g_Bradford{
        Float4{0.8951f, 0.2664f, -0.1614f, 0.0f},
        Float4{-0.7502f, 1.7135f, 0.0367f, 0.0f},
        Float4{0.0389f, -0.0685f, 1.0296f, 0.0f},
        OneW<Float4>()
    };

    static const MFloat4x4 g_BradfordInv{
        Float4{0.9869929f, -0.1470543f, 0.1599627f, 0.0f},
        Float4{0.4323053f, 0.5183603f, 0.0492912f, 0.0f},
        Float4{-0.0085287f, 0.0400428f, 0.9684867f, 0.0f},
        OneW<Float4>()
    };

    MFloat4x4 chromaticAdaptation(
        Float srcWhiteX, Float srcWhiteY,
        Float dstWhiteX, Float dstWhiteY
    ) noexcept
    {
        const auto srcXYZ = Float4{srcWhiteX / srcWhiteY, 1.0f, (1.0f - srcWhiteX - srcWhiteY) / srcWhiteY, 0.0f};
        const auto dstXYZ = Float4{dstWhiteX / dstWhiteY, 1.0f, (1.0f - dstWhiteX - dstWhiteY) / dstWhiteY, 0.0f};

        const auto srcCone = g_Bradford * srcXYZ;
        const auto dstCone = g_Bradford * dstXYZ;

        const auto scale = dstCone / srcCone;

        const auto diag = vectorization::scale(Identity<MFloat4x4>(), oneW(scale));

        return g_BradfordInv * diag * g_Bradford;
    }
}
