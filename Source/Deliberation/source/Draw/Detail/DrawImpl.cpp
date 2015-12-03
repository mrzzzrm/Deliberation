#include "DrawImpl.h"

#include <algorithm>

#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/ProgramInterface.h>

namespace deliberation
{

namespace detail
{

DrawImpl::DrawImpl(Context & context,
                   Program & program):
    context(context),
    program(program),
    indexBuffer(nullptr),
    glVertexArray(0u)
{
    /*
        TODO
            This stuff could be cached per program
    */

    // Create UniformHandleImpls
    {
        uniforms.reserve(program.interface().uniforms().size());
        for (auto & uniform : program.interface().uniforms())
        {
            uniforms.emplace_back(uniform.type(), uniform.location());
        }
    }

    // Create TextureBindings
    {
        textureBindings.reserve(program.interface().samplers().size());
        for (auto & sampler : program.interface().samplers())
        {
            textureBindings.emplace_back(sampler.type(), sampler.location());
        }
    }
}

}

}



//    for (auto & uniform : m_program.layout().uniforms())
//    {
//        d_unsetUniformLocations.insert(uniform.location());
//    }
