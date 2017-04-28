#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/DataLayoutField.h>

namespace deliberation
{

namespace data_layout_detail
{

template<typename Head>
void appendPacked(std::vector<DataLayoutField> & fields, size_t & stride, const std::string * name, size_t offset)
{
    auto & type = Type::resolve<Head>();
    auto field = DataLayoutField(*name, type, offset);

    fields.push_back(field);
    stride = offset + type.size();
}

template<typename Head, typename Neck, typename ... Tail>
void appendPacked(std::vector<DataLayoutField> & fields, size_t & stride, const std::string * names, size_t offset)
{
    auto & type = Type::resolve<Head>();

    appendPacked<Head>(fields, stride, &names[0], offset);
    appendPacked<Neck, Tail...>(fields, stride, &names[1], offset + type.size());
}

}

template<typename T>
DataLayout DataLayout::fromStructOrClass(const std::vector<detail::NamedDataMemberOf<T>> & members)
{
    std::vector<DataLayoutField> fields;

    T dummy;

    for (auto & member : members)
    {
        auto & type = member.type();
        auto field = DataLayoutField(member.name(), type, member.offset(dummy));
        fields.push_back(field);
    }

    return DataLayout(std::move(fields), sizeof(T));
}

}



