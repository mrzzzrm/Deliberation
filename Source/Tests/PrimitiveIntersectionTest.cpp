#include <Deliberation/Core/Math/PrimitiveIntersection.h>

#include "gtest/gtest.h"

using namespace deliberation;

class PrimitiveIntersectionTest : public ::testing::Test
{
};

TEST_F(PrimitiveIntersectionTest, LineSphereIntersection)
{
    {
        auto a = glm::vec3(-1.0f, -1.0f, 0.0f);
        auto b = glm::vec3(1.0f, 2.0f, 0.0f) - a;
        auto c = glm::vec3(-1.0f, 2.0f, 2.0f) - a;

        ASSERT_FALSE(LineSphereIntersection(a, b, {2.0f, 0.0f, 0.0f}, 1.0f));
        ASSERT_TRUE(LineSphereIntersection(a, b, {0.0f, 0.0f, 0.0f}, 1.0f));
        ASSERT_FALSE(LineSphereIntersection(a, c, {0.0f, 0.0f, 0.0f}, 1.0f));
        ASSERT_TRUE(LineSphereIntersection(a, b, {0.0f, 0.0f, 0.0f}, 3.0f));
        ASSERT_TRUE(LineSphereIntersection(a, b, {2.0f, 3.0f, 0.0f}, 2.0f));
        ASSERT_FALSE(LineSphereIntersection(a, b, {2.0f, 3.0f, 0.0f}, 1.3f));
        ASSERT_TRUE(LineSphereIntersection(a, b, a, 10.0f));
    }
}
