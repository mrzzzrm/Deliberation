#include <Deliberation/Draw/Context.h>

#include <iostream>

#include <glbinding/gl/functions.h>

#include <Deliberation/Draw/Buffer.h>

#include "Detail/BufferImpl.h"
#include "Detail/DrawImpl.h"
#include "Detail/ProgramImpl.h"
#include "BufferUploadExecution.h"

namespace deliberation
{

Context::Context()
{
    std::cout << "Creating Deliberation Context" << std::endl;
}

Buffer Context::createBuffer(const BufferLayout & layout)
{
    return Buffer(*this, layout);
}

Program Context::createProgram(const std::vector<std::string> & paths)
{
    Program program;
    program.m_impl = std::make_shared<detail::ProgramImpl>(paths);
    return program;
}

Draw Context::createDraw(const Program & program, gl::GLenum primitive, const std::string & name)
{
    auto impl = std::make_shared<detail::DrawImpl>(*this, program);
    impl->state.setPrimitive(primitive);
    impl->name = name;

    Draw draw;
    draw.impl = impl;

    return draw;
}

Draw Context::createDraw(const Program & program, const DrawState & drawState, const std::string & name = std::string());
{
    auto impl = std::make_shared<detail::DrawImpl>(*this, program);
    impl->state = drawState;
    impl->name = name;

    Draw draw;
    draw.impl = impl;

    return draw;
}

void Context::allocateBuffer(detail::BufferImpl & buffer)
{
    Assert(buffer.glName == 0u, "");

    gl::GLuint glName;
    gl::glGenBuffers(1, &glName);
    Assert(glName != 0u, "");

    buffer.glName = glName;
}

void Context::deallocateBuffer(detail::BufferImpl & buffer)
{
    Assert(buffer.glName != 0u, "");

    m_glStateManager.deleteBuffer(buffer.glName);

    buffer.glName = 0u;
}

void Context::scheduleBufferUpload(const BufferUpload & upload)
{
//    switch(m_mode)
//    {
//    case Mode::Immediate:
        executeBufferUpload(upload);
//        break;
//    case Mode::Deferred:
//        m_bufferUploadCommands.push_back(command);
//        m_commands.push_back(CommandType::BufferUpload);
//        break;
//    default:
//        Fail("");
//   }
}

void Context::executeBufferUpload(const BufferUpload & upload)
{
    BufferUploadExecution(m_glStateManager, upload).perform();
}

};

