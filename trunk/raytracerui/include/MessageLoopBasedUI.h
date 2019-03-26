#pragma once

#include <Windows.h>

namespace raytracerui
{

    class MessageLoopBasedUI
    {
    public:

        virtual ~MessageLoopBasedUI();

        virtual LRESULT run() = 0;
    };

}
