#include <Deliberation/Draw/Uniform.h>

namespace deliberation
{

Uniform::Uniform():
    m_impl(nullptr)
{
}

Uniform::Uniform(detail::UniformImpl & impl):
    m_impl(&impl)
{

}

}


