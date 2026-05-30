#include "raytracing_tests.h"
#include "raytracing/shading/spectral/metal_ior.h"
#include "raytracing/shading/spectral/wavelengths.h"

#include <vectorization.h>

namespace raytracer::test
{
    using namespace raytracer::spectral;

    TEST_CLASS (MetalIORTest)
    {
        public
        :
        TEST_METHOD(allMetalsHavePositiveN)
        {
            const Metal metals[] = {Metal::Cu, Metal::Au, Metal::Al, Metal::Fe, Metal::Ag};
            for (const auto metal : metals)
            {
                const auto w = wavelengthsAt(380.0f);
                const auto ior = metalIOR(metal, w);
                const Float nvals[] = {
                    x1(ior.n), x2(ior.n), x3(ior.n), x4(ior.n),
                    x5(ior.n), x6(ior.n), x7(ior.n), x8(ior.n)
                };
                for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
                {
                    Assert::IsTrue(nvals[i] > 0.0f, L"n > 0", LINE_INFO());
                }
            }
        }

        TEST_METHOD(allMetalsHaveNonNegativeK)
        {
            const Metal metals[] = {Metal::Cu, Metal::Au, Metal::Al, Metal::Fe, Metal::Ag};
            for (const auto metal : metals)
            {
                const auto w = wavelengthsAt(380.0f);
                const auto ior = metalIOR(metal, w);
                const Float kvals[] = {
                    x1(ior.k), x2(ior.k), x3(ior.k), x4(ior.k),
                    x5(ior.k), x6(ior.k), x7(ior.k), x8(ior.k)
                };
                for (ASizeT i = 0; i < SPECTRAL_SAMPLES; ++i)
                {
                    Assert::IsTrue(kvals[i] >= 0.0f, L"k >= 0", LINE_INFO());
                }
            }
        }

        TEST_METHOD(silverHasLowNInVisible)
        {
            const auto w = wavelengthsAt(430.0f);
            const auto ior = metalIOR(Metal::Ag, w);
            Assert::IsTrue(x1(ior.n) < 0.5f, L"Ag n low at 430nm", LINE_INFO());
            Assert::IsTrue(x2(ior.n) < 0.5f, L"Ag n low at 480nm", LINE_INFO());
        }

        TEST_METHOD(constantAcrossWavelengths)
        {
            const auto w = wavelengthsAt(380.0f);
            const auto ior = metalIOR(Metal::Cu, w);
            Assert::AreEqual(x1(ior.n), x4(ior.n), 1e-6f, L"Cu n constant across lanes", LINE_INFO());
            Assert::AreEqual(x1(ior.k), x7(ior.k), 1e-6f, L"Cu k constant across spectral lanes", LINE_INFO());
        }

        TEST_METHOD(aluminumHasHighExtinction)
        {
            const auto w = wavelengthsAt(500.0f);
            const auto ior = metalIOR(Metal::Al, w);
            Assert::IsTrue(x1(ior.k) > 5.0f, L"Al has high extinction", LINE_INFO());
        }
    };
}
