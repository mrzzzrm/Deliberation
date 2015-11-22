#pragma once

namespace deliberation
{

class Draw;
class GLStateManager;

namespace detail
{

class DrawImpl;

class DrawExecution final
{
public:
    DrawExecution(GLStateManager & glStateManager,
                  const Draw & draw);

    void perform();

private:
    void drawElementsInstanced() const;
    void drawElements() const;
    void drawArrays() const;
    void drawArraysInstanced() const;

    unsigned int elementCount() const;
    unsigned int vertexCount() const;
    unsigned int instanceCount() const;

    void applyDepthState();
    void applyBlendState();
    void applyCullState();
    void applyRasterizerState();
    void applyStencilState();

private:
    GLStateManager & m_glStateManager;
    const DrawImpl & m_drawImpl;
};

}

}


