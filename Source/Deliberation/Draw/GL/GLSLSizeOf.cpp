#include <Deliberation/Draw/GL/GLSLSizeOf.h>

#include <cassert>
#include <iostream>

#include <glm/glm.hpp>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{
gl::GLint GLSLSizeOf(gl::GLenum type)
{
    switch (type)
    {
    case gl::GL_BYTE: return sizeof(gl::GLbyte);
    case gl::GL_UNSIGNED_BYTE: return sizeof(gl::GLubyte);
    case gl::GL_SHORT: return sizeof(gl::GLshort);
    case gl::GL_UNSIGNED_SHORT: return sizeof(gl::GLushort);
    case gl::GL_INT: return sizeof(gl::GLint);
    case gl::GL_UNSIGNED_INT: return sizeof(gl::GLuint);

    case gl::GL_FLOAT: return sizeof(gl::GLfloat);

    case gl::GL_INT_VEC2: return sizeof(glm::ivec2);
    case gl::GL_INT_VEC3: return sizeof(glm::ivec3);
    case gl::GL_INT_VEC4: return sizeof(glm::ivec4);

    case gl::GL_UNSIGNED_INT_VEC2: return sizeof(glm::uvec2);
    case gl::GL_UNSIGNED_INT_VEC3: return sizeof(glm::uvec3);
    case gl::GL_UNSIGNED_INT_VEC4: return sizeof(glm::uvec4);

    case gl::GL_FLOAT_VEC2: return sizeof(glm::vec2);
    case gl::GL_FLOAT_VEC3: return sizeof(glm::vec3);
    case gl::GL_FLOAT_VEC4:
        return sizeof(glm::vec4);

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

    case gl::GL_FLOAT_MAT2: return sizeof(glm::mat2);
    case gl::GL_FLOAT_MAT3: return sizeof(glm::mat3);
    case gl::GL_FLOAT_MAT4:
        return sizeof(glm::mat4);

    /*
        Image specification
    */
    // As described in docs
    case gl::GL_RGBA32F: return 4 * 4;
    case gl::GL_RGBA32I: return 4 * 4;
    case gl::GL_RGBA32UI: return 4 * 4;
    case gl::GL_RGBA16: return 4 * 2;
    case gl::GL_RGBA16F: return 4 * 2;
    case gl::GL_RGBA16I: return 4 * 2;
    case gl::GL_RGBA16UI: return 4 * 2;
    case gl::GL_RGBA8: return 4 * 1;
    case gl::GL_SRGB8_ALPHA8: return 4 * 1;
    case gl::GL_RGB10_A2: return 4;
    case gl::GL_RGB10_A2UI: return 4;
    case gl::GL_R11F_G11F_B10F: return 4;
    case gl::GL_RG32F: return 2 * 4;
    case gl::GL_RG32I: return 2 * 4;
    case gl::GL_RG32UI: return 2 * 4;
    case gl::GL_RG16: return 2 * 2;
    case gl::GL_RG16F: return 2 * 2;
    case gl::GL_RGB16I: return 3 * 2;
    case gl::GL_RGB16UI: return 3 * 2;
    case gl::GL_RG8: return 2 * 1;
    case gl::GL_RG8I: return 2 * 1;
    case gl::GL_RG8UI: return 2 * 1;
    case gl::GL_R32F: return 1 * 4;
    case gl::GL_R32I: return 1 * 4;
    case gl::GL_R32UI: return 1 * 4;
    case gl::GL_R16F: return 1 * 2;
    case gl::GL_R16I: return 1 * 2;
    case gl::GL_R16UI: return 1 * 2;
    case gl::GL_R8: return 1 * 1;
    case gl::GL_R8I: return 1 * 1;
    case gl::GL_R8UI: return 1 * 1;
    case gl::GL_RGBA16_SNORM:
        return 4 * 2; // TODO: Verify
    case gl::GL_RGBA8_SNORM:
        return 4 * 1; // TODO: Verify
    case gl::GL_RGB32F: return 3 * 4;
    case gl::GL_RGB32I: return 3 * 4;
    case gl::GL_RGB32UI: return 3 * 4;
    case gl::GL_RGB16_SNORM:
        return 3 * 4; // TODO: Verify
    case gl::GL_RGB16F: return 3 * 2;
    case gl::GL_RGB16: return 3 * 2;
    case gl::GL_RGB8_SNORM:
        return 3 * 1; // TODO: Verify
    case gl::GL_RGB8: return 3 * 1;
    case gl::GL_RGB8I: return 3 * 1;
    case gl::GL_RGB8UI: return 3 * 1;
    case gl::GL_RGB9_E5: return 3;
    case gl::GL_RG16_SNORM:
        return 2 * 2; // TODO: Verify
    case gl::GL_RG8_SNORM:
        return 2 * 1; // TODO: Verify
    case gl::GL_R16_SNORM:
        return 1 * 2; // TODO: Verify
    case gl::GL_R8_SNORM:
        return 1 * 1; // TODO: Verify
    case gl::GL_DEPTH_COMPONENT32F: return 1 * 4;
    case gl::GL_DEPTH_COMPONENT24: return 1 * 3;
    case gl::GL_DEPTH_COMPONENT16: return 1 * 2;
    case gl::GL_DEPTH32F_STENCIL8:
        return 4 + 1; // TODO: Verify
    case gl::GL_DEPTH24_STENCIL8:
        return 3 + 1; // TODO: Verify

    // Not listed in the 4.5 standard, might be deprecated
    case gl::GL_RGB: return 3 * 4;
    case gl::GL_RGBA: return 4 * 4;
    case gl::GL_ALPHA: return 4;
    case gl::GL_ALPHA8: return 1;
    case gl::GL_ALPHA16:
        return 2;

    // Not listed in docs, but in standard (4.5)
    case gl::GL_R3_G3_B2: return 1;
    case gl::GL_RGBA4: return 2;
    case gl::GL_RGB565: return 2;
    case gl::GL_RGB5_A1:
        return 2;

    // TODO Can't track these right now, dunno if I ever can
    case gl::GL_COMPRESSED_RG_RGTC2:
    case gl::GL_COMPRESSED_SIGNED_RG_RGTC2:
    case gl::GL_COMPRESSED_RED_RGTC1:
    case gl::GL_COMPRESSED_SIGNED_RED_RGTC1: return 0;

    default:
        std::cout << "No such type " << type << std::endl;
        // Fail("No such type");
    }

    return -1;
}
}
