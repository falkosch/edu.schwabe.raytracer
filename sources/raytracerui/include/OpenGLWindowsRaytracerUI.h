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

        bool showLog{false};

        int balancerIndex{1};

        int sceneBalancerThreshold{25};

        int meshBalancerIndex{1};

        int meshBalancerThreshold{5};

        int sceneIndex{0};

        Scene* scene{};

        Resources* resources{};

    protected:
        void reshape(const Size2& newSize) override;

        void display() override;

        void repaint() override;

        void onRenderComplete() override;

        void onTogglePanel(WPARAM key) override;

        LRESULT forwardInputMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

    private:
        void renderUI();

    public:
        OpenGLWindowsRaytracerUI(
            Raytracer& raytracer, const RaytraceParameters& parameters, const Size2& fastPreviewSize,
            Scene& scene, Resources& resources
        );

        ~OpenGLWindowsRaytracerUI() override;

        void notifyUpdate(const RaytraceConfiguration& configuration) override;
    };
}
