#include <Deliberation/Font/Font.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Context.h>

namespace deliberation
{

namespace detail
{

class TTF_Font_Wrapper
{
public:
    TTF_Font_Wrapper(const std::string & path, unsigned int size):
        font(nullptr)
    {
        font = TTF_OpenFont(path.c_str(), size);
        Assert(font, "Failed to open font '" + path + "'");
    }

    ~TTF_Font_Wrapper()
    {
        if (font)
        {
            TTF_CloseFont(font);
        }
    }

    TTF_Font * font;
};

}

Font::Font():
    m_context(nullptr)
{

}

Font::Font(Context & context, const std::string & path):
    m_context(&context),
    m_path(path)
{

}

Font::~Font() = default;

Texture Font::render(const std::string & text,
                     unsigned int size,
                     const glm::vec4 & color) const
{
    Assert(m_context, "");

    auto i = m_fontBySize.find(size);
    if (i == m_fontBySize.end())
    {
        auto result = m_fontBySize.emplace(size, std::unique_ptr<detail::TTF_Font_Wrapper>(new detail::TTF_Font_Wrapper(m_path, size)));
        Assert(result.second, "");

        i = result.first;
    }

    auto & font = i->second;

    SDL_Color sdlColor;
    sdlColor.r = color.r * 255;
    sdlColor.g = color.g * 255;
    sdlColor.b = color.b * 255;
    sdlColor.a = color.a * 255;

    auto * surface = TTF_RenderText_Blended(font->font, text.c_str(), sdlColor);

    auto texture = m_context->createTexture(surface);

    SDL_FreeSurface(surface);

    return texture;
}

}

