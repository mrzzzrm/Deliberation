#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLSLSizeOf.h>
#include <Deliberation/Draw/BufferLayoutField.h>

namespace deliberation
{

namespace context_detail
{

template<typename Head>
void appendPacked(std::vector<BufferLayoutField> & fields, size_t & stride, const std::string * name, gl::GLuint offset)
{
    auto type = GLSLTypeOf<Head>::value;
    auto field = BufferLayoutField(*name, GLSLSizeOf(type), type, offset);

    fields.push_back(field);
    stride = offset + GLSLSizeOf(type);
}

template<typename Head, typename Neck, typename ... Tail>
void appendPacked(std::vector<BufferLayoutField> & fields, size_t & stride, const std::string * names, gl::GLuint offset)
{
    auto type = GLSLTypeOf<Head>::value;

    appendPacked<Head>(fields, stride, &names[0], offset);
    appendPacked<Neck, Tail...>(fields, stride, &names[1], offset + GLSLSizeOf(type));
}

}

template<typename ... Types>
BufferLayout Context::createPackedBufferLayout(const std::vector<std::string> & names)
{
    Assert(names.size() == sizeof...(Types), "Length mismatch");
    Assert(!names.empty(), "Can't create empty layout");

    std::vector<BufferLayoutField> fields;
    size_t stride = 0u;

    context_detail::appendPacked<Types...>(fields, stride, names.data(), 0);

    return BufferLayout(fields, stride);
}

template<typename T>
BufferLayout Context::createBufferLayout(const std::vector<detail::NamedDataMemberOf<T>> & members)
{
    std::vector<BufferLayoutField> fields;

    T dummy;

    for (auto & member : members)
    {
        auto type = member.type();
        auto field = BufferLayoutField(member.name(), GLSLSizeOf(type), type, member.offset(dummy));
        fields.push_back(field);
    }

    return BufferLayout(fields, sizeof(T));
}

template<typename T>
BufferUpload Context::createBufferUpload(Buffer & buffer, const std::vector<T> & data)
{
    return BufferUpload(*this, buffer, data);
}

template<typename T>
BufferUpload Context::createBufferUpload(Buffer & buffer, const std::vector<T> && data)
{
    return BufferUpload(*this, buffer, std::move(data));
}

template<typename T>
BufferUpload Context::createRawBufferUpload(Buffer & buffer, const std::vector<T> & data, unsigned int count)
{
    return BufferUpload(*this, buffer, data, count);
}

template<typename T>
BufferUpload Context::createRawBufferUpload(Buffer & buffer, const std::vector<T> && data, unsigned int count)
{
    return BufferUpload(*this, buffer, std::move(data), count);
}

}

