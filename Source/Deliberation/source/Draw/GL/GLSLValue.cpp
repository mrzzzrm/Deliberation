//#include <Deliberation/Draw/GL/GLSLValue.h>
//
//#include <cassert>
//#include <sstream>
//
//namespace deliberation
//{
//
//GLSLValue::GLSLValue():
//    m_type(gl::GL_INVALID_ENUM)
//{
//
//}
//
//GLSLValue::GLSLValue(const GLSLValue & other) = default;
//
//bool GLSLValue::isValid() const
//{
//    return m_type != gl::GL_INVALID_ENUM;
//}
//
//const void * GLSLValue::data() const
//{
//    return m_data.data();
//}
//
//gl::GLenum GLSLValue::type() const
//{
//    return m_type;
//}
//
//std::string GLSLValue::toString() const
//{
//    std::stringstream stream;
//    stream << "(" << m_type << " ";
//    for (auto b : m_data)
//    {
//        stream << (int)b << " ";
//    }
//    stream << ")";
//    return stream.str();
//}
//
//}
//
//
