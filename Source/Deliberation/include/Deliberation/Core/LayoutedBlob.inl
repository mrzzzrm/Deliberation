namespace deliberation
{

template<typename T>
TypedBlobField<T> LayoutedBlob::field(const DataLayoutField & field)
{
    return TypedBlobField<T>(m_data, m_layout, field);
}

}