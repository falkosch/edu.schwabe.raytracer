#include "raytracing/shading/brdf/SpecularAlbedo.h"

#include "raytracing/common/RefractionIndices.h"
#include "raytracing/shading/brdf/fresnel_reflectance.h"
#include "raytracing/shading/brdf/ggx.h"
#include "raytracing/shading/sampling.h"

namespace raytracer::brdf
{
    using vectorization::max;
    using vectorization::min;
    using vectorization::sqrt;

    namespace
    {
        // VNDF samples per entry, and sqrt(R0) of the largest index ratio the dielectric tables hold (3)
        constexpr ASizeT SchlickSamples = 1024;
        constexpr ASizeT DielectricSamples = 256;
        constexpr Float MaxIndexRoot = 0.5f;

        constexpr auto CosineSteps = SpecularAlbedo::CosineSteps;
        constexpr auto RoughnessSteps = SpecularAlbedo::RoughnessSteps;
        constexpr auto IndexSteps = SpecularAlbedo::IndexSteps;

        Float cosineOf(const ASizeT i)
        {
            return (static_cast<Float>(i) + Half<Float>()) / static_cast<Float>(CosineSteps);
        }

        Float roughnessOf(const ASizeT j)
        {
            return static_cast<Float>(j) / static_cast<Float>(RoughnessSteps - One<ASizeT>());
        }

        Float indexRootOf(const ASizeT k)
        {
            return MaxIndexRoot * static_cast<Float>(k) / static_cast<Float>(IndexSteps - One<ASizeT>());
        }

        // A position on a grid of the steps: the neighbouring grid points and the weight of the upper one
        struct GridPosition
        {
            ASizeT lower;
            ASizeT upper;
            Float weight;
        };

        GridPosition gridPosition(const Float position, const ASizeT steps)
        {
            const auto last = static_cast<Float>(steps - One<ASizeT>());
            const auto clamped = clamp(position, Zero<Float>(), last);
            const auto lower = static_cast<ASizeT>(clamped);
            return {lower, min(lower + One<ASizeT>(), steps - One<ASizeT>()), clamped - static_cast<Float>(lower)};
        }

        GridPosition cosinePosition(const Float cosine)
        {
            return gridPosition(cosine * static_cast<Float>(CosineSteps) - Half<Float>(), CosineSteps);
        }

        GridPosition roughnessPosition(const Float roughness)
        {
            return gridPosition(roughness * static_cast<Float>(RoughnessSteps - One<ASizeT>()), RoughnessSteps);
        }

        // sqrt(R0) of the indices, |n1 - n2| / (n1 + n2), on the index grid
        GridPosition indexPosition(const Float4& eta)
        {
            const auto ratio = x(eta);
            const auto root = vectorization::abs(One<Float>() - ratio) / (One<Float>() + ratio);
            return gridPosition(
                root / MaxIndexRoot * static_cast<Float>(IndexSteps - One<ASizeT>()), IndexSteps
            );
        }

        // The direction at the cosine to the normal z, in the xz-plane
        Float4 directionAt(const Float cosine)
        {
            return Float4(sqrt(max(Zero<Float>(), One<Float>() - sqr(cosine))), Zero<Float>(), cosine, Zero<Float>());
        }

        // A table entry's lanes for the viewer's side: (E_R, E_T, 0, 0) from (entering, leaving, entering, leaving)
        Float4 sideLanes(const Float4& entry, const Float4& eta)
        {
            const auto entering = swizzle<0, 2, 0, 2>(entry);
            const auto leaving = swizzle<1, 3, 1, 3>(entry);
            const auto lanes = select(xxxx(eta) > One<Float4>(), leaving, entering);
            return blend<false, false, true, true>(lanes, Zero<Float4>());
        }

