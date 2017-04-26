#pragma once

#include <map>

#include <glm/glm.hpp>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/Viewport.h>

#include <Deliberation/Draw/Texture.h>

#include <Deliberation/Font/Label.h>



namespace deliberation
{

class LabelRenderer;
class Font;

class MultilineLabel final
{
public:
    MultilineLabel(const Font & font);

    void setPosition(const glm::vec2 & position);
    void setCenter(const glm::vec2 & center);
    void setColor(const glm::vec3 & color);
    void setText(size_t line, const std::string & text);
    void setFontSize(unsigned int fontSize);

    void render(LabelRenderer & renderer, const Viewport & viewport);

private:
    const Font &        m_font;
    glm::vec2           m_position;
    glm::vec2           m_center;
    glm::vec3           m_color;
    std::map<size_t, std::shared_ptr<Label>>
                        m_labelByLine;
    unsigned int        m_fontSize = 18;
};

}

