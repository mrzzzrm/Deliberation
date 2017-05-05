#pragma once

#include <glbinding/gl/enum.h>

#include <stdint.h>
#include <memory>

namespace deliberation
{

enum class QueryType: unsigned int
{
    TimeElapsed                   = (unsigned int)gl::GL_TIME_ELAPSED,
    SamplesPassed                 = (unsigned int)gl::GL_SAMPLES_PASSED,
    AnySamplesPassed              = (unsigned int)gl::GL_ANY_SAMPLES_PASSED,
    PrimitivesGenerated           = (unsigned int)gl::GL_PRIMITIVES_GENERATED,
    TransformFeedbackPrimitives   = (unsigned int)gl::GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
};

class QueryImpl;

class Query final
{
public:
    Query();
    ~Query();

    int64_t resultI() const;
    uint64_t resultU() const;

    QueryType type() const;

    bool isActive() const;
    bool isResultAvailable() const;

    void begin();
    void end();

private:
    friend class DrawContext;

private:
    Query(const std::shared_ptr<QueryImpl> & impl);

private:
    std::shared_ptr<QueryImpl> m_impl;
};

}

