#pragma once

#include <memory>

namespace deliberation
{

class DrawImpl;
class ProgramImpl;
class DrawContext;
class ProgramInterface;

class Program final
{
public:
    Program();

    DrawContext & drawContext() const;

    const ProgramInterface & interface() const;

private:
    friend class DrawContext;
    friend class Draw;
    friend class DrawImpl;

private:
    Program(const std::shared_ptr<ProgramImpl> & impl);

private:
    std::shared_ptr<ProgramImpl> m_impl;
};

}

