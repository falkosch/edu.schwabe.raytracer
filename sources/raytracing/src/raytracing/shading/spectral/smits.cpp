#include "raytracing/shading/spectral/smits.h"

#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

namespace raytracer::spectral
{
    static constexpr ASizeT SMITS_SAMPLES = 32;
    static constexpr Float SMITS_LAMBDA_MIN = 380.0f;
    static constexpr Float SMITS_LAMBDA_MAX = 720.0f;

    // PBRT v3 wavelength grid for Smits basis spectra (32 samples, 380-720nm)
    // Karl vom Berge's re-derivation at higher resolution than Smits' original 10 bins.
    // Source: github.com/mmp/pbrt-v3, src/core/spectrum.cpp
    static constexpr Float g_lambda[SMITS_SAMPLES] = {
        380.000000f, 390.967743f, 401.935486f, 412.903229f, 423.870972f, 434.838715f,
        445.806458f, 456.774200f, 467.741943f, 478.709686f, 489.677429f, 500.645172f,
        511.612915f, 522.580627f, 533.548340f, 544.516052f, 555.483765f, 566.451477f,
        577.419189f, 588.386902f, 599.354614f, 610.322327f, 621.290039f, 632.257751f,
        643.225464f, 654.193176f, 665.160889f, 676.128601f, 687.096313f, 698.064026f,
        709.031738f, 720.000000f
    };

    static constexpr Float g_white[SMITS_SAMPLES] = {
        1.0618958571f, 1.0615019980f, 1.0614335380f, 1.0622711655f,
        1.0622036218f, 1.0625059965f, 1.0623938487f, 1.0624706448f,
        1.0625048145f, 1.0624366131f, 1.0620694239f, 1.0613167587f,
        1.0610334029f, 1.0613868565f, 1.0614215366f, 1.0620336151f,
        1.0625497455f, 1.0624317488f, 1.0625249141f, 1.0624277664f,
        1.0624749854f, 1.0625538581f, 1.0625326910f, 1.0623922312f,
        1.0623650980f, 1.0625256477f, 1.0612277620f, 1.0594262609f,
        1.0599810758f, 1.0602547314f, 1.0601263046f, 1.0606565757f
    };

    static constexpr Float g_cyan[SMITS_SAMPLES] = {
        1.0414628021f, 1.0328661534f, 1.0126146229f, 1.0350460525f,
        1.0078661447f, 1.0422280385f, 1.0442596738f, 1.0535238290f,
        1.0180776227f, 1.0442729909f, 1.0529362542f, 1.0537034271f,
        1.0533901869f, 1.0537782701f, 1.0527093770f, 1.0530449040f,
        1.0550554640f, 1.0553673611f, 1.0454306635f, 0.6234895064f,
        0.1803807161f, -0.0076303759f, -0.0001521785f, -0.0075102257f,
        -0.0021708639f, 0.0006591947f, 0.0122788153f, -0.0044669776f,
        0.0171197991f, 0.0049211090f, 0.0058762925f, 0.0252593994f
    };

    static constexpr Float g_magenta[SMITS_SAMPLES] = {
        0.9942213815f, 0.9898693712f, 0.9829365829f, 0.9962786840f,
        1.0198955019f, 1.0166395501f, 1.0220913179f, 0.9965166604f,
        1.0097766179f, 1.0215422471f, 0.6403195339f, 0.0025012379f,
        0.0065339940f, 0.0028334080f, -0.0000000001f, -0.0090592292f,
        0.0033936718f, -0.0030638741f, 0.2220393617f, 0.6314114002f,
        0.9748098558f, 0.9720956233f, 1.0173770303f, 0.9987519432f,
        0.9470172574f, 0.8525862315f, 0.9489779858f, 0.9475187610f,
        0.9959894419f, 0.8630135150f, 0.8915098785f, 0.8486649265f
    };

    static constexpr Float g_yellow[SMITS_SAMPLES] = {
        0.0055740623f, -0.0047982832f, -0.0052536564f, -0.0064571480f,
        -0.0059693515f, -0.0021836716f, 0.0167811206f, 0.0960963554f,
        0.2121735708f, 0.3616913329f, 0.5396101154f, 0.7440881049f,
        0.9220957115f, 1.0460304298f, 1.0513824989f, 1.0511991822f,
        1.0510530912f, 1.0517397230f, 1.0516043087f, 1.0511944032f,
        1.0511590326f, 1.0516612465f, 1.0514038527f, 1.0515941029f,
        1.0511460437f, 1.0515123759f, 1.0508871370f, 1.0508923708f,
        1.0477492816f, 1.0493272144f, 1.0435963333f, 1.0392280772f
    };

