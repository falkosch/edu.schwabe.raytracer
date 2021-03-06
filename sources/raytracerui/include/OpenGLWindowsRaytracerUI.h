#pragma once

#include "WindowsRaytracerUI.h"

#include <GL/glew.h>
#include <GL/gl.h>

namespace raytracerui
{
    using namespace vectorization;
    using namespace raytracer;

    class OpenGLWindowsRaytracerUI : public WindowsRaytracerUI
    {
        HDC hDC;

        HGLRC hGLRC;

        GLuint outputSurface;

    protected:

        void reshape(const Int2 & newSize);

        void display();

    public:

        OpenGLWindowsRaytracerUI(Raytracer & raytracer, const RaytraceParameters & parameters, const ASizeT fastPreviewSize);

        virtual ~OpenGLWindowsRaytracerUI();

        void notifyUpdate(const RaytraceConfiguration & configuration);
    };
}
