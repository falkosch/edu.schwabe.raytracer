#pragma once

#include <Windows.h>

namespace raytracer
{

    class MessageLoopBasedUI
    {
    public:

        virtual ~MessageLoopBasedUI();

        virtual LRESULT run() = 0;
    };

}
