#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <variant>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Detail/UniformImpl.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/DrawState.h>

#include "AttributeBinding.h"
#include "BufferBinding.h"
#include "SamplerImpl.h"
#include "UniformBufferBinding.h"

namespace deliberation
{

class Context;
class Program;

namespace detail
{

class ProgramImpl;


using VertexAttributeValueBinding = std::experimental::variant<
    u32, i32, float, double,
    glm::uvec2, glm::ivec2, glm::vec2, glm::dvec2,
    glm::uvec3, glm::ivec3, glm::vec3, glm::dvec3,
    glm::uvec4, glm::ivec4, glm::vec4, glm::dvec4,
    glm::mat2, glm::mat3, std::unique_ptr<glm::mat4>,
    std::unique_ptr<glm::dmat2>, std::unique_ptr<glm::dmat3>, std::unique_ptr<glm::dmat4>
    >;

struct VertexAttributeBufferBinding
{
    std::shared_ptr<BufferImpl> buffer;
    size_t                      fieldIndex = 0;

    bool                        ranged = false;
    size_t                      first = 0;
    size_t                      count = 0;
    size_t                      divisor = 0;
};

using VertexAttributeBinding = std::experimental::variant<VertexAttributeValueBinding,
    VertexAttributeBufferBinding>;

class DrawImpl final
{
public:
    DrawImpl(Context & context,
             const Program & program);

    Context &                                   context;
    std::string                                 name;
    std::shared_ptr<ProgramImpl>                program;
    BufferBinding                               indexBufferBinding;
    bool                                        indexBufferBindingDirty = true;
    std::vector<VertexAttributeBinding>         vertexAttributes;
    std::vector<size_t>                         dirtyVertexAttributes;
    DrawState                                   state;
    Framebuffer                                 framebuffer;
    gl::GLuint                                  glVertexArray;
    std::vector<UniformImpl>                    uniforms;
    std::vector<SamplerImpl>                    samplers;
    std::vector<Optional<UniformBufferBinding>> uniformBuffers;
};

}

}

