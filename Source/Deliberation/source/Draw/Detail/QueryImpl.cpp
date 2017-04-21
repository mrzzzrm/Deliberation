#include "QueryImpl.h"

#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/GL/GLStateManager.h>

namespace deliberation
{

namespace detail
{

QueryImpl::QueryImpl(DrawContext & context, QueryType type):
    m_context(context),
    m_type(type),
    m_glName(0),
    m_glTarget(gl::GL_NONE),
    m_active(false),
    m_resultsAvailable(false)
{
    switch (type)
    {
    case Query_TimeElapsed:                 m_glTarget = gl::GL_TIME_ELAPSED; break;
    case Query_SamplesPassed:               m_glTarget = gl::GL_SAMPLES_PASSED; break;
    case Query_AnySamplesPassed:            m_glTarget = gl::GL_ANY_SAMPLES_PASSED; break;
    case Query_PrimitivesGenerated:         m_glTarget = gl::GL_PRIMITIVES_GENERATED; break;
    case Query_TransformFeedbackPrimitives: m_glTarget = gl::GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN; break;
    default:
        Fail("");
    }

    m_context.m_glStateManager.genQueries(1, &m_glName);
    Assert(m_glName != 0, "Failed to create Query");
}

QueryImpl::~QueryImpl()
{
    if (m_glName != 0)
    {
        m_context.m_glStateManager.deleteQueries(1, &m_glName);
    }
}

QueryType QueryImpl::type() const
{
    return m_type;
}

int64_t QueryImpl::resultI() const
{
    Assert(m_glName != 0, "");
    Assert(!m_active, "Query still active");

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
    Assert(m_glName != 0, "");
    Assert(!m_active, "Query still active");

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

bool QueryImpl::isActive() const
{
    return m_active;
}

bool QueryImpl::isResultAvailable() const
{
    Assert(m_glName != 0, "");
    Assert(!m_active, "Query still active");

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
    Assert(!m_active, "Can't restart query");
    Assert(m_glName != 0, "");

    m_context.m_glStateManager.beginQuery(m_glTarget, m_glName);

    m_active = true;
    m_resultsAvailable = false;
}

void QueryImpl::end()
{
    Assert(m_glName != 0, "");

    m_context.m_glStateManager.endQuery(m_glTarget);

    m_active = false;
}

}

}

