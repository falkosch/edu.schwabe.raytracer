#pragma once

#include <Windows.h>

namespace raytracerui
{
    class MessageLoopBasedUI
    {
    public:

        virtual ~MessageLoopBasedUI();

        virtual const WPARAM run() = 0;
    };
}
