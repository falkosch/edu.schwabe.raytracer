#pragma once

#include "IrradianceRecord.h"

#include <unordered_map>
#include <vector>

namespace raytracer
{
    using namespace vectorization;

    struct SpectralContext;

    // Irradiance caching (Ward et al. 1988): the irradiance of diffuse hits interpolated between records of it, in
    // place of their diffuse rays. A record i is valid at a point x of normal n where its error estimate
    // eps = |x - x_i| / R_i + 0.5 sqrt(1 - n . n_i) (half Ward's normal term) stays below the accuracy a and x doesn't
    // lie behind it (R_i capped at its maxRadius); it weighs 1 / eps - 1 / a, so that it fades out at the border of
    // its validity. The spectral contexts form sets of neighbouring hero wavelengths (a sub-sample's, which its pixels
    // take in turn), and each set has records of its own: a record keeps the context its irradiance is at, and a query
    // of another context of the set takes it at its own wavelengths, interpolated between the record's
    // (spectral::shiftWavelengths, a step of 50 nm apart). The records come in levels: each level has its own grid of
    // cells as large as the level's largest validity radius a R_i, so that a record lies in at most 8 cells and a query
    // reads one cell per level. Each level clamps the radii by the neighbours' (Krivanek et al. 2006):
    // R_i <= R_j + |x_i - x_j| for every record j of the set, so that a record whose rays misjudge its surroundings (a
    // point in a seam whose rays pass through the other surface) can't spread far beyond its neighbours. The clamp
    // takes the radii uncapped: a cap bounds its own record only. Adding records isn't thread-safe; interpolating is.
    class IrradianceCache
    {
    public:
        // Records for the contextCount spectral contexts from contexts on, in sets of contextsPerSet (at most
        // spectral::SPECTRAL_SAMPLES; hero wavelengths within a step of LAMBDA_MIN)
        IrradianceCache(const SpectralContext* contexts, ASizeT contextCount, ASizeT contextsPerSet, Float accuracy);

        // The index of the spectral context, or contextCount for one without records
        ASizeT contextIndex(const SpectralContext* context) const noexcept;

        ASizeT getContextCount() const noexcept;

        ASizeT getSetCount() const noexcept;

        Float getAccuracy() const noexcept;

        // The number of records of all contexts
        ASizeT size() const noexcept;

        // The irradiance at the point of the normal interpolated from the records of the context's set valid there
        // (at the context's wavelengths), and their coverage: the sum of their weights times the accuracy (1 for a
        // single record at half its validity radius); 0 where no record is valid (irradiance stays)
        Float interpolate(
            ASizeT context, const Float4& position, const Float4& normal, spectral::Spectrum& irradiance
        ) const noexcept;

        // Adds a level of records, those of each set (records of radius 0 are skipped; the others of a context of the
        // set), and clamps the radii of the set's records by their neighbours'
        void addLevel(const std::vector<std::vector<IrradianceRecord>>& levelRecords);

    private:
        // A level's grid: the cell size and the records (indices into the context's records) in each cell
        struct Level
        {
            Float cellSize;
            std::unordered_map<UInt_64, std::vector<UInt_32>> cells;
        };

        const SpectralContext* contexts;
        ASizeT contextCount;
        ASizeT contextsPerSet;
        Float accuracy;

        // per set
        std::vector<std::vector<IrradianceRecord>> records;
        std::vector<std::vector<Level>> levels;

        static UInt_64 cellKey(const Int4& cell) noexcept;

        // R_i <= R_j + |x_i - x_j| for every pair of the records
        static void clampByNeighbours(std::vector<IrradianceRecord>& setRecords);
    };
}
