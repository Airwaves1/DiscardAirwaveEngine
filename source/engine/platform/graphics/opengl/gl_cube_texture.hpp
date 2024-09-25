#pragma once

#include "render/texture/cube_texture.hpp"

namespace Airwave
{
class OpenGLCubeTexture : public CubeTexture
{
  public:
    OpenGLCubeTexture(const std::array<std::string, 6>& faces);
    virtual ~OpenGLCubeTexture();

    virtual void bind(uint32_t slot = 0) const override;
    virtual void unbind() const override;

    virtual void setData(void *data, uint32_t size) override {}

    virtual uint32_t getRendererID() const override { return m_rendererID; }

  private:

    void loadTexutres(const std::array<std::string, 6> &faces);
    void createEmptyTexture() {}

    uint32_t m_rendererID;
};

} // namespace Airwave
