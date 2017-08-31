#include <Deliberation/Core/Math/PolygonClipping.h>

#include <iostream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/PrimitiveIntersection.h>
#include <Deliberation/Core/Math/Ray2D.h>
#include <Deliberation/Core/StreamUtils.h>

namespace deliberation
{
void PolygonClipping2D(
    const Span<glm::vec2> &  subject,
    const Span<glm::vec2> &  clipping,
    std::vector<glm::vec2> & result)
{
    Assert(subject.size() > 2);
    Assert(clipping.size() > 2);

    std::vector<glm::vec2> input;
    std::vector<glm::vec2> output;

    /**
     * Determine winding order of clipping polygon
     */
    bool clippingCCW = false;
    {
        auto edge0 = glm::vec2(
            clipping[1].x - clipping[0].x, clipping[1].y - clipping[0].y);
        auto edge1 = glm::vec2(
            clipping[2].x - clipping[1].x, clipping[2].y - clipping[1].y);
        auto sign = (edge0.x * edge1.y) - (edge0.y * edge1.x);
        clippingCCW = sign >= 0;
    }

    //    std::cout << "Clipping: ";
    //    for (auto & v : clipping.toVector()) {
    //        std::cout << v << "; ";
    //    }
    //    std::cout << std::endl;

    output = subject.toVector();

    for (int c0 = 0; c0 < clipping.size(); c0++)
    {
        auto c1 = (c0 + 1) % clipping.size();

        auto & C0 = clipping[c0];
        auto & C1 = clipping[c1];

        Ray2D edge =
            clippingCCW ? Ray2D::fromTo(C0, C1) : Ray2D::fromTo(C1, C0);
        //        std::cout << edge << " " << clippingCCW << ": ";
        //        for (auto & v : output) {
        //            std::cout << v << " ";
        //        }
        //        std::cout << " -> ";

        input = output;
        output.clear();

        auto S = input.back();

        for (int i = 0; i < input.size(); i++)
        {
            auto & E = input[i];

            if (PointRay2DHalfspace(E, edge) <= 0)
            {
                if (PointRay2DHalfspace(S, edge) > 0)
                {
                    bool exists;
                    output.push_back(Ray2DIntersectionPoint(
                        Ray2D::fromTo(S, E), edge, exists));
                    Assert(exists);
                }
                output.push_back(E);
            }
            else if (PointRay2DHalfspace(S, edge) <= 0)
            {
                bool exists;
                output.push_back(
                    Ray2DIntersectionPoint(Ray2D::fromTo(S, E), edge, exists));
                Assert(exists);
            }

            S = E;
        }

        //        for (auto & v : output) {
        //            std::cout << v << " ";
        //        }
        //        std::cout << std::endl;
    }

    /**
     * Populate result
     */
    result = output;
}
}
