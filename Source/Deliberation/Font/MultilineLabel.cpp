#include <Deliberation/Font/MultilineLabel.h>

#include <Deliberation/Font/Label.h>
#include <Deliberation/Font/LabelRenderer.h>

namespace deliberation {

MultilineLabel::MultilineLabel(const Font &font) :
    m_font(font)
{

}

void MultilineLabel::setPosition(const glm::vec2 &position)
{
    m_position = position;
}

void MultilineLabel::setCenter(const glm::vec2 &center)
{
    m_center = center;
}

void MultilineLabel::setColor(const glm::vec3 &color)
{
    m_color = color;
}

void MultilineLabel::setText(size_t line, const std::string &text)
{
    auto iter = m_labelByLine.find(line);
    if (iter == m_labelByLine.end()) {
        auto label = std::make_shared<Label>(m_font);
        bool success;
        std::tie(iter, success) = m_labelByLine.emplace(line, label);
    }

    iter->second->setText(text);
}

void MultilineLabel::setFontSize(unsigned int fontSize)
{
    m_fontSize = fontSize;
}

void MultilineLabel::render(LabelRenderer &renderer, const Viewport &viewport)
{
    if (m_labelByLine.empty()) return;

    float y = 1.0f;

    for (const auto &pair : m_labelByLine) {
        auto & label = *pair.second;

        label.setCenter({-1.0f, 1.0f});

        label.setPosition({-1.0f, y});

        renderer.render(label, viewport);

        y -= 2.0f * ((float)label.texture().height() / viewport.height());
    }


}

}