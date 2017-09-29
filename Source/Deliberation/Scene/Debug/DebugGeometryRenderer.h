#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Core/DataLayout.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class DebugGeometryNode;

class DebugGeometryRenderer : public SingleNodeRenderer
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
    DebugGeometryRenderer();

    const BuildIns & buildIns() const { return m_buildIns; }

    std::shared_ptr<DebugGeometryNode> addNode();
    void removeNode(const std::shared_ptr<DebugGeometryNode> & node);

    void render() override;

private:
    BuildIns                                        m_buildIns;
    std::vector<std::shared_ptr<DebugGeometryNode>> m_nodes;
};
}