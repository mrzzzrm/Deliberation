#pragma once

#include <cinttypes>
#include <string>
#include <vector>

#include <glbinding/gl/enum.h>

class GLSLValue final
{
public:
    GLSLValue();

    template<typename T>
    GLSLValue(const T & value);

    GLSLValue(const GLSLValue & other);

    bool isValid() const;

    gl::GLenum type() const;
    const void * data() const;

    template<typename T>
    T value() const;

    template<typename T>
    void set(const T & value);

    std::string toString() const;

private:
    std::vector<uint8_t> m_data;
    gl::GLenum m_type;
};

#include "GLSLValue.inl"
