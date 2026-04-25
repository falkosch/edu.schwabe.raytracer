#include "OpenGLWindowsRaytracerUI.h"
#include "stdafx.h"

#include <commdlg.h>

#include <algorithm>
#include <logging.h>
#include <string>

#include <raytracing/geometry/partitioning/treebalancer/ArithmeticMeanKDTreeBalancer.h>
#include <raytracing/geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.h>
#include <raytracing/geometry/partitioning/treebalancer/FixedIterationsSAHKDTreeBalancer.h>
#include <raytracing/geometry/partitioning/treebalancer/MaxAxisKDTreeBalancer.h>
#include <raytracing/geometry/partitioning/treebalancer/MedianKDTreeBalancer.h>
#include <raytracing/geometry/partitioning/treebalancer/RotatingAxisKDTreeBalancer.h>
#include <raytracing/scene/Scene.h>

#include <imgui.h>
#include <imgui_impl_opengl2.h>
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static const auto Log = logging::scope("UI");

namespace raytracerui
{
  std::unique_ptr<const KDTreeBalancer> createBalancer(int index, int threshold)
  {
    switch (index)
    {
    case 0: return std::make_unique<FixedIterationsSAHKDTreeBalancer>(static_cast<ASizeT>(threshold));
    case 1: return std::make_unique<BruteForceSAHKDTreeBalancer>();
    case 2: return std::make_unique<MedianKDTreeBalancer>();
    case 3: return std::make_unique<ArithmeticMeanKDTreeBalancer>();
    case 4: return std::make_unique<MaxAxisKDTreeBalancer>();
    case 5: return std::make_unique<RotatingAxisKDTreeBalancer>();
    default: return nullptr;
    }
  }

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
    case VK_F1: showControls = !showControls;
      break;
    case VK_F2: showMetrics = !showMetrics;
      break;
    case VK_F3: showConfig = !showConfig;
      break;
    case VK_F4: showLog = !showLog;
      break;
    default: return;
    }
    repaint();
  }

  LRESULT OpenGLWindowsRaytracerUI::forwardInputMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);

    // Repaint after input when ImGui panels are visible so hover/focus state stays fresh.
    const auto& io = ImGui::GetIO();
    const bool hasVisiblePanels = showControls || showMetrics || showConfig || showLog;
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
        ImGui::SeparatorText("Movement");
        ImGui::BulletText("W/A/S/D: Move forward/left/back/right");
        ImGui::BulletText("Space/C: Move up/down");
        ImGui::BulletText("Right Mouse Drag: Rotate camera");
        ImGui::BulletText("R: Reset camera");

        ImGui::SeparatorText("Interaction Mode (1/2/3)");
        ImGui::BulletText("1: Camera (right-drag rotates)");
        ImGui::BulletText("2: Object (right-drag moves,");
        ImGui::BulletText("   left-drag scales)");
        ImGui::BulletText("3: Light (right-drag moves)");

        ImGui::SeparatorText("Rendering");
        ImGui::BulletText("Enter: Full quality render");
        ImGui::BulletText("Save As: PNG/BMP (in Config)");
        ImGui::BulletText("E: Cycle display mode");
        ImGui::BulletText("T: Toggle fast preview");
        ImGui::BulletText("Q: Cycle culling orientation");

        ImGui::SeparatorText("Panels");
        ImGui::BulletText("F1: Controls");
        ImGui::BulletText("F2: Metrics");
        ImGui::BulletText("F3: Configuration");
        ImGui::BulletText("F4: Log");
      }
      ImGui::End();
    }

    if (showMetrics)
    {
      ImGui::SetNextWindowPos(ImVec2(10, 400), ImGuiCond_FirstUseEver);
      ImGui::SetNextWindowSize(ImVec2(320, 0), ImGuiCond_FirstUseEver);
      if (ImGui::Begin("Metrics (F2)", &showMetrics))
      {
        const auto& s = lastStatistics;

        // Performance
        ImGui::SeparatorText("Performance");

        const auto totalRays = static_cast<double>(s.primaryRays + s.secondaryRays + s.shadowRays + s.objectShadowRays);
        if (lastRenderDuration > 0.0)
        {
          ImGui::Text("Duration: %.4f s  (%.2f MRays/s)", lastRenderDuration, totalRays / lastRenderDuration / 1e6);
        }
        else
        {
          ImGui::Text("Duration: %.4f s", lastRenderDuration);
        }

        ImGui::Text("Resolution: %zu x %zu",
                    static_cast<size_t>(x(parameters.resolution)),
                    static_cast<size_t>(y(parameters.resolution)));

        const auto totalTicks = static_cast<double>(s.intersectionTicks + s.shadingTicks);
        if (totalTicks > 0.0)
        {
          ImGui::Text("Intersection: %.1f%%  Shading: %.1f%%",
                      100.0 * static_cast<double>(s.intersectionTicks) / totalTicks,
                      100.0 * static_cast<double>(s.shadingTicks) / totalTicks);
        }

        // Ray Statistics
        ImGui::SeparatorText("Ray Statistics");

        auto rayLine = [](const char* label, ASizeT total, ASizeT missed)
        {
          if (total > Zero<ASizeT>())
          {
            ImGui::Text("%-16s %zu  (%.1f%% miss)", label, static_cast<size_t>(total),
                        100.0 * static_cast<double>(missed) / static_cast<double>(total));
          }
          else
          {
            ImGui::Text("%-16s %zu", label, static_cast<size_t>(total));
          }
        };

        rayLine("Primary:", s.primaryRays, s.missedPrimaryRays);
        rayLine("Secondary:", s.secondaryRays, s.missedSecondaryRays);
        rayLine("Shadow:", s.shadowRays, s.missedShadowRays);
        rayLine("Obj Shadow:", s.objectShadowRays, s.objectMissedShadowRays);

        const auto primaryHits = s.primaryRays - s.missedPrimaryRays;
        if (primaryHits > Zero<ASizeT>())
        {
          ImGui::Text("Avg bounces/hit: %.2f",
                      static_cast<double>(s.secondaryRays) / static_cast<double>(primaryHits));
          ImGui::Text("Avg shadows/hit: %.2f",
                      static_cast<double>(s.shadowRays) / static_cast<double>(primaryHits));
        }

        if (s.kdTreeNodesVisited > Zero<ASizeT>() && totalRays > 0.0)
        {
          ImGui::Text("Avg KD nodes/ray: %.1f",
                      static_cast<double>(s.kdTreeNodesVisited) / totalRays);
        }

        // Camera
        if (const auto* cam = parameters.camera)
        {
          if (ImGui::TreeNode("Camera"))
          {
            const auto pos = cam->getWorldPosition();
            const auto& vm = cam->getViewMatrix();
            ImGui::Text("Pos:  %.3f  %.3f  %.3f", x(pos), y(pos), z(pos));
            ImGui::Text("View: %.3f  %.3f  %.3f", x(vm.row2), y(vm.row2), z(vm.row2));
            ImGui::TreePop();
          }
        }

        // Scene Info (collapsible)
        if (const auto* scene = dynamic_cast<const Scene*>(parameters.sceneShader))
        {
          if (ImGui::TreeNode("Scene Info"))
          {
            ImGui::Text("Objects: %zu", scene->getSceneObjects().size());
            ImGui::Text("Lights:  %zu", scene->getLights().size());

            ASizeT totalFaces = Zero<ASizeT>();
            for (const auto& obj : scene->getSceneObjects())
            {
              if (const auto* form = obj->getForm())
                totalFaces += form->getFaceCount();
            }
            ImGui::Text("Faces:   %zu", static_cast<size_t>(totalFaces));

            if (const auto* graph = scene->getSceneGraph())
            {
              ImGui::Text("KD-Tree: %zu nodes, depth %zu",
                          static_cast<size_t>(graph->nodeCount()),
                          static_cast<size_t>(graph->depth()));
            }

            ImGui::TreePop();
          }
        }

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

        const char* balancerItems[] = {
          "Fixed Iterations SAH", "Brute Force SAH", "Median",
          "Arithmetic Mean", "Max Axis", "Rotating Axis"
        };

        bool sceneBalancerChanged = ImGui::Combo("Scene KD-tree balancer", &balancerIndex, balancerItems, 6);
        if (balancerIndex == 0)
          sceneBalancerChanged |= ImGui::SliderInt("Scene SAH threshold", &sceneBalancerThreshold, 1, 100);
        if (sceneBalancerChanged)
        {
          if (auto* scene = dynamic_cast<Scene*>(parameters.sceneShader))
          {
            auto balancer = std::make_shared<std::unique_ptr<const KDTreeBalancer>>(
              createBalancer(balancerIndex, sceneBalancerThreshold));
            raytracer->enqueueWork([scene, balancer] {
              scene->setTreeBalancer(std::move(*balancer));
            });
          }
          changed = true;
        }

        bool meshBalancerChanged = ImGui::Combo("Mesh KD-tree balancer", &meshBalancerIndex, balancerItems, 6);
        if (meshBalancerIndex == 0)
          meshBalancerChanged |= ImGui::SliderInt("Mesh SAH threshold", &meshBalancerThreshold, 1, 100);
        if (meshBalancerChanged)
        {
          if (auto* scene = dynamic_cast<Scene*>(parameters.sceneShader))
          {
            const auto idx = meshBalancerIndex;
            const auto thr = meshBalancerThreshold;
            raytracer->enqueueWork([scene, idx, thr] {
              scene->setMeshTreeBalancer([idx, thr] { return createBalancer(idx, thr); });
            });
          }
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

        auto fov = static_cast<float>(parameters.perspectiveFOV);
        if (ImGui::SliderFloat("Field of View", &fov, 10.0f, 120.0f, "%.0f deg"))
        {
          parameters.perspectiveFOV = fov;
          parameters.camera->setProjection(
            parameters.perspectiveFOV, convert<Float2>(screenSize), parameters.perspectiveZPlanes
          );
          changed = true;
        }

        const char* modeItems[] = {"Camera", "Object", "Light"};
        auto modeIndex = static_cast<int>(interactionMode);
        if (ImGui::Combo("Interaction Mode", &modeIndex, modeItems, 3))
        {
          interactionMode = static_cast<InteractionMode>(modeIndex);
        }

        ImGui::Separator();
        if (ImGui::Button("Reset Camera"))
        {
          parameters.camera->setViewMatrix(initialViewMatrix);
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

            Log.info([path] { return "Saving image ... "; });
            if (ext == "png")
              output->saveAsPNG(path);
            else
              output->saveAsBMP(path);
            Log.info([path] { return path; });
          }
        }

        if (changed)
        {
          triggerRaytracing(true);
        }
      }
      ImGui::End();
    }

    if (showLog)
    {
      ImGui::SetNextWindowPos(ImVec2(10, 500), ImGuiCond_FirstUseEver);
      ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);
      if (ImGui::Begin("Log (F4)", &showLog))
      {
        if (ImGui::Button("Clear"))
        {
          logging::Logger::instance().displayBuffer().clear();
        }
        ImGui::Separator();

        ImGui::BeginChild("LogScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        const auto lines = logging::Logger::instance().displayBuffer().snapshot();
        for (const auto& line : lines)
        {
          ImVec4 color;
          switch (line.level)
          {
          case logging::LogLevel::Error: color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
            break;
          case logging::LogLevel::Warn: color = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);
            break;
          default: color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
            break;
          }
          ImGui::PushStyleColor(ImGuiCol_Text, color);
          ImGui::TextUnformatted(line.text.c_str());
          ImGui::PopStyleColor();
        }

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
          ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
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
