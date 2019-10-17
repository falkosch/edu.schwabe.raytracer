#include "OpenGLWindowsRaytracerUI.h"
#include "stdafx.h"

namespace raytracerui
{
    const Float4_32 QUAD_VERTICES[] = {
        Float4_32(0.f, 0.f, 0.f, 1.f),
        Float4_32(0.f, 1.f, 0.f, 1.f),
        Float4_32(1.f, 1.f, 0.f, 1.f),
        Float4_32(1.f, 0.f, 0.f, 1.f)
    };

    const Float4_32 QUAD_TEXCOORDS[] = {
        Float4_32(1.f, 1.f, 1.f, 0.f),
        Float4_32(0.f, 0.f, 0.f, 1.f)
    };

    OpenGLWindowsRaytracerUI::OpenGLWindowsRaytracerUI(
        Raytracer & raytracerIn,
        const RaytraceParameters & parametersIn,
        const ASizeT fastPreviewSizeIn)
        :
        WindowsRaytracerUI(raytracerIn, parametersIn, fastPreviewSizeIn),
        hDC(),
        hGLRC(),
        outputSurface() {
        // get the device context (DC)
        hDC = GetDC(hWnd);

        // set the pixel format for the DC
        PIXELFORMATDESCRIPTOR pfd{
            sizeof(PIXELFORMATDESCRIPTOR), // nSize
            1, // nVersion
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // dwFlags
            PFD_TYPE_RGBA, // iPixelType
            32, // cColorBits
            0, // cRedBits
            0, // cRedShift
            0, // cGreenBits
            0, // cGreenShift
            0, // cBlueBits
            0, // cBlueShift
            0, // cAlphaBits
            0, // cAlphaShift
            0, // cAccumBits
            0, // cAccumRedBits
            0, // cAccumGreenBits
            0, // cAccumBlueBits
            0, // cAccumAlphaBits
            32, // cDepthBits
            0, // cStencilBits
            0, // cAuxBuffers
            PFD_MAIN_PLANE, // iLayerType
            0, // bReserved
            0, // dwLayerMask
            0, // dwVisibleMask
            0 // dwDamageMask
        };
        SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);

        // create and enable the render context (RC)
        hGLRC = wglCreateContext(hDC);
        wglMakeCurrent(hDC, hGLRC);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        const Float44 orthoProjection = ortho(1.0f, 0.0f, 0.0f, 1.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(reinterpret_cast<const GLfloat * const>(&orthoProjection));

        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &outputSurface);
        glBindTexture(GL_TEXTURE_2D, outputSurface);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    OpenGLWindowsRaytracerUI::~OpenGLWindowsRaytracerUI() {
        wglMakeCurrent(hDC, nullptr);
        wglDeleteContext(hGLRC);
        ReleaseDC(hWnd, hDC);
    }

    void OpenGLWindowsRaytracerUI::reshape(const Int2 & newSize) {
        WindowsRaytracerUI::reshape(newSize);
        glViewport(0, 0, x(newSize), y(newSize));
    }

    void OpenGLWindowsRaytracerUI::display() {
        PAINTSTRUCT psPaint{ };
        BeginPaint(hWnd, &psPaint);
        EndPaint(hWnd, &psPaint);

        raytracer->requestUpdate();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(4, GL_FLOAT, 0, QUAD_VERTICES);

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, QUAD_TEXCOORDS);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
    }

    void OpenGLWindowsRaytracerUI::notifyUpdate(const RaytraceConfiguration & configuration) {
        RaytracerUI::notifyUpdate(configuration);
        if (this->outputHDR) {
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA32F,
                static_cast<GLsizei>(x(configuration.resolution)),
                static_cast<GLsizei>(y(configuration.resolution)),
                0,
                GL_RGBA,
                GL_FLOAT,
                outputHDR->getData()
            );
        }
    }
}