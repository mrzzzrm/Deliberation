#include "BufferUploadExecution.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLStateManager.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/BufferUpload.h>

#include "Detail/BufferImpl.h"

namespace deliberation
{

BufferUploadExecution::BufferUploadExecution(GLStateManager & glStateManager,
                                             const BufferUpload & upload):
    m_glStateManager(glStateManager),
    m_upload(upload)
{

}

void BufferUploadExecution::perform()
{
    auto & buffer = m_upload.buffer();
    auto glBufferName = buffer.m_impl->glName;

    Assert(glBufferName != 0, "");

    auto glTarget = gl::GL_COPY_WRITE_BUFFER;

    m_glStateManager.bindBuffer(glTarget, glBufferName);
    gl::glBufferData(glTarget, m_upload.data().size(), m_upload.data().ptr(), gl::GL_STATIC_DRAW);

    buffer.m_impl->count = m_upload.count();
}

}

