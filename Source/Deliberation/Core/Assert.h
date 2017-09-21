#pragma once

#include <cassert>
#include <iostream>
#include <string>

#include <Deliberation/Core/Log.h>
#include <Deliberation/Deliberation.h>

// @{
/**
 * Asserts even available in Release builds
 */
#define AssertF(expr, msg, ...)                                                 \
    {                                                                          \
        ::deliberation::AssertImpl(                                            \
            __FILE__, __func__, __LINE__, (expr), (msg), __VA_ARGS__);         \
    }
#define AssertM(expr, msg)                                                 \
    {                                                                          \
        ::deliberation::AssertImpl(                                            \
            __FILE__, __func__, __LINE__, (expr), (msg));         \
    }
#define Assert(expr)                                                 \
    {                                                                          \
        ::deliberation::AssertImpl(                                            \
            __FILE__, __func__, __LINE__, (expr), nullptr);         \
    }
// @}

#if DELIBERATION_BUILD_TYPE_DEBUG

// @{
/**
 * Asserts only included in debug builds, for hot loops etc
 */
#define DebugAssertF(expr, msg, ...)                                                 \
    {                                                                          \
        ::deliberation::AssertImpl(                                            \
            __FILE__, __func__, __LINE__, (expr), (msg), __VA_ARGS__);         \
    }
#define DebugAssertM(expr, msg)                                                 \
    {                                                                          \
        ::deliberation::AssertImpl(                                            \
            __FILE__, __func__, __LINE__, (expr), (msg));         \
    }
#define DebugAssert(expr)                                                 \
    {                                                                          \
        ::deliberation::AssertImpl(                                            \
            __FILE__, __func__, __LINE__, (expr), nullptr);         \
    }
// @}


#define Fail(msg)                                                              \
    {                                                                          \
        ::deliberation::FailImpl(__FILE__, __func__, __LINE__, (msg));         \
    }

#else

#define DebugAssertF(expr, msg)
#define DebugAssertM(expr, msg)
#define DebugAssert(expr, msg)
#define Fail(msg)

#endif

namespace deliberation
{

template<typename T, typename ... Args>
inline void AssertImpl(
    const char * file,
    const char * function,
    unsigned int line,
    const T &    expr,
    const char * msg,
    Args &&...   args)
{
    if (expr) return;

    Log->error("\"------------------- ASSERT: {}: {}():{} -------------------", file, function, line);
    if (msg != nullptr) Log->error(msg, std::forward<Args>(args)...);

    assert(false);
}

template<typename T, typename ... Args>
inline void AssertImpl(
    const char *        file,
    const char *        function,
    unsigned int        line,
    const T &           expr,
    const std::string & msg,
    Args &&...   args)
{
    AssertImpl(file, function, line, static_cast<bool>(expr), msg.c_str(), std::forward<Args>(args)...);
}

inline void FailImpl(
    const char * file,
    const char * function,
    unsigned int line,
    const char * msg)
{
    std::cerr << "------------------- FAIL: " << file << ":" << line << " ("
              << function << ") -------------------" << std::endl
              << msg << std::endl;
    assert(false);
}

inline void FailImpl(
    const char *        file,
    const char *        function,
    unsigned int        line,
    const std::string & msg)
{
    FailImpl(file, function, line, msg.c_str());
}

}