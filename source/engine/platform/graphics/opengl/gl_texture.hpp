#pragma once

#include "render/texture/texture_2d.hpp"
#include "gl_cube_texture.hpp"
#include <glad/glad.h>
#include <vector>

namespace Airwave
{
class OpenGLTexture2D : public Texture2D
{
  public:
    OpenGLTexture2D(const std::string &path, const TextureSpecification &spec);
    OpenGLTexture2D(uint32_t width, uint32_t height, const TextureSpecification &spec);
    virtual ~OpenGLTexture2D();

    virtual void resize(uint32_t width, uint32_t height) override;

    virtual void bind(uint32_t slot = 0) const override;
    virtual void unbind() const override;

    virtual uint32_t getWidth() const override { return m_width; }
    virtual uint32_t getHeight() const override { return m_height; }

    virtual void setData(void *data, uint32_t size) override;

    virtual uint32_t getRendererID() const override { return m_rendererID; }

    virtual std::shared_ptr<Texture> clone() override;

  private:
    GLenum TextureWrapModeToGL(TextureWrapMode mode);
    GLenum TextureFilterModeToGL(TextureFilterMode mode);
    GLenum TextureFormatToGL(TextureFormat format);
    GLenum TextureInternalFormatToGL(TextureInternalFormat format);
    uint32_t GetChannelCount(GLenum format);
    std::vector<unsigned char> getTextureData();

    friend class OpenGLCubeTexture;
  private:
    uint32_t m_rendererID;
};

} // namespace Airwave
