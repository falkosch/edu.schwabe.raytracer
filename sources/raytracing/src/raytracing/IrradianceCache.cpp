#include "raytracing/IrradianceCache.h"
#include "../stdafx.h"

#include "raytracing/shading/SpectralContext.h"

#include <cassert>

namespace raytracer
{
    namespace
    {
        // the least error estimate a weight takes, so that a query at a record's point stays finite
        constexpr Float LeastError = 1e-3f;

        // how far a point may lie behind a record (Ward's in-front test), relative to the record's radius
        constexpr Float BehindTolerance = 0.05f;

        // the weight of the normals' term in the error estimate: half of Ward's, so that a record is valid up to 16
        // instead of 8 degrees from its normal at a = 0.1. With Ward's, records on curved surfaces (a sphere in a
        // preview) covered too few pixels for the finest grid, and the hits between them traced their own rays.
        constexpr Float NormalWeight = 0.5f;

        // the bits per axis of a cell key, and the offset that keeps the cell coordinates positive
        constexpr UInt_64 KeyBits = 21;
        constexpr Int_32 KeyOffset = 1 << 20;
    }

    IrradianceCache::IrradianceCache(
        const SpectralContext* const contextsIn, const ASizeT contextCountIn, const ASizeT contextsPerSetIn,
        const Float accuracyIn
    )
        : contexts(contextsIn), contextCount(contextCountIn), contextsPerSet(contextsPerSetIn), accuracy(accuracyIn),
          records(contextCountIn / contextsPerSetIn), levels(contextCountIn / contextsPerSetIn)
    {
        assert(contextsPerSet > Zero<ASizeT>() && contextsPerSet <= spectral::SPECTRAL_SAMPLES);
        assert(contextCount % contextsPerSet == Zero<ASizeT>());
    }

    ASizeT IrradianceCache::contextIndex(const SpectralContext* const context) const noexcept
    {
        if (context < contexts || context >= contexts + contextCount)
        {
            return contextCount;
        }
        return static_cast<ASizeT>(context - contexts);
    }

    ASizeT IrradianceCache::getContextCount() const noexcept
    {
        return contextCount;
    }

    ASizeT IrradianceCache::getSetCount() const noexcept
    {
        return records.size();
    }

    Float IrradianceCache::getAccuracy() const noexcept
    {
        return accuracy;
    }

    ASizeT IrradianceCache::size() const noexcept
    {
        auto count = Zero<ASizeT>();
        for (const auto& setRecords : records)
        {
            count += setRecords.size();
        }
        return count;
    }

    UInt_64 IrradianceCache::cellKey(const Int4& cell) noexcept
    {
        const auto offset = cell + Int4(KeyOffset);
        return static_cast<UInt_64>(x(offset)) | (static_cast<UInt_64>(y(offset)) << KeyBits)
            | (static_cast<UInt_64>(z(offset)) << (KeyBits + KeyBits));
    }

