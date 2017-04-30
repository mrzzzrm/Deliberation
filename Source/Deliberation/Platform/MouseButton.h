#pragma once

#include <Deliberation/Deliberation.h>

namespace deliberation
{

enum MouseButton
{
    MouseButton_None    = 0,
    MouseButton_Left    = 1 << 0,
    MouseButton_Middle  = 1 << 1,
    MouseButton_Right   = 1 << 2,
    MouseButton_X1      = 1 << 3,
    MouseButton_X2      = 1 << 4,
    MouseButton_Unknown = 1 << 5
};

}