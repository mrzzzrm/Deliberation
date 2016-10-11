#include <iostream>

#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Core/Math/Morton.h>
#include <Deliberation/Core/Math/PolygonClipping.h>
#include <Deliberation/Core/Math/PrimitiveIntersection.h>
#include <Deliberation/Core/Math/Ray2D.h>

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

void PointRay2DHalfspaceExample()
{
    std::cout << "----------- PointRay2DHalfspaceExample -----------" << std::endl;

    Ray2D r1({0.0f, 0.0f}, {5.0f, 2.0f});
    Ray2D r2({-3.0f, 1.0f}, {5.0f, 2.0f});

    std::cout << "  " << r1 << std::endl;
    std::cout << "   " << PointRay2DHalfspace({0.0f, 3.0f}, r1) << std::endl;
    std::cout << "   " << PointRay2DHalfspace({3.0f, 0.0f}, r1) << std::endl;
    std::cout << "   " << PointRay2DHalfspace({0.0f, -3.0f}, r1) << std::endl;
    std::cout << "   " << PointRay2DHalfspace({-3.0f, 0.0f}, r1) << std::endl;

    std::cout << "  " << r2 << std::endl;
    std::cout << "   " << PointRay2DHalfspace({0.0f, 3.0f}, r2) << std::endl;
    std::cout << "   " << PointRay2DHalfspace({3.0f, 0.0f}, r2) << std::endl;
    std::cout << "   " << PointRay2DHalfspace({0.0f, -3.0f}, r2) << std::endl;
    std::cout << "   " << PointRay2DHalfspace({-3.0f, 0.0f}, r2) << std::endl;

}

void PolygonClippingExample()
{
    std::cout << "----------- PolygonClippingExample -----------" << std::endl;

    std::vector<glm::vec2> result;

    glm::vec2 subject[] =
    {
        {0.0f, 0.0f},
        {1.0f, 5.0f},
        {5.0f, 5.0f},
        {6.0f, 0.0f},
    };
    glm::vec2 clip[] =
    {
        {3.0f, -1.0f},
        {3.0f, 6.0f},
        {1.0f, 9.0f},
        {-3.0f, 4.0f},
    };

    PolygonClipping2D(
        subject,
        clip,
        result
    );

    std::cout << "  Clipped: ";
    for (auto & v : result)
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;

}

int main(int argc, char * argv[])
{
//    Morton2DExample();
//    Morton3DExample();
//    PointRay2DHalfspaceExample();
//    PolygonClippingExample();

    return 0;
}

