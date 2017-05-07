#pragma once

#include <string>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

#include <Deliberation/Draw/Enum.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class RasterizerState final
{
public:
    RasterizerState();
    RasterizerState(DrawPrimitive primitive);
    RasterizerState(DrawPrimitive primitive,
                    float pointSize,
                    float lineWidth);

    DrawPrimitive primitive() const;
    float pointSize() const;
    float lineWidth() const;
    const glm::uvec4 & scissorRect() const;
    bool scissorRectEnabled() const;

    void setPrimitive(DrawPrimitive primitive);
    void setPointSize(float pointSize);
    void setLineWidth(float lineWidth);

    void enableScissorRect(u32 x, u32 y, u32 width, u32 height);
    void disableScissorRect();

    std::string toString() const;

private:
    DrawPrimitive   m_primitive;
    float           m_pointSize;
    float           m_lineWidth;
    glm::uvec4      m_scissorRect;
    bool            m_scissorRectEnabled = false;
};

}

