#pragma once

#include <glbinding/gl/types.h>

namespace deliberation
{

class GLStateManager;
class TextureUpload;

class TextureUploadExecution final
{
public:
    TextureUploadExecution(GLStateManager & glStateManager,
                           const TextureUpload & upload);

    void perform();

private:
    void uploadSurfaces(gl::GLenum type);
    gl::GLenum faceTarget(gl::GLenum type, unsigned int face) const;

private:
    GLStateManager &        m_glStateManager;
    const TextureUpload &   m_upload;
};

}

