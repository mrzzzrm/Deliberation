#pragma once



namespace deliberation
{

struct BasicVertex
{
    glm::vec3 position;
};

struct ColoredVertex
{
    glm::vec3 position;
    glm::vec3 color;
};

struct OrientedVertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

}