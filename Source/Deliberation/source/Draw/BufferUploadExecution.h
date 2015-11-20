#pragma once

namespace deliberation
{

class BufferUpload;
class GLStateManager;

class BufferUploadExecution final
{
public:
    BufferUploadExecution(GLStateManager & glStateManager,
                          const BufferUpload & upload);

    void perform();

private:
    GLStateManager & m_glStateManager;
    const BufferUpload & m_upload;
};

}

