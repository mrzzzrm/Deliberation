namespace deliberation
{

template<typename T>
VertexAttribute<T>::VertexAttribute(Draw & draw, const ProgramInterfaceVertexAttribute & attribute):
    m_draw(draw),
    m_attribute(attribute)
{

}

template<typename T>
void VertexAttribute<T>::set(const T & value)
{
    detail::SetVertexAttribute(m_draw, m_attribute, (const void*)&value);
}

}