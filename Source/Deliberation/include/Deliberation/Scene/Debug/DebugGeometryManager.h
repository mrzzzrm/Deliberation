#pragma once

#include <vector>

#include <Deliberation/Core/DataLayout.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class DELIBERATION_API DebugGeometryManager final
{
public:
    struct BuildIns
    {
        /**
         * Programs
         */
        Program     shadedProgram;
        Program     unicolorProgram;
        Program     vertexColorProgram;

        /**
         * Layouts
         */
        DataLayout  shadedDataLayout;
        DataLayout  unicolorDataLayout;
        DataLayout  vertexColorDataLayout;

        /**
         * Build in meshes
         */
        Buffer      boxTrianglesVertexBuffer;
        Buffer      boxTrianglesIndexBuffer;

        Buffer      boxLinesVertexBuffer;
        Buffer      boxLinesIndexBuffer;

        Buffer      coneVertexBuffer;
        Buffer      coneIndexBuffer;

        Buffer      sphereVertexBuffer;
        Buffer      sphereIndexBuffer;

        Buffer      pointVertexBuffer;
    };
    
public:
    DebugGeometryManager(Context & context);

    Context & context() const;
    const BuildIns & buildIns() const;

private:
    Context &   m_context;
    BuildIns    m_buildIns;
};

}