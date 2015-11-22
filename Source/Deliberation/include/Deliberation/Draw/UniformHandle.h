#pragma once

#include <memory>

#include "GL/GLSLValue.h"

class Draw;

class UniformHandle
{
private:
    template<typename T>
    void set(const T & value);

private:
    friend class Draw;

private:
    UniformHandle(std::shared_ptr<GLSLValue> & value);

private:
    std::shared_ptr<GLSLValue> m_value;
};

#include "UniformHandle.inl"
