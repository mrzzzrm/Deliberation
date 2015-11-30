#pragma once

namespace deliberation
{

class Clear;
class GLStateManager;

class ClearExecution
{
public:
    ClearExecution(GLStateManager & glStateManager, const Clear & clear);

    void perform();

private:
    GLStateManager & m_glStateManager;
    const Clear & m_clear;
};

}

