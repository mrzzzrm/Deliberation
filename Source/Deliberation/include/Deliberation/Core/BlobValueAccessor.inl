#pragma once

namespace deliberation
{

template<typename BlobType>
BlobValueAccessorData::BlobValueAccessorData(BlobType & data,
                                             const DataLayout & layout,
                                             const DataLayoutField & field):
    data(data), layout(layout), field(field)
{}

}