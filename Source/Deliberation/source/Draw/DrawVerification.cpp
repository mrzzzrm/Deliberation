//#include "DrawVerification.h"
//
//#include "Draw.h"
//
//DrawVerification::DrawVerification(const Draw & command):
//    m_command(command),
//    m_dirty(true),
//    m_passed(false)
//{
//
//}
//
//bool DrawVerification::passed() const
//{
//    if (m_dirty)
//    {
//        perform();
//    }
//
//    return m_passed;
//}
//
//std::string DrawVerification::toString() const
//{
//    if (m_dirty)
//    {
//        perform();
//    }
//
//    return m_report;
//}
//
//void DrawVerification::perform() const
//{
//    if (!m_dirty)
//    {
//        return;
//    }
//
//    m_passed = true;
//    m_passed &= verifyProgram();
//    m_passed &= verifyVAO();
//    m_passed &= m_command.m_state.hasViewport();
//    m_passed &= verifyOutput();
//    m_passed &= verifyUniforms();
//
//    if (!m_passed)
//    {
//        m_reportStream << "Draw '" << m_command.name() << "' failed verification" << std::endl;
//
//        m_report = m_reportStream.str();
//    }
//
//    m_dirty = false;
//}
//
//bool DrawVerification::verifyOutput() const
//{
//    if (!m_command.m_output.engaged())
//    {
//        return false;
//    }
//
//    auto & output = m_command.m_output.get();
//    auto & fragmentOutputs = m_command.m_program.layout().fragmentOutputs();
//
//    if (output.mode() == DrawOutputConfig::ToBackbuffer)
//    {
//        if (fragmentOutputs.size() == 1)
//        {
//            return true;
//        }
//        else
//        {
//            m_reportStream << fragmentOutputs.size() << " outputs in draw to backbuffer" << std::endl;
//            return false;
//        }
//    }
//    else
//    {
//        bool passed = true;
//
//        for (auto & fragmentOutput : fragmentOutputs)
//        {
//            auto location = fragmentOutput.location();
//
//            if (location >= output.rts().size())
//            {
//                m_reportStream << "Fragment output " << fragmentOutput.toString() << " not mapped to render target" << std::endl;
//                passed = false;
//            }
//        }
//
//        return passed;
//    }
//}
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
//
