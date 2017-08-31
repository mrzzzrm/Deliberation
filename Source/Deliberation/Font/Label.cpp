#include <Deliberation/Font/Label.h>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/Viewport.h>

#include <Deliberation/Font/Font.h>

namespace deliberation
{
Label::Label()
    : m_font(nullptr)
    , m_fontSize(16u)
    , m_textureDirty(false)
    , m_transformCachedViewport(0u, 0u, 0u, 0u)
    , m_transformDirty(true)
{
}

Label::Label(const Font & font)
    : m_font(&font)
    , m_fontSize(16u)
    , m_textureDirty(false)
    , m_transformCachedViewport(0u, 0u, 0u, 0u)
    , m_transformDirty(true)
{
}

const glm::vec2 & Label::position() const { return m_position; }

const glm::vec2 & Label::center() const { return m_center; }

const glm::vec3 & Label::color() const { return m_color; }

const std::string & Label::text() const { return m_text; }

const Texture & Label::texture() const
{
    if (m_textureDirty)
    {
        renderText();
    }

    return m_texture;
}

unsigned int Label::fontSize() const { return m_fontSize; }

const glm::mat4 & Label::transform(const Viewport & viewport) const
{
    if (m_transformDirty || viewport != m_transformCachedViewport)
    {
        updateTransform(viewport);
    }

    return m_transform;
}

void Label::setPosition(const glm::vec2 & position)
{
    if (m_position == position)
    {
        return;
    }

    m_position = position;
    m_transformDirty = true;
}

void Label::setCenter(const glm::vec2 & center)
{
    if (m_center == center)
    {
        return;
    }

    m_center = center;
    m_transformDirty = true;
}

void Label::setColor(const glm::vec3 & color) { m_color = color; }

void Label::setText(const std::string & text)
{
    if (m_text == text)
    {
        return;
    }

    m_text = text;
    m_textureDirty = true;
    m_transformDirty = true;
}

void Label::setFontSize(unsigned int fontSize)
{
    if (m_fontSize == fontSize)
    {
        return;
    }

    m_fontSize = fontSize;
    m_textureDirty = true;
    m_transformDirty = true;
}

void Label::renderText() const
{
    AssertM(m_font, "Hollow Label object can't set text");

    if (!m_textureDirty) return;

    m_texture =
        m_font->render(m_text, m_fontSize, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    m_textureDirty = false;
}

void Label::updateTransform(const Viewport & viewport) const
{
    AssertM(viewport.width() != 0 && viewport.height() != 0, "Invalid viewport");

    auto align = glm::translate(glm::mat4(1.0f), glm::vec3(-m_center, 0.0f));

    auto scale = glm::scale(
        glm::mat4(1.0f),
        glm::vec3(
            float(texture().width()) / viewport.width(),
            float(texture().height()) / viewport.height(),
            1.0f));

    auto translate =
        glm::translate(glm::mat4(1.0f), glm::vec3(m_position, 0.0f));

    m_transform = translate * scale * align;

    m_transformDirty = false;
    m_transformCachedViewport = viewport;
}
}
