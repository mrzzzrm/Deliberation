#pragma once

namespace deliberation
{

template<typename BlobType>
BlobValueAccessorData<BlobType>::BlobValueAccessorData(BlobType & data,
                                             const DataLayout & layout,
                                             const DataLayoutField & field):
    data(data), layout(layout), field(field)
{}

}