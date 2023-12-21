#include "WindowsRaytracerUI.h"
#include "stdafx.h"

#include <iostream>
#include <map>

#include <windowsx.h>

namespace raytracerui {
  const Float WindowsRaytracerUI::MOUSE_SENSITIVITY = 0.02f;

  std::map<HWND, WindowsRaytracerUI *> createUIs{};

  WNDCLASSEX WindowsRaytracerUI::windowClass{
      sizeof(WNDCLASSEX),
      CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
      WindowsRaytracerUI::DelegatingWndProc,
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
      Raytracer &raytracerIn, const RaytraceParameters &parametersIn, ASizeT &fastPreviewSizeIn
  )
      : RaytracerUI(raytracerIn, parametersIn, fastPreviewSizeIn), previousMousePosition(), activeDrag(DragTypes::None),
        hWnd() {
    InitWindow();
  }

  WindowsRaytracerUI::~WindowsRaytracerUI() {
    createUIs.erase(this->hWnd);
    DestroyWindow(hWnd);
  }

  void WindowsRaytracerUI::InitWindowClass() {
    windowClass.hInstance = GetModuleHandle(nullptr);
    windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassEx(&windowClass);
  }

  void WindowsRaytracerUI::InitWindow() {
    InitWindowClass();
    hWnd = CreateWindowEx(
        WS_EX_APPWINDOW, windowClass.lpszClassName, TEXT("WindowsRaytracerUI"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
        CW_USEDEFAULT, 528, 551, nullptr, nullptr, windowClass.hInstance, this
    );
    createUIs.insert({this->hWnd, this});
    ShowWindow(this->hWnd, SW_SHOWDEFAULT);
  }

  WPARAM WindowsRaytracerUI::run() {
    MSG msg{};
    BOOL bRet;
    while (0 != (bRet = GetMessage(&msg, hWnd, 0, 0))) {
      // catch GetMessage returns due to an error
      if (bRet == -1) {
        // no error handling here
        break;
      }

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    return msg.wParam;
  }

  void WindowsRaytracerUI::delegateReshape() {
    RECT rect{};
    GetClientRect(hWnd, &rect);
    reshape(Int2(rect.right, rect.bottom) - Int2(rect.left, rect.top));
  }

  void WindowsRaytracerUI::triggerRaytracing(bool fastPreview) {
    RaytracerUI::triggerRaytracing(fastPreview);
    InvalidateRect(hWnd, nullptr, false);
  }

  void WindowsRaytracerUI::display() {
    raytracer->requestUpdate();

    if (output) {
      // output->saveAsBMP("output.bmp");

      auto bitmapInfo = output->getBITMAPINFO();
      auto bitmap = output->getBITMAP();

      PAINTSTRUCT psPaint{};
      auto hdc = BeginPaint(hWnd, &psPaint);
      StretchDIBits(
          hdc, int{}, int{}, x(screenSize), y(screenSize), int{}, int{}, bitmap.bmWidth, bitmap.bmHeight, bitmap.bmBits,
          &bitmapInfo, DIB_RGB_COLORS, SRCCOPY
      );
      EndPaint(hWnd, &psPaint);
    }
  }

  void WindowsRaytracerUI::keyPressed(WPARAM wParam) {
    switch (wParam) {
    case 'Q':
      parameters.cullingOrientation = ((parameters.cullingOrientation + 2) % 3) - 1;
      std::cout << "Culling orientation: " << parameters.cullingOrientation << std::endl;
      break;

    case 'W':
      std::cout << "Saving ray-traced image ...";
      output->saveAsBMP("ray-traced.bmp");
      std::cout << " ray-traced.bmp" << std::endl;
      break;

    case 'E':
      showMapIndex = (showMapIndex + 1) % 3;
      break;

    case 'R':
      parameters.camera->resetView();
      break;

    case 'T':
      disableFastPreview = !disableFastPreview;
      std::cout << "Fast preview disabled: " << disableFastPreview << std::endl;
      break;

    case 'A':
      parameters.samplingFactor *= 0.5f;
      std::cout << "Sampling-factor: " << parameters.samplingFactor << std::endl;
      break;

    case 'S':
      parameters.samplingFactor += 1.0f;
      std::cout << "Sampling-factor: " << parameters.samplingFactor << std::endl;
      break;

    case 'D':
      parameters.maxTraceDepth -= select(parameters.maxTraceDepth > ASizeT{0}, ASizeT{1}, ASizeT{0});
      std::cout << "Max trace-depth: " << parameters.maxTraceDepth << std::endl;
      break;

    case 'F':
      parameters.maxTraceDepth += ASizeT{1};
      std::cout << "Max trace-depth: " << parameters.maxTraceDepth << std::endl;
      break;

    case 'G':
      parameters.rayPacketSize -= select(parameters.rayPacketSize > ASizeT{0}, ASizeT{1}, ASizeT{0});
      std::cout << "Ray packet size: " << parameters.rayPacketSize << std::endl;
      break;

    case 'H':
      parameters.rayPacketSize += ASizeT{1};
      std::cout << "Ray packet size: " << parameters.rayPacketSize << std::endl;
      break;

    case 'J':
      parameters.superSamplingFactor -= select(parameters.superSamplingFactor > ASizeT{0}, ASizeT{1}, ASizeT{0});
      std::cout << "Super-sampling factor: " << parameters.superSamplingFactor << std::endl;
      break;

    case 'K':
      parameters.superSamplingFactor += ASizeT{1};
      std::cout << "Super-sampling factor: " << parameters.superSamplingFactor << std::endl;
      break;

    case VK_SPACE:
      break;

    case VK_RETURN:
      triggerRaytracing(false);
      return;

    default:
      return;
    }

    triggerRaytracing(true);
  }

  void WindowsRaytracerUI::mousePressed(MouseButtons button, ButtonStates state, const Int2 &position) {
    previousMousePosition = position;

    switch (button) {
    case MouseButtons::Left:
      if (state == ButtonStates::Down) {
        if (GetKeyState(VK_CONTROL) & 0x8000)
          activeDrag = DragTypes::ShiftXY;
        else if (GetKeyState(VK_SHIFT) & 0x8000)
          activeDrag = DragTypes::ShiftZ;
        else if (GetKeyState(VK_MENU) & 0x8000) // ALT key
          activeDrag = DragTypes::Scale;
        else
          activeDrag = DragTypes::Rotate;
      } else if (state == ButtonStates::Up) {
        activeDrag = DragTypes::None;
      }
      break;

    case MouseButtons::Right:
      if (state == ButtonStates::Down) {
        if (GetKeyState(VK_SHIFT) & 0x8000) {
          activeDrag = DragTypes::Object;
        } else {
          activeDrag = DragTypes::Light;
        }
      } else if (state == ButtonStates::Up) {
        activeDrag = DragTypes::None;
      }
      break;

    default:
      break;
    }
  }

  void WindowsRaytracerUI::mouseDragged(const Int2 &position) {
    auto delta = convert<Float2>(position - previousMousePosition);
    previousMousePosition = position;

    switch (activeDrag) {
    case DragTypes::Rotate:
      parameters.camera->rotate(Float3(x(delta), y(delta)));
      triggerRaytracing(true);
      break;

    case DragTypes::ShiftXY:
      parameters.camera->translate(Float3(-x(delta), y(delta)) * MOUSE_SENSITIVITY);
      triggerRaytracing(true);
      break;

    case DragTypes::ShiftZ:
      parameters.camera->translate(Float3(0.0f, 0.0f, x(delta) - y(delta)) * MOUSE_SENSITIVITY);
      triggerRaytracing(true);
      break;

    case DragTypes::Scale:
      parameters.camera->scale(Float3((x(delta) + y(delta)) * MOUSE_SENSITIVITY) + One<Float3>());
      triggerRaytracing(true);
      break;

    case DragTypes::Light: {
      auto &lights = parameters.sceneShader->getLights();
      auto lastLight = lights.back();
      lastLight->position += Float4(0.0f, 0.0f, x(delta) - y(delta)) * MOUSE_SENSITIVITY;
      triggerRaytracing(true);
      break;
    }

    case DragTypes::Object: {
      auto scene = dynamic_cast<Scene *const>(parameters.sceneShader);
      auto &sceneObjects = scene->getSceneObjects();
      auto lastSceneObject = sceneObjects.back();
      lastSceneObject->translate(Float3(0.0f, 0.0f, x(delta) - y(delta)) * MOUSE_SENSITIVITY);
      triggerRaytracing(true);
      break;
    }

    case DragTypes::None:
    default:
      break;
    }
  }

  LRESULT WindowsRaytracerUI::WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
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

    case WM_KEYUP:
      keyPressed(wParam);
      break;

    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return LRESULT{0};
  }

  LRESULT CALLBACK WindowsRaytracerUI::DelegatingWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto targetedUIIterator = createUIs.find(hWnd);
    if (targetedUIIterator != createUIs.end()) {
      return targetedUIIterator->second->WndProc(msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }

  Int2 WindowsRaytracerUI::getXY(const LPARAM lParam) {
    return Int2(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  }
}