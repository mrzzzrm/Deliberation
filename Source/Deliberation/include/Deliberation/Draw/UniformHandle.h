#pragma once

#include <memory>

#include "GL/GLSLValue.h"

class DrawCommand;

class UniformHandle
{
private:
    template<typename T>
    void set(const T & value);

private:
    friend class DrawCommand;

private:
    UniformHandle(std::shared_ptr<GLSLValue> & value);

private:
    std::shared_ptr<GLSLValue> m_value;
};

#include "UniformHandle.inl"
