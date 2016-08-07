#include <Deliberation/Deliberation.h>

#include <iostream>
#include <cassert>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <SDL2/SDL_ttf.h>

namespace
{

std::string prefixPath(".");

}

namespace deliberation
{

void init()
{
    // SDL_ttf
    {
        auto r = TTF_Init();
        if (r)
        {
            std::cout << "Failed to init SDL_ttf: '" << TTF_GetError() << "'" << std::endl;
        }
    }
}

void shutdown()
{
    // SDL_ttf
    if (TTF_WasInit())
    {
        TTF_Quit();
    }
}

const std::string & prefixPath()
{
    return ::prefixPath;
}

void setPrefixPath(const std::string & prefixPath)
{
    if (prefixPath.empty())
    {
        ::prefixPath = ".";
    }
    else
    {
        ::prefixPath = prefixPath;
    }
}

std::string dataPath(const std::string path)
{
    return prefixPath() + "/" + path;
}

void EnableGLErrorChecks()
{
    glbinding::setCallbackMask(glbinding::CallbackMask::After | glbinding::CallbackMask::ParametersAndReturnValue);
    glbinding::setAfterCallback([](const glbinding::FunctionCall & call)
    {
        glbinding::setCallbackMask(glbinding::CallbackMask::None);
        gl::GLenum error;
        while((error = gl::glGetError()) != gl::GL_NO_ERROR)
        {
            std::cout << "GL Error: " << error << std::endl;
            std::cout << "  after calling ";
            std::cout << call.function->name() << "(";
            for (unsigned i = 0; i < call.parameters.size(); ++i)
            {
                std::cout << call.parameters[i]->asString();
                if (i < call.parameters.size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << ")";
            std::cout << std::endl;
            assert(false);
        }
        glbinding::setCallbackMask(glbinding::CallbackMask::After | glbinding::CallbackMask::ParametersAndReturnValue);
    });
}

void EnableGLErrorChecksAndLogging()
{
    glbinding::setCallbackMask(glbinding::CallbackMask::After | glbinding::CallbackMask::ParametersAndReturnValue);
    glbinding::setAfterCallback([](const glbinding::FunctionCall & call)
    {
        std::cout << call.function->name() << "(";
        for (unsigned i = 0; i < call.parameters.size(); ++i)
        {
            std::cout << call.parameters[i]->asString();
            if (i < call.parameters.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << ")";
        std::cout << std::endl;

        glbinding::setCallbackMask(glbinding::CallbackMask::None);
        gl::GLenum error;
        while((error = gl::glGetError()) != gl::GL_NO_ERROR)
        {
            std::cout << "GL Error: " << error << std::endl;
            std::cout << "  after calling ";
            std::cout << call.function->name() << "(";
            for (unsigned i = 0; i < call.parameters.size(); ++i)
            {
                std::cout << call.parameters[i]->asString();
                if (i < call.parameters.size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << ")";
            std::cout << std::endl;
            assert(false);
        }
        glbinding::setCallbackMask(glbinding::CallbackMask::After | glbinding::CallbackMask::ParametersAndReturnValue);
    });
}

void DisableGLErrorChecks()
{
    glbinding::setCallbackMask(glbinding::CallbackMask::None);
}

}

