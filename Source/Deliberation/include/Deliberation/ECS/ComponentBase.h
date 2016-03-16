#pragma once

#include <cstdlib>
#include <string>

namespace deliberation
{

class ComponentBase
{
public:
    virtual ~ComponentBase();

    virtual std::size_t index() const = 0;
    virtual std::string name() const = 0;
};

}

