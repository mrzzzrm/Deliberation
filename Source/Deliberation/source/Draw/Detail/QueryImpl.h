#pragma once

#include <stdint.h>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Enum.h>

namespace deliberation
{

class DrawContext;

namespace detail
{

class QueryImpl final
{
public:
    QueryImpl(DrawContext & context, QueryType type);
    ~QueryImpl();

    QueryType type() const;

    int64_t resultI() const;
    uint64_t resultU() const;

    bool isActive() const;
    bool isResultAvailable() const;

    void begin();
    void end();

private:
    DrawContext &           m_context;
    QueryType           m_type;
    gl::GLuint          m_glName;
    gl::GLenum          m_glTarget;
    bool                m_active;
    mutable bool        m_resultsAvailable;

    mutable union
    {
        uint64_t u;
        int64_t i;
    } m_result;
};

}

}

