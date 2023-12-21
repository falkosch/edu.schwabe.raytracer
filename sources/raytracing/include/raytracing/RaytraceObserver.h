#pragma once

namespace raytracer {
  // forward declaration of RaytraceParameters for RaytraceObserver
  struct RaytraceConfiguration;

  class RaytraceObserver {
  public:
    virtual ~RaytraceObserver();

    virtual void notifyUpdate(const RaytraceConfiguration &raytraceConfiguration) = 0;
  };
}
