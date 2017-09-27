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
    GetGlobal<InputManager>()->addLayer(m_inputLayerWrapper);
}

void AbstractActivity::deactiveInputLayer()
{
    if (!m_inputLayerWrapper) return;

    GetGlobal<InputManager>()->removeLayer(m_inputLayerWrapper);
}

}
