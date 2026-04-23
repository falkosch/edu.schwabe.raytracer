#pragma once

#include "WindowsRaytracerUI.h"

#include <GL/glew.h>

#include <GL/gl.h>

#include <mutex>

namespace raytracerui
{
  using namespace vectorization;
  using namespace raytracer;

  class OpenGLWindowsRaytracerUI : public WindowsRaytracerUI
  {
    HDC hDC;

    HGLRC hGLRC;

    GLuint outputSurface;

    std::mutex pendingMutex;

    RaytraceConfiguration pendingConfig{};

    bool hasPendingResult{false};

    bool showControls{false};

    bool showMetrics{false};

    bool showConfig{false};

  protected:
    void reshape(const Int2& newSize) override;

    void display() override;

    void repaint() override;

    void onRenderComplete() override;

    void onTogglePanel(WPARAM key) override;

    LRESULT forwardInputMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

  private:
    void renderUI();

  public:

    OpenGLWindowsRaytracerUI(Raytracer& raytracer, const RaytraceParameters& parameters, ASizeT fastPreviewSize);

    ~OpenGLWindowsRaytracerUI() override;

    void notifyUpdate(const RaytraceConfiguration& configuration) override;
  };
}
