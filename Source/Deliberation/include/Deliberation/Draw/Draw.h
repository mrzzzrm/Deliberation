#pragma once

#include <string>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
    class DrawImpl;
}

class Context;
class DrawState;

class Draw final
{
public:
    const std::string & name() const;

//    bool hasOutput() const;
//    const DrawOutputConfig & output() const;

    DrawState & state();
    const DrawState & state() const;

//    UniformHandle uniform(const std::string & name);
//    TextureHandle texture(const std::string & name);

//    void setOutput(const DrawOutputConfig & output);

    void setState(const DrawState & state);

    void setIndexBuffer(const Buffer & buffer);
    void addVertexBuffer(const Buffer & buffer);
    void addVertexBufferRange(const Buffer & buffer, unsigned int first, unsigned int count);
    void addInstanceBuffer(const Buffer & buffer, unsigned int divisor = 1u);
    void addInstanceBufferRange(const Buffer & buffer, unsigned int first, unsigned int count, unsigned int divisor = 1u);

    void schedule();

    std::string toString() const;

private:
    friend class Context;
    friend class DrawExecution;
    friend class DrawVerification;

private:
    Draw(const std::shared_ptr<detail::DrawImpl> & impl);

    void init();

private:
    std::shared_ptr<detail::DrawImpl> m_impl;
};

}

#include <Deliberation/Draw/Draw.inl>
