#include <Deliberation/ECS/UntypedActivity.h>

#include <Deliberation/Platform/Application.h>

namespace deliberation
{

UntypedActivity::~UntypedActivity()
{
    deactiveInputLayer();
}

void UntypedActivity::activateInputLayer(i32 inputPriority)
{
    m_inputLayerWrapper = std::make_shared<InputLayerWrapper>(*this, inputPriority);
    Application::get().inputManager().addLayer(m_inputLayerWrapper);
}

void UntypedActivity::deactiveInputLayer()
{
    if (!m_inputLayerWrapper) return;

    Application::get().inputManager().removeLayer(m_inputLayerWrapper);
}

}
