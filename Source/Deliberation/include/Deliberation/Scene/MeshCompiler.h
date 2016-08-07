#pragma once

#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Draw/Enum.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Mesh;

class MeshCompiler final
{
public:
    struct Compilation
    {
        Compilation(const DataLayout & vertexLayout, size_t numVertices, size_t numIndices);

        LayoutedBlob vertices;
        LayoutedBlob indices;
    };

public:
    MeshCompiler();

    Compilation compile(const Mesh & mesh, PrimitiveType primitive = Primitive_Triangles) const;

private:
    Compilation compileTriangles(const Mesh & mesh) const;
    Compilation compileLines(const Mesh & mesh) const;
};

}