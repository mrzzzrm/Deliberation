#include "QueryImpl.h"

#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/GL/GLStateManager.h>

namespace deliberation
{
QueryImpl::QueryImpl(DrawContext & drawContext, QueryType type)
    : m_drawContext(drawContext)
    , m_type(type)
    , m_glName(0)
    , m_glTarget(gl::GL_NONE)
    , m_active(false)
    , m_resultsAvailable(false)
{
    m_drawContext.m_glStateManager.genQueries(1, &m_glName);
    AssertM(m_glName != 0, "Failed to create Query");
}

QueryImpl::~QueryImpl()
{
    if (m_glName != 0)
    {
        m_drawContext.m_glStateManager.deleteQueries(1, &m_glName);
    }
}

QueryType QueryImpl::type() const { return m_type; }

int64_t QueryImpl::resultI() const
{
    Assert(m_glName != 0);
    AssertM(!m_active, "Query still active");

    if (m_resultsAvailable)
    {
        return m_result.i;
    }

    gl::GLint result;
    gl::glGetQueryObjectiv(m_glName, gl::GL_QUERY_RESULT, &result);

    m_result.i = result;
    m_resultsAvailable = true;

    return m_result.i;
}

uint64_t QueryImpl::resultU() const
{
    Assert(m_glName != 0);
    AssertM(!m_active, "Query still active");

    if (m_resultsAvailable)
    {
        return m_result.u;
    }

    gl::GLuint result;
    gl::glGetQueryObjectuiv(m_glName, gl::GL_QUERY_RESULT, &result);

    m_result.u = result;
    m_resultsAvailable = true;

    return m_result.u;
}

bool QueryImpl::isActive() const { return m_active; }

bool QueryImpl::isResultAvailable() const
{
    Assert(m_glName != 0);
    AssertM(!m_active, "Query still active");

    if (m_resultsAvailable)
    {
        return true;
    }

    gl::GLint available;
    gl::glGetQueryObjectiv(m_glName, gl::GL_QUERY_RESULT_AVAILABLE, &available);

    m_resultsAvailable = (gl::GLboolean)available == gl::GL_TRUE;

    return m_resultsAvailable;
}

void QueryImpl::begin()
{
    AssertM(!m_active, "Can't restart query");
    Assert(m_glName != 0);

    m_drawContext.m_glStateManager.beginQuery((gl::GLenum)m_type, m_glName);

    m_active = true;
    m_resultsAvailable = false;
}

void QueryImpl::end()
{
    Assert(m_glName != 0);

    m_drawContext.m_glStateManager.endQuery((gl::GLenum)m_type);

    m_active = false;
}
}
