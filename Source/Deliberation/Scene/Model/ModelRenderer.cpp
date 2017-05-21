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

namespace deliberation
{
struct ModelInstancesDraw
{
    Draw   draw;
    Buffer instanceBuffer;
};

class ModelRendererMainNode : public RenderNode
{
  public:
    ModelRendererMainNode(
        RenderManager & renderManager, ModelRenderer & modelRenderer)
        : RenderNode(renderManager), m_modelRenderer(modelRenderer)
    {
        m_instanceLayout = DataLayout("Transform", Type_Mat4);
        m_instanceDataStaging = LayoutedBlob(m_instanceLayout);

        auto & viewUniformBlockLayout =
            m_modelRenderer.m_gbufferProgram.interface()
                .uniformBlock("ViewBlock")
                ->layout();
        m_viewUniformBuffer =
            renderManager.drawContext().createBuffer(viewUniformBlockLayout);
        m_viewUniformData = LayoutedBlob(viewUniformBlockLayout, 1);

        m_viewField = m_viewUniformData.field<glm::mat4>("View");
        m_projectionField = m_viewUniformData.field<glm::mat4>("Projection");
        m_viewProjectionField =
            m_viewUniformData.field<glm::mat4>("ViewProjection");
    }

    void render() override
    {
        const auto & camera = m_renderManager.mainCamera();

        // Update view data
        m_viewField[0] = camera.view();
        m_projectionField[0] = camera.projection();
        m_viewProjectionField[0] = camera.viewProjection();
        m_viewUniformBuffer.upload(m_viewUniformData);

        // Render model instances
        for (auto & pair : m_modelRenderer.m_instancesByModel)
        {
            const auto & model = pair.first;
            auto         iter = m_drawPerModel.find(model);
            if (iter == m_drawPerModel.end())
            {
                auto & drawContext = m_renderManager.drawContext();

                ModelInstancesDraw modelRenderContext;

                modelRenderContext.instanceBuffer =
                    m_renderManager.drawContext().createBuffer(
                        m_instanceLayout);

                auto & draw = modelRenderContext.draw;
                draw = drawContext.createDraw(m_modelRenderer.m_gbufferProgram);
                draw.setUniformBuffer("ViewBlock", m_viewUniformBuffer);
                draw.addInstanceBuffer(modelRenderContext.instanceBuffer);
                draw.addVertexBuffer(model->vertexBuffer);
                if (model->hasIndices) draw.setIndexBuffer(model->indexBuffer);
                draw.setFramebuffer(m_renderManager.gbuffer());

                const auto & vertexLayout = model->vertexBuffer.layout();
                if (!vertexLayout.hasField("Color"))
                    draw.setAttribute("Color", glm::vec3(1.0f, 0.0f, 0.5f));
                if (!vertexLayout.hasField("Normal"))
                    draw.setAttribute("Normal", glm::vec3(0.0f, 1.0f, 0.0f));

                auto result = m_drawPerModel.emplace(model, modelRenderContext);
                iter = result.first;
            }

            const auto & instances = pair.second;

            m_instanceDataStaging.resize(instances.size());
            auto transforms =
                m_instanceDataStaging.iterator<glm::mat4>("Transform");
            for (const auto & instance : pair.second)
            {
                transforms.put(instance->transform.matrix());
            }

            auto & instancesDraw = iter->second;
            instancesDraw.instanceBuffer.upload(m_instanceDataStaging);
            instancesDraw.draw.render();
        }
    }

  private:
    ModelRenderer & m_modelRenderer;
    DataLayout      m_instanceLayout;
    std::unordered_map<std::shared_ptr<Model>, ModelInstancesDraw>
                 m_drawPerModel;
    LayoutedBlob m_instanceDataStaging;
    Buffer       m_viewUniformBuffer;

    LayoutedBlob                      m_viewUniformData;
    TypedBlobValueAccessor<glm::mat4> m_viewField;
    TypedBlobValueAccessor<glm::mat4> m_projectionField;
    TypedBlobValueAccessor<glm::mat4> m_viewProjectionField;
};
}

namespace deliberation
{
ModelRenderer::ModelRenderer(RenderManager & renderManager)
    : Renderer(renderManager)
{
    m_gbufferProgram = renderManager.drawContext().createProgram(
        {DeliberationDataPath("Data/Shaders/ModelRenderer.vert"),
         DeliberationDataPath("Data/Shaders/ModelRenderer_GBuffer.frag")});
}

std::shared_ptr<Model> ModelRenderer::addModel(const std::string & path)
{
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::string error;
    bool        success =
        tinyobj::LoadObj(&attrib, &shapes, &materials, &error, path.c_str());

    if (!error.empty())
    {
        std::cout << "ModelRenderer: Error loading '" << path << "': " << error
                  << std::endl;
        if (!success) return {};
    }

    for (const auto & shape : shapes)
    {
        size_t shapeNumVertices = 0;
        for (const auto & numVertices : shape.mesh.num_face_vertices)
        {
            Assert(numVertices == 3, "ModelRenderer: Model not triangulated");
            shapeNumVertices += numVertices;
        }

        DataLayout vertexDataLayout("Position", Type_Vec3);

        auto hasNormals = false;
        if (!attrib.normals.empty())
        {
            vertexDataLayout.addField({"Normal", Type_Vec3});
            hasNormals = true;
        }

        LayoutedBlob vertices(vertexDataLayout, shapeNumVertices);

        auto positions = vertices.iterator<glm::vec3>("Position");

        TypedBlobIterator<glm::vec3> normals;
        if (hasNormals) normals = vertices.iterator<glm::vec3>("Normal");

        size_t indexOffset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            for (auto v = 0; v < 3; v++)
            {
                tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

                const auto vx = attrib.vertices[3 * idx.vertex_index + 0];
                const auto vy = attrib.vertices[3 * idx.vertex_index + 1];
                const auto vz = attrib.vertices[3 * idx.vertex_index + 2];

                positions.put({vx, vy, vz});

                if (hasNormals)
                {
                    const auto nx = attrib.normals[3 * idx.normal_index + 0];
                    const auto ny = attrib.normals[3 * idx.normal_index + 1];
                    const auto nz = attrib.normals[3 * idx.normal_index + 2];

                    normals.put({nx, ny, nz});
                }
            }

            indexOffset += 3;
        }

        return addModel(MeshData(std::move(vertices), {}));
    }
}

std::shared_ptr<Model> ModelRenderer::addModel(const MeshData & meshData)
{
    auto model = std::make_shared<Model>();
    model->vertexBuffer =
        m_renderManager.drawContext().createBuffer(meshData.vertices());

    if (meshData.indices().empty())
    {
        model->hasIndices = false;
    }
    else
    {
        model->indexBuffer =
            m_renderManager.drawContext().createBuffer(meshData.indices());
        model->hasIndices = true;
    }

    // Bounds
    auto positions = meshData.vertices().citerator<glm::vec3>("Position");

    auto position = positions.get();
    model->bounds = AABB(position, position);
    for (size_t i = 1; i < meshData.vertices().count(); i++)
    {
        auto position = positions.get();
        model->bounds.enlargedToContain(position);
    }

    return model;
}

std::shared_ptr<ModelInstance>
ModelRenderer::addModelInstance(const std::shared_ptr<Model> & model)
{
    auto instance = std::make_shared<ModelInstance>();
    m_instancesByModel[model].emplace_back(instance);
    return instance;
}

void ModelRenderer::registerRenderNodes()
{
    m_renderManager.registerRenderNode(
        std::make_shared<ModelRendererMainNode>(m_renderManager, *this),
        RenderPhase::GBuffer);
}
}