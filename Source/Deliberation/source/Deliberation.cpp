#include <Deliberation/Deliberation.h>

#include <iostream>
#include <cassert>

#include <IL/il.h>
#include <IL/ilu.h>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <SDL2/SDL_ttf.h>

namespace deliberation
{

void init()
{
    // DevIL
    {
        /*
            TODO
                Error checking
        */

        ilInit();
        ilEnable(IL_ORIGIN_SET);
        iluInit();
    }

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

}