    static constexpr Float g_red[SMITS_SAMPLES] = {
        0.1657560487f, 0.1184644280f, 0.1240829333f, 0.1137127206f,
        0.0789924345f, 0.0322056036f, -0.0107983654f, 0.0180519755f,
        0.0053407197f, 0.0136549187f, -0.0059564214f, -0.0018444365f,
        -0.0105718844f, -0.0029375521f, -0.0107904763f, -0.0080224307f,
        -0.0022669168f, 0.0070200240f, -0.0081528469f, 0.6077286697f,
        0.9883156087f, 0.9939169104f, 1.0039338995f, 0.9923449986f,
        0.9992653086f, 1.0084621558f, 0.9835829683f, 1.0085023660f,
        0.9745113833f, 0.9854326957f, 0.9349576398f, 0.9871390779f
    };

    static constexpr Float g_green[SMITS_SAMPLES] = {
        0.0026494154f, -0.0050175013f, -0.0125472363f, -0.0094554964f,
        -0.0125260862f, -0.0079170698f, -0.0079955735f, -0.0093559433f,
        0.0654686120f, 0.3957287552f, 0.7524402230f, 0.9637647869f,
        0.9985443386f, 0.9999297703f, 0.9993908675f, 0.9999437227f,
        0.9993912181f, 0.9991123731f, 0.9601958488f, 0.6318627934f,
        0.2579740103f, 0.0094014889f, -0.0030798346f, -0.0045230367f,
        -0.0068933410f, -0.0090352196f, -0.0085913667f, -0.0083690869f,
        -0.0078685832f, -0.0000083658f, 0.0054301225f, -0.0027745590f
    };

    static constexpr Float g_blue[SMITS_SAMPLES] = {
        0.9920977147f, 0.9887642606f, 0.9953904074f, 0.9952931735f,
        0.9918144741f, 1.0002584040f, 0.9996847844f, 0.9998812077f,
        0.9850401215f, 0.7902984905f, 0.5608219862f, 0.3313345851f,
        0.1369241084f, 0.0189149066f, -0.0000051130f, -0.0004239549f,
        -0.0004193459f, 0.0017473028f, 0.0037999160f, -0.0005510147f,
        -0.0000437167f, 0.0075874502f, 0.0257956508f, 0.0381683765f,
        0.0494895864f, 0.0495959923f, 0.0498148195f, 0.0398409111f,
        0.0305010249f, 0.0212430548f, 0.0069596532f, 0.0041733649f
    };

    static Float lookupBasis(const Float* basis, const Float wavelength) noexcept
    {
        const Float clamped = clamp(wavelength, SMITS_LAMBDA_MIN, SMITS_LAMBDA_MAX);
        const Float step = (SMITS_LAMBDA_MAX - SMITS_LAMBDA_MIN) / static_cast<Float>(SMITS_SAMPLES - 1);
        const Float idx = (clamped - SMITS_LAMBDA_MIN) / step;
        const ASizeT i0 = min(static_cast<ASizeT>(idx), SMITS_SAMPLES - 2);
        const Float frac = idx - static_cast<Float>(i0);
        return mix(basis[i0], basis[i0 + 1], frac);
    }

    Float8 rgbToSpectrum(const Float4& rgb, const Float8& wavelengths) noexcept
    {
        const Float r = x(rgb), g = y(rgb), b = z(rgb);
        const Float* basis1;
        const Float* basis2;
        const Float* basis3;
        Float w1, w2, w3;

        if (r <= g && r <= b)
        {
            w1 = r;
            if (g <= b)
            {
                w2 = g - r;
                w3 = b - g;
                basis2 = g_cyan;
                basis3 = g_blue;
            }
            else
            {
                w2 = b - r;
                w3 = g - b;
                basis2 = g_cyan;
                basis3 = g_green;
            }
        }
        else if (g <= r && g <= b)
        {
            w1 = g;
            if (r <= b)
            {
                w2 = r - g;
                w3 = b - r;
                basis2 = g_magenta;
                basis3 = g_blue;
            }
            else
            {
                w2 = b - g;
                w3 = r - b;
                basis2 = g_magenta;
                basis3 = g_red;
            }
        }
        else
        {
            w1 = b;
            if (r <= g)
            {
                w2 = r - b;
                w3 = g - r;
                basis2 = g_yellow;
                basis3 = g_green;
            }
            else
            {
                w2 = g - b;
                w3 = r - g;
                basis2 = g_yellow;
                basis3 = g_red;
            }
        }

        basis1 = g_white;

        const Float wl[SPECTRAL_SAMPLES] = {
            x1(wavelengths), x2(wavelengths), x3(wavelengths), x4(wavelengths),
            x5(wavelengths), x6(wavelengths), x7(wavelengths), x8(wavelengths)
        };

        Float result[SPECTRAL_SAMPLES]{};
        for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
        {
            const Float s = w1 * lookupBasis(basis1, wl[i])
                + w2 * lookupBasis(basis2, wl[i])
                + w3 * lookupBasis(basis3, wl[i]);
            result[i] = max(Zero<Float>(), s);
        }

        return Float8(result);
    }
}
