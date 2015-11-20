#include <Deliberation/Draw/GL/GLSLSizeOf.h>

#include <cassert>
#include <iostream>

#include <glm/glm.hpp>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

gl::GLint GLSLSizeOf(gl::GLenum type)
{
    switch(type)
    {
    case gl::GL_INT:               return sizeof(gl::GLint);
    case gl::GL_UNSIGNED_INT:      return sizeof(gl::GLuint);
    case gl::GL_FLOAT:             return sizeof(gl::GLfloat);

    case gl::GL_INT_VEC2:          return sizeof(glm::ivec2);
    case gl::GL_INT_VEC3:          return sizeof(glm::ivec3);
    case gl::GL_INT_VEC4:          return sizeof(glm::ivec4);

    case gl::GL_UNSIGNED_INT_VEC2: return sizeof(glm::uvec2);
    case gl::GL_UNSIGNED_INT_VEC3: return sizeof(glm::uvec3);
    case gl::GL_UNSIGNED_INT_VEC4: return sizeof(glm::uvec4);

    case gl::GL_FLOAT_VEC2:        return sizeof(glm::vec2);
    case gl::GL_FLOAT_VEC3:        return sizeof(glm::vec3);
    case gl::GL_FLOAT_VEC4:        return sizeof(glm::vec4);

    /*case gl::GL_DOUBLE:            return sizeof(gl::GLdouble);
    case gl::GL_DOUBLE_VEC2:       return sizeof(glm:dvec2);
    case gl::GL_DOUBLE_VEC3:       return sizeof(glm:dvec3);
    case gl::GL_DOUBLE_VEC4:       return sizeof(glm:dvec4);
    case gl::GL_UNSIGNED_INT_VEC2: return sizeof(glm::uvec2);
    case gl::GL_UNSIGNED_INT_VEC3: return sizeof(glm::uvec3);
    case gl::GL_UNSIGNED_INT_VEC4: return sizeof(glm::uvec4);​
    case gl::GL_BOOL:              return sizeof(gl::GLboolean);​
    case gl::GL_BOOL_VEC2:         return sizeof(glm::bvec2);
    case gl::GL_BOOL_VEC3:         return sizeof(glm::bvec3);
    case gl::GL_BOOL_VEC4:         return sizeof(glm::bvec4);*/

    case gl::GL_FLOAT_MAT2:        return sizeof(glm::mat2);
    case gl::GL_FLOAT_MAT3:        return sizeof(glm::mat3);
    case gl::GL_FLOAT_MAT4:        return sizeof(glm::mat4);

    default:
        std::cout << "No such type " << type << std::endl;
        Fail("No such type");
    }

    return -1;
}

}


