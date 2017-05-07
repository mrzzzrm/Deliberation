namespace deliberation
{
template<typename T>
TextureLoaderUnicolorImpl<T>::TextureLoaderUnicolorImpl(
    const glm::uvec2 & size, const T & value)
    : m_size(size), m_value(value)
{
}

template<typename T>
TextureBinary TextureLoaderUnicolorImpl<T>::load()
{
    auto data = createPixelData();
    auto format = PixelFormat::resolve<T>();

    return TextureBinary(
        SurfaceBinary(std::move(data), m_size.x, m_size.y, format));
}

template<typename T>
Blob TextureLoaderUnicolorImpl<T>::createPixelData() const
{
    const auto numPixels = m_size.x * m_size.y;

    std::vector<T> pixels(numPixels);

    for (size_t y = 0; y < m_size.y; y++)
    {
        for (size_t x = 0; x < m_size.x; x++)
        {
            pixels[y * m_size.x + x] = m_value;
        }
    }

    return Blob(pixels);
}
}