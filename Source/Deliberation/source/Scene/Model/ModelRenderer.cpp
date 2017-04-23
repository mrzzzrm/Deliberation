#include <Deliberation/Scene/Model/ModelRenderer.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>

using namespace deliberation;

namespace
{

class ModelRendererMainNode:
    public RenderNode
{
public:
    ModelRendererMainNode(RenderManager & renderManager, ModelRenderer & modelRenderer):
        RenderNode(renderManager),
        m_modelRenderer(modelRenderer)
    {

    }

    void render() override
    {
        for (auto & pair : m_modelRenderer.m_instancesByModel)
        {
            const auto & model = pair.first;
            auto iter = m_instanceDataPerModel.find(model);
            if (iter == m_instanceDataPerModel.end())
            {
                auto result =
                    m_instanceDataPerModel.emplace(model, m_renderManager.drawContext().createBuffer(m_instanceLayout));
                iter = result.first;
            }

            auto & instanceBuffer = iter->second;

            for (const auto & instance : pair.second)
            {

            }
        }
    }

private:
    ModelRenderer &                                     m_modelRenderer;
    DataLayout                                          m_instanceLayout;
    std::unordered_map<std::shared_ptr<Model>, Buffer>  m_instanceDataPerModel;
};

}

namespace deliberation
{

ModelRenderer::ModelRenderer(RenderManager & renderManager):
    Renderer(renderManager)
{
    renderManager.registerRenderer(shared_from_this());
}

std::shared_ptr<Model> ModelRenderer::addModel(const std::string & path)
{

}

std::shared_ptr<Model> ModelRenderer::addModel(const MeshData & meshData)
{

}

std::shared_ptr<ModelInstance> ModelRenderer::addModelInstance(const std::shared_ptr<Model> & model)
{

}

void ModelRenderer::registerRenderNodes()
{
    m_renderManager.registerRenderNode(std::make_shared<ModelRendererMainNode>(m_renderManager, *this));
}

}