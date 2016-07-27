#pragma once

#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Draw/Enum.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Mesh2;

class MeshCompiler2 final
{
public:
    struct Compilation
    {
        Compilation(const DataLayout & vertexLayout, size_t numVertices);

        LayoutedBlob vertices;
        LayoutedBlob indices;
    };

public:
    MeshCompiler2();

    Compilation compile(const Mesh2 & mesh, PrimitiveType primitive = Primitive_Triangles) const;

private:
    Compilation compileTriangles(const Mesh2 & mesh) const;
    Compilation compileLines(const Mesh2 & mesh) const;
};

}