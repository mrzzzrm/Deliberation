#include <Deliberation/Scene/Model/ModelRenderer.h>

#include <tiny_obj_loader.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/MeshData.h>
#include <Deliberation/Scene/Model/Model.h>
#include <Deliberation/Scene/Model/ModelInstance.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>

using namespace deliberation;

namespace
{

struct ModelInstancesDraw
{
    Draw    draw;
    Buffer  instanceBuffer;
};

class ModelRendererMainNode:
    public RenderNode
{
public:
    ModelRendererMainNode(RenderManager & renderManager, ModelRenderer & modelRenderer):
        RenderNode(renderManager),
        m_modelRenderer(modelRenderer)
    {
        m_instanceLayout = DataLayout("Transform", Type_Mat4);
        m_instanceDataStaging = LayoutedBlob(m_instanceLayout);

        auto & viewUniformBlockLayout = m_modelRenderer.m_gbufferProgram.interface().uniformBlock("View")->layout();
        m_viewUniformBuffer = renderManager.drawContext().createBuffer(viewUniformBlockLayout);
        m_viewUniformData = LayoutedBlob(viewUniformBlockLayout);

        m_viewField = m_viewUniformData.field<glm::mat4>("View");
        m_projectionField = m_viewUniformData.field<glm::mat4>("Projection");
        m_viewProjectionField = m_viewUniformData.field<glm::mat4>("ViewProjection");
    }

    void render() override
    {
        const auto & camera = m_renderManager.mainCamera();

        // Update view data
        m_viewField[0] = camera.view();
        m_projectionField[0] = camera.projection();
        m_viewProjectionField[0] = camera.viewProjection();
        m_viewUniformBuffer.scheduleUpload(m_viewUniformData);

        // Render model instances
        for (auto & pair : m_modelRenderer.m_instancesByModel)
        {
            const auto & model = pair.first;
            auto iter = m_drawPerModel.find(model);
            if (iter == m_drawPerModel.end())
            {
                auto & drawContext = m_renderManager.drawContext();

                ModelInstancesDraw modelRenderContext;

                modelRenderContext.instanceBuffer = m_renderManager.drawContext().createBuffer(m_instanceLayout);

                modelRenderContext.draw = drawContext.createDraw(m_modelRenderer.m_gbufferProgram);
                modelRenderContext.draw.setUniformBuffer("View", m_viewUniformBuffer);
                modelRenderContext.draw.addInstanceBuffer(modelRenderContext.instanceBuffer);
                modelRenderContext.draw.addVertexBuffer(model->vertexBuffer);
                modelRenderContext.draw.setIndexBuffer(model->indexBuffer);
                modelRenderContext.draw.setFramebuffer(m_renderManager.gbuffer());

                auto result = m_drawPerModel.emplace(model, modelRenderContext);
                iter = result.first;
            }

            const auto & instances = pair.second;

            m_instanceDataStaging.reserve(instances.size());
            auto transforms = m_instanceDataStaging.iterator("Transform");
            for (const auto & instance : pair.second)
            {
                transforms.put(instance->transform.matrix());
            }

            auto & instancesDraw = iter->second;
            instancesDraw.instanceBuffer.scheduleUpload(m_instanceDataStaging);
            instancesDraw.draw.schedule();
        }
    }

private:
    ModelRenderer &                     m_modelRenderer;
    DataLayout                          m_instanceLayout;
    std::unordered_map<std::shared_ptr<Model>, ModelInstancesDraw>
                                        m_drawPerModel;
    LayoutedBlob                        m_instanceDataStaging;
    Buffer                              m_viewUniformBuffer;

    LayoutedBlob                        m_viewUniformData;
    TypedBlobValueAccessor<glm::mat4>   m_viewField;
    TypedBlobValueAccessor<glm::mat4>   m_projectionField;
    TypedBlobValueAccessor<glm::mat4>   m_viewProjectionField;
};

}

namespace deliberation
{

ModelRenderer::ModelRenderer(RenderManager & renderManager):
    Renderer(renderManager)
{
    renderManager.registerRenderer(shared_from_this());

    m_gbufferProgram = renderManager.drawContext().createProgram({
            DeliberationDataPath("Data/Shaders/ModelRenderer.vert"),
            DeliberationDataPath("Data/Shaders/ModelRenderer_GBuffer.frag")
    });
}

std::shared_ptr<Model> ModelRenderer::addModel(const std::string & path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string error;
    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &error, path.c_str());

    if (!error.empty() || !success)
    {
        std::cout << "ModelRenderer: Error loading '" << path << "': " << error << std::endl;
        return {};
    }

    LayoutedBlob vertices({{{"Position", Type_Vec3}, {"Normal", Type_Vec3}}});
    LayoutedBlob indices({{"Index", Type_U32}});

    for (size_t s = 0; s < shapes.size(); s++)
    {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                float vx = attrib.vertices[3*idx.vertex_index+0];
                float vy = attrib.vertices[3*idx.vertex_index+1];
                float vz = attrib.vertices[3*idx.vertex_index+2];
                float nx = attrib.normals[3*idx.normal_index+0];
                float ny = attrib.normals[3*idx.normal_index+1];
                float nz = attrib.normals[3*idx.normal_index+2];
                float tx = attrib.texcoords[2*idx.texcoord_index+0];
                float ty = attrib.texcoords[2*idx.texcoord_index+1];
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }

    return MeshData(vertices, indices);
}

std::shared_ptr<Model> ModelRenderer::addModel(const MeshData & meshData)
{
    auto model = std::make_shared<Model>();
    model->vertexBuffer = m_renderManager.drawContext().createBuffer(meshData.vertices());
    model->indexBuffer = m_renderManager.drawContext().createBuffer(meshData.indices());

    return model;
}

std::shared_ptr<ModelInstance> ModelRenderer::addModelInstance(const std::shared_ptr<Model> & model)
{
    auto instance = std::make_shared<ModelInstance>();
    m_instancesByModel[model].emplace_back(instance);
    return instance;
}

void ModelRenderer::registerRenderNodes()
{
    m_renderManager.registerRenderNode(std::make_shared<ModelRendererMainNode>(m_renderManager, *this),
                                       RenderPhase::GBuffer);
}

}