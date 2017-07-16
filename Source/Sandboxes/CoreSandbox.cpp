#include <chrono>
#include <iostream>

#include <boost/variant.hpp>

#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/Transform3D.h>

using namespace deliberation;


int main(int argc, char * argv[])
{
    Transform3D transform3D;
    transform3D.setScale(1);

    std::cout << transform3D.pointLocalToWorld({1.0f, 0.0f, 0.0f}) << std::endl;

    transform3D.setScale(2);
    std::cout << transform3D.pointLocalToWorld({1.0f, 0.0f, 0.0f}) << std::endl;

    return 0;
}
