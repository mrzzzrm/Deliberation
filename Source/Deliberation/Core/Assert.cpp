#include "assert.h"

#include <cassert>
#include <iostream>
#include <string>

void __Assert(const std::string & file,
              const std::string & function,
              unsigned int line,
              bool expr,
              const std::string & msg)
{
    if (expr)
    {
        return;
    }

    std::cerr << "------------------- ASSERT: " << file << ": " << line << " (" << function << ") -------------------" << std::endl <<
                               msg << std::endl;
}

void __Fail(const std::string & file,
            const std::string & function,
            unsigned int line,
            const std::string & msg)
{
    std::cerr << "------------------- FAIL: " << file << ":" << line << " (" << function << ") -------------------" << std::endl <<
                             msg << std::endl;
}

