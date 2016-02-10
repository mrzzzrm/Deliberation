#include <iostream>

#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Math/Morton.h>

using namespace deliberation;

void Morton2DExample()
{
    std::cout << "----------- Morton2DExample ----------" << std::endl;
    for (auto key = 0; key < 16; key++)
    {
        std::cout << key << ": " << Morton2D(key).toVec() << std::endl;
    }
}

void Morton3DExample()
{
    std::cout << "----------- Morton3DExample ----------" << std::endl;
    Morton3D m0({0, 0, 0});
    Morton3D m1({1, 0, 0});
    Morton3D m2({0, 1, 0});
    Morton3D m3({0, 1, 1});
    Morton3D m4({2, 0, 0});
    Morton3D m5({3, 3, 3});

    std::cout << m0.toString() << std::endl;
    std::cout << m1.toString() << std::endl;
    std::cout << m2.toString() << std::endl;
    std::cout << m3.toString() << std::endl;
    std::cout << m4.toString() << std::endl;
    std::cout << m5.toString() << std::endl;

    std::cout << m0.toIndex() << std::endl;

    std::cout << m3.toVec() << std::endl;
}

int main(int argc, char * argv[])
{
    Morton2DExample();
    Morton3DExample();
    return 0;
}

