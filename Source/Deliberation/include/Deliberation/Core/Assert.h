#pragma once

#include <cassert>
#include <string>
#include <iostream>

#include <Deliberation/Deliberation.h>

#if DELIBERATION_BUILD_TYPE_DEBUG

#define Assert(expr, msg) { ::deliberation::AssertImpl(__FILE__, __func__, __LINE__, (expr), (msg)); assert((expr)); }
#define Fail(msg) { ::deliberation::FailImpl(__FILE__, __func__, __LINE__, (msg)); assert(false); }

#else

#define Assert(expr, msg)
#define Fail(msg)

#endif

namespace deliberation
{

#if DELIBERATION_BUILD_TYPE_DEBUG

inline void DELIBERATION_API AssertImpl(const char * file,
                                 const char * function,
                                 unsigned int line,
                                 bool expr,
                                 const char * msg)
{
    if (expr)
    {
        return;
    }

    std::cerr << "------------------- ASSERT: " << file << ": " << line << " (" << function << ") -------------------" << std::endl <<
                               msg << std::endl;
}

inline void DELIBERATION_API AssertImpl(const char * file,
                                 const char * function,
                                 unsigned int line,
                                 bool expr,
                                 const std::string & msg)
{
    AssertImpl(file, function, line, expr, msg.c_str());
}

inline void DELIBERATION_API AssertImpl(const char * file,
                                 const char * function,
                                 unsigned int line,
                                 const void * ptr,
                                 const char * msg)
{
    AssertImpl(file, function, line, ptr != nullptr, msg);
}

inline void DELIBERATION_API AssertImpl(const char * file,
                                 const char * function,
                                 unsigned int line,
                                 const void * ptr,
                                 const std::string & msg)
{
    AssertImpl(file, function, line, ptr != nullptr, msg);
}

inline void DELIBERATION_API FailImpl(const char * file,
                               const char * function,
                               unsigned int line,
                               const char * msg)
{
    std::cerr << "------------------- FAIL: " << file << ":" << line << " (" << function << ") -------------------" << std::endl <<
                             msg << std::endl;
}

inline void DELIBERATION_API FailImpl(const char * file,
                               const char * function,
                               unsigned int line,
                               const std::string & msg)
{
    FailImpl(file, function, line, msg.c_str());
}

#endif

}