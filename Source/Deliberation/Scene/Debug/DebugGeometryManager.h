#pragma once

#include <vector>

#include <Deliberation/Core/DataLayout.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class DebugGeometryManager final
{
  public:
    static constexpr float ARROW_CONE_HEIGHT = 0.2f;

  public:
    struct BuildIns
    {
        /**
         * Programs
         */
        Program shadedProgram;
        Program unicolorProgram;
        Program vertexColorProgram;

        /**
         * Layouts
         */
        DataLayout shadedDataLayout;
        DataLayout unicolorDataLayout;
        DataLayout vertexColorDataLayout;

        /**
         * Build in meshes
         */
        Buffer boxTrianglesVertexBuffer;
        Buffer boxTrianglesIndexBuffer;

        Buffer boxLinesVertexBuffer;
        Buffer boxLinesIndexBuffer;

        Buffer coneVertexBuffer;
        Buffer coneIndexBuffer;

        Buffer sphereVertexBuffer;
        Buffer sphereIndexBuffer;

        Buffer pointVertexBuffer;
    };

  public:
    DebugGeometryManager(DrawContext & drawContext);

    DrawContext &    drawContext() const;
    const BuildIns & buildIns() const;

  private:
    DrawContext & m_drawContext;
    BuildIns      m_buildIns;
};
}