namespace deliberation
{
template<typename T>
void VertexAttribute::set(const T & value)
{
    AssertM(Type::resolve<T>() == m_attribute.type(), "Type mismatch");

    set((const void *)&value);
}
}