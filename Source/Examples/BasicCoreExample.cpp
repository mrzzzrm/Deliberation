#include <iostream>
#include <chrono>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/DataLayoutField.h>
#include <Deliberation/Core/Types.h>
#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/LinearOctree.h>
#include <Deliberation/Core/RingBuffer.h>
#include <Deliberation/Core/SparseVector.h>
#include <Deliberation/Core/TypeID.h>
#include <Deliberation/Core/IntTypes.h>

using namespace deliberation;

void LinearMapExample()
{
    std::cout << "----------- LinearMapExample ----------" << std::endl;

    LinearMap<float> map;
    const auto & cmap = map;

    map[2] = 15.0f;
    map[5] = 25.0f;
    map[15] = 1337.0f;

    for (auto & pair : map)
    {
        std::cout << pair.first << " = " << pair.second << std::endl;
    }

    for (auto & pair : cmap)
    {
        std::cout << pair.first << " = " << pair.second << std::endl;
    }
}

void LinearOctreeExample()
{
    std::cout << "----------- LinearOctreeExample ----------" << std::endl;

    LinearOctree<int> tree(3);
    for (auto & node : tree.nodes())
    {
        node = 0;
    }

    auto iter = tree.root();
    std::cout << "Num nudes = " << tree.nodes().size() << std::endl;

    iter.node() = 5;

    iter.toFirstChild();
    iter.node() = 6;
    iter.toNextSibling();
    iter.node() = 7;

    iter.toFirstChild();
    iter.node() = 8;
    iter.toNextSibling();
    iter.toParent();
    iter.node() = 9; // Should overwrite 7

    iter.toNextSibling();
    iter.toNextSibling();
    iter.toFirstChild();
    iter.node() = 12;

    std::cout << tree.toString() << std::endl;
}

void RingBufferExample()
{
    std::cout << "----------- RingBufferExample - push() ----------" << std::endl;
    RingBuffer<int> buf(3);

    std::cout << buf.toString() << std::endl;

    buf.push(1);
    buf.push(3);

    std::cout << buf.toString() << std::endl;

    buf.push(7);

    std::cout << buf.toString() << std::endl;

    buf.push(4);

    std::cout << buf.toString() << std::endl;

    buf.push(2);

    std::cout << buf.toString() << std::endl;

    buf.push(8);
    buf.push(9);
    buf.push(0);

    std::cout << buf.toString() << std::endl;

    std::cout << "----------- RingBufferExample - reserve() ----------" << std::endl;

    buf.reserve(5);
    std::cout << buf.toString() << std::endl;
    buf.push(1);
    buf.push(3);
    std::cout << buf.toString() << std::endl;
    buf.push(7);
    buf.push(5);
    std::cout << buf.toString() << std::endl;
    buf.reserve(2);
    std::cout << buf.toString() << std::endl;
    buf.push(9);
    std::cout << buf.toString() << std::endl;
}

void TypeIDExample()
{
    struct A {};
    struct B {};
    class C {};
    class D {};

    std::cout << "----------- TypeIDExample ----------" << std::endl;

    std::cout << "TypeID<A>: " << TypeID::value<A>() << std::endl;
    std::cout << "TypeID<B>: " << TypeID::value<B>() << std::endl;
    std::cout << "TypeID<C>: " << TypeID::value<C>() << std::endl;
    std::cout << "TypeID<D>: " << TypeID::value<D>() << std::endl;
    std::cout << "TypeID<uint32_t>: " << TypeID::value<uint32_t>() << std::endl;

    std::cout << "TypeID<A, A>: " << TypeID::value<A, A>() << std::endl;
    std::cout << "TypeID<A, B>: " << TypeID::value<A, B>() << std::endl;
    std::cout << "TypeID<B, A>: " << TypeID::value<B, A>() << std::endl;
    std::cout << "TypeID<B, B>: " << TypeID::value<B, B>() << std::endl;
}

void SparseVectorExample()
{
    std::cout << "----------- SparseVectorExample -----------" << std::endl;

    SparseVector<int> vec;

    vec.insert(1337);
    vec.insert(42);

    std::cout << vec.toString() << std::endl;

    vec.erase(0);

    std::cout << vec.toString() << std::endl;

    vec.insert(7331);
    vec.insert(24);

    std::cout << vec.toString() << std::endl;

    for (auto i : vec)
    {
        std::cout << "  " << i << std::endl;
    }

    vec.clear();

    vec.insert(1);
    vec.insert(2);
    vec.insert(3);
    vec.insert(4);

    std::cout << vec.toString() << std::endl;
}

void DataLayoutExample()
{
    std::cout << "----------- DataLayoutExample -----------" << std::endl;

    DataLayout layout();

}

void LayoutedBlobExample()
{
    std::cout << "----------- LayoutedBlobExample -----------" << std::endl;

    std::cout << Type_I32.name() << ": " << Type_I32.typeID() << std::endl;
    std::cout << Type_Vec3.name() << ": " << Type_Vec3.typeID() << std::endl;

    DataLayout layout({
                          {"Age", Type_I32},
                          {"Direction", Type_Vec3}
                      });

    LayoutedBlob blob = LayoutedBlob(layout, 3);

    auto age = layout.field("Age");
    auto direction = layout.field("Direction");

    auto ages = blob.field<i32>(age);
    auto directions = blob.field<glm::vec3>(direction);

    directions[0] = glm::vec3(1, 2, 3);
    directions[1] = glm::vec3(4, 5, 6);
    directions[2] = glm::vec3(7, 8, 9);

    ages[0] = 12;
    ages[1] = 42;
    ages[2] = 89;

    for (int i = 0; i < blob.count(); i++)
    {
        std::cout << "  " << i << ": " << ages[i] << " " << directions[i] << std::endl;
    }
}

int main(int argc, char * argv[])
{
    LinearMapExample();
    LinearOctreeExample();
    RingBufferExample();
    TypeIDExample();
    SparseVectorExample();
    DataLayoutExample();
    LayoutedBlobExample();

    return 0;
}

