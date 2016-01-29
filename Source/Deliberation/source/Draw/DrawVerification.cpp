#include "DrawVerification.h"

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/ProgramInterface.h>

namespace deliberation
{

DrawVerification::DrawVerification(const Draw & draw):
    m_draw(draw),
    m_dirty(true),
    m_passed(false)
{

}

bool DrawVerification::passed() const
{
    if (m_dirty)
    {
        perform();
    }

    return m_passed;
}

std::string DrawVerification::toString() const
{
    if (m_dirty)
    {
        perform();
    }

    return m_report;
}

void DrawVerification::perform() const
{
    if (!m_dirty)
    {
        return;
    }

    m_passed = true;
//    m_passed &= verifyProgram();
//    m_passed &= verifyVAO();
//    m_passed &= m_command.m_state.hasViewport();
    m_passed &= verifyFramebuffer();
//    m_passed &= verifyUniforms();

    if (!m_passed)
    {
        m_reportStream << "Draw '" << m_draw.name() << "' failed verification" << std::endl;

        m_report = m_reportStream.str();
    }

    m_dirty = false;
}

bool DrawVerification::verifyFramebuffer() const
{
    auto & framebuffer = m_draw.framebuffer();
    auto & fragmentOutputs = m_draw.program().interface().fragmentOutputs();

    if (framebuffer.isBackbuffer())
    {
        if (fragmentOutputs.size() == 1)
        {
            return true;
        }
        else
        {
            m_reportStream << fragmentOutputs.size() << " outputs in draw to backbuffer" << std::endl;
            return false;
        }
    }
    else
    {
        bool passed = true;

        // Check if every fragmentOutput has a render target mapped to it
        for (auto & fragmentOutput : fragmentOutputs)
        {
            auto location = fragmentOutput.location();

            if (!framebuffer.renderTarget(location))
            {
                m_reportStream << "Fragment output " << fragmentOutput.toString() << " not mapped to render target" << std::endl;
                passed = false;
            }
        }

        // Check if every renderTarget is being written to
        for (auto rt = 0u; rt < framebuffer.renderTargets().size(); rt++)
        {
            if (!framebuffer.renderTarget(rt))
            {
                continue;
            }

            if (!m_draw.program().interface().fragmentOutputByLocation(rt))
            {
                m_reportStream << "RenderTarget " << std::to_string(rt) << " has no FragmentOutput mapped to it" << std::endl;
                passed = false;
            }
        }

        return passed;
    }
}
//
//bool DrawVerification::verifyProgram() const
//{
//    if (m_command.m_program.isValid())
//    {
//        return true;
//    }
//
//    m_reportStream << "Program is invalid" << std::endl;
//
//    return false;
//}
//
//bool DrawVerification::verifyVAO() const
//{
//    if (m_command.m_vao.isComplete())
//    {
//        return true;
//    }
//
//    m_reportStream << "VAO is invalid" << std::endl;
//    m_reportStream << m_command.m_vao.toString() << std::endl;
//
//    return false;
//}
//
//bool DrawVerification::verifyUniforms() const
//{
//    if (m_command.d_unsetUniformLocations.empty())
//    {
//        return true;
//    }
//
//    auto layout = m_command.m_program.layout();
//
//    m_reportStream << "Uniforms aren't  set:" << std::endl;
//    for (auto uniformLocation : m_command.d_unsetUniformLocations)
//    {
//        auto & uniform = layout.uniformByLocation(uniformLocation);
//        m_reportStream << "  " << uniform.toString() << std::endl;
//    }
//
//    return false;
//}

}

