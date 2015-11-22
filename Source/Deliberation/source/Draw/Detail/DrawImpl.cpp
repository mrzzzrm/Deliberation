#include "DrawImpl.h"

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
}

}

}



//    for (auto & uniform : m_program.layout().uniforms())
//    {
//        d_unsetUniformLocations.insert(uniform.location());
//    }
