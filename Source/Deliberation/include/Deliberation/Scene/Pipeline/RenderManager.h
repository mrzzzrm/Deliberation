#pragma once

namespace deliberation
{

class DrawContext;

class RenderManager
{
public:
    RenderManager(DrawContext & context);
    virtual ~RenderManager() = default;

};

}