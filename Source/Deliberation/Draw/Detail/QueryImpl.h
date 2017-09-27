#pragma once

#include <stdint.h>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Query.h>

namespace deliberation
{
class DrawContext;

class QueryImpl final
{
public:
    QueryImpl(QueryType type);
    ~QueryImpl();

    QueryType type() const;

    int64_t  resultI() const;
    uint64_t resultU() const;

    bool isActive() const;
    bool isResultAvailable() const;

    void begin();
    void end();

private:
    QueryType     m_type;
    gl::GLuint    m_glName;
    gl::GLenum    m_glTarget;
    bool          m_active;
    mutable bool  m_resultsAvailable;

    mutable union
    {
        uint64_t u;
        int64_t  i;
    } m_result;
};
}
