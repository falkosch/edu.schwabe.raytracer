#pragma once

namespace raytracer
{

    // forward decleration of RaytraceParameters for RaytraceObserver
    struct RaytraceConfiguration;

    class RaytraceObserver
    {
    public:

        virtual ~RaytraceObserver();

        virtual void notifyUpdate(const RaytraceConfiguration & raytraceConfiguration) = 0;
    };

}
