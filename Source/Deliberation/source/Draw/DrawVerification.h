//#pragma once
//
//#include <string>
//#include <sstream>
//
//class Draw;
//
//class DrawVerification
//{
//public:
//    DrawVerification(const Draw & command);
//
//    bool passed() const;
//
//    std::string toString() const;
//
//    void perform() const;
//
//private:
//    bool verifyOutput() const;
//    bool verifyProgram() const;
//    bool verifyVAO() const;
//    bool verifyUniforms() const;
//
//private:
//    const Draw & m_command;
//    mutable bool m_dirty;
//    mutable bool m_passed;
//    mutable std::string m_report;
//    mutable std::stringstream m_reportStream;
//};
//
