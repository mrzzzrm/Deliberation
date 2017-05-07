#pragma once

#include <string>

#include <glbinding/gl/enum.h>

#include <Deliberation/Draw/Enum.h>

namespace deliberation
{
// TODO: Separate blend factors
// TODO: Constant Color/Alpha
// TODO: Blend state for individual draw buffers

class BlendState final
{
  public:
    static BlendState disabled();

  public:
    BlendState();
    BlendState(
        BlendEquation equation, BlendFactor sfactor, BlendFactor dfactor);

    bool          enabled() const;
    BlendEquation equation() const;
    BlendFactor   sfactor() const;
    BlendFactor   dfactor() const;

    std::string toString() const;

  private:
    bool          m_enabled;
    BlendEquation m_equation;
    BlendFactor   m_sfactor;
    BlendFactor   m_dfactor;
};
}
