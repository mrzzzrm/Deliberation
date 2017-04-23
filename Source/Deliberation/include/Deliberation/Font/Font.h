#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include <SDL_ttf.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

namespace detail
{
    class TTF_Font_Wrapper;
}

class DrawContext;
class Texture;

class DELIBERATION_API Font final
{
public:
    Font();
    Font(DrawContext & drawContext, const std::string & path);
    ~Font();

    DrawContext & drawContext() const;

    Texture render(const std::string & text, unsigned int size, const glm::vec4 & color) const;

private:
    DrawContext * m_drawContext;
    std::string m_path;
    mutable std::unordered_map<unsigned int,
                               std::shared_ptr<detail::TTF_Font_Wrapper>> m_fontBySize;
};

}

