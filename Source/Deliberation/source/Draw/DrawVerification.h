#pragma once

#include <string>
#include <sstream>

namespace deliberation
{

class Draw;

class DrawVerification
{
public:
    DrawVerification(const Draw & draw);

    bool passed() const;

    std::string toString() const;

    void perform() const;

private:
    bool verifyFramebuffer() const;
//    bool verifyProgram() const;
//    bool verifyVAO() const;
//    bool verifyUniforms() const;

private:
    const Draw &                m_draw;
    mutable bool                m_dirty;
    mutable bool                m_passed;
    mutable std::string         m_report;
    mutable std::stringstream   m_reportStream;
};

}

