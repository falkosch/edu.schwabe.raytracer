#include "raytracing/common/Tools.h"
#include "../../stdafx.h"

#include <Windows.h>

namespace raytracer
{

    const UInt_64 Tools::seed()
    {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        return static_cast<UInt_64>(counter.HighPart ^ counter.LowPart);
    }

}
