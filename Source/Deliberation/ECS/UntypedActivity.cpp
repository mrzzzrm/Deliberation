#include <Deliberation/ECS/UntypedActivity.h>

#include <Deliberation/Platform/App.h>

namespace deliberation
{

UntypedActivity::~UntypedActivity()
{
    deactiveInputLayer();
}

void UntypedActivity::activateInputLayer(i32 inputPriority)
{
    m_inputLayerWrapper = std::make_shared<InputLayerWrapper>(*this, inputPriority);
    App::get().inputManager().addLayer(m_inputLayerWrapper);
}

void UntypedActivity::deactiveInputLayer()
{
    if (!m_inputLayerWrapper) return;

    App::get().inputManager().removeLayer(m_inputLayerWrapper);
}

}
