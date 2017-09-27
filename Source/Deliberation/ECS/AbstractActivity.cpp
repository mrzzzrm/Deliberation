#include <Deliberation/ECS/AbstractActivity.h>

#include <Deliberation/Platform/App.h>

namespace deliberation
{

AbstractActivity::~AbstractActivity()
{
    deactiveInputLayer();
}

void AbstractActivity::activateInputLayer(i32 inputPriority)
{
    m_inputLayerWrapper = std::make_shared<InputLayerWrapper>(*this, inputPriority);
    App::get().inputManager().addLayer(m_inputLayerWrapper);
}

void AbstractActivity::deactiveInputLayer()
{
    if (!m_inputLayerWrapper) return;

    App::get().inputManager().removeLayer(m_inputLayerWrapper);
}

}
