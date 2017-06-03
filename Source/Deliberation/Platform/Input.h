#pragma once

#include <bitset>
#include <functional>
#include <map>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <SDL.h>

#include <Deliberation/Core/Chrono.h>

#include <Deliberation/Platform/KeyMap.h>
#include <Deliberation/Platform/MouseButtonEvent.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class InputLayer;

class Input final
{
  public:
    static constexpr DurationMillis CLICK_TIMEOUT = 300;

  public:
    Input();

    bool      mouseButtonDown(MouseButton button) const;
    bool      keyPressed(Key key) const;
    glm::vec2 mousePosition() const;

    void addLayer(const std::shared_ptr<InputLayer> & layer);
    void removeLayer(const std::shared_ptr<InputLayer> & layer);

    void onSDLInputEvent(const SDL_Event & event);

    void onFrameBegin();

  private:
    template<typename T>
    std::shared_ptr<InputLayer> processEvent(
        T event, const std::function<void(InputLayer &, T &)> & fn) const;

    glm::vec2 sdlMousePositionToNdc(i32 x, i32 y) const;

  private:
    std::vector<std::shared_ptr<InputLayer>> m_layers;
    std::map<MouseButton, TimestampMillis>   m_downTimestampByMouseButton;
    std::bitset<(size_t)MouseButton::Count> m_mouseButtonsDown;

    std::shared_ptr<InputLayer> m_mouseOwningLayer;
};
}
