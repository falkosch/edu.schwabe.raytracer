#pragma once

#include "MessageLoopBasedUI.h"
#include "RaytracerUI.h"

namespace raytracerui
{
  using namespace vectorization;
  using namespace raytracer;

  enum class MouseButtons : ASizeT { Left, Right };

  enum class ButtonStates : ASizeT { Down, Up };

  enum class DragTypes : ASizeT { None, Rotate, Scale, Light, Object };

  enum class InteractionMode : ASizeT { Camera, Object, Light };

  class WindowsRaytracerUI : public RaytracerUI, public MessageLoopBasedUI
  {
    static const Float MOUSE_SENSITIVITY;

    static constexpr Float CAMERA_STEP = 0.05f;

    static constexpr UINT_PTR MOVEMENT_TIMER_ID = 1;

    static WNDCLASSEX windowClass;

    Int2 previousMousePosition;

    DragTypes activeDrag;

    bool movementTimerActive{false};

    bool keyDown[256]{};

    LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);

    void delegateReshape();

    void keyPressed(WPARAM wParam);

    void mousePressed(MouseButtons button, ButtonStates state, const Int2& position);

    void mouseDragged(const Int2& position);

    void updateMovement();

    void InitWindow();

    static void InitWindowClass();

    static LRESULT CALLBACK DelegatingWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    static Int2 getXY(LPARAM lParam);

  protected:
    static constexpr UINT WM_RENDER_COMPLETE = WM_APP + 1;

    HWND hWnd;

    InteractionMode interactionMode{InteractionMode::Camera};

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
