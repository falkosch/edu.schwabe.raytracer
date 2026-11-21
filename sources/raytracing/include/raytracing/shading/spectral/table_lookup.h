#pragma once

#include <vectorization.h>

namespace raytracer::spectral
{
    using namespace vectorization;

    // Position of each wavelength lane on a uniform wavelength grid. Computing it once lets several tables that share
    // the grid (the three CMFs, the seven Smits basis spectra) be interpolated at the same wavelengths.
    struct TableCoordinates
    {
        // Lower grid index per lane, as a float
        Float8 index;
        // Interpolation weight between the lower and the upper grid entry
        Float8 fraction;
    };

    // Locates the wavelengths on the grid tableMin + i * tableStep, i in [0, tableSize). Wavelengths outside the grid
    // are clamped to its first or last entry.
    TableCoordinates tableCoordinates(const Float8& wavelengths, Float tableMin, Float tableStep,
                                      ASizeT tableSize) noexcept;

    // Linearly interpolates the table at the given grid coordinates.
    Float8 lookupTable(const Float* table, const TableCoordinates& coordinates) noexcept;
}
