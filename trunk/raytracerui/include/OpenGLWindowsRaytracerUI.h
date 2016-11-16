#pragma once

#include "WindowsRaytracerUI.h"

#include <GL/GL.h>

namespace raytracer
{

    class OpenGLWindowsRaytracerUI : public WindowsRaytracerUI
    {
        HDC hDC;

        HGLRC hGLRC;

        GLuint outputSurface;

    protected:

        void reshape(const Int2 & newSize);

        void display();

    public:

        ALIGNED_ALLOCATORS(__alignof(OpenGLWindowsRaytracerUI));

        OpenGLWindowsRaytracerUI(Raytracer & raytracer, const RaytraceParameters & parameters, const ASizeT fastPreviewSize);

        virtual ~OpenGLWindowsRaytracerUI();

        void notifyUpdate(const RaytraceConfiguration & configuration);

    };

}
