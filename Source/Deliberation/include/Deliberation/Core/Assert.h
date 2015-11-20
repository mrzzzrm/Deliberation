#pragma once

#include <cassert>
#include <string>

#include <Deliberation/Deliberation_API.h>

/*
    TODO
        - namespace / prefixing
*/

#define Assert(expr, msg) { __Assert(__FILE__, __func__, __LINE__, (expr), (msg)); assert((expr)); }
#define Fail(msg) { __Fail(__FILE__, __func__, __LINE__, (msg)); assert(false); }

void DELIBERATION_API __Assert(const std::string & file,
                               const std::string & function,
                               unsigned int line,
                               bool expr,
                               const std::string & msg);

void DELIBERATION_API __Fail(const std::string & file,
                             const std::string & function,
                             unsigned int line,
                             const std::string & msg);