    Float IrradianceCache::interpolate(
        const ASizeT context, const Float4& position, const Float4& normal, spectral::Spectrum& irradiance
    ) const noexcept
    {
        const auto set = context / contextsPerSet;
        const auto first = set * contextsPerSet;
        const auto& setRecords = records[set];
        const auto inverseAccuracy = reciprocal(accuracy);
        // the weighted sums of the records of each context of the set, and the contexts that have any
        alignas(YMM_ALIGNMENT) spectral::SpectralVector sums[spectral::SPECTRAL_SAMPLES];
        for (auto i = Zero<ASizeT>(); i < contextsPerSet; ++i)
        {
            sums[i] = Zero<spectral::SpectralVector>();
        }
        auto summed = Zero<UInt_32>();
        auto weightSum = Zero<Float>();
        for (const auto& level : levels[set])
        {
            const auto cell = level.cells.find(cellKey(convert<Int4>(floor(position / Float4(level.cellSize)))));
            if (cell == level.cells.end())
            {
                continue;
            }
            for (const auto index : cell->second)
            {
                const auto& record = setRecords[index];
                const auto radius = record.validRadius();
                const auto offset = position - record.position;
                // a record in front of the point (it lies behind the record's surface) doesn't apply
                if (dot3(offset, normal + record.normal) < -Two<Float>() * BehindTolerance * radius)
                {
                    continue;
                }
                const auto normalError =
                    vectorization::sqrt(max(Zero<Float>(), One<Float>() - dot3(normal, record.normal)));
                const auto error = length3(offset) / radius + NormalWeight * normalError;
                if (!(error < accuracy))
                {
                    continue;
                }
                const auto weight = reciprocal(max(error, LeastError)) - inverseAccuracy;
                const auto recordContext = record.context - first;
                sums[recordContext] =
                    multiplyAdd(record.irradiance.data, spectral::SpectralVector(weight), sums[recordContext]);
                summed |= One<UInt_32>() << recordContext;
                weightSum += weight;
            }
        }
        if (!(weightSum > Zero<Float>()))
        {
            return Zero<Float>();
        }
        // the other contexts' sums at this context's wavelengths
        const auto own = context - first;
        auto sum = sums[own];
        for (auto i = Zero<ASizeT>(); i < contextsPerSet; ++i)
        {
            if (i != own && (summed & (One<UInt_32>() << i)) != Zero<UInt_32>())
            {
                const auto steps =
                    (contexts[context].heroLambda - contexts[first + i].heroLambda) / spectral::DELTA_LAMBDA;
                sum += spectral::shiftWavelengths(spectral::Spectrum{sums[i]}, steps).data;
            }
        }
        irradiance = spectral::Spectrum{sum * spectral::SpectralVector(reciprocal(weightSum))};
        return weightSum * accuracy;
    }

    void IrradianceCache::clampByNeighbours(std::vector<IrradianceRecord>& setRecords)
    {
        auto radii = std::vector<Float>(setRecords.size());
#pragma omp parallel for schedule(dynamic, 64)
        for (int i = 0; i < static_cast<int>(setRecords.size()); ++i)
        {
            const auto& record = setRecords[static_cast<ASizeT>(i)];
            auto radius = record.radius;
            for (const auto& neighbour : setRecords)
            {
                // only a smaller neighbour nearer than the difference of the radii shrinks it: no square root for the
                // others
                const auto offset = record.position - neighbour.position;
                if (neighbour.radius < radius && dot3(offset, offset) < sqr(radius - neighbour.radius))
                {
                    radius = neighbour.radius + length3(offset);
                }
            }
            radii[static_cast<ASizeT>(i)] = radius;
        }
        for (auto i = Zero<ASizeT>(); i < setRecords.size(); ++i)
        {
            setRecords[i].radius = radii[i];
        }
    }

    void IrradianceCache::addLevel(const std::vector<std::vector<IrradianceRecord>>& levelRecords)
    {
        for (auto set = Zero<ASizeT>(); set < records.size(); ++set)
        {
            auto& setRecords = records[set];
            const auto first = setRecords.size();
            for (const auto& record : levelRecords[set])
            {
                if (record.radius > Zero<Float>())
                {
                    assert(record.context / contextsPerSet == set);
                    setRecords.push_back(record);
                }
            }
            if (setRecords.size() == first)
            {
                continue;
            }
            // the radii only shrink: the cells of the earlier levels still hold every record that reaches into them
            clampByNeighbours(setRecords);
            auto cellSize = Zero<Float>();
            for (auto index = first; index < setRecords.size(); ++index)
            {
                cellSize = max(cellSize, accuracy * setRecords[index].validRadius());
            }
            auto level = Level{cellSize, {}};
            for (auto index = first; index < setRecords.size(); ++index)
            {
                const auto& record = setRecords[index];
                const auto validity = Float4(accuracy * record.validRadius());
                const auto lower = convert<Int4>(floor((record.position - validity) / Float4(cellSize)));
                const auto upper = convert<Int4>(floor((record.position + validity) / Float4(cellSize)));
                for (auto cz = z(lower); cz <= z(upper); ++cz)
                {
                    for (auto cy = y(lower); cy <= y(upper); ++cy)
                    {
                        for (auto cx = x(lower); cx <= x(upper); ++cx)
                        {
                            level.cells[cellKey(Int4(cx, cy, cz, 0))].push_back(static_cast<UInt_32>(index));
                        }
                    }
                }
            }
            levels[set].push_back(std::move(level));
        }
    }
}
