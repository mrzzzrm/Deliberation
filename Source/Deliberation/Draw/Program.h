#pragma once

#include <memory>

#include <Deliberation/Draw/AbstractDrawObject.h>

namespace deliberation
{
class DrawImpl;
class ProgramImpl;
class DrawContext;
class ProgramInterface;

class Program final:
    public AbstractDrawObject<ProgramImpl>
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
    explicit Program(const std::shared_ptr<ProgramImpl> & impl);
};
}
