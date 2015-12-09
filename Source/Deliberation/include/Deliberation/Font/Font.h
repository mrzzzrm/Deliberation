#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include <SDL2/SDL_ttf.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

namespace detail
{
    class TTF_Font_Wrapper;
}

class Context;
class Texture;

class DELIBERATION_API Font final
{
public:
    Font();
    Font(Context & context, const std::string & path);
    ~Font();

    Texture render(const std::string & text, unsigned int size, const glm::vec4 & color) const;

private:
    Context * m_context;
    std::string m_path;
    mutable std::unordered_map<unsigned int,
                               std::unique_ptr<detail::TTF_Font_Wrapper>> m_fontBySize;
};

}

