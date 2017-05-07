namespace deliberation
{
template<typename T>
void VertexAttribute::set(const T & value)
{
    Assert(Type::resolve<T>() == m_attribute.type(), "Type mismatch");

    set((const void *)&value);
}
}