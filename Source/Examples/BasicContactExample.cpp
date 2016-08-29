#include <iostream>

#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Physics/Contacts/BoxContact.h>
#include <Deliberation/Physics/BoxShape.h>
#include <Deliberation/Physics/RigidBody.h>

using namespace deliberation;

void BoxBoxSATExample()
{
    std::cout << "----------- BoxBoxExample - not rotated ----------" << std::endl;

    auto shape0 = std::make_shared<BoxShape>(glm::vec3(2, 1, 3));
    auto shape1 = std::make_shared<BoxShape>(glm::vec3(5, 3, 1));

    RigidBody body0(shape0);
    RigidBody body1(shape1);

    body0.transform().setPosition({4, 3, 0});
    body1.transform().setPosition({-5, 0, 0});

    BoxContact contact(body0, body1);
    contact.update();

    std::cout << "  Intersection: " << contact.intersect() << std::endl;
    std::cout << "  Separation: " << contact.separation() << std::endl;
    std::cout << "  Normal: " << contact.normal() << std::endl;
}

int main()
{
    BoxBoxExample();
    return 0;
}