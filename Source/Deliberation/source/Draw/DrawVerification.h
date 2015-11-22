#pragma once

#include <string>
#include <sstream>

class DrawCommand;

class DrawVerification
{
public:
    DrawVerification(const DrawCommand & command);

    bool passed() const;

    std::string toString() const;

    void perform() const;

private:
    bool verifyOutput() const;
    bool verifyProgram() const;
    bool verifyVAO() const;
    bool verifyUniforms() const;

private:
    const DrawCommand & m_command;
    mutable bool m_dirty;
    mutable bool m_passed;
    mutable std::string m_report;
    mutable std::stringstream m_reportStream;
};

