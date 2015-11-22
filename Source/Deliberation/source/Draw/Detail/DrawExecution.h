#pragma once

class DrawCommand;
class GLStateManager;

class DrawExecution final
{
public:
    DrawExecution(GLStateManager & glStateManager,
                  const DrawCommand & command);

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
    const DrawCommand & m_command;
};

