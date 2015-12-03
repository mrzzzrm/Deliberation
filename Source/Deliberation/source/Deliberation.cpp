#include <Deliberation/Deliberation.h>

#include <iostream>
#include <cassert>

#include <IL/il.h>
#include <IL/ilu.h>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

namespace deliberation
{

void init()
{
    ilInit();
    ilEnable(IL_ORIGIN_SET);
    iluInit();
}

void shutdown()
{

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

