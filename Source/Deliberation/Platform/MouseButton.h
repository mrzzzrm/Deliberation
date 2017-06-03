#pragma once

#include <Deliberation/Deliberation.h>

namespace deliberation
{
enum class MouseButton
{
    None = 0,
    Left,
    Middle,
    Right,
    X1,
    X2,
    Unknown,

    Count // Meta
};

enum class MouseButtonMask
{
    None = 0,
    Left = 1 << (i32)MouseButton::Left,
    Middle = 1 << (i32)MouseButton::Middle,
    Right = 1 << (i32)MouseButton::Right,
    X1 = 1 << (i32)MouseButton::X1,
    X2 = 1 << (i32)MouseButton::X2,
    Unknown = 1 << (i32)MouseButton::Unknown
};
}