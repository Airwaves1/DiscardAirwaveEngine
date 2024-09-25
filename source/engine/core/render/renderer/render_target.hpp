#pragma once

#include "render/renderer/render_target.hpp"
#include "render/texture/texture_2d.hpp"

namespace Airwave
{
class RenderTarget
{
  public:
    RenderTarget(uint32_t width, uint32_t height, TextureSpecification spec, uint32_t attachmentCount = 1)
        : m_width(width), m_height(height)
    {
        m_colorAttachments.reserve(attachmentCount);
        for (uint32_t i = 0; i < attachmentCount; i++)
        {
            m_colorAttachments.push_back(Texture2D::Create(width, height, spec));
        }
        
    }

    std::shared_ptr<Texture2D> getColorAttachment(uint32_t index = 0) const
    {
        return m_colorAttachments[index];
    }

    void setSize(uint32_t width, uint32_t height)
    {
        if(m_width == width && m_height == height)
        {
            return;
        }

        m_width = width;
        m_height = height;

        for (auto &colorAttachment : m_colorAttachments)
        {
            colorAttachment->resize(width, height);
        }
        
    }


    uint32_t getWidth() const
    {
        return m_width;
    }

    uint32_t getHeight() const
    {
        return m_height;
    }

  private:
    uint32_t m_width;
    uint32_t m_height;
    std::vector<std::shared_ptr<Texture2D>> m_colorAttachments;
};

} // namespace Airwave
