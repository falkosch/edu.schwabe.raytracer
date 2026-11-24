#pragma once

#include "ButtonStates.h"
#include "DragTypes.h"
#include "InteractionMode.h"
#include "MessageLoopBasedUI.h"
#include "MouseButtons.h"
#include "RaytracerUI.h"

namespace raytracerui
{
    using namespace vectorization;
    using namespace raytracer;

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

        bool exitAfterFirstImage{false};

        LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);

        void delegateReshape();

        void keyPressed(WPARAM wParam);

        void mousePressed(MouseButtons button, ButtonStates state, const Int2& position);

        void mouseDragged(const Int2& position);

        void updateMovement();

        // Creates the window and shows it, or with hidden starts the first render as its WM_SIZE would
        void InitWindow(bool hidden);

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
        // A hidden window shows nothing (--exit-after-render: a render for a script)
        WindowsRaytracerUI(
            Raytracer& raytracer, const RaytraceParameters& parameters, const Size2& fastPreviewSize, bool hidden
        );

        ~WindowsRaytracerUI() override;

        // Closes the (hidden) window once the first image is rendered and saved (--exit-after-render).
        void setExitAfterFirstImage(bool value);

        WPARAM run() override;
    };
}
