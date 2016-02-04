#include <iostream>
#include <chrono>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/LinearMap.h>

void LinearMapExample()
{
    std::cout << "----------- LinearMapExample ----------" << std::endl;

    deliberation::LinearMap<float> map;
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

int main(int argc, char * argv[])
{
    LinearMapExample();
    return 0;
}

