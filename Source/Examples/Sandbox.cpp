#include <iostream>

#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Core/Math/Morton.h>
#include <Deliberation/Core/Math/PolygonClipping.h>
#include <Deliberation/Core/Math/PrimitiveIntersection.h>
#include <Deliberation/Core/Math/Ray2D.h>

using namespace deliberation;


int main(int argc, char * argv[])
{
    glm::vec3 axisAngle(1.0f, 1.0f, 1.0f);

    glm::quat orientation0;
    glm::quat orientation1;

    orientation0 = orientation0 * glm::quat(axisAngle);


    return 0;
}

