#include "vectorization/constants/values/epsilon.h"

#include "vectorization/constants/values/half.h"
#include "vectorization/constants/values/one.h"

#include <logging.h>

#include <sstream>
#include <string>

static const auto Log = logging::scope("Epsilon");

namespace vectorization
{
    template <typename T>
    T benchmarkMachineEpsilon() noexcept
    {
        T f = One<T>();
        T e = f;
        while (f + One<T>() != One<T>())
        {
            e = f;
            f *= Half<T>();
        }
        Log.info([name = std::string(typeid(T).name()), e]
        {
            // std::to_string uses fixed notation with 6 decimals, which prints these as 0.000000
            std::ostringstream message;
            message << "Machine epsilon for " << name << " is " << e;
            return message.str();
        });
        return e;
    }

    const Float_32 Epsilon_Float_32 = benchmarkMachineEpsilon<Float_32>();
    const Float_64 Epsilon_Float_64 = benchmarkMachineEpsilon<Float_64>();

    template <>
    Float_32 Epsilon<Float_32>() noexcept
    {
        return Epsilon_Float_32;
    }

    template <>
    Float_64 Epsilon<Float_64>() noexcept
    {
        return Epsilon_Float_64;
    }

    template <>
    PackedFloat2_128 Epsilon<PackedFloat2_128>() noexcept
    {
        return _mm_set1_pd(Epsilon_Float_64);
    }

    template <>
    PackedFloat4_128 Epsilon<PackedFloat4_128>() noexcept
    {
        return _mm_set1_ps(Epsilon_Float_32);
    }

    template <>
    PackedFloat4_256 Epsilon<PackedFloat4_256>() noexcept
    {
        return _mm256_set1_pd(Epsilon_Float_64);
    }

    template <>
    PackedFloat8_256 Epsilon<PackedFloat8_256>() noexcept
    {
        return _mm256_set1_ps(Epsilon_Float_32);
    }
}
