#pragma once

#include "RaytracerUI.h"
#include "MessageLoopBasedUI.h"

namespace raytracerui
{
    using namespace vectorization;
    using namespace raytracer;

    enum class MouseButtons : ASizeT
    {
        Left, Right
    };

    enum class ButtonStates : ASizeT
    {
        Down, Up
    };

    enum class DragTypes : ASizeT
    {
        None,
        Rotate,
        ShiftXY,
        ShiftZ,
        Scale,
        Light,
        Object
    };

    class WindowsRaytracerUI : public RaytracerUI, public MessageLoopBasedUI
    {
        static const Float MOUSE_SENSITIVITY;

        static WNDCLASSEX windowClass;

        Int2 previousMousePosition;

        DragTypes activeDrag;

        const LRESULT WndProc(const UINT message, const WPARAM wParam, const LPARAM lParam);

        void delegateReshape();

        void triggerRaytracing(const bool fastPreview);

        void keyPressed(const WPARAM wParam);

        void mousePressed(const MouseButtons button, const ButtonStates state, const Int2 & position);

        void mouseDragged(const Int2 & position);

        void InitWindow();

        static void InitWindowClass();

        static LRESULT CALLBACK DelegatingWndProc(const HWND hwnd, const UINT msg, const WPARAM wparam, const LPARAM lparam);

        static const Int2 getXY(const LPARAM lParam);

    protected:

        HWND hWnd;

        virtual void display();

    public:

        WindowsRaytracerUI(Raytracer & raytracer, const RaytraceParameters & parameters, const ASizeT & fastPreviewSize);

        virtual ~WindowsRaytracerUI();

        const WPARAM run();
    };
}
