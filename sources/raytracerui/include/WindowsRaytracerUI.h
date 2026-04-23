#pragma once

#include "MessageLoopBasedUI.h"
#include "RaytracerUI.h"

namespace raytracerui
{
  using namespace vectorization;
  using namespace raytracer;

  enum class MouseButtons : ASizeT { Left, Right };

  enum class ButtonStates : ASizeT { Down, Up };

  enum class DragTypes : ASizeT { None, Rotate, ShiftXY, ShiftZ, Scale, Light, Object };

  class WindowsRaytracerUI : public RaytracerUI, public MessageLoopBasedUI
  {
    static const Float MOUSE_SENSITIVITY;

    static WNDCLASSEX windowClass;

    Int2 previousMousePosition;

    DragTypes activeDrag;

    LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);

    void delegateReshape();

    void keyPressed(WPARAM wParam);

    void mousePressed(MouseButtons button, ButtonStates state, const Int2& position);

    void mouseDragged(const Int2& position);

    void InitWindow();

    static void InitWindowClass();

    static LRESULT CALLBACK DelegatingWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    static Int2 getXY(LPARAM lParam);

  protected:
    static constexpr UINT WM_RENDER_COMPLETE = WM_APP + 1;

    HWND hWnd;

    void triggerRaytracing(bool fastPreview) override;

    virtual void onTogglePanel(WPARAM key);

    virtual void display();

    virtual void repaint();

    virtual void onRenderComplete();

    virtual LRESULT forwardInputMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

  public:
    WindowsRaytracerUI(Raytracer& raytracer, const RaytraceParameters& parameters, ASizeT& fastPreviewSize);

    ~WindowsRaytracerUI() override;

    WPARAM run() override;
  };
}
