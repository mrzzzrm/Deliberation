#pragma once

#include <Deliberation/Scene/Pipeline/Renderer.h>

namespace deliberation
{

class MeshData;
class RenderManager;

class ModelRenderer:
    public Renderer
{
public:
    ModelRenderer(RenderManager & renderManager);
    virtual ~ModelRenderer() = default;

    void addModel(const MeshData & meshData);

private:

};

}