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
        PIXELFORMATDESCRIPTOR pfd;
        ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = One<WORD>();
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = pfd.cDepthBits = static_cast<BYTE>(32);
        pfd.iLayerType = PFD_MAIN_PLANE;
        SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);

        // create and enable the render context (RC)
        hGLRC = wglCreateContext(hDC);
        wglMakeCurrent(hDC, hGLRC);

        glClearColor(Zero<GLclampf>(), Zero<GLclampf>(), Zero<GLclampf>(), Zero<GLclampf>());
        glMatrixMode(GL_PROJECTION);
        const Float44 orthoProjection = ortho(One<Float>(), Zero<Float>(), Zero<Float>(), One<Float>());
        glLoadMatrixf(reinterpret_cast<const GLfloat * const>(&orthoProjection));

        glEnable(GL_TEXTURE_2D);
        glGenTextures(One<GLsizei>(), &outputSurface);
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
        glViewport(Zero<GLint>(), Zero<GLint>(), x(newSize), y(newSize));
    }

    void OpenGLWindowsRaytracerUI::display() {
        PAINTSTRUCT psPaint;
        ZeroMemory(&psPaint, sizeof(PAINTSTRUCT));
        BeginPaint(hWnd, &psPaint);
        EndPaint(hWnd, &psPaint);

        raytracer->requestUpdate();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(4, GL_FLOAT, Zero<GLsizei>(), QUAD_VERTICES);

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(Two<GLint>(), GL_FLOAT, Zero<GLsizei>(), QUAD_TEXCOORDS);

        glDrawArrays(GL_TRIANGLE_FAN, Zero<GLsizei>(), 4);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
    }

    void OpenGLWindowsRaytracerUI::notifyUpdate(const RaytraceConfiguration & configuration) {
        RaytracerUI::notifyUpdate(configuration);
        if (outputHDR) {
            glTexImage2D(
                static_cast<GLenum>(GL_TEXTURE_2D),
                Zero<GLint>(),
                GL_RGBA32F,
                static_cast<GLsizei>(x(configuration.resolution)),
                static_cast<GLsizei>(y(configuration.resolution)),
                Zero<GLint>(),
                static_cast<GLenum>(GL_RGBA),
                static_cast<GLenum>(GL_FLOAT),
                outputHDR->getData());
        }
    }
}