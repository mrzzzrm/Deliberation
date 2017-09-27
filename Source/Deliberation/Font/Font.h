#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include <SDL_ttf.h>

namespace deliberation
{
namespace detail
{
    class TTF_Font_Wrapper;
}

class DrawContext;
class Texture;

class Font final
{
public:
    Font();
    Font(const std::string & path);
    ~Font();

    Texture render(
        const std::string & text,
        unsigned int        size,
        const glm::vec4 &   color) const;

private:
    std::string   m_path;
    mutable std::
        unordered_map<unsigned int, std::shared_ptr<detail::TTF_Font_Wrapper>>
            m_fontBySize;
};
}
