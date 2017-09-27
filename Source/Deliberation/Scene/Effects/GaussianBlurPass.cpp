#include <Deliberation/Scene/Effects/GaussianBlurPass.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/MathUtils.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/Effects/GaussianBlur.h>

namespace deliberation
{
GaussianBlurPass::GaussianBlurPass(
    GaussianBlur & gaussianBlur, Framebuffer & lFb, Framebuffer & rFb)
    : m_gaussianBlur(gaussianBlur)
    , m_lFb(lFb)
    , m_lSurface(lFb.colorTargets()[0].surface)
    , m_rFb(rFb)
    , m_rSurface(rFb.colorTargets()[0].surface)
{
    m_configBuffer =
        GetGlobal<DrawContext>()->createBuffer(m_gaussianBlur.configBlockLayout(), 1);

    m_offsetsField = m_configBuffer.layout().field("Offsets");
    m_weightsField = m_configBuffer.layout().field("Weights");
    m_numSamplesField = m_configBuffer.layout().field("NumSamples");

    setStandardDeviation(1.0f);
}

u32 GaussianBlurPass::radius() const { return m_radius; }

void GaussianBlurPass::setStandardDeviation(float s)
{
    const auto THRESHOLD = 0.005f;
    const auto var2 = 2.0f * s * s;

    const auto a = sqrt(var2 * glm::pi<float>());

    const auto b = log(a * THRESHOLD);
    AssertM(b < 0.0f, "Illegal Standard Deviation");

    const auto c = -var2 * b;
    const auto d = sqrt(c);

    m_radius = (u32)ceil(d);

    AssertM(m_radius > 0, "Radius can't be zero");

    auto numInterpolatedSamples =
        1 +
        ((m_radius - 1) % 2 == 0 ? (m_radius - 1) / 2 : (m_radius - 1) / 2 + 1);

    AssertM(
        numInterpolatedSamples < m_gaussianBlur.maxNumSamples(),
        "Invalid number of samples: " + std::to_string(numInterpolatedSamples));

    auto dist = GaussianSamples(s, m_radius);

    m_configBuffer.mapped(BufferMapping::WriteOnly, [&](LayoutedBlob & data) {
        auto * offsets = &data.field<float>(m_offsetsField)[0];
        auto * weights = &data.field<float>(m_weightsField)[0];
        data.field<i32>(m_numSamplesField)[0] = numInterpolatedSamples;

        offsets[0] = 0.0f;
        weights[0] = dist[0];

        for (size_t s = 1; s < numInterpolatedSamples; s++)
        {
            const auto baseOffset = (s - 1) * 2 + 1;

            if (baseOffset >= dist.size() - 1)
            {
                offsets[s] = baseOffset;
                weights[s] = dist[offsets[s]];
            }
            else
            {
                const auto sum = dist[baseOffset] + dist[baseOffset + 1];
                offsets[s] = baseOffset + dist[baseOffset + 1] / sum;
                weights[s] = sum / 1.0f;

                Assert(weights[s] < 1.0f);
            }
        }
    });
}

void GaussianBlurPass::render()
{
    m_gaussianBlur.setConfigBuffer(m_configBuffer);

    m_gaussianBlur.setInput(m_lSurface);
    m_gaussianBlur.setFramebuffer(
        m_rFb,
        {{"Blurred", "Color"}}); // TODO: don't create FB-binding dynamically
    m_gaussianBlur.renderHBlur();

    m_gaussianBlur.setInput(m_rSurface);
    m_gaussianBlur.setFramebuffer(
        m_lFb,
        {{"Blurred", "Color"}}); // TODO: don't create FB-binding dynamically
    m_gaussianBlur.renderVBlur();
}
}
