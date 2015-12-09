#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Viewport.h>

#include <Deliberation/Draw/Texture.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Font;

class Label final
{
public:
    Label();
    Label(const Font & font);

    const glm::vec2 & position() const;
    const glm::vec2 & center() const;
    const glm::vec3 & color() const;
    const std::string & text() const;
    unsigned int fontSize() const;
    const Texture & texture() const;
    const glm::mat4 & transform(const Viewport & viewport) const;

    void setPosition(const glm::vec2 & position);
    void setCenter(const glm::vec2 & center);
    void setColor(const glm::vec3 & color);
    void setText(const std::string & text);
    void setFontSize(unsigned int fontSize);

private:
    void renderText() const;
    void updateTransform(const Viewport & viewport) const;

private:
    const Font *        m_font;
    glm::vec2           m_position;
    glm::vec2           m_center;
    glm::vec3           m_color;
    std::string         m_text;
    mutable Texture     m_texture;
    mutable bool        m_textureDirty;
    unsigned int        m_fontSize;
    mutable glm::mat4   m_transform;
    mutable Viewport    m_transformCachedViewport;
    mutable bool        m_transformDirty;
};

}

