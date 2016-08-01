#include <Deliberation/Scene/MeshCompiler.h>

#include <iostream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Scene/Mesh.h>

#define VERBOSE 1

namespace deliberation
{

MeshCompiler::Compilation::Compilation(const DataLayout & vertexLayout, size_t numVertices):
    vertices(vertexLayout, numVertices),
    indices({"Index", Type_U32}, numVertices)
{

}

MeshCompiler::MeshCompiler()
{

}

MeshCompiler::Compilation MeshCompiler::compile(const Mesh & mesh, PrimitiveType primitive) const
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

MeshCompiler::Compilation MeshCompiler::compileTriangles(const Mesh & mesh) const
{
    size_t numVertices = 0;

    /**
     * Determine number of vertices
     */
    for (std::size_t f = 0u; f < mesh.faces().size(); f++)
    {
        auto & face = mesh.faces()[f];
        numVertices += 3 * (face.indices.size() - 2);
    }

    /**
     * Compose vertex layout
     */
    auto vertexFields = mesh.vertices().layout().descs();
    auto faceFields = mesh.faceAttributes().layout().descs();

    auto combinedFields = vertexFields;
    combinedFields.insert(combinedFields.end(), faceFields.begin(), faceFields.end());

    DataLayout vertexLayout(combinedFields);

    Compilation result(vertexLayout, numVertices);

    auto indices = result.indices.field<u32>("Index");

    size_t c = 0;

    for (std::size_t f = 0u; f < mesh.faces().size(); f++)
    {
        auto & face = mesh.faces()[f];
        auto faceAttributes = mesh.faceAttributes()[f];

        for (std::size_t v = 1; v  < face.indices.size() - 1; v++)
        {
            for (auto fi = 0; fi < vertexFields.size(); fi++) {
                auto srcField = mesh.vertices().layout().field(fi);
                auto dstField = vertexLayout.field(fi);

                result.vertices[c + 0].value(dstField) = mesh.faceVertex(f, 0).value(srcField);
                result.vertices[c + 1].value(dstField) = mesh.faceVertex(f, v).value(srcField);
                result.vertices[c + 2].value(dstField) = mesh.faceVertex(f, v + 1).value(srcField);
            }

            for (auto fi = 0; fi < mesh.faceAttributes().layout().fields().size(); fi++) {
                auto srcField = mesh.faceAttributes().layout().field(fi);
                auto dstField = vertexLayout.field(fi + vertexFields.size());

                result.vertices[c + 0].value(dstField) = faceAttributes.value(srcField);
                result.vertices[c + 1].value(dstField) = faceAttributes.value(srcField);
                result.vertices[c + 2].value(dstField) = faceAttributes.value(srcField);
            }

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

MeshCompiler::Compilation MeshCompiler::compileLines(const Mesh & mesh) const
{
    Compilation result(mesh.vertices().layout(), 0);
    return result;
}

}
