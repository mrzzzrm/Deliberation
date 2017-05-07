#pragma once

#include <Deliberation/Core/LayoutedBlob.h>

namespace deliberation
{
class Mesh;

enum class MeshCompilerPrimitive
{
    Triangles,
    Lines
};

class MeshCompiler final
{
  public:
    struct Compilation
    {
        Compilation(
            const DataLayout & vertexLayout,
            size_t             numVertices,
            size_t             numIndices);

        LayoutedBlob vertices;
        LayoutedBlob indices;
    };

  public:
    MeshCompiler();

    Compilation compile(
        const Mesh &          mesh,
        MeshCompilerPrimitive primitive =
            MeshCompilerPrimitive::Triangles) const;

  private:
    Compilation compileTriangles(const Mesh & mesh) const;
    Compilation compileLines(const Mesh & mesh) const;
};
}