        // The cosine-weighted means over the hemisphere of the rows of cosines: 2 sum(E(mu) mu) / steps (midpoints)
        void averageRows(
            const std::vector<Float4, AlignedAllocator<Float4>>& table,
            std::vector<Float4, AlignedAllocator<Float4>>& averages
        )
        {
            for (auto row = Zero<ASizeT>(); row < averages.size(); ++row)
            {
                auto sum = Zero<Float4>();
                for (auto i = Zero<ASizeT>(); i < CosineSteps; ++i)
                {
                    sum = multiplyAdd(table[row * CosineSteps + i], Float4(cosineOf(i)), sum);
                }
                averages[row] = sum * Float4(Two<Float>() / static_cast<Float>(CosineSteps));
            }
        }
    }

    const SpecularAlbedo& SpecularAlbedo::tables()
    {
        static const SpecularAlbedo instance;
        return instance;
    }

    SpecularAlbedo::SpecularAlbedo()
        : schlickTable(RoughnessSteps * CosineSteps), averageSchlickTable(RoughnessSteps),
          dielectricTable(IndexSteps * RoughnessSteps * CosineSteps),
          averageDielectricTable(IndexSteps * RoughnessSteps)
    {
        const auto normal = OneZ<Float4>();
        const auto offset = sequenceOffset2D(Zero<UInt_32>());

        // Schlick's lobe: each VNDF sample reflects G2 / G1(V) (the BRDF times the cosine per the samples' density),
        // times 1 or (1 - cos)^5 of its microfacet
#pragma omp parallel for schedule(dynamic, 8)
        for (int entry = 0; entry < static_cast<int>(schlickTable.size()); ++entry)
        {
            const auto index = static_cast<ASizeT>(entry);
            const auto cosine = cosineOf(index % CosineSteps);
            const auto roughness = roughnessOf(index / CosineSteps);
            const auto V = directionAt(cosine);
            const auto G1 = smithG1(cosine, roughness);
            auto fresnelOne = Zero<Float_64>();
            auto schlickTerm = Zero<Float_64>();
            for (auto sample = Zero<ASizeT>(); sample < SchlickSamples; ++sample)
            {
                const auto microfacet = sampleGGXNormal(normal, V, sequencePoint2D(offset, sample), roughness);
                const auto L = reflect(-V, microfacet);
                const auto NoL = z(L);
                if (!(NoL > Zero<Float>()))
                {
                    continue;
                }
                const auto weight = ggxG_smith(cosine, NoL, roughness) / G1;
                const auto oneMinusCos = One<Float>() - max(dot3(V, microfacet), Zero<Float>());
                fresnelOne += static_cast<Float_64>(weight);
                schlickTerm += static_cast<Float_64>(weight * sqr(sqr(oneMinusCos)) * oneMinusCos);
            }
            const auto perSample = One<Float_64>() / static_cast<Float_64>(SchlickSamples);
            const auto E1 = fresnelOne * perSample;
            const auto E5 = schlickTerm * perSample;
            // the part of the compensated albedo that is quadratic in F0 and (1 - F0), for its mean
            const auto compensatedTerm = E1 > Zero<Float_64>() ? E5 * (One<Float_64>() - E1) / E1 : Zero<Float_64>();
            schlickTable[index] = Float4(
                static_cast<Float>(E1), static_cast<Float>(E5), static_cast<Float>(compensatedTerm), Zero<Float>()
            );
        }

        // A dielectric interface: each VNDF sample reflects its microfacet's Fresnel reflectance and refracts the
        // rest, both times G2 / G1(V) of their direction; a microfacet that reflects totally refracts nothing
#pragma omp parallel for schedule(dynamic, 8)
        for (int entry = 0; entry < static_cast<int>(dielectricTable.size()); ++entry)
        {
            const auto index = static_cast<ASizeT>(entry);
            const auto cosine = cosineOf(index % CosineSteps);
            const auto roughness = roughnessOf(index / CosineSteps % RoughnessSteps);
            const auto root = indexRootOf(index / (CosineSteps * RoughnessSteps));
            const auto denser = (One<Float>() + root) / (One<Float>() - root);
            const auto V = directionAt(cosine);
            const auto G1 = smithG1(cosine, roughness);
            const Float4 sides[2] = {
                RefractionIndices::refractionEta(One<Float>(), denser),
                RefractionIndices::refractionEta(denser, One<Float>())
            };
            Float_64 reflected[2] = {Zero<Float_64>(), Zero<Float_64>()};
            Float_64 transmitted[2] = {Zero<Float_64>(), Zero<Float_64>()};
            for (auto sample = Zero<ASizeT>(); sample < DielectricSamples; ++sample)
            {
                const auto microfacet = sampleGGXNormal(normal, V, sequencePoint2D(offset, sample), roughness);
                const auto VoH = dotv(V, microfacet);
                const auto L = reflect(-V, microfacet);
                const auto NoL = z(L);
                for (auto side = 0; side < 2; ++side)
                {
                    const auto& eta = sides[side];
                    const auto F = x(fresnelReflectance(VoH, eta));
                    if (NoL > Zero<Float>())
                    {
                        reflected[side] += static_cast<Float_64>(F * ggxG_smith(cosine, NoL, roughness) / G1);
                    }
                    const auto T = refractEta(-V, microfacet, -VoH, xxxx(eta));
                    const auto NoT = -z(T);
                    if (!allTrue3(!T) && NoT > Zero<Float>())
                    {
                        transmitted[side] +=
                            static_cast<Float_64>((One<Float>() - F) * ggxG_smith(cosine, NoT, roughness) / G1);
                    }
                }
            }
            const auto perSample = One<Float_64>() / static_cast<Float_64>(DielectricSamples);
            dielectricTable[index] = Float4(
                static_cast<Float>(reflected[0] * perSample), static_cast<Float>(reflected[1] * perSample),
                static_cast<Float>(transmitted[0] * perSample), static_cast<Float>(transmitted[1] * perSample)
            );
        }

        averageRows(schlickTable, averageSchlickTable);
        // the dielectric lobes' means with the multiple scattering compensated: each side's lobes over their sum
        auto compensated = dielectricTable;
        for (auto& entry : compensated)
        {
            const auto sum = entry + swizzle<2, 3, 0, 1>(entry);
            entry = select(sum > Zero<Float4>(), entry / sum, Zero<Float4>());
        }
        averageRows(compensated, averageDielectricTable);
    }

