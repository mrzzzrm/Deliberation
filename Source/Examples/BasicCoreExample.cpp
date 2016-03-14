#include <iostream>
#include <chrono>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/LinearOctree.h>
#include <Deliberation/Core/RingBuffer.h>
#include <Deliberation/Core/TypeID.h>

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

int main(int argc, char * argv[])
{
    LinearMapExample();
    LinearOctreeExample();
    RingBufferExample();
    TypeIDExample();

    return 0;
}

