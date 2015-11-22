#pragma once

#include <string>
#include <vector>

#include <globjects/base/ref_ptr.h>

#include <Deliberation/Draw/ProgramInterface.h>

namespace globjects
{
    class Program;
}

namespace deliberation
{

namespace detail
{

class ProgramImpl final
{
public:
    ProgramImpl(const std::vector<std::string> & paths);

    globjects::ref_ptr<globjects::Program> globjectsProgram;
    ProgramInterface interface;
};

}

}

