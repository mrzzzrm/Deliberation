namespace deliberation
{
template<typename T, typename... Args>
std::shared_ptr<T> RenderManager::addRenderer(Args &&... args)
{
    auto renderer = std::make_shared<T>(*this, std::forward<Args>(args)...);
    m_renderers.emplace_back(renderer);
    m_pipelineBuild = false;
    return renderer;
}
}