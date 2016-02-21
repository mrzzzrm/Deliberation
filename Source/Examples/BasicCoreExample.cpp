#include <iostream>
#include <chrono>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/LinearOctree.h>

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

int main(int argc, char * argv[])
{
    LinearMapExample();
    LinearOctreeExample();
    return 0;
}

