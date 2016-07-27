namespace deliberation
{

template<typename T>
TypedBlobField<T> LayoutedBlob::field(const std::string & name)
{
    return field<T>(m_layout.field(name));
}

template<typename T>
TypedBlobField<T> LayoutedBlob::field(const DataLayoutField & field)
{
    return TypedBlobField<T>(m_data, m_layout, field);
}

template<typename T>
void LayoutedBlob::assign(const std::string & name, const std::vector<T> & values)
{
    field<T>(name).assign(values);
}

}