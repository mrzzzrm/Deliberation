#include <Deliberation/Scene/MeshCompiler.h>

#include <iostream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Scene/Mesh.h>

#define VERBOSE 0

namespace deliberation
{

MeshCompiler::Compilation::Compilation(const DataLayout & vertexLayout, size_t numVertices, size_t numIndices):
    vertices(vertexLayout, numVertices),
    indices({"Index", Type_U32}, numIndices)
{

}

MeshCompiler::MeshCompiler()
{

}

MeshCompiler::Compilation MeshCompiler::compile(const Mesh & mesh, MeshCompilerPrimitive primitive) const
{
    if (primitive == MeshCompilerPrimitive::Triangles)
    {
        return compileTriangles(mesh);
    }
    else if (primitive == MeshCompilerPrimitive::Lines)
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

    Compilation result(vertexLayout, numVertices, numVertices);

    auto indices = result.indices.field<u32>("Index");

    size_t c = 0;

    for (std::size_t f = 0u; f < mesh.faces().size(); f++)
    {
        auto & face = mesh.faces()[f];

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
                auto faceAttributes = mesh.faceAttributes()[f];

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
    std::cout << "Mesh Compilation Triangles:" << std::endl;
    std::cout << "  Vertices: " << result.vertices.count() << std::endl;
    std::cout << result.vertices.toString() << std::endl;
    std::cout << "  Indices: " << result.indices.count() << std::endl;
    std::cout << result.indices.toString() << std::endl;
#endif

    return result;
}

MeshCompiler::Compilation MeshCompiler::compileLines(const Mesh & mesh) const
{
    /**
     * Count vertices
     */
    size_t numVertices = 0;
    size_t numIndices = 0;
    for (std::size_t f = 0u; f < mesh.faces().size(); f++)
    {
        numVertices += mesh.faces()[f].indices.size();
        numIndices += 2 * mesh.faces()[f].indices.size();
    }

    /**
     * Combined Vertex Layout
     */
    auto layout = DataLayout::concatenate({mesh.vertices().layout(), mesh.faceAttributes().layout()});
    std::vector<std::pair<DataLayoutField, DataLayoutField>> fromVertexFields;
    for (auto & field : mesh.vertices().layout().fields())
    {
        fromVertexFields.push_back({layout.field(field.name()), field});
    }
    std::vector<std::pair<DataLayoutField, DataLayoutField>> fromFaceFields;
    for (auto & field : mesh.faceAttributes().layout().fields())
    {
        fromFaceFields.push_back({layout.field(field.name()), field});
    }

    /**
     * Result
     */
    Compilation result(layout, numVertices, numIndices);

    auto indices = result.indices.field<u32>("Index");

    /**
     * Compile mesh
     */
    auto vIndex = 0u;
    auto iIndex = 0u;
    for (size_t f = 0u; f < mesh.faces().size(); f++)
    {
        auto & face = mesh.faces()[f];

        auto baseIndex = vIndex;

        /**
         * Generate face outline vertices
         */
        for (size_t v = 0; v < face.indices.size(); v++)
        {
            auto b = v < face.indices.size() - 1 ? v + 1 : 0;

            for (auto &field : fromVertexFields) {
                result.vertices[vIndex].value(field.first) = mesh.faceVertex(f, v).value(field.second);
            }
            for (auto &field : fromFaceFields) {
                result.vertices[vIndex].value(field.first) = mesh.faceVertex(f, v).value(field.second);
            }

            vIndex++;
        }

        /**
         * Generate face outline indices
         */
        for (size_t v = 0; v < face.indices.size(); v++)
        {
            indices[iIndex + 0] = baseIndex + v;
            indices[iIndex + 1] = baseIndex + (v + 1 < face.indices.size() ? v + 1 : 0);

            iIndex += 2;
        }
    }

#if VERBOSE
    std::cout << "Mesh Compilation Lines:" << std::endl;
    std::cout << "  Vertices: " << result.vertices.count() << std::endl;
    std::cout << result.vertices.toString() << std::endl;
    std::cout << "  Indices: " << result.indices.count() << std::endl;
    std::cout << result.indices.toString() << std::endl;
#endif

    return result;
}

}
