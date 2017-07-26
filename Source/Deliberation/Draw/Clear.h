#pragma once

#include <memory>
#include <vector>

#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

#include <Deliberation/Draw/AbstractDrawObject.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class ClearImpl;
class Framebuffer;

class Clear final:
    public AbstractDrawObject<ClearImpl>
{
public:
    Clear() = default;
    Clear(const std::shared_ptr<ClearImpl> & impl);

    const Framebuffer & framebuffer() const;

    void setColor(const glm::vec4 & color);
    void setColor(const std::string & name, const glm::vec4 & color);
    void disableColor();
    void disableColor(const std::string & name);

    void setDepth(float depth);
    void disableDepth();

    void setStencil(u32 stencil);
    void disableStencil();

    void render();

private:
    friend class DrawContext;
    friend class ClearExecution;
};
}
