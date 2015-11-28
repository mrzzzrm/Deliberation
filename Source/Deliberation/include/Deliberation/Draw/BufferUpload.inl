#include "Buffer.h"

#include <string>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/BufferLayout.h>

namespace deliberation
{

template<typename T>
BufferUpload::BufferUpload(Context & context, Buffer & buffer, const std::vector<T> & data):
    m_context(context),
    m_buffer(buffer),
    m_blob(data),
    m_count(data.size())
{
}

template<typename T>
BufferUpload::BufferUpload(Context & context, Buffer & buffer, std::vector<T> && data):
    m_context(context),
    m_buffer(buffer)
{
    m_count = data.size();
    m_blob = Blob(std::move(data));
}

template<typename T>
BufferUpload::BufferUpload(Context & context, Buffer & buffer, const std::vector<T> & data, unsigned int count):
    m_context(context),
    m_buffer(buffer),
    m_blob(data),
    m_count(count)
{
    Assert(sizeof(T) == m_buffer.layout().stride(),
           "BufferLayout stride " + std::to_string(m_buffer.layout().stride()) +
           " differs from data size " + std::to_string(sizeof(T)));

}

template<typename T>
BufferUpload::BufferUpload(Context & context, Buffer & buffer, std::vector<T> && data, unsigned int count):
    m_context(context),
    m_buffer(buffer),
    m_blob(std::move(data)),
    m_count(count)
{
    Assert(sizeof(T) == m_buffer.layout().stride(),
           "BufferLayout stride " + std::to_string(m_buffer.layout().stride()) +
           " differs from data size " + std::to_string(sizeof(T)));
}

}

