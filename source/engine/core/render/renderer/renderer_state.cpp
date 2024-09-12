#include "renderer_state.hpp"

namespace Airwave
{

    std::vector<std::string> RenderStateManager::GetRenderStateStrings(RenderState state)
    {
        std::vector<std::string> result;
        if ((state & RenderState::DepthTest) == RenderState::DepthTest)
            result.push_back("DepthTest");
        if ((state & RenderState::DepthWrite) == RenderState::DepthWrite)

            result.push_back("DepthWrite");
        if ((state & RenderState::Blend) == RenderState::Blend)
            result.push_back("Blend");
        if ((state & RenderState::CullFace) == RenderState::CullFace)
            result.push_back("CullFace");
        if ((state & RenderState::ScissorTest) == RenderState::ScissorTest)
            result.push_back("ScissorTest");
        if ((state & RenderState::StencilTest) == RenderState::StencilTest)
            result.push_back("StencilTest");
        if ((state & RenderState::Multisample) == RenderState::Multisample)
            result.push_back("Multisample");
        if ((state & RenderState::LineSmooth) == RenderState::LineSmooth)
            result.push_back("LineSmooth");

        return result;
    }

    std::vector<std::string> RenderStateManager::GetClearFlagStrings(RenderClearFlag flag)
    {
        std::vector<std::string> result;
        if ((flag & RenderClearFlag::Color) == RenderClearFlag::Color)
            result.push_back("Color");
        if ((flag & RenderClearFlag::Depth) == RenderClearFlag::Depth)
            result.push_back("Depth");
        if ((flag & RenderClearFlag::Stencil) == RenderClearFlag::Stencil)
            result.push_back("Stencil");

        return result;
    }




} // namespace Airwave