    Float4 SpecularAlbedo::schlick(const Float cosine, const Float roughness) const noexcept
    {
        const auto c = cosinePosition(cosine);
        const auto r = roughnessPosition(roughness);
        const auto row = [&](const ASizeT j)
        {
            return mix(schlickTable[j * CosineSteps + c.lower], schlickTable[j * CosineSteps + c.upper], c.weight);
        };
        return mix(row(r.lower), row(r.upper), r.weight);
    }

    Float4 SpecularAlbedo::averageSchlick(const Float roughness) const noexcept
    {
        const auto r = roughnessPosition(roughness);
        return mix(averageSchlickTable[r.lower], averageSchlickTable[r.upper], r.weight);
    }

    Float4 SpecularAlbedo::dielectric(const Float cosine, const Float roughness, const Float4& eta) const noexcept
    {
        const auto c = cosinePosition(cosine);
        const auto r = roughnessPosition(roughness);
        const auto n = indexPosition(eta);
        const auto row = [&](const ASizeT k, const ASizeT j)
        {
            const auto start = (k * RoughnessSteps + j) * CosineSteps;
            return mix(dielectricTable[start + c.lower], dielectricTable[start + c.upper], c.weight);
        };
        const auto plane = [&](const ASizeT k)
        {
            return mix(row(k, r.lower), row(k, r.upper), r.weight);
        };
        return sideLanes(mix(plane(n.lower), plane(n.upper), n.weight), eta);
    }

    Float4 SpecularAlbedo::averageDielectric(const Float roughness, const Float4& eta) const noexcept
    {
        const auto r = roughnessPosition(roughness);
        const auto n = indexPosition(eta);
        const auto plane = [&](const ASizeT k)
        {
            const auto start = k * RoughnessSteps;
            return mix(averageDielectricTable[start + r.lower], averageDielectricTable[start + r.upper], r.weight);
        };
        return sideLanes(mix(plane(n.lower), plane(n.upper), n.weight), eta);
    }
}
