#include "OpenGLWindowsRaytracerUI.h"
#include "stdafx.h"

#include <commdlg.h>

#include <algorithm>
#include <iostream>

#include <imgui.h>
#include <imgui_impl_opengl2.h>
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace raytracerui
{
  const Float4_32 QUAD_VERTICES[] = {
    Float4_32(0.f, 0.f, 0.f, 1.f), Float4_32(0.f, 1.f, 0.f, 1.f), Float4_32(1.f, 1.f, 0.f, 1.f),
    Float4_32(1.f, 0.f, 0.f, 1.f)
  };

  const Float4_32 QUAD_TEX_COORDS[] = {Float4_32(1.f, 1.f, 1.f, 0.f), Float4_32(0.f, 0.f, 0.f, 1.f)};

  OpenGLWindowsRaytracerUI::OpenGLWindowsRaytracerUI(
    Raytracer& raytracerIn, const RaytraceParameters& parametersIn, ASizeT fastPreviewSizeIn
  )
    : WindowsRaytracerUI(raytracerIn, parametersIn, fastPreviewSizeIn), hDC(), hGLRC(), outputSurface()
  {
    // set the pixel format for the device context
    hDC = GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd{
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA,
      32,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      PFD_MAIN_PLANE,
      0,
      0,
      0,
      0
    };
    SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);

    // create and enable the render context (RC)
    hGLRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGLRC);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    auto projection = ortho(1.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(reinterpret_cast<const GLfloat*const>(&projection));

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &outputSurface);
    glBindTexture(GL_TEXTURE_2D, outputSurface);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Initialize Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplOpenGL2_Init();
    ImGui::StyleColorsDark();
    ImGui::GetStyle().Alpha = 0.9f;
  }

  OpenGLWindowsRaytracerUI::~OpenGLWindowsRaytracerUI()
  {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    raytracer->stop();
    wglMakeCurrent(hDC, nullptr);
    wglDeleteContext(hGLRC);
    ReleaseDC(hWnd, hDC);
  }

  void OpenGLWindowsRaytracerUI::reshape(const Int2& newSize)
  {
    WindowsRaytracerUI::reshape(newSize);
    glViewport(0, 0, x(newSize), y(newSize));
  }

  void OpenGLWindowsRaytracerUI::display()
  {
    PAINTSTRUCT psPaint{};
    BeginPaint(hWnd, &psPaint);
    EndPaint(hWnd, &psPaint);
    repaint();
  }

  void OpenGLWindowsRaytracerUI::repaint()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the raytraced image quad
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_FLOAT, 0, QUAD_VERTICES);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, QUAD_TEX_COORDS);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    // Render ImGui overlay on top
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    renderUI();

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
  }

  void OpenGLWindowsRaytracerUI::onTogglePanel(WPARAM key)
  {
    switch (key)
    {
    case VK_F1: showControls = !showControls; break;
    case VK_F2: showMetrics = !showMetrics; break;
    case VK_F3: showConfig = !showConfig; break;
    default: return;
    }
    repaint();
  }

  LRESULT OpenGLWindowsRaytracerUI::forwardInputMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);

    // Repaint after input when ImGui panels are visible so hover/focus state stays fresh.
    const auto& io = ImGui::GetIO();
    const bool hasVisiblePanels = showControls || showMetrics || showConfig;
    switch (msg)
    {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL:
      if (hasVisiblePanels) InvalidateRect(hwnd, nullptr, FALSE);
      if (io.WantCaptureMouse) return 1;
      break;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_CHAR:
      if (hasVisiblePanels) InvalidateRect(hwnd, nullptr, FALSE);
      if (io.WantCaptureKeyboard) return 1;
      break;
    default:
      break;
    }
    return 0;
  }

  void OpenGLWindowsRaytracerUI::renderUI()
  {
    if (showControls)
    {
      ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
      ImGui::SetNextWindowSize(ImVec2(280, 0), ImGuiCond_FirstUseEver);
      if (ImGui::Begin("Controls (F1)", &showControls))
      {
        ImGui::SeparatorText("Camera");
        ImGui::BulletText("Left Mouse Drag: Rotate");
        ImGui::BulletText("Ctrl + Left Drag: Pan XY");
        ImGui::BulletText("Shift + Left Drag: Zoom");
        ImGui::BulletText("Alt + Left Drag: Scale");
        ImGui::BulletText("R: Reset camera");

        ImGui::SeparatorText("Scene");
        ImGui::BulletText("Right Mouse Drag: Move light");
        ImGui::BulletText("Shift + Right Drag: Move object");

        ImGui::SeparatorText("Rendering");
        ImGui::BulletText("Enter: Full quality render");
        ImGui::BulletText("W: Quick save as BMP");
        ImGui::BulletText("Save As: PNG/BMP (in Config)");
        ImGui::BulletText("E: Cycle display mode");
        ImGui::BulletText("T: Toggle fast preview");

        ImGui::SeparatorText("Parameters");
        ImGui::BulletText("A/S: Sampling factor -/+");
        ImGui::BulletText("D/F: Trace depth -/+");
        ImGui::BulletText("G/H: Ray packet size -/+");
        ImGui::BulletText("J/K: Super-sampling -/+");
        ImGui::BulletText("Q: Cycle culling orientation");

        ImGui::SeparatorText("Panels");
        ImGui::BulletText("F1: Controls");
        ImGui::BulletText("F2: Metrics");
        ImGui::BulletText("F3: Configuration");
      }
      ImGui::End();
    }

    if (showMetrics)
    {
      ImGui::SetNextWindowPos(ImVec2(10, 400), ImGuiCond_FirstUseEver);
      ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
      if (ImGui::Begin("Metrics (F2)", &showMetrics))
      {
        ImGui::Text("Duration: %.4f s", lastRenderDuration);
        ImGui::Text("Resolution: %zu x %zu",
                    static_cast<size_t>(x(parameters.resolution)),
                    static_cast<size_t>(y(parameters.resolution)));
        ImGui::Separator();
        ImGui::Text("Primary Rays:    %zu / %zu missed",
                    static_cast<size_t>(lastStatistics.primaryRays),
                    static_cast<size_t>(lastStatistics.missedPrimaryRays));
        ImGui::Text("Secondary Rays:  %zu / %zu missed",
                    static_cast<size_t>(lastStatistics.secondaryRays),
                    static_cast<size_t>(lastStatistics.missedSecondaryRays));
        ImGui::Text("Shadow Rays:     %zu / %zu missed",
                    static_cast<size_t>(lastStatistics.shadowRays),
                    static_cast<size_t>(lastStatistics.missedShadowRays));
        ImGui::Text("Obj Shadow Rays: %zu / %zu missed",
                    static_cast<size_t>(lastStatistics.objectShadowRays),
                    static_cast<size_t>(lastStatistics.objectMissedShadowRays));
        ImGui::Separator();
        ImGui::Text("Frame: %zu", static_cast<size_t>(frameCount));
      }
      ImGui::End();
    }

    if (showConfig)
    {
      ImGui::SetNextWindowPos(ImVec2(static_cast<float>(x(screenSize)) - 310.f, 10.f), ImGuiCond_FirstUseEver);
      ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
      if (ImGui::Begin("Configuration (F3)", &showConfig))
      {
        bool changed = false;

        auto samplingFactor = static_cast<float>(parameters.samplingFactor);
        if (ImGui::SliderFloat("Sampling Factor", &samplingFactor, 0.125f, 4.0f, "%.3f"))
        {
          parameters.samplingFactor = samplingFactor;
          changed = true;
        }

        auto maxTraceDepth = static_cast<int>(parameters.maxTraceDepth);
        if (ImGui::SliderInt("Max Trace Depth", &maxTraceDepth, 0, 31))
        {
          parameters.maxTraceDepth = static_cast<ASizeT>(maxTraceDepth);
          changed = true;
        }

        auto superSampling = static_cast<int>(parameters.superSamplingFactor);
        if (ImGui::SliderInt("Super Sampling", &superSampling, 0, 4))
        {
          parameters.superSamplingFactor = static_cast<ASizeT>(superSampling);
          changed = true;
        }

        auto rayPacketSize = static_cast<int>(parameters.rayPacketSize);
        if (ImGui::SliderInt("Ray Packet Size", &rayPacketSize, 0, 31))
        {
          parameters.rayPacketSize = static_cast<ASizeT>(rayPacketSize);
          changed = true;
        }

        const char* cullingItems[] = {"Back-face", "None", "Front-face"};
        auto cullingIndex = parameters.cullingOrientation + 1;
        if (ImGui::Combo("Culling", &cullingIndex, cullingItems, 3))
        {
          parameters.cullingOrientation = cullingIndex - 1;
          changed = true;
        }

        if (ImGui::Checkbox("Disable Fast Preview", &disableFastPreview))
        {
          changed = true;
        }

        const char* displayItems[] = {"Image", "Timing Map", "Depth Map"};
        auto displayIndex = static_cast<int>(showMapIndex);
        if (ImGui::Combo("Display Mode", &displayIndex, displayItems, 3))
        {
          showMapIndex = static_cast<ASizeT>(displayIndex);
          changed = true;
        }

        ImGui::Separator();
        if (ImGui::Button("Reset Camera"))
        {
          parameters.camera->resetView();
          changed = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Full Render"))
        {
          triggerRaytracing(false);
        }

        if (output && ImGui::Button("Save As..."))
        {
          char filename[MAX_PATH] = "ray-traced.png";
          OPENFILENAMEA ofn{};
          ofn.lStructSize = sizeof(ofn);
          ofn.hwndOwner = hWnd;
          ofn.lpstrFilter = "PNG Files (*.png)\0*.png\0BMP Files (*.bmp)\0*.bmp\0";
          ofn.lpstrFile = filename;
          ofn.nMaxFile = MAX_PATH;
          ofn.lpstrInitialDir = "data";
          ofn.Flags = OFN_OVERWRITEPROMPT;
          ofn.lpstrDefExt = "png";

          if (GetSaveFileNameA(&ofn))
          {
            std::string path(filename);
            std::string ext = path.substr(path.find_last_of('.') + 1);
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            std::cout << "Saving image ... ";
            if (ext == "png")
              output->saveAsPNG(path);
            else
              output->saveAsBMP(path);
            std::cout << path << std::endl;
          }
        }

        if (changed)
        {
          triggerRaytracing(true);
        }
      }
      ImGui::End();
    }
  }

  void OpenGLWindowsRaytracerUI::notifyUpdate(const RaytraceConfiguration& configuration)
  {
    // Called from the worker thread — do not call OpenGL here.
    // Store the completed configuration and marshal to the UI thread.
    {
      std::lock_guard lock(pendingMutex);
      pendingConfig = configuration;
      hasPendingResult = true;
    }
    PostMessage(hWnd, WM_RENDER_COMPLETE, 0, 0);
  }

  void OpenGLWindowsRaytracerUI::onRenderComplete()
  {
    RaytraceConfiguration config;
    {
      std::lock_guard lock(pendingMutex);
      if (!hasPendingResult) return;
      config = pendingConfig;
      hasPendingResult = false;
    }

    // Process on UI thread: stats, image selection, bitmap conversion
    RaytracerUI::notifyUpdate(config);

    // Upload texture to OpenGL (must happen on the UI thread that owns the GL context)
    if (this->outputHDR)
    {
      glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA32F, static_cast<GLsizei>(x(config.resolution)),
        static_cast<GLsizei>(y(config.resolution)), 0, GL_RGBA, GL_FLOAT, outputHDR->getData()
      );
    }

    InvalidateRect(hWnd, nullptr, FALSE);
  }
}
