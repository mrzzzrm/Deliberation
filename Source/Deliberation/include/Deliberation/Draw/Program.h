#pragma once

#include <memory>

namespace deliberation
{

namespace detail
{
    class ProgramImpl;
}

class ProgramInterface;

class Program
{
public:
    bool isValid() const;

    const ProgramInterface & interface() const;

private:
    friend class Context;

private:
    std::shared_ptr<detail::ProgramImpl> m_impl;
};

}

