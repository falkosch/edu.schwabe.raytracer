#include "WindowsRaytracerUI.h"
#include "stdafx.h"

#include <logging.h>
#include <map>
#include <string>

#include <windowsx.h>

static const auto Log = logging::scope("UI");

namespace raytracerui
{
    const Float WindowsRaytracerUI::MOUSE_SENSITIVITY = 0.02f;

    std::map<HWND, WindowsRaytracerUI*> createUIs{};

    WNDCLASSEX WindowsRaytracerUI::windowClass{
        sizeof(WNDCLASSEX),
        CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
        DelegatingWndProc,
        0,
        0,
        nullptr,
        nullptr,
        nullptr,
        reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1),
        nullptr,
        TEXT("WindowsRaytracerUIWndClass"),
        nullptr
    };

    WindowsRaytracerUI::WindowsRaytracerUI(
        Raytracer& raytracerIn, const RaytraceParameters& parametersIn, const Size2& fastPreviewSizeIn
    )
        : RaytracerUI(raytracerIn, parametersIn, fastPreviewSizeIn), previousMousePosition(),
          activeDrag(DragTypes::None),
          hWnd()
    {
        InitWindow();
    }

    WindowsRaytracerUI::~WindowsRaytracerUI()
    {
        if (movementTimerActive) KillTimer(hWnd, MOVEMENT_TIMER_ID);
        createUIs.erase(this->hWnd);
        DestroyWindow(hWnd);
    }

    void WindowsRaytracerUI::InitWindowClass()
    {
        windowClass.hInstance = GetModuleHandle(nullptr);
        windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        RegisterClassEx(&windowClass);
    }

    void WindowsRaytracerUI::InitWindow()
    {
        const auto windowSize = Size2(16, 39) + fastPreviewSize;
        InitWindowClass();
        hWnd = CreateWindowEx(
            WS_EX_APPWINDOW, windowClass.lpszClassName, TEXT("WindowsRaytracerUI"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
            CW_USEDEFAULT, static_cast<int>(x(windowSize)), static_cast<int>(y(windowSize)), nullptr, nullptr,
            windowClass.hInstance, this);
        createUIs.insert({this->hWnd, this});
        ShowWindow(this->hWnd, SW_SHOWDEFAULT);
    }

    WPARAM WindowsRaytracerUI::run()
    {
        MSG msg{};
        BOOL bRet;
        while (0 != (bRet = GetMessage(&msg, hWnd, 0, 0)))
        {
            // catch GetMessage returns due to an error
            if (bRet == -1)
            {
                // no error handling here
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return msg.wParam;
    }

    void WindowsRaytracerUI::delegateReshape()
    {
        RECT rect{};
        GetClientRect(hWnd, &rect);
        reshape(Size2(rect.right, rect.bottom) - Size2(rect.left, rect.top));
    }

    void WindowsRaytracerUI::triggerRaytracing(const bool fastPreview)
    {
        RaytracerUI::triggerRaytracing(fastPreview);
        InvalidateRect(hWnd, nullptr, false);
    }

    void WindowsRaytracerUI::display()
    {
        raytracer->requestUpdate();

        if (output)
        {
            const auto bitmapInfo = output->getBITMAPINFO();
            auto [bmType, bmWidth, bmHeight, bmWidthBytes, bmPlanes, bmBitsPixel, bmBits] = output->getBITMAP();

            PAINTSTRUCT psPaint{};
            const auto hdc = BeginPaint(hWnd, &psPaint);
            StretchDIBits(
                hdc, int{}, int{}, static_cast<int>(x(screenSize)), static_cast<int>(y(screenSize)), int{}, int{},
                bmWidth, bmHeight, bmBits, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY
            );
            EndPaint(hWnd, &psPaint);
        }
    }

    void WindowsRaytracerUI::keyPressed(const WPARAM wParam)
    {
        switch (wParam)
        {
        case 'Q':
            parameters.cullingOrientation = ((parameters.cullingOrientation + 2) % 3) - 1;
            Log.info([v = parameters.cullingOrientation] { return "Culling orientation: " + std::to_string(v); });
            break;

        case 'E':
            showMapIndex = (showMapIndex + 1) % 3;
            break;

        case 'R':
            parameters.camera->setViewMatrix(initialViewMatrix);
            break;

        case 'T':
            disableFastPreview = !disableFastPreview;
            Log.info([v = disableFastPreview] { return "Fast preview disabled: " + std::to_string(v); });
            break;

        case 'G':
            parameters.rayPacketSize -= select(parameters.rayPacketSize > ASizeT{0}, ASizeT{1}, ASizeT{0});
            Log.info([v = parameters.rayPacketSize] { return "Ray packet size: " + std::to_string(v); });
            break;

        case 'H':
            parameters.rayPacketSize += ASizeT{1};
            Log.info([v = parameters.rayPacketSize] { return "Ray packet size: " + std::to_string(v); });
            break;

        case 'J':
            parameters.superSamplingFactor -= select(parameters.superSamplingFactor > ASizeT{0}, ASizeT{1}, ASizeT{0});
            Log.info([v = parameters.superSamplingFactor] { return "Super-sampling factor: " + std::to_string(v); });
            break;

        case 'K':
            parameters.superSamplingFactor += ASizeT{1};
            Log.info([v = parameters.superSamplingFactor] { return "Super-sampling factor: " + std::to_string(v); });
            break;

        case '1':
            interactionMode = InteractionMode::Camera;
            return;

        case '2':
            interactionMode = InteractionMode::Object;
            return;

        case '3':
            interactionMode = InteractionMode::Light;
            return;

        case VK_RETURN:
            triggerRaytracing(false);
            return;

        default:
            return;
        }

        triggerRaytracing(true);
    }

    void WindowsRaytracerUI::updateMovement()
    {
        auto move = Zero<Float3>();
        if (keyDown['W']) move = move + Float3(0, 0, CAMERA_STEP);
        if (keyDown['S']) move = move + Float3(0, 0, -CAMERA_STEP);
        if (keyDown['A']) move = move + Float3(CAMERA_STEP, 0, 0);
        if (keyDown['D']) move = move + Float3(-CAMERA_STEP, 0, 0);
        if (keyDown[VK_SPACE]) move = move + Float3(0, -CAMERA_STEP, 0);
        if (keyDown['C']) move = move + Float3(0, CAMERA_STEP, 0);

        if (!allTrue3(move == Zero<Float3>()))
        {
            parameters.camera->translate(move);
            triggerRaytracing(true);
        }
        else
        {
            KillTimer(hWnd, MOVEMENT_TIMER_ID);
            movementTimerActive = false;
        }
    }

    void WindowsRaytracerUI::mousePressed(const MouseButtons button, const ButtonStates state, const Int2& position)
    {
        previousMousePosition = position;

        if (state == ButtonStates::Up)
        {
            activeDrag = DragTypes::None;
            return;
        }

        switch (button)
        {
        case MouseButtons::Left:
            if (interactionMode == InteractionMode::Object)
                activeDrag = DragTypes::Scale;
            break;

        case MouseButtons::Right:
            switch (interactionMode)
            {
            case InteractionMode::Camera: activeDrag = DragTypes::Rotate;
                break;
            case InteractionMode::Object: activeDrag = DragTypes::Object;
                break;
            case InteractionMode::Light: activeDrag = DragTypes::Light;
                break;
            }
            break;

        default:
            break;
        }
    }

    void WindowsRaytracerUI::mouseDragged(const Int2& position)
    {
        const auto delta = convert<Float2>(position - previousMousePosition);
        previousMousePosition = position;

        switch (activeDrag)
        {
        case DragTypes::Rotate:
            parameters.camera->rotate(Float3(x(delta), y(delta)));
            triggerRaytracing(true);
            break;

        case DragTypes::Scale:
            {
                const auto scene = dynamic_cast<Scene*const>(parameters.sceneShader);
                const auto& sceneObjects = scene->getSceneObjects();
                const auto& lastSceneObject = sceneObjects.back();
                lastSceneObject->scale(Float3((x(delta) + y(delta)) * MOUSE_SENSITIVITY) + One<Float3>());
                triggerRaytracing(true);
                break;
            }

        case DragTypes::Light:
            {
                const auto& lights = parameters.sceneShader->getLights();
                const auto& lastLight = lights.back();
                lastLight->position += Float4(0.0f, 0.0f, x(delta) - y(delta)) * MOUSE_SENSITIVITY;
                triggerRaytracing(true);
                break;
            }

        case DragTypes::Object:
            {
                const auto scene = dynamic_cast<Scene*const>(parameters.sceneShader);
                const auto& sceneObjects = scene->getSceneObjects();
                const auto& lastSceneObject = sceneObjects.back();
                lastSceneObject->translate(Float3(0.0f, 0.0f, x(delta) - y(delta)) * MOUSE_SENSITIVITY);
                triggerRaytracing(true);
                break;
            }

        case DragTypes::None:
        default:
            break;
        }
    }

    LRESULT WindowsRaytracerUI::forwardInputMessage(HWND, UINT, WPARAM, LPARAM)
    {
        return 0;
    }

    LRESULT WindowsRaytracerUI::WndProc(const UINT message, const WPARAM wParam, const LPARAM lParam)
    {
        if (forwardInputMessage(hWnd, message, wParam, lParam))
            return LRESULT{1};

        switch (message)
        {
        case WM_CLOSE:
            PostQuitMessage(0);
            return DefWindowProc(hWnd, message, wParam, lParam);

        case WM_ERASEBKGND:
            break;

        case WM_PAINT:
            display();
            break;

        case WM_SIZE:
            delegateReshape();
            break;

        case WM_LBUTTONDOWN:
            mousePressed(MouseButtons::Left, ButtonStates::Down, getXY(lParam));
            break;

        case WM_LBUTTONUP:
            mousePressed(MouseButtons::Left, ButtonStates::Up, getXY(lParam));
            break;

        case WM_RBUTTONDOWN:
            mousePressed(MouseButtons::Right, ButtonStates::Down, getXY(lParam));
            break;

        case WM_RBUTTONUP:
            mousePressed(MouseButtons::Right, ButtonStates::Up, getXY(lParam));
            break;

        case WM_MOUSEMOVE:
            mouseDragged(getXY(lParam));
            break;

        case WM_KEYDOWN:
            keyDown[wParam & 0xFF] = true;
            if ((wParam == VK_F1 || wParam == VK_F2 || wParam == VK_F3 || wParam == VK_F4) && !(lParam & 0x40000000))
            {
                onTogglePanel(wParam);
            }
            else if (!movementTimerActive && (wParam == 'W' || wParam == 'A' || wParam == 'S' || wParam == 'D'
                || wParam == VK_SPACE || wParam == 'C'))
            {
                SetTimer(hWnd, MOVEMENT_TIMER_ID, 16, nullptr);
                movementTimerActive = true;
            }
            break;

        case WM_KEYUP:
            keyDown[wParam & 0xFF] = false;
            keyPressed(wParam);
            break;

        case WM_TIMER:
            if (wParam == MOVEMENT_TIMER_ID) updateMovement();
            break;

        case WM_RENDER_COMPLETE:
            onRenderComplete();
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        return LRESULT{0};
    }

    void WindowsRaytracerUI::onTogglePanel(WPARAM)
    {
    }

    void WindowsRaytracerUI::repaint()
    {
    }

    void WindowsRaytracerUI::onRenderComplete()
    {
    }

    LRESULT CALLBACK
    WindowsRaytracerUI::DelegatingWndProc(const HWND hwnd, const UINT msg, const WPARAM wparam, const LPARAM lparam)
    {
        if (const auto targetedUIIterator = createUIs.find(hwnd); targetedUIIterator != createUIs.end())
        {
            return targetedUIIterator->second->WndProc(msg, wparam, lparam);
        }
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    Int2 WindowsRaytracerUI::getXY(const LPARAM lParam)
    {
        return Int2(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    }
}
