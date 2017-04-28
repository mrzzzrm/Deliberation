#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <Deliberation/Draw/Program.h>

#include <Deliberation/Scene/Pipeline/Renderer.h>

namespace deliberation
{

class MeshData;
class Model;
class ModelInstance;

class ModelRenderer:
    public Renderer
{
public:
    ModelRenderer(RenderManager & renderManager);

    std::shared_ptr<Model> addModel(const std::string & path);
    std::shared_ptr<Model> addModel(const MeshData & meshData);

    std::shared_ptr<ModelInstance> addModelInstance(const std::shared_ptr<Model> & model);

    void registerRenderNodes() override;

private:
    friend class ModelRendererMainNode;

private:
    std::unordered_map<std::shared_ptr<Model>,
        std::vector<std::shared_ptr<ModelInstance>>> m_instancesByModel;
    Program m_gbufferProgram;
};

}