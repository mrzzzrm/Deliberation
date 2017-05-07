namespace deliberation
{

template<typename T>
VertexAttribute<T>::VertexAttribute(const std::shared_ptr<DrawImpl> & drawImpl, const ProgramInterfaceVertexAttribute & attribute):
    m_drawImpl(drawImpl),
    m_attribute(attribute)
{

}

template<typename T>
void VertexAttribute<T>::set(const T & value)
{
    detail::SetVertexAttribute(*m_drawImpl, m_attribute, (const void*)&value);
}

}