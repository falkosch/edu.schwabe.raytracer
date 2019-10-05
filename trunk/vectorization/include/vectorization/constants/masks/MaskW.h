#pragma once

#include "../../architecture.h"

namespace vectorization
{
    template <typename V>
    const V MaskW() noexcept;

    template <>
    const PackedFloat4_128 MaskW<PackedFloat4_128>() noexcept;
}
