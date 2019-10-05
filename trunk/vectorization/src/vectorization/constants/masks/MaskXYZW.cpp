#include "vectorization/constants/masks/MaskXYZW.h"

#include "vectorization/constants/masks/MaskAll.h"

namespace vectorization
{
    template <>
    const PackedFloat4_128 MaskXYZW<PackedFloat4_128>() noexcept {
        return MaskAll<PackedFloat4_128>();
    }
}