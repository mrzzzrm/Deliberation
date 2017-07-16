#pragma once

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class InputEvent
{
public:
    virtual ~InputEvent() = default;

    void consume() { m_consumed = true; }

    bool isConsumed() const { return m_consumed; }

private:
    bool m_consumed = false;
};
}