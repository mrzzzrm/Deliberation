#pragma once

#include <Deliberation/Platform/KeyMap.h>
#include <Deliberation/Platform/InputEvent.h>

namespace deliberation
{

class KeyEvent:
    public InputEvent
{
public:
    KeyEvent(Key key):
        m_key(key)
    {}

    Key key() const { return m_key; }

private:
    Key m_key;
};

}