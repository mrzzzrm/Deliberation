#include <Deliberation/Scene/MeshCompiler2.h>

#include <iostream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Scene/Mesh2.h>

#define VERBOSE 1

namespace deliberation
{

MeshCompiler2::Compilation::Compilation(const DataLayout & vertexLayout, size_t numVertices):
    vertices(vertexLayout, numVertices),
    indices({"Index", Type_U32}, numVertices)
{

}

MeshCompiler2::MeshCompiler2()
{

}

MeshCompiler2::Compilation MeshCompiler2::compile(const Mesh2 & mesh, PrimitiveType primitive) const
{
    if (primitive == Primitive_Triangles)
    {
        return compileTriangles(mesh);
    }
    else if (primitive == Primitive_Lines)
    {
        return compileLines(mesh);
    }
    else
    {
        Fail("Unsupported PrimitiveType");
    }
}

MeshCompiler2::Compilation MeshCompiler2::compileTriangles(const Mesh2 & mesh) const
{
    size_t numVertices = 0;

    for (std::size_t f = 0u; f < mesh.faces().size(); f++)
    {
        auto & face = mesh.faces()[f];

        numVertices += 3 * (face.indices.size() - 2);
    }

    Compilation result(mesh.vertices().layout(), numVertices);

    auto indices = result.indices.field<u32>("Index");

    size_t c = 0;

    for (std::size_t f = 0u; f < mesh.faces().size(); f++)
    {
        auto & face = mesh.faces()[f];

        for (std::size_t v = 1; v  < face.indices.size() - 1; v++)
        {
            result.vertices[c + 0] = mesh.faceVertex(f, 0);
            result.vertices[c + 1] = mesh.faceVertex(f, v);
            result.vertices[c + 2] = mesh.faceVertex(f, v + 1);

            indices[c + 0] = c + 0;
            indices[c + 1] = c + 1;
            indices[c + 2] = c + 2;

            c += 3;
        }
    }


#if VERBOSE
    std::cout << "Mesh Compilation:" << std::endl;
    std::cout << "  Vertices: " << result.vertices.count() << std::endl;
    std::cout << "  Indices: " << result.indices.count() << std::endl;
#endif

    return result;
}

MeshCompiler2::Compilation MeshCompiler2::compileLines(const Mesh2 & mesh) const
{
    Compilation result(mesh.vertices().layout(), 0);
    return result;
}

}
