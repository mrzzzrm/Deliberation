#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

namespace deliberation
{

template<> struct GLSLTypeOf<gl::GLboolean>{ static constexpr gl::GLenum value = gl::GL_BOOL;             };
template<> struct GLSLTypeOf<gl::GLint>    { static constexpr gl::GLenum value = gl::GL_INT;              };
template<> struct GLSLTypeOf<gl::GLuint>   { static constexpr gl::GLenum value = gl::GL_UNSIGNED_INT;     };
template<> struct GLSLTypeOf<gl::GLfloat>  { static constexpr gl::GLenum value = gl::GL_FLOAT;            };

template<> struct GLSLTypeOf<glm::ivec2>   { static constexpr gl::GLenum value = gl::GL_INT_VEC2;          };
template<> struct GLSLTypeOf<glm::uvec2>   { static constexpr gl::GLenum value = gl::GL_UNSIGNED_INT_VEC2; };
template<> struct GLSLTypeOf<glm::vec2>    { static constexpr gl::GLenum value = gl::GL_FLOAT_VEC2;        };
template<> struct GLSLTypeOf<glm::ivec3>   { static constexpr gl::GLenum value = gl::GL_INT_VEC3;          };
template<> struct GLSLTypeOf<glm::uvec3>   { static constexpr gl::GLenum value = gl::GL_UNSIGNED_INT_VEC3; };
template<> struct GLSLTypeOf<glm::vec3>    { static constexpr gl::GLenum value = gl::GL_FLOAT_VEC3;        };
template<> struct GLSLTypeOf<glm::ivec4>   { static constexpr gl::GLenum value = gl::GL_INT_VEC4;          };
template<> struct GLSLTypeOf<glm::uvec4>   { static constexpr gl::GLenum value = gl::GL_UNSIGNED_INT_VEC4; };
template<> struct GLSLTypeOf<glm::vec4>    { static constexpr gl::GLenum value = gl::GL_FLOAT_VEC4;        };
template<> struct GLSLTypeOf<glm::quat>    { static constexpr gl::GLenum value = gl::GL_FLOAT_VEC4;        };
template<> struct GLSLTypeOf<glm::mat2>    { static constexpr gl::GLenum value = gl::GL_FLOAT_MAT2;        };
template<> struct GLSLTypeOf<glm::mat3>    { static constexpr gl::GLenum value = gl::GL_FLOAT_MAT3;        };
template<> struct GLSLTypeOf<glm::mat4>    { static constexpr gl::GLenum value = gl::GL_FLOAT_MAT4;        };

}